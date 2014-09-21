#include "game.h"

void detect_collisions()
{
   int i, j;
   for(i = 0; i < num_entities; i++)
   {
      for(j = 0; j < num_entities; j++)
      {
         if (j != i)
         {
            entity_t *e1 = (entity_t*)entities[i];
            entity_t *e2 = (entity_t*)entities[j];

            if (e1->type != e2->type &&
                e1->x < e2->x + e2->w &&
                e1->x + e1->w > e2->x &&
                e1->y < e2->y + e2->h &&
                e1->h + e1->y > e2->y &&
                e1->on_collide != NULL)
            {
               int e1cx = e1->x + e1->w / 2;
               int e2cx = e2->x + e2->w / 2;
               int dx = e1cx < e2cx ? e2->x - (e1->x + e1->w) : (e2->x + e2->w) - e1->x;

               int e1cy = e1->y + e1->h / 2;
               int e2cy = e2->y + e2->h / 2;
               int dy = e1cy < e2cy ? e2->y - (e1->y + e1->h) : (e2->y + e2->h) - e1->y;

               e1->on_collide(e1, e2, dx, dy);
            }
         }
      }
   }
}

bool solid_at(int x, int y)
{
   int i;
   for(i = 0; i < num_entities; i++)
   {
      entity_t *e1 = (entity_t*)entities[i];

      if (x >= e1->x && x < e1->x + e1->w
       && y >= e1->y && y < e1->y + e1->h
       && e1->type == 1)
      {
         return true;
      }
   }

   return false;
}
