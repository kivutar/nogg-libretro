#include "game.h"

static void update(entity_t *self)
{
}

static void draw(entity_t *self)
{
   //draw_rect((int)self->x+camera.x, (int)self->y, self->w, self->h, 0xffff0000);
}

entity_t* ground_new(int x, int y, int w, int h)
{
   entity_t *self = NULL;
   self = (entity_t*)realloc(self, sizeof(entity_t));

   self->x = x;
   self->y = y;
   self->w = w;
   self->h = h;

   self->anim = NULL;
   self->update = &update;
   self->draw = &draw;
   self->on_collide = NULL;

   num_entities++;
   entities = (entity_t**)realloc(entities, num_entities * sizeof(entity_t));
   entities[num_entities-1] = self;

   return self;
}