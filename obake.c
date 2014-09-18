#include "game.h"

static anim_t anim_obake_left;
static anim_t anim_obake_right;

static void update(entity_t *self)
{
   /*self->t += 0.05;
   self->f = cos(self->t)*3;
   self->anim = self->d ? &anim_obake_right : &anim_obake_left;*/
}

static void draw(entity_t *self)
{
   draw_anim(
      self->x,
      self->y + self->f,
      self->anim);
}

entity_t* obake_new()
{
   anim_obake_left.surface = surface_new("/usr/share/obake/obake_left.png");
   anim_obake_left.t = 0;
   anim_obake_left.p = 1;
   anim_obake_left.w = 48;
   anim_obake_left.h = 48;

   anim_obake_right.surface = surface_new("/usr/share/obake/obake_right.png");
   anim_obake_right.t = 0;
   anim_obake_right.p = 1;
   anim_obake_right.w = 48;
   anim_obake_right.h = 48;

   entity_t *self = NULL;
   self = (entity_t*)realloc(self, sizeof(entity_t));

   self->x = SCREEN_WIDTH/2  - self->w/2;
   self->y = SCREEN_HEIGHT/2 - self->h/2;
   self->w = 48;
   self->h = 48;
   self->anim = &anim_obake_left;
   self->update = &update;
   self->draw = &draw;
   self->on_collide = NULL;

   num_entities++;
   entities = (entity_t**)realloc(entities, num_entities * sizeof(entity_t));
   entities[num_entities-1] = self;

   return self;
}
