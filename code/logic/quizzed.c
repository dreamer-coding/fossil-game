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
    {"2+2?",{"1","2","4","8"},2,1},
    {"9*3?",{"12","27","18","21"},1,1},
    {"5^2?",{"10","25","15","20"},1,1},
    {"sqrt(64)?",{"6","7","8","9"},2,1},
    {"12/3?",{"3","4","5","6"},1,1},
    {"Prime number?",{"9","15","17","21"},2,1},
    {"15% of 200?",{"20","25","30","35"},2,1},
    {"7 factorial?",{"5040","720","40320","840"},0,2},
    {"Pi approx?",{"3.14","2.17","1.61","4.12"},0,1},
    {"Binary of 5?",{"101","110","111","100"},0,2},
    
    {"Derivative x^2?",{"x","2x","x^2","1"},1,2},
    {"Integral x dx?",{"x^2/2","x","1/x","lnx"},0,2},
    {"Limit sin(x)/x as x->0?",{"1","0","∞","-1"},0,3},
    {"d/dx ln(x)?",{"1/x","x","lnx","e^x"},0,3},
    {"Integral e^x?",{"e^x","x","lnx","1"},0,2},
    
    {"Matrix det of identity?",{"0","1","2","-1"},1,2},
    {"Angle sum triangle?",{"180","360","90","270"},0,1},
    {"cos(0)?",{"0","1","-1","0.5"},1,1},
    {"sin(90°)?",{"1","0","-1","0.5"},0,1},
    {"tan(45°)?",{"0","1","2","-1"},1,2},
    
    {"Log10(100)?",{"1","2","3","10"},1,1},
    {"Vector dot orthogonal?",{"0","1","-1","∞"},0,2},
    {"Golden ratio approx?",{"1.6","2.7","3.14","1.2"},0,3},
    {"Derivative constant?",{"0","1","x","∞"},0,1},
    {"Integral constant c?",{"cx","c","1","0"},0,2},
    
    {"Area circle?",{"πr²","2πr","πd","r²"},0,1},
    {"Slope formula?",{"dy/dx","x/y","xy","y-x"},0,2},
    {"Complex i^2?",{"-1","1","0","i"},0,2},
    {"Probability heads coin?",{"0.5","1","0","0.25"},0,1},
    {"Sum 1..10?",{"55","50","45","60"},0,1}
};

/* ---------- SCIENCE ---------- */
static const gen_question_t science_bank[]={
    {"Water formula?",{"H2O","CO2","O2","NaCl"},0,1},
    {"Red planet?",{"Earth","Mars","Jupiter","Venus"},1,1},
    {"Sun type?",{"Star","Planet","Comet","Asteroid"},0,1},
    {"Gas we breathe?",{"Oxygen","Nitrogen","CO2","Helium"},0,1},
    {"DNA shape?",{"Helix","Circle","Line","Cube"},0,1},
    
    {"Light speed km/s?",{"300000","300","30000","3"},0,2},
    {"Boiling water °C?",{"100","90","80","120"},0,1},
    {"Electron charge?",{"Neg","Pos","Neutral","Var"},0,1},
    {"Photosynthesis uses?",{"Light","Heat","Sound","Wind"},0,1},
    {"Largest organ?",{"Skin","Heart","Liver","Brain"},0,1},
    
    {"Earth layer outer?",{"Crust","Core","Mantle","Shell"},0,1},
    {"Chemical pH neutral?",{"7","0","14","5"},0,1},
    {"Atomic center?",{"Nucleus","Electron","Proton","Shell"},0,1},
    {"Hubble observed?",{"Galaxies","Atoms","Cells","Molecules"},0,2},
    {"Gravity accel m/s²?",{"9.8","3","20","1"},0,2},
    
    {"Newton law count?",{"3","2","5","4"},0,1},
    {"Human bones approx?",{"206","150","300","120"},0,2},
    {"Sound needs?",{"Medium","Vacuum","Light","Gravity"},0,1},
    {"Cell power plant?",{"Mitochondria","Nucleus","Ribosome","Membrane"},0,1},
    {"Closest star?",{"Sun","Alpha","Sirius","Betel"},0,1},
    
    {"Earth age billions?",{"4.5","2","10","1"},0,3},
    {"Plate movement?",{"Tectonics","Fusion","Spin","Drift"},0,1},
    {"Energy unit?",{"Joule","Volt","Amp","Newton"},0,1},
    {"Largest planet?",{"Jupiter","Saturn","Earth","Mars"},0,1},
    {"Magnetic poles?",{"2","1","4","0"},0,1},
    
    {"Human genome pairs?",{"23","46","12","18"},0,2},
    {"Laser type?",{"Light","Sound","Heat","Matter"},0,1},
    {"Main gas sun?",{"Hydrogen","Oxygen","Iron","Carbon"},0,1},
    {"Telescope sees?",{"Light","Sound","Atoms","Heat"},0,1},
    {"Virus alive?",{"Debated","Yes","No","Always"},0,3}
};

