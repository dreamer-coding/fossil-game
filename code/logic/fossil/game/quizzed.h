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

/* Quiz lifecycle */
int fossil_game_quizzed_create(const char* quiz_id);
int fossil_game_quizzed_remove(const char* quiz_id);

/* Question management */
int fossil_game_quizzed_add_question(
    const char* quiz_id,
    const char* question_id,
    const char* question_text,
    const char** options,
    int num_options,
    int correct_index);

int fossil_game_quizzed_remove_question(
    const char* quiz_id,
    const char* question_id);

/* Gameplay */
int fossil_game_quizzed_ask(
    const char* quiz_id,
    const char* player_id,
    char* out_question,
    int max_len);

int fossil_game_quizzed_answer(
    const char* quiz_id,
    const char* player_id,
    const char* answer);

/* Scoring */
int fossil_game_quizzed_score(const char* quiz_id,const char* player_id);
int fossil_game_quizzed_reset(const char* quiz_id,const char* player_id);

/* AI helpers */
int fossil_game_quizzed_ai_generate(
    const char* quiz_id,
    const char* topic,
    int difficulty);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <vector>
#include <string>

namespace fossil::game {

class Quiz {
    const char* id;
public:
    explicit Quiz(const char* quiz_id) : id(quiz_id) {}

    void add_question(const char* qid,const char* text,
                      const std::vector<const char*>& options,
                      int correct_index);

    void remove_question(const char* qid);

    std::string ask(const char* player_id);
    bool answer(const char* player_id,const char* answer);

    int score(const char* player_id);
    void reset(const char* player_id);

    void ai_generate(const char* topic,int difficulty);
};

}

#endif
#endif
