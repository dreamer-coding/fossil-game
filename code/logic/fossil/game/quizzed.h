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

/* Quiz creation */
int fossil_game_quizzed_create(const char* quiz_id);

/* Add question (AI generated or manual) */
int fossil_game_quizzed_add_question(const char* quiz_id,const char* question_id,const char* text);
int fossil_game_quizzed_add_option(const char* quiz_id,const char* question_id,const char* option_id,const char* text,int correct);

/* Ask question */
const char* fossil_game_quizzed_ask(const char* quiz_id,const char* player_id);

/* Answer question */
int fossil_game_quizzed_answer(const char* quiz_id,const char* player_id,const char* option_id);

/* Player score */
int fossil_game_quizzed_score(const char* quiz_id,const char* player_id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace fossil::game {
class Quizzed {
    const char* id;
public:
    Quizzed(const char* qid):id(qid){}
    const char* ask(const char* player){ return fossil_game_quizzed_ask(id,player); }
    void answer(const char* player,const char* option){ fossil_game_quizzed_answer(id,player,option); }
    int score(const char* player){ return fossil_game_quizzed_score(id,player); }
};
}
#endif

#endif