/* ---------- HISTORY ---------- */
static const gen_question_t history_bank[]={
    {"First US president?",{"Washington","Adams","Lincoln","Jeff"},0,1},
    {"WWII end?",{"1945","1940","1939","1950"},0,1},
    {"Roman numeral 50?",{"L","V","X","C"},0,1},
    {"Pyramids built by?",{"Egyptians","Romans","Greeks","Maya"},0,1},
    {"Cold War rivals?",{"US-USSR","US-China","UK-France","Rome-Greece"},0,1},
    
    {"Magna Carta year?",{"1215","1300","1500","1100"},0,2},
    {"Industrial origin?",{"Britain","France","US","China"},0,1},
    {"Berlin Wall fall?",{"1989","1970","1995","1961"},0,1},
    {"Printing press?",{"Gutenberg","Edison","Tesla","Newton"},0,1},
    {"French revolution?",{"1789","1700","1800","1600"},0,1},
    
    {"Julius Caesar empire?",{"Roman","Greek","Persian","Ottoman"},0,1},
    {"Mongol leader?",{"Genghis","Kublai","Attila","Napoleon"},0,1},
    {"US Civil War end?",{"1865","1850","1870","1880"},0,1},
    {"Moon landing year?",{"1969","1959","1975","1981"},0,1},
    {"UN founded?",{"1945","1920","1955","1935"},0,1},
    
    {"Black Death century?",{"14th","10th","18th","20th"},0,2},
    {"Renaissance region?",{"Italy","France","China","US"},0,1},
    {"Spartan city?",{"Greece","Rome","Egypt","Persia"},0,1},
    {"First computers WWII?",{"Yes","No","Later","Unknown"},0,2},
    {"Great Wall country?",{"China","India","Japan","Korea"},0,1},
    
    {"Alexander from?",{"Macedon","Rome","Egypt","Persia"},0,1},
    {"Napoleon defeated at?",{"Waterloo","Trafalgar","Paris","Berlin"},0,1},
    {"US independence year?",{"1776","1800","1750","1812"},0,1},
    {"Ottoman capital?",{"Istanbul","Cairo","Rome","Athens"},0,2},
    {"Vikings from?",{"Scandinavia","France","Italy","Spain"},0,1},
    
    {"Silk Road traded?",{"Goods","Ideas","Both","None"},2,1},
    {"First writing?",{"Sumer","Rome","China","Egypt"},0,2},
    {"Feudal system?",{"Medieval","Modern","Ancient","Future"},0,1},
    {"Titanic sank?",{"1912","1900","1920","1890"},0,1},
    {"US constitution year?",{"1787","1800","1760","1795"},0,2}
};

