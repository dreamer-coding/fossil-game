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
    p->player_id=strdup(player_id);
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
    q->id=strdup(quiz_id);
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

    nq->id=strdup(question_id);
    nq->text=strdup(question_text);
    nq->option_count=num_options;
    nq->correct_index=correct_index;

    nq->options=malloc(sizeof(char*)*num_options);
    for(int i=0;i<num_options;i++)
        nq->options[i]=strdup(options[i]);

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
   AI helpers (stub)
   ============================================================ */

int fossil_game_quizzed_ai_generate(
    const char* quiz_id,
    const char* topic,
    int difficulty)
{
    /* Placeholder — later could hook into Fossil AI */
    (void)quiz_id;
    (void)topic;
    (void)difficulty;
    return 0;
}
