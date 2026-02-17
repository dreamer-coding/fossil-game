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
#include "fossil/game/score.h"
#include <stdlib.h>
#include <string.h>

/* ============================================================
   Internal structures
   ============================================================ */

typedef struct {
    char* id;
    int score;

    char** achievements;
    int achievement_count;
} player_score_t;

typedef struct {
    char* id;
    char** players;
    int count;
} leaderboard_t;


/* Global registries */
static player_score_t* g_players=NULL;
static int g_player_count=0;

static leaderboard_t* g_boards=NULL;
static int g_board_count=0;


/* ============================================================
   Helpers
   ============================================================ */

static player_score_t* find_player(const char* id)
{
    for(int i=0;i<g_player_count;i++)
        if(strcmp(g_players[i].id,id)==0)
            return &g_players[i];

    /* create automatically */
    g_players=realloc(g_players,sizeof(player_score_t)*(g_player_count+1));
    player_score_t* p=&g_players[g_player_count++];

    p->id=strdup(id);
    p->score=0;
    p->achievements=NULL;
    p->achievement_count=0;
    return p;
}

static int cmp_scores_desc(const void* a,const void* b)
{
    const player_score_t* pa=*(const player_score_t**)a;
    const player_score_t* pb=*(const player_score_t**)b;
    return pb->score - pa->score;
}

/* ============================================================
   Score updates
   ============================================================ */

int fossil_game_score_update(const char* player_id,int points)
{
    if(!player_id) return -1;
    player_score_t* p=find_player(player_id);
    p->score+=points;
    return 0;
}

int fossil_game_score_get(const char* player_id,int* out_points)
{
    if(!player_id||!out_points) return -1;
    player_score_t* p=find_player(player_id);
    *out_points=p->score;
    return 0;
}

int fossil_game_score_reset(const char* player_id)
{
    if(!player_id) return -1;
    player_score_t* p=find_player(player_id);
    p->score=0;
    return 0;
}

/* ============================================================
   Leaderboards
   ============================================================ */

int fossil_game_score_leaderboard(
    const char* leaderboard_id,
    const char*** out_player_ids,
    int* out_count)
{
    (void)leaderboard_id; /* currently unused */

    if(!out_player_ids||!out_count) return -1;

    if(g_player_count==0){
        *out_player_ids=NULL;
        *out_count=0;
        return 0;
    }

    player_score_t** sorted=malloc(sizeof(player_score_t*)*g_player_count);
    for(int i=0;i<g_player_count;i++)
        sorted[i]=&g_players[i];

    qsort(sorted,g_player_count,sizeof(player_score_t*),cmp_scores_desc);

    const char** result=malloc(sizeof(char*)*g_player_count);
    for(int i=0;i<g_player_count;i++)
        result[i]=sorted[i]->id;

    free(sorted);

    *out_player_ids=result;
    *out_count=g_player_count;
    return 0;
}

/* ============================================================
   Matchmaking (skill proximity search)
   ============================================================ */

int fossil_game_score_matchmaking(
    const char* player_id,
    char*** out_opponents,
    int* out_count)
{
    if(!player_id||!out_opponents||!out_count) return -1;

    player_score_t* me=find_player(player_id);

    /* simple window search */
    int window=100;

    char** matches=NULL;
    int count=0;

    for(int i=0;i<g_player_count;i++)
    {
        player_score_t* p=&g_players[i];
        if(p==me) continue;

        int diff=p->score - me->score;
        if(diff<0) diff=-diff;

        if(diff<=window)
        {
            matches=realloc(matches,sizeof(char*)*(count+1));
            matches[count++]=p->id;
        }
    }

    *out_opponents=matches;
    *out_count=count;
    return 0;
}

/* ============================================================
   Achievements
   ============================================================ */

int fossil_game_score_add_achievement(
    const char* player_id,
    const char* achievement_id)
{
    if(!player_id||!achievement_id) return -1;

    player_score_t* p=find_player(player_id);

    /* prevent duplicates */
    for(int i=0;i<p->achievement_count;i++)
        if(strcmp(p->achievements[i],achievement_id)==0)
            return 0;

    p->achievements=realloc(
        p->achievements,
        sizeof(char*)*(p->achievement_count+1));

    p->achievements[p->achievement_count++]=strdup(achievement_id);
    return 0;
}

int fossil_game_score_has_achievement(
    const char* player_id,
    const char* achievement_id)
{
    if(!player_id||!achievement_id) return 0;

    player_score_t* p=find_player(player_id);

    for(int i=0;i<p->achievement_count;i++)
        if(strcmp(p->achievements[i],achievement_id)==0)
            return 1;

    return 0;
}
