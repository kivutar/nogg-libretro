#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdbool.h>
#include "libretro.h"
#include "rpng.h"
#include "json.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

retro_log_printf_t log_cb;
retro_video_refresh_t video_cb;
retro_audio_sample_t audio_cb;
retro_audio_sample_batch_t audio_batch_cb;
retro_environment_t environ_cb;
retro_input_poll_t input_poll_cb;
retro_input_state_t input_state_cb;

typedef struct entity_s
{
   int   w;
   int   h;
   int   tw;
   int   th;
   float x;
   float y;
   float v;
   float f;
   float t;
   bool  d;
   uint32_t *image;
   void (*update)(struct entity_s *);
   void (*draw)(struct entity_s *);
} entity_t;

typedef struct
{
   int up;
   int down;
   int left;
   int right;
   int start;
   int select;
} key_state_t;

key_state_t ks;

uint32_t *img_blueflame;
uint32_t *img_forestground;
uint32_t *anim;

void draw_rect(int x, int y, int w, int h, uint32_t c);
void blit(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int orig_x, int orig_y);
void draw_image(int x, int y, int w, int h, uint32_t *data);
void draw_tile(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int id);
void add_entity(entity_t ent);
void render_game();
void load_game();

entity_t map_new();
entity_t obake_new();
entity_t flame_new(entity_t *center);

uint32_t fb[SCREEN_WIDTH * SCREEN_HEIGHT * 2];
size_t fbpitch;

#endif
