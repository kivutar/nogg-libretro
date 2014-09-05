#include "libretro.h"

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

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
   int up;
   int down;
   int left;
   int right;
   int start;
   int select;
} key_state_t;

float frame_time = 0;

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
   info->library_name     = "Test";
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

void fill_rect(int x, int y, int w, int h, uint32_t c)
{
   /*if (x < 0) {
      w += x;
      x = 0;
   }
   if (y < 0) {
      h += y;
      y = 0;
   }
   if (x+w > SCREEN_WIDTH) {
      w -= SCREEN_WIDTH - (x+w);
   }
   if (y+h > SCREEN_HEIGHT) {
      h -= SCREEN_HEIGHT - (y+h);
   }*/

   int i, j;
   for (j = y; j < y+h; j++)
      for (i = x; i < x+w; i++)
         fb[j * (fbpitch >> 1) + i] = c;
}

float x, y;
float v = 1.0;

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

   if (ks.up)    { y -= v; }
   if (ks.down)  { y += v; }
   if (ks.left)  { x -= v; }
   if (ks.right) { x += v; }

   fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff000000);

   fill_rect((int)x, (int)y, 10, 10, 0xffff0000);

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
