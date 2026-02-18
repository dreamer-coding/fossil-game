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
#ifndef FOSSIL_GAME_QUIZZED_H
#define FOSSIL_GAME_QUIZZED_H

#ifdef __cplusplus
extern "C" {
#endif

int fossil_game_quizzed_create(const char* quiz_id);

int fossil_game_quizzed_add_question(
    const char* quiz_id,
    const char* question_id,
    const char* text);

int fossil_game_quizzed_add_option(
    const char* quiz_id,
    const char* question_id,
    const char* option_id,
    const char* text,
    int correct);

const char* fossil_game_quizzed_ask(const char* quiz_id,const char* player_id);

int fossil_game_quizzed_answer(
    const char* quiz_id,
    const char* player_id,
    const char* option_id);

int fossil_game_quizzed_score(const char* quiz_id,const char* player_id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class quizzed{
    const char* id;
public:
    quizzed(const char* q):id(q){}
    const char* ask(const char* p){ return fossil_game_quizzed_ask(id,p); }
};
}
#endif

#endif
