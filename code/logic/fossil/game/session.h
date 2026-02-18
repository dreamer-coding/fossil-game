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
#ifndef FOSSIL_GAME_SESSION_H
#define FOSSIL_GAME_SESSION_H

#ifdef __cplusplus
extern "C" {
#endif

int fossil_game_session_create(const char* session_id);
int fossil_game_session_destroy(const char* session_id);

int fossil_game_session_start(const char* session_id);
int fossil_game_session_stop(const char* session_id);
int fossil_game_session_tick(const char* session_id);

int fossil_game_session_add_player(const char* session_id,const char* player_id);
int fossil_game_session_remove_player(const char* session_id,const char* player_id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class Session {
    const char* id;
public:
    Session(const char* i):id(i){}
    void start(){ fossil_game_session_start(id); }
    void stop(){ fossil_game_session_stop(id); }
    void tick(){ fossil_game_session_tick(id); }
    void add_player(const char* p){ fossil_game_session_add_player(id,p); }
    void remove_player(const char* p){ fossil_game_session_remove_player(id,p); }
};
}
#endif

#endif
