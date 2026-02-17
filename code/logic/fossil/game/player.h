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
#ifndef FOSSIL_GAME_PLAYER_H
#define FOSSIL_GAME_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Lifecycle */
int fossil_game_player_create(const char* player_id);
int fossil_game_player_remove(const char* player_id);

/* Attributes */
int fossil_game_player_set_attribute(const char* player_id,const char* key,const void* value);
int fossil_game_player_get_attribute(const char* player_id,const char* key,void* out_value);

/* Inventory */
int fossil_game_player_inventory_add(const char* player_id,const char* item_id,int count);
int fossil_game_player_inventory_remove(const char* player_id,const char* item_id,int count);
int fossil_game_player_inventory_list(const char* player_id,const char*** out_items,int* out_count);

/* Controls */
int fossil_game_player_enable_control(const char* player_id,const char* control);
int fossil_game_player_disable_control(const char* player_id,const char* control);

/* NPC / AI */
int fossil_game_player_npc_update(const char* npc_id);

/* Multiplayer */
int fossil_game_player_join_session(const char* player_id,const char* session_id);
int fossil_game_player_leave_session(const char* player_id);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <vector>

namespace fossil::game {

class Player {
    const char* id;
public:
    explicit Player(const char* player_id) : id(player_id) {}

    void set_attribute(const char* key,const void* value);
    void* get_attribute(const char* key);

    void inventory_add(const char* item_id,int count);
    void inventory_remove(const char* item_id,int count);
    std::vector<const char*> inventory_list() const;

    void enable_control(const char* control);
    void disable_control(const char* control);

    void npc_update();

    void join_session(const char* session_id);
    void leave_session();
};

}

#endif
#endif
