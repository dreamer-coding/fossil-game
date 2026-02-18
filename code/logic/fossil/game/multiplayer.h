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
#ifndef FOSSIL_GAME_MULTIPLAYER_H
#define FOSSIL_GAME_MULTIPLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

int fossil_game_multiplayer_create_session(const char* session_id);
int fossil_game_multiplayer_destroy_session(const char* session_id);

int fossil_game_multiplayer_join(const char* session_id,const char* player_id);
int fossil_game_multiplayer_leave(const char* session_id,const char* player_id);

int fossil_game_multiplayer_broadcast(const char* session_id,const char* message);
int fossil_game_multiplayer_send(const char* session_id,const char* player_id,const char* message);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class Multiplayer {
    const char* id;
public:
    Multiplayer(const char* s):id(s){}
    void join(const char* p){ fossil_game_multiplayer_join(id,p); }
    void leave(const char* p){ fossil_game_multiplayer_leave(id,p); }
    void broadcast(const char* msg){ fossil_game_multiplayer_broadcast(id,msg); }
    void send(const char* p,const char* msg){ fossil_game_multiplayer_send(id,p,msg); }
};
}
#endif

#endif
