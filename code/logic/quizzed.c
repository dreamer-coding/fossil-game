/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "fossil/game/quizzed.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char* fossil_strdup(const char* s)
{
    if(!s) return NULL;

    size_t len = 0;
    while(s[len]) len++;

    char* out = (char*)malloc(len + 1);
    if(!out) return NULL;

    for(size_t i=0;i<=len;i++)
        out[i] = s[i];

    return out;
}

/* ============================================================
   Internal structures
   ============================================================ */

typedef struct {
    char* id;
    char* text;
    char** options;
    int option_count;
    int correct_index;
} question_t;

typedef struct {
    char* player_id;
    int score;
    int current_question;
} player_state_t;

typedef struct {
    char* id;

    question_t* questions;
    int question_count;

    player_state_t* players;
    int player_count;

} quiz_t;


/* Global registry (simple dynamic array) */
static quiz_t* g_quizzes = NULL;
static int g_quiz_count = 0;


/* ============================================================
   Helpers
   ============================================================ */

static quiz_t* find_quiz(const char* id)
{
    for(int i=0;i<g_quiz_count;i++)
        if(strcmp(g_quizzes[i].id,id)==0)
            return &g_quizzes[i];
    return NULL;
}

static player_state_t* find_player(quiz_t* q,const char* player_id)
{
    for(int i=0;i<q->player_count;i++)
        if(strcmp(q->players[i].player_id,player_id)==0)
            return &q->players[i];

    /* create if missing */
    q->players=realloc(q->players,sizeof(player_state_t)*(q->player_count+1));
    player_state_t* p=&q->players[q->player_count++];
    p->player_id=fossil_strdup(player_id);
    p->score=0;
    p->current_question=0;
    return p;
}

static void free_question(question_t* q)
{
    free(q->id);
    free(q->text);
    for(int i=0;i<q->option_count;i++) free(q->options[i]);
    free(q->options);
}

/* ============================================================
   Quiz lifecycle
   ============================================================ */

int fossil_game_quizzed_create(const char* quiz_id)
{
    if(!quiz_id) return -1;
    if(find_quiz(quiz_id)) return -2;

    g_quizzes=realloc(g_quizzes,sizeof(quiz_t)*(g_quiz_count+1));
    quiz_t* q=&g_quizzes[g_quiz_count++];

    memset(q,0,sizeof(*q));
    q->id=fossil_strdup(quiz_id);
    return 0;
}

int fossil_game_quizzed_remove(const char* quiz_id)
{
    for(int i=0;i<g_quiz_count;i++)
    {
        if(strcmp(g_quizzes[i].id,quiz_id)==0)
        {
            quiz_t* q=&g_quizzes[i];

            for(int j=0;j<q->question_count;j++)
                free_question(&q->questions[j]);

            free(q->questions);

            for(int j=0;j<q->player_count;j++)
                free(q->players[j].player_id);

            free(q->players);
            free(q->id);

            memmove(&g_quizzes[i],&g_quizzes[i+1],
                    sizeof(quiz_t)*(g_quiz_count-i-1));

            g_quiz_count--;
            return 0;
        }
    }
    return -1;
}

/* ============================================================
   Question management
   ============================================================ */

int fossil_game_quizzed_add_question(
    const char* quiz_id,
    const char* question_id,
    const char* question_text,
    const char** options,
    int num_options,
    int correct_index)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q || !question_id || !question_text) return -1;
    if(num_options<=0) return -2;

    q->questions=realloc(q->questions,sizeof(question_t)*(q->question_count+1));
    question_t* nq=&q->questions[q->question_count++];

    nq->id=fossil_strdup(question_id);
    nq->text=fossil_strdup(question_text);
    nq->option_count=num_options;
    nq->correct_index=correct_index;

    nq->options=malloc(sizeof(char*)*num_options);
    for(int i=0;i<num_options;i++)
        nq->options[i]=fossil_strdup(options[i]);

    return 0;
}

int fossil_game_quizzed_remove_question(
    const char* quiz_id,
    const char* question_id)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q) return -1;

    for(int i=0;i<q->question_count;i++)
    {
        if(strcmp(q->questions[i].id,question_id)==0)
        {
            free_question(&q->questions[i]);
            memmove(&q->questions[i],&q->questions[i+1],
                    sizeof(question_t)*(q->question_count-i-1));
            q->question_count--;
            return 0;
        }
    }
    return -2;
}

/* ============================================================
   Gameplay
   ============================================================ */

int fossil_game_quizzed_ask(
    const char* quiz_id,
    const char* player_id,
    char* out_question,
    int max_len)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q || q->question_count==0) return -1;

    player_state_t* p=find_player(q,player_id);
    int idx=p->current_question % q->question_count;

    snprintf(out_question,max_len,"%s",q->questions[idx].text);
    return 0;
}

int fossil_game_quizzed_answer(
    const char* quiz_id,
    const char* player_id,
    const char* answer)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q) return -1;

    player_state_t* p=find_player(q,player_id);
    int idx=p->current_question % q->question_count;
    question_t* qu=&q->questions[idx];

    if(answer && atoi(answer)==qu->correct_index)
        p->score++;

    p->current_question++;
    return 0;
}

/* ============================================================
   Scoring
   ============================================================ */

