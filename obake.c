#include "game.h"

static entity_t self;
static anim_t anim_obake_left;
static anim_t anim_obake_right;

static void update(entity_t *self)
{
   /*if (ks.up)    { self->y -= self->v; }
   if (ks.down)  { self->y += self->v; }
   if (ks.left)  { self->x -= self->v; self->d = false; }
   if (ks.right) { self->x += self->v; self->d = true; }*/

   self->t += 0.05;
   self->f = cos(self->t)*3;
   self->anim = self->d ? &anim_obake_right : &anim_obake_left;

   camera.x = - self->x + SCREEN_WIDTH/2 - self->w/2;
   if (camera.x > 0)
      camera.x = 0;
}

static void draw(entity_t *self)
{
   draw_anim(
      (int)self->x,
      (int)self->y + (int)self->f,
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

   self.w = 48;
   self.h = 48;
   self.x = SCREEN_WIDTH/2  - self.w/2;
   self.y = SCREEN_HEIGHT/2 - self.h/2;
   self.v = 1.0;
   self.anim = &anim_obake_left;
   self.update = &update;
   self.draw = &draw;

   add_entity(flame_new(&self));

   return &self;
}
