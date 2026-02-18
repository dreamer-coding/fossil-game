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
int fossil_game_player_destroy(const char* player_id);

/* Attributes */
int fossil_game_player_set_attr(const char* player_id,const char* key,const char* value);
const char* fossil_game_player_get_attr(const char* player_id,const char* key);

/* Inventory */
int fossil_game_player_add_item(const char* player_id,const char* item_id);
int fossil_game_player_remove_item(const char* player_id,const char* item_id);
int fossil_game_player_has_item(const char* player_id,const char* item_id);

/* Features */
int fossil_game_player_enable_feature(const char* player_id,const char* feature);
int fossil_game_player_disable_feature(const char* player_id,const char* feature);
int fossil_game_player_has_feature(const char* player_id,const char* feature);

/* Sessions */
int fossil_game_player_join_session(const char* player_id,const char* session_id);
int fossil_game_player_leave_session(const char* player_id,const char* session_id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class player {
    const char* id;
public:
    player(const char* i):id(i){}
    void set_attr(const char* k,const char* v){ fossil_game_player_set_attr(id,k,v); }
    const char* get_attr(const char* k)const{ return fossil_game_player_get_attr(id,k); }
    void add_item(const char* item){ fossil_game_player_add_item(id,item); }
    void remove_item(const char* item){ fossil_game_player_remove_item(id,item); }
    void enable_feature(const char* f){ fossil_game_player_enable_feature(id,f); }
};
}
#endif

#endif
