#include "libretro.h"

#include "rpng.h"
#include "json.h"

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdbool.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

static uint32_t fb[SCREEN_WIDTH * SCREEN_HEIGHT * 2];
size_t fbpitch = SCREEN_WIDTH * sizeof(uint32_t);

retro_log_printf_t log_cb;
retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

typedef struct
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

float frame_time = 0;

uint32_t *img_obake_left = NULL;
uint32_t *img_obake_right = NULL;
uint32_t *img_blueflame = NULL;
uint32_t *img_forestground = NULL;
uint32_t *anim = NULL;

entity_t obake;
entity_t flame1;
entity_t flame2;
entity_t flame3;

void retro_init(void)
{
}

void retro_deinit(void)
{
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   (void)port;
   (void)device;
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "Obake";
   info->library_version  = "v1.0";
   info->need_fullpath    = true;
   info->valid_extensions = NULL;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->timing.fps = 60.0;
   info->timing.sample_rate = 48000.0;

   info->geometry.base_width   = SCREEN_WIDTH;
   info->geometry.base_height  = SCREEN_HEIGHT;
   info->geometry.max_width    = SCREEN_WIDTH;
   info->geometry.max_height   = SCREEN_HEIGHT;
   info->geometry.aspect_ratio = 4.0/3.0;
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   int no_rom = true;
   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);

   struct retro_log_callback logging;
   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_reset(void)
{
}

static void frame_time_cb(retro_usec_t usec)
{
   frame_time = usec / 1000000.0;
}

void draw_rect(int x, int y, int w, int h, uint32_t c)
{
   int i, j;
   for (j = y; j < y+h; j++)
      for (i = x; i < x+w; i++)
         fb[j * (fbpitch >> 1) + i] = c;
}

void draw_anim(int x, int y, int w, int h, int tw, int th, uint32_t *data, int id)
{
   int i, j, ii, jj = 0;
   int imgpitch = tw * sizeof(uint16_t);
   for (j = y; j < y+h; j++) {
      ii = id * w;
      if (j >= 0 && j < SCREEN_HEIGHT) {
         for (i = x; i < x+w; i++) {
            if (i >= 0 && i < SCREEN_WIDTH) {
               uint32_t c = data[jj * (imgpitch >> 1) + ii];
               if (0xff000000 & c)
                  fb[j * (fbpitch >> 1) + i] = c;
            }
            ii++;
         }
      }
      jj++;
   }
}

void draw_image(int x, int y, int w, int h, uint32_t *data)
{
   draw_anim(x, y, w, h, w, h, data, 0);
}

void draw_tile(int x, int y, int w, int h, uint32_t *data, int id)
{
   int i, j, ii, jj = 0;
   int idy = (id/20)*16;
   int idx = (id%20)*16;
   for (j = y; j < y+h; j++) {
      ii = idy * w;
      if (j >= 0 && j < SCREEN_HEIGHT) {
         for (i = x; i < x+w; i++) {
            int imgpitch = 386 * sizeof(uint16_t);
            uint32_t c = data[jj * (imgpitch >> 1) + ii];
            if (i >= 0 && i < SCREEN_WIDTH) {
               if (0xff000000 & c)
                  fb[j * (fbpitch >> 1) + i] = c;
            }
            ii++;
         }
      }
      jj++;
   }
}