/* ---------- PROGRAMMING ---------- */
static const gen_question_t prog_bank[]={
    {"C creator?",{"Ritchie","Torvalds","Stroustrup","Gosling"},0,1},
    {"malloc does?",{"Alloc","Free","Print","Loop"},0,1},
    {"Null pointer?",{"0","1","-1","255"},0,1},
    {"Header for printf?",{"stdio.h","stdlib.h","string.h","math.h"},0,1},
    {"Binary of 2?",{"10","11","01","00"},0,1},
    
    {"Stack vs heap?",{"Memory","IO","Math","File"},0,1},
    {"C++ RAII?",{"Lifetime","Math","Threads","Cache"},0,2},
    {"Python type?",{"Dynamic","Static","Manual","None"},0,1},
    {"Git command clone?",{"clone","copy","pull","init"},0,1},
    {"SQL selects?",{"SELECT","GET","FIND","READ"},0,1},
    
    {"Big-O best search?",{"O(1)","O(n)","O(log n)","O(n²)"},0,2},
    {"Binary search needs?",{"Sorted","Random","Linked","None"},0,1},
    {"Pointer holds?",{"Address","Value","Index","Loop"},0,1},
    {"C string end?",{"\\0","EOF","/","."},0,1},
    {"Compilation step?",{"Link","Run","Sleep","Stop"},0,1},
    
    {"Thread vs process?",{"Memory","Color","File","Signal"},0,2},
    {"HTTP port?",{"80","21","22","25"},0,1},
    {"Linux kernel in?",{"C","Java","Rust","Python"},0,1},
    {"Recursive call?",{"Self","Other","Loop","None"},0,1},
    {"Hash table avg?",{"O(1)","O(n)","O(logn)","O(n²)"},0,2},
    
    {"Endian difference?",{"Byte order","Bits","Threads","Speed"},0,2},
    {"JSON format?",{"Text","Binary","Video","Audio"},0,1},
    {"UTF-8 type?",{"Encoding","Protocol","File","Socket"},0,1},
    {"Semaphore controls?",{"Access","Speed","Memory","Files"},0,2},
    {"API stands?",{"Interface","Process","Address","Input"},0,1},
    
    {"Compiler vs interpreter?",{"Ahead","Same","None","Visual"},0,2},
    {"DNS resolves?",{"Name->IP","IP->Name","Port","File"},0,1},
    {"Stack overflow?",{"Deep recursion","Disk full","CPU hot","IO fail"},0,1},
    {"Binary tree height?",{"Levels","Nodes","Leaves","Edges"},0,2},
    {"Virtual memory?",{"Abstraction","Disk","GPU","Cache"},0,2}
};

/* ---------- FALLBACK GENERAL ---------- */
static const gen_question_t general_bank[] = {
    {"Which is a fruit?",{"Carrot","Apple","Potato","Onion"},1,1},
    {"How many days in a week?",{"5","6","7","8"},2,1},
    {"Opposite of hot?",{"Cold","Dry","Wet","Bright"},0,1},
    {"Largest mammal?",{"Elephant","Whale","Lion","Horse"},1,1},
    {"Primary colors?",{"Red, Blue, Green","Red, Blue, Yellow","Red, Yellow, Purple","Blue, Yellow, Green"},1,1},
    {"Continent with Egypt?",{"Asia","Europe","Africa","South America"},2,1},
    {"Fastest land animal?",{"Cheetah","Lion","Tiger","Horse"},0,2},
    {"Currency of Japan?",{"Yuan","Yen","Dollar","Rupee"},1,1},
    {"How many planets in solar system?",{"7","8","9","10"},1,1},
    {"H2O is known as?",{"Oxygen","Water","Hydrogen","Salt"},1,1},
    
    {"Smallest prime?",{"0","1","2","3"},2,1},
    {"Opposite of day?",{"Night","Dusk","Morning","Sun"},0,1},
    {"Human body temperature °C?",{"37","35","40","39"},0,1},
    {"Ocean covering most Earth?",{"Atlantic","Indian","Pacific","Arctic"},2,1},
    {"Fastest bird?",{"Falcon","Eagle","Owl","Hawk"},0,2},
    {"Longest river?",{"Amazon","Nile","Yangtze","Mississippi"},1,1},
    {"Light travels in?",{"Air","Vacuum","Water","Metal"},1,2},
    {"Common gas we breathe?",{"CO2","Oxygen","Nitrogen","Methane"},1,1},
    {"How many continents?",{"5","6","7","8"},2,1},
    {"Freezing point water °C?",{"0","32","100","-1"},0,1},
    
    {"Largest desert?",{"Sahara","Gobi","Arabian","Kalahari"},0,1},
    {"Primary shape with 3 sides?",{"Square","Triangle","Circle","Hexagon"},1,1},
    {"Largest planet?",{"Earth","Jupiter","Mars","Venus"},1,1},
    {"Largest ocean?",{"Atlantic","Indian","Pacific","Arctic"},2,1},
    {"Opposite of up?",{"Down","Left","Right","Over"},0,1},
    {"Day after Monday?",{"Tuesday","Wednesday","Thursday","Friday"},0,1},
    {"Color of sky?",{"Red","Blue","Green","Yellow"},1,1},
    {"Sound travels fastest in?",{"Air","Water","Steel","Vacuum"},2,2},
    {"Primary programming language?",{"C","Python","Java","All"},3,2},
    {"Largest organ in human body?",{"Liver","Skin","Heart","Brain"},1,1}
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
