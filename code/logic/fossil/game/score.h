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
#ifndef FOSSIL_GAME_SCOREBOARD_H
#define FOSSIL_GAME_SCOREBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Player score submission */
int fossil_game_scoreboard_submit(const char* board_id,const char* player_id,int score);

/* Query score */
int fossil_game_scoreboard_get(const char* board_id,const char* player_id,int* out);

/* Ranking */
int fossil_game_scoreboard_rank(const char* board_id,const char* player_id,int* out);

/* Leaderboard */
const char* fossil_game_scoreboard_leaderboard(const char* board_id);

/* AI matchmaking */
const char* fossil_game_scoreboard_matchmake(const char* board_id,const char* player_id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class Scoreboard {
    const char* id;
public:
    Scoreboard(const char* i):id(i){}
    void submit(const char* p,int s){ fossil_game_scoreboard_submit(id,p,s); }
    int get(const char* p){ int out=0; fossil_game_scoreboard_get(id,p,&out); return out; }
    int rank(const char* p){ int r=0; fossil_game_scoreboard_rank(id,p,&r); return r; }
    const char* leaderboard(){ return fossil_game_scoreboard_leaderboard(id); }
    const char* matchmake(const char* p){ return fossil_game_scoreboard_matchmake(id,p); }
};
}
#endif

#endif
