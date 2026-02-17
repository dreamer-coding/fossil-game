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
#ifndef FOSSIL_GAME_SCORE_H
#define FOSSIL_GAME_SCORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Score updates */
int fossil_game_score_update(const char* player_id,int points);
int fossil_game_score_get(const char* player_id,int* out_points);
int fossil_game_score_reset(const char* player_id);

/* Leaderboards */
int fossil_game_score_leaderboard(
    const char* leaderboard_id,
    const char*** out_player_ids,
    int* out_count);

/* Matchmaking */
int fossil_game_score_matchmaking(
    const char* player_id,
    char*** out_opponents,
    int* out_count);

/* Achievements */
int fossil_game_score_add_achievement(
    const char* player_id,
    const char* achievement_id);

int fossil_game_score_has_achievement(
    const char* player_id,
    const char* achievement_id);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <vector>

namespace fossil::game {

class Score {
public:
    void update(const char* player_id,int points);
    int get(const char* player_id);
    void reset(const char* player_id);

    std::vector<const char*> leaderboard(const char* leaderboard_id);
    std::vector<const char*> matchmaking(const char* player_id);

    void add_achievement(const char* player_id,const char* achievement_id);
    bool has_achievement(const char* player_id,const char* achievement_id);
};

}

#endif
#endif
