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
#include "fossil/game/player.h"
#include <stdlib.h>
#include <string.h>

/* ============================================================
   Internal Structures
   ============================================================ */

typedef struct fossil_game_player_attr {
    char* key;
    void* value;
} fossil_game_player_attr;

typedef struct fossil_game_player_item {
    char* item_id;
    int   count;
} fossil_game_player_item;

typedef struct fossil_game_player_control {
    char* name;
    int enabled;
} fossil_game_player_control;

typedef struct fossil_game_player {
    char* id;

    fossil_game_player_attr* attrs;
    size_t attr_count;

    fossil_game_player_item* inventory;
    size_t inventory_count;

    fossil_game_player_control* controls;
    size_t control_count;

    char* session_id;
} fossil_game_player;

/* ============================================================
   Registry
   ============================================================ */

static fossil_game_player** g_players = NULL;
static size_t g_player_count = 0;

static fossil_game_player* find_player(const char* id)
{
    if(!id) return NULL;
    for(size_t i=0;i<g_player_count;i++){
        if(g_players[i] && strcmp(g_players[i]->id,id)==0)
            return g_players[i];
    }
    return NULL;
}

/* ============================================================
   Lifecycle
   ============================================================ */

int fossil_game_player_create(const char* player_id)
{
    if(!player_id) return -1;
    if(find_player(player_id)) return -2;

    fossil_game_player* p = calloc(1,sizeof(*p));
    if(!p) return -3;

    p->id = strdup(player_id);
    if(!p->id){ free(p); return -3; }

    fossil_game_player** tmp =
        realloc(g_players,(g_player_count+1)*sizeof(*g_players));
    if(!tmp){ free(p->id); free(p); return -3; }

    g_players = tmp;
    g_players[g_player_count++] = p;
    return 0;
}

int fossil_game_player_remove(const char* player_id)
{
    if(!player_id) return -1;

    for(size_t i=0;i<g_player_count;i++){
        fossil_game_player* p=g_players[i];
        if(p && strcmp(p->id,player_id)==0){

            free(p->id);
            free(p->attrs);
            free(p->inventory);
            free(p->controls);
            free(p->session_id);
            free(p);

            g_players[i]=g_players[g_player_count-1];
            g_player_count--;
            return 0;
        }
    }
    return -2;
}

/* ============================================================
   Attributes
   ============================================================ */

int fossil_game_player_set_attribute(const char* player_id,const char* key,const void* value)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!key) return -1;

    for(size_t i=0;i<p->attr_count;i++){
        if(strcmp(p->attrs[i].key,key)==0){
            p->attrs[i].value=(void*)value;
            return 0;
        }
    }

    fossil_game_player_attr* tmp =
        realloc(p->attrs,(p->attr_count+1)*sizeof(*tmp));
    if(!tmp) return -2;

    p->attrs=tmp;
    p->attrs[p->attr_count].key=strdup(key);
    p->attrs[p->attr_count].value=(void*)value;
    p->attr_count++;
    return 0;
}

int fossil_game_player_get_attribute(const char* player_id,const char* key,void* out_value)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!key||!out_value) return -1;

    for(size_t i=0;i<p->attr_count;i++){
        if(strcmp(p->attrs[i].key,key)==0){
            *(void**)out_value=p->attrs[i].value;
            return 0;
        }
    }
    return -2;
}

/* ============================================================
   Inventory
   ============================================================ */

int fossil_game_player_inventory_add(const char* player_id,const char* item_id,int count)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!item_id||count<=0) return -1;

    for(size_t i=0;i<p->inventory_count;i++){
        if(strcmp(p->inventory[i].item_id,item_id)==0){
            p->inventory[i].count+=count;
            return 0;
        }
    }

    fossil_game_player_item* tmp =
        realloc(p->inventory,(p->inventory_count+1)*sizeof(*tmp));
    if(!tmp) return -2;

    p->inventory=tmp;
    p->inventory[p->inventory_count].item_id=strdup(item_id);
    p->inventory[p->inventory_count].count=count;
    p->inventory_count++;
    return 0;
}

int fossil_game_player_inventory_remove(const char* player_id,const char* item_id,int count)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!item_id||count<=0) return -1;

    for(size_t i=0;i<p->inventory_count;i++){
        if(strcmp(p->inventory[i].item_id,item_id)==0){
            if(p->inventory[i].count<count) return -2;
            p->inventory[i].count-=count;
            return 0;
        }
    }
    return -3;
}

int fossil_game_player_inventory_list(const char* player_id,const char*** out_items,int* out_count)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!out_items||!out_count) return -1;

    const char** arr = malloc(sizeof(char*)*p->inventory_count);
    if(!arr) return -2;

    for(size_t i=0;i<p->inventory_count;i++)
        arr[i]=p->inventory[i].item_id;

    *out_items=arr;
    *out_count=(int)p->inventory_count;
    return 0;
}

/* ============================================================
   Controls
   ============================================================ */

static int set_control(const char* player_id,const char* name,int enabled)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!name) return -1;

    for(size_t i=0;i<p->control_count;i++){
        if(strcmp(p->controls[i].name,name)==0){
            p->controls[i].enabled=enabled;
            return 0;
        }
    }

    fossil_game_player_control* tmp =
        realloc(p->controls,(p->control_count+1)*sizeof(*tmp));
    if(!tmp) return -2;

    p->controls=tmp;
    p->controls[p->control_count].name=strdup(name);
    p->controls[p->control_count].enabled=enabled;
    p->control_count++;
    return 0;
}

int fossil_game_player_enable_control(const char* player_id,const char* control){
    return set_control(player_id,control,1);
}
int fossil_game_player_disable_control(const char* player_id,const char* control){
    return set_control(player_id,control,0);
}

/* ============================================================
   NPC / AI
   ============================================================ */

int fossil_game_player_npc_update(const char* npc_id)
{
    /* Stub for future AI logic */
    fossil_game_player* p=find_player(npc_id);
    if(!p) return -1;
    return 0;
}

/* ============================================================
   Multiplayer
   ============================================================ */

int fossil_game_player_join_session(const char* player_id,const char* session_id)
{
    fossil_game_player* p=find_player(player_id);
    if(!p||!session_id) return -1;

    free(p->session_id);
    p->session_id=strdup(session_id);
    return p->session_id?0:-2;
}

int fossil_game_player_leave_session(const char* player_id)
{
    fossil_game_player* p=find_player(player_id);
    if(!p) return -1;

    free(p->session_id);
    p->session_id=NULL;
    return 0;
}
