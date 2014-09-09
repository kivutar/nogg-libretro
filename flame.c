#include "game.h"

static entity_t self;
static entity_t *c;
static uint32_t *img_flame;
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
      (int)self->x,
      (int)self->y + (int)self->f,
      self->anim);
}

entity_t* flame_new(entity_t *center)
{
   c = center;

   unsigned width, height = 0;
   rpng_load_image_argb("/usr/share/obake/blueflame.png", &img_flame, &width, &height);

   anim_flame.image = img_flame;
   anim_flame.t = 0;
   anim_flame.p = 10;
   anim_flame.w = 16;
   anim_flame.h = 16;
   anim_flame.tw = 48;
   anim_flame.th = 16;

   self.w = 16;
   self.h = 16;
   self.anim = &anim_flame;
   self.update = &update;
   self.draw = &draw;

   return &self;
}
