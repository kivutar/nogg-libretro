#include "game.h"

static entity_t *c;
static anim_t anim_flame;

static void update(entity_t *self)
{
   self->t += 0.05;

   self->x = c->x + c->w/2 - self->w/2 + cos(self->t)*30;
   self->y = c->y + c->h/2 - self->w/2 + sin(self->t)*30 + c->f;
}

static void draw(entity_t *self)
{
   draw_anim(
      self->x,
      self->y + self->f,
      self->anim);
}

entity_t* flame_new(entity_t *center)
{
   c = center;

   anim_flame.surface = surface_new("/usr/share/obake/blueflame.png");
   anim_flame.t = 0;
   anim_flame.p = 10;
   anim_flame.w = 16;
   anim_flame.h = 16;

   entity_t *self = NULL;
   self = (entity_t*)realloc(self, sizeof(entity_t));

   self->x = 0;
   self->y = 0;
   self->w = 16;
   self->h = 16;
   self->anim = &anim_flame;
   self->update = &update;
   self->draw = &draw;
   self->on_collide = NULL;

   num_entities++;
   entities = (entity_t**)realloc(entities, num_entities * sizeof(entity_t));
   entities[num_entities-1] = self;

   return self;
}
