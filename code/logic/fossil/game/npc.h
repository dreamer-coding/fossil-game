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
#ifndef FOSSIL_GAME_CLINKER_H
#define FOSSIL_GAME_CLINKER_H

#ifdef __cplusplus
extern "C" {
#endif

int fossil_game_clinker_create(const char* npc_id,const char* archetype);
int fossil_game_clinker_destroy(const char* npc_id);

/* Traits and AI behaviors */
int fossil_game_clinker_set_trait(const char* npc_id,const char* key,const char* value);
const char* fossil_game_clinker_get_trait(const char* npc_id,const char* key);

int fossil_game_clinker_tick(const char* npc_id);
const char* fossil_game_clinker_choose_action(const char* npc_id);

int fossil_game_clinker_enable_feature(const char* npc_id,const char* feature);
int fossil_game_clinker_disable_feature(const char* npc_id,const char* feature);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class Clinker {
    const char* id;
public:
    Clinker(const char* i):id(i){}
    void tick(){ fossil_game_clinker_tick(id); }
    const char* chooseAction(){ return fossil_game_clinker_choose_action(id); }
    void enableFeature(const char* f){ fossil_game_clinker_enable_feature(id,f); }
    void disableFeature(const char* f){ fossil_game_clinker_disable_feature(id,f); }
};
}
#endif

#endif
