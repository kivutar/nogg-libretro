#include "game.h"

entity_t map;
entity_t obake;
entity_t flame1;
entity_t flame2;
entity_t flame3;

void draw_rect(int x, int y, int w, int h, uint32_t c)
{
   int i, j;
   for (j = y; j < y+h; j++)
      for (i = x; i < x+w; i++)
         fb[j * (fbpitch >> 1) + i] = c;
}

void blit(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int orig_x, int orig_y)
{
   int i, j;
   int jj = orig_y;
   int imgpitch = total_w * sizeof(uint16_t);
   for (j = dest_y; j < dest_y+h; j++) {
      int ii = orig_x;
      if (j >= 0 && j < SCREEN_HEIGHT) {
         for (i = dest_x; i < dest_x+w; i++) {
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
   blit(x, y, w, h, w, h, data, 0, 0);
}

void draw_tile(int dest_x, int dest_y, int w, int h, int total_w, int total_h, uint32_t *data, int id)
{
   int orig_x = (id-1)/(total_w/w)*w;
   int orig_y = (id-1)%(total_w/w)*w;
   blit(dest_x, dest_y, w, h, total_w, total_h, data, orig_x, orig_y);
}

void load_game()
{
   map = map_new();

   obake = obake_new();

   flame1 = flame_new(&obake);
   flame1.t = 0;
   flame2 = flame_new(&obake);
   flame2.t = 1;
   flame3 = flame_new(&obake);
   flame3.t = 2;
}

void render_game()
{
   obake.update(&obake);
   flame1.update(&flame1);
   flame2.update(&flame2);
   flame3.update(&flame3);

   draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff000000);

   map.draw(&map);
   obake.draw(&obake);
   flame1.draw(&flame1);
   flame2.draw(&flame2);
   flame3.draw(&flame3);
   
   video_cb(fb, SCREEN_WIDTH, SCREEN_HEIGHT, fbpitch*2);
}