void retro_run(void)
{
   key_state_t ks;

   input_poll_cb();

   ks.up     = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP);
   ks.right  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT);
   ks.down   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN);
   ks.left   = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT);
   ks.start  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START);
   ks.select = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT);

   if (ks.up)    { obake.y -= obake.v; }
   if (ks.down)  { obake.y += obake.v; }
   if (ks.left)  { obake.x -= obake.v; obake.d = false; }
   if (ks.right) { obake.x += obake.v; obake.d = true;  }

   draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff000000);

   obake.t += 0.05;
   obake.f = cos(obake.t)*3;
   obake.image = obake.d ? img_obake_right : img_obake_left;
   draw_image((int)obake.x, (int)obake.y + (int)obake.f, obake.w, obake.h, obake.image);

   if (flame1.t == 30)
      flame1.t = 0;

   flame1.x = obake.x + obake.w/2 - flame1.w/2 + cos(obake.t)*30;
   flame1.y = obake.y + obake.h/2 - flame1.w/2 + sin(obake.t)*30 + obake.f;
   draw_anim((int)flame1.x, (int)flame1.y + (int)flame1.f, flame1.w, flame1.h, flame1.tw, flame1.th, flame1.image, flame1.t++/10);

   flame2.x = obake.x + obake.w/2 - flame2.w/2 + cos(obake.t+1)*30;
   flame2.y = obake.y + obake.h/2 - flame2.w/2 + sin(obake.t+1)*30 + obake.f;
   draw_anim((int)flame2.x, (int)flame2.y + (int)flame2.f, flame2.w, flame2.h, flame2.tw, flame2.th, flame2.image, flame1.t++/10);

   flame3.x = obake.x + obake.w/2 - flame3.w/2 + cos(obake.t+2)*30;
   flame3.y = obake.y + obake.h/2 - flame3.w/2 + sin(obake.t+2)*30 + obake.f;
   draw_anim((int)flame3.x, (int)flame3.y + (int)flame3.f, flame3.w, flame3.h, flame3.tw, flame3.th, flame3.image, flame1.t++/10);

   /*char jsonstring[4096*2];
   FILE *fp = fopen("/usr/share/obake/test.json", "rb");
   if (fp)
   {
      fread(jsonstring, sizeof(char), sizeof(jsonstring)-1, fp);
      fclose(fp);
   }

   json_value data = * json_parse(jsonstring, strlen(jsonstring));

   int i, j, k;
   for(i = 0; i < data.u.object.length; i++) {
      if (!strcmp(data.u.object.values[i].name, "layers")) {
         json_value *layers = data.u.object.values[i].value;
         for(j = 0; j < layers->u.array.length; j++) {
            json_value *layer = layers->u.array.values[0];
            for(k = 0; k < layer->u.object.length; k++) {
               if (!strcmp(layer->u.object.values[k].name, "data")) {
                  json_value *layerdata = layer->u.object.values[k].value;

                  int x, y;
                  for (y = 0; y < 15; y++) {
                     for (x = 0; x < 20; x++) {
                        draw_tile(x*16, y*16, 16, 16, img_forestground, 
                           layerdata->u.array.values[y*20+x]->u.integer);
                     }
                  }

               }
            }
         }
      }
   }*/

   video_cb(fb, SCREEN_WIDTH, SCREEN_HEIGHT, fbpitch*2);
}

bool retro_load_game(const struct retro_game_info *info)
{
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
      return false;

   struct retro_frame_time_callback frame_cb = { frame_time_cb, 1000000 / 60 };
   frame_cb.callback(frame_cb.reference);
   environ_cb(RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK, &frame_cb);

   unsigned width = 0, height = 0;
   rpng_load_image_argb("/usr/share/obake/obake_left.png", &img_obake_left, &width, &height);
   rpng_load_image_argb("/usr/share/obake/obake_right.png", &img_obake_right, &width, &height);
   rpng_load_image_argb("/usr/share/obake/blueflame.png", &img_blueflame, &width, &height);
   rpng_load_image_argb("/usr/share/obake/forestground.png", &img_forestground, &width, &height);

   obake.w = 48;
   obake.h = 48;
   obake.x = SCREEN_WIDTH/2  - obake.w/2;
   obake.y = SCREEN_HEIGHT/2 - obake.h/2;
   obake.v = 1.0;
   obake.image = img_obake_left;

   flame1.w = 16;
   flame1.h = 16;
   flame1.tw = 16*3;
   flame1.th = 16;
   flame1.image = img_blueflame;

   flame2.w = 16;
   flame2.h = 16;
   flame2.tw = 16*3;
   flame2.th = 16;
   flame2.image = img_blueflame;

   flame3.w = 16;
   flame3.h = 16;
   flame3.tw = 16*3;
   flame3.th = 16;
   flame3.image = img_blueflame;

   (void)info;
   return true;
}

void retro_unload_game(void)
{
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   (void)type;
   (void)info;
   (void)num;
   return false;
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return 0;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return 0;
}

void *retro_get_memory_data(unsigned id)
{
   return 0;
}

size_t retro_get_memory_size(unsigned id)
{
   return 0;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}
