#include "game.h"

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
   unsigned width = 0, height = 0;
   rpng_load_image_argb("/usr/share/obake/blueflame.png", &img_blueflame, &width, &height);
   rpng_load_image_argb("/usr/share/obake/forestground.png", &img_forestground, &width, &height);

   obake_new();

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
}

void render_game()
{
   obake.update(&obake);

   draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff000000);

   char jsonstring[4096*2];
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
                        int id = layerdata->u.array.values[y*20+x]->u.integer;
                        if (id)
                           draw_tile(x*16, y*16, 16, 16, 16*4, 16*2, img_forestground, id);
                     }
                  }

               }
            }
         }
      }
   }

   obake.draw(&obake);

   if (flame1.t == 30)
      flame1.t = 0;

   flame1.x = obake.x + obake.w/2 - flame1.w/2 + cos(obake.t)*30;
   flame1.y = obake.y + obake.h/2 - flame1.w/2 + sin(obake.t)*30 + obake.f;
   blit((int)flame1.x, (int)flame1.y + (int)flame1.f, flame1.w, flame1.h, flame1.tw, flame1.th, flame1.image, (int)(flame1.t++/10)*flame1.w, 16);

   flame2.x = obake.x + obake.w/2 - flame2.w/2 + cos(obake.t+1)*30;
   flame2.y = obake.y + obake.h/2 - flame2.w/2 + sin(obake.t+1)*30 + obake.f;
   blit((int)flame2.x, (int)flame2.y + (int)flame2.f, flame2.w, flame2.h, flame2.tw, flame2.th, flame2.image, (int)(flame1.t++/10)*flame1.w, 0);

   flame3.x = obake.x + obake.w/2 - flame3.w/2 + cos(obake.t+2)*30;
   flame3.y = obake.y + obake.h/2 - flame3.w/2 + sin(obake.t+2)*30 + obake.f;
   blit((int)flame3.x, (int)flame3.y + (int)flame3.f, flame3.w, flame3.h, flame3.tw, flame3.th, flame3.image, (int)(flame1.t++/10)*flame1.w, 0);

   video_cb(fb, SCREEN_WIDTH, SCREEN_HEIGHT, fbpitch*2);
}