int fossil_game_quizzed_score(const char* quiz_id,const char* player_id)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q) return -1;
    player_state_t* p=find_player(q,player_id);
    return p->score;
}

int fossil_game_quizzed_reset(const char* quiz_id,const char* player_id)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q) return -1;
    player_state_t* p=find_player(q,player_id);
    p->score=0;
    p->current_question=0;
    return 0;
}

/* ============================================================
   Built-in procedural knowledge base
   ============================================================ */

typedef struct {
    const char* question;
    const char* options[4];
    int correct;
    int min_difficulty;
} gen_question_t;

typedef struct {
    const char* topic;
    const gen_question_t* questions;
    int count;
} gen_category_t;


/* ---------- MATH ---------- */
static const gen_question_t math_bank[]={
    {"What is 2+2?",{"1","2","4","8"},2,1},
    {"What is 9*3?",{"12","27","18","21"},1,1},
    {"What is sqrt(81)?",{"7","8","9","6"},2,2},
    {"Derivative of x^2?",{"x","2x","x^2","1"},1,3},
    {"Integral of 1/x?",{"ln(x)","x","1/x","x^2"},0,4}
};

/* ---------- SCIENCE ---------- */
static const gen_question_t science_bank[]={
    {"Water chemical formula?",{"H2O","CO2","NaCl","O2"},0,1},
    {"Planet known as Red Planet?",{"Earth","Mars","Jupiter","Venus"},1,1},
    {"Speed of light approx?",{"300 km/s","300,000 km/s","30,000 km/s","3,000 km/s"},1,2},
    {"Electron charge?",{"Positive","Neutral","Negative","Variable"},2,2},
    {"DNA shape?",{"Circle","Double helix","Square","Line"},1,1}
};

/* ---------- HISTORY ---------- */
static const gen_question_t history_bank[]={
    {"First US president?",{"Lincoln","Washington","Adams","Jefferson"},1,1},
    {"Year WWII ended?",{"1940","1945","1950","1939"},1,1},
    {"Roman numeral for 50?",{"L","V","C","X"},0,1},
    {"Empire ruled by Julius Caesar?",{"Greek","Roman","Persian","Ottoman"},1,1}
};

/* ---------- PROGRAMMING ---------- */
static const gen_question_t prog_bank[]={
    {"C language creator?",{"Ritchie","Torvalds","Stroustrup","Gosling"},0,1},
    {"What is malloc used for?",{"IO","Memory alloc","Threads","Math"},1,1},
    {"Null pointer value?",{"0","1","-1","255"},0,1},
    {"Keyword for constant in C?",{"let","const","immutable","fixed"},1,1}
};

/* ---------- FALLBACK GENERAL ---------- */
static const gen_question_t general_bank[]={
    {"Which is a fruit?",{"Carrot","Apple","Potato","Onion"},1,1},
    {"How many days in week?",{"5","6","7","8"},2,1},
    {"Opposite of hot?",{"Cold","Dry","Wet","Bright"},0,1}
};


/* Category registry */
static const gen_category_t categories[]={
    {"math",math_bank,sizeof(math_bank)/sizeof(*math_bank)},
    {"science",science_bank,sizeof(science_bank)/sizeof(*science_bank)},
    {"history",history_bank,sizeof(history_bank)/sizeof(*history_bank)},
    {"programming",prog_bank,sizeof(prog_bank)/sizeof(*prog_bank)},
    {"general",general_bank,sizeof(general_bank)/sizeof(*general_bank)}
};

static const gen_category_t* find_category(const char* topic)
{
    if(!topic) return &categories[4]; /* general */

    for(size_t i=0;i<sizeof(categories)/sizeof(*categories);i++)
        if(strcmp(categories[i].topic,topic)==0)
            return &categories[i];

    return &categories[4];
}


/* ============================================================
   AI generator (data-driven)
   ============================================================ */

int fossil_game_quizzed_ai_generate(
    const char* quiz_id,
    const char* topic,
    int difficulty)
{
    quiz_t* q=find_quiz(quiz_id);
    if(!q) return -1;

    if(difficulty<1) difficulty=1;
    if(difficulty>5) difficulty=5;

    const gen_category_t* cat=find_category(topic);

    /* gather eligible questions for difficulty */
    int pool_count=0;
    const gen_question_t* pool[64];

    for(int i=0;i<cat->count;i++)
        if(cat->questions[i].min_difficulty<=difficulty)
            pool[pool_count++]=&cat->questions[i];

    /* fallback if nothing matches */
    if(pool_count==0){
        for(int i=0;i<general_bank[0].min_difficulty;i++);
        for(int i=0;i<(int)(sizeof(general_bank)/sizeof(*general_bank));i++)
            pool[pool_count++]=&general_bank[i];
    }

    /* pick question */
    const gen_question_t* gq=pool[rand()%pool_count];

    /* create unique ID using ALL params */
    char qid[128];
    snprintf(qid,sizeof(qid),
             "ai_%s_%s_%d_%d",
             quiz_id,
             topic?topic:"general",
             difficulty,
             q->question_count);

    /* forward into quiz system */
    return fossil_game_quizzed_add_question(
        quiz_id,
        qid,
        gq->question,
        gq->options,
        4,
        gq->correct);
}
