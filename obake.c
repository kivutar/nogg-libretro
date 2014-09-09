#include "game.h"

static entity_t self;
static uint32_t *img_obake_left;
static uint32_t *img_obake_right;
static anim_t anim_obake_left;
static anim_t anim_obake_right;

static void update(entity_t *self)
{
   if (ks.up)    { self->y -= self->v; }
   if (ks.down)  { self->y += self->v; }
   if (ks.left)  { self->x -= self->v; self->d = false; }
   if (ks.right) { self->x += self->v; self->d = true; }

   self->t += 0.05;
   self->f = cos(self->t)*3;
   self->anim = self->d ? &anim_obake_right : &anim_obake_left;
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
   unsigned width, height = 0;
   rpng_load_image_argb("/usr/share/obake/obake_left.png", &img_obake_left, &width, &height);
   rpng_load_image_argb("/usr/share/obake/obake_right.png", &img_obake_right, &width, &height);

   anim_obake_left.image = img_obake_left;
   anim_obake_left.t = 0;
   anim_obake_left.p = 1;
   anim_obake_left.w = 48;
   anim_obake_left.h = 48;
   anim_obake_left.tw = 48;
   anim_obake_left.th = 48;

   anim_obake_right.image = img_obake_right;
   anim_obake_right.t = 0;
   anim_obake_right.p = 1;
   anim_obake_right.w = 48;
   anim_obake_right.h = 48;
   anim_obake_right.tw = 48;
   anim_obake_right.th = 48;

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
