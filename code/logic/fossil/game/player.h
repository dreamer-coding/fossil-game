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

/* Player lifecycle */
int fossil_game_player_create(const char* player_id);
int fossil_game_player_destroy(const char* player_id);

/* Player attributes */
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

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class Player {
    const char* id;
public:
    Player(const char* i):id(i){}
    void setAttr(const char* k,const char* v){ fossil_game_player_set_attr(id,k,v); }
    const char* getAttr(const char* k){ return fossil_game_player_get_attr(id,k); }
    void addItem(const char* i){ fossil_game_player_add_item(id,i); }
    void removeItem(const char* i){ fossil_game_player_remove_item(id,i); }
    void enableFeature(const char* f){ fossil_game_player_enable_feature(id,f); }
    void disableFeature(const char* f){ fossil_game_player_disable_feature(id,f); }
    bool hasFeature(const char* f){ return fossil_game_player_has_feature(id,f)!=0; }
};
}
#endif

#endif
