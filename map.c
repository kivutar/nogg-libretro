#include "game.h"
#include "json.h"

static uint32_t *img_map;
static json_value data;

static void update(entity_t *self)
{
}

static void draw(entity_t *self)
{
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
                           draw_tile(x*16, y*16, 16, 16, 16*4, 16*2, img_map, id);
                     }
                  }

               }
            }
         }
      }
   }
}

entity_t map_new()
{
   unsigned width, height = 0;
   rpng_load_image_argb("/usr/share/obake/forestground.png", &img_map, &width, &height);

   char jsonstring[4096*2];
   FILE *fp = fopen("/usr/share/obake/test.json", "rb");
   if (fp)
   {
      fread(jsonstring, sizeof(char), sizeof(jsonstring)-1, fp);
      fclose(fp);
   }

   data = * json_parse(jsonstring, strlen(jsonstring));

   entity_t self;
   self.update = &update;
   self.draw = &draw;
   return self;
}
