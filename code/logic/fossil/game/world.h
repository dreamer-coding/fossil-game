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
#ifndef FOSSIL_GAME_WORLD_H
#define FOSSIL_GAME_WORLD_H

#ifdef __cplusplus
extern "C" {
#endif

int fossil_game_world_create_room(const char* room_id);
int fossil_game_world_destroy_room(const char* room_id);

int fossil_game_world_set_desc(const char* room_id,const char* text);

int fossil_game_world_link(const char* from,const char* to,const char* direction);
const char* fossil_game_world_move(const char* player_id,const char* direction);

const char* fossil_game_world_where(const char* player_id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class world{
public:
    static void create_room(const char* r){ fossil_game_world_create_room(r); }
};
}
#endif

#endif
