#include "game.h"
#include "json.h"

static entity_t self;
static surface_t surf_map;
static json_value data;
static int width, height, tilewidth, tileheight;
surface_t tileset_surfaces[10];

static void update(entity_t *self)
{
}

static void draw(entity_t *self)
{
   int i, j, k;
   for(i = 0; i < data.u.object.length; i++) {

      if (!strcmp(data.u.object.values[i].name, "width"))
         width = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "height"))
         height = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "tilewidth"))
         tilewidth = data.u.object.values[i].value->u.integer;

      if (!strcmp(data.u.object.values[i].name, "tileheight"))
         tileheight = data.u.object.values[i].value->u.integer;


      if (!strcmp(data.u.object.values[i].name, "tilesets")) {
         json_value *tilesets = data.u.object.values[i].value;
         for(j = 0; j < tilesets->u.array.length; j++) {
            json_value *tileset = tilesets->u.array.values[j];

            //char filename[1024] = {0};
            //char *filename = malloc(2048);

            for(k = 0; k < tileset->u.object.length; k++)
               if (!strcmp(tileset->u.object.values[k].name, "image"))
               {
                  //char* imagename = malloc(1024);
                  //imagename = tileset->u.object.values[i].value->u.string.ptr;
                  //sprintf(filename, "%s/%s", "/usr/share/obake", imagename);


                  //filename = strcat("/usr/share/obake/", tileset->u.object.values[i].value->u.string.ptr);

                  //string lala = tileset->u.object.values[i].value->u.string.ptr;

                  /*size_t len1 = strlen("/usr/share/obake/");
                  size_t len2 = strlen(tileset->u.object.values[i].value->u.string.ptr);
                  char *filename = (char*) malloc(len1 + len2 + 1);

                  memcpy(filename, "/usr/share/obake/", len1);
                  memcpy(filename+len1, tileset->u.object.values[i].value->u.string.ptr, len2+1);*/

                  /*if (j == 0)
                     tileset_surfaces[j] = surface_new("/usr/share/obake/forestground.png");
                  if (j == 1)
                     tileset_surfaces[j] = surface_new("/usr/share/obake/blueflame.png");*/
               }


                  //imagename = tileset->u.object.values[i].value->u.string.ptr;


         }
      }

      if (!strcmp(data.u.object.values[i].name, "layers")) {
         json_value *layers = data.u.object.values[i].value;
         for(j = 0; j < layers->u.array.length; j++) {
            json_value *layer = layers->u.array.values[j];
            for(k = 0; k < layer->u.object.length; k++) {
               if (!strcmp(layer->u.object.values[k].name, "data")) {
                  json_value *layerdata = layer->u.object.values[k].value;

                  int x, y;
                  for (y = 0; y < height; y++) {
                     for (x = 0; x < width; x++) {
                        int id = layerdata->u.array.values[y*width+x]->u.integer;
                        if (id)
                        {
                           /*int tileset_id = 0;

                           int tileset_numtiles = (tileset_surfaces[1].w/tilewidth)
                              *(tileset_surfaces[1].h/tileheight) + (tileset_surfaces[0].w/tilewidth)
                              *(tileset_surfaces[0].h/tileheight);

                           if (id > tileset_numtiles)
                           {
                              tileset_id =2;
                              id -= tileset_numtiles;
                           }

                           tileset_numtiles = (tileset_surfaces[0].w/tilewidth)
                              *(tileset_surfaces[0].h/tileheight);

                           if (id > tileset_numtiles)
                           {
                              tileset_id = 1;
                              id -= tileset_numtiles;
                           }*/

                           int tileset_id = 0;
                           int tileset_numtiles = 0;

                           tileset_numtiles = (tileset_surfaces[1].w/tilewidth)
                              *(tileset_surfaces[1].h/tileheight) + (tileset_surfaces[0].w/tilewidth)
                              *(tileset_surfaces[0].h/tileheight);

                           if (id > tileset_numtiles)
                           {
                              tileset_id = 2;
                              id -= tileset_numtiles;
                           }

                           tileset_numtiles = (tileset_surfaces[0].w/tilewidth)
                              *(tileset_surfaces[0].h/tileheight);

                           if (id > tileset_numtiles)
                           {
                              tileset_id = 1;
                              id -= tileset_numtiles;
                           }



                           draw_tile(
                              x*tilewidth, 
                              y*tileheight, 
                              tilewidth, 
                              tileheight, 
                              tileset_surfaces[tileset_id].w, 
                              tileset_surfaces[tileset_id].h, 
                              tileset_surfaces[tileset_id].image, 
                              id);
                        }
                     }
                  }

               }
            }
         }
      }

   }
}

entity_t* map_new()
{
   tileset_surfaces[0] = surface_new("/usr/share/obake/forestground.png");
   tileset_surfaces[1] = surface_new("/usr/share/obake/blueflame.png");
   tileset_surfaces[2] = surface_new("/usr/share/obake/forestpagode.png");

   char jsonstring[4096*2];
   FILE *fp = fopen("/usr/share/obake/test.json", "rb");
   if (fp)
   {
      fread(jsonstring, sizeof(char), sizeof(jsonstring)-1, fp);
      fclose(fp);
   }

   data = * json_parse(jsonstring, strlen(jsonstring));

   self.update = &update;
   self.draw = &draw;
   return &self;
}
