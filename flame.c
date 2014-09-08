#include "game.h"

static uint32_t *img_flame;
static entity_t *c;
static int at = 0;

static void update(entity_t *self)
{
   self->t += 0.05;

   at++;
   if (at == 30)
      at = 0;

   self->x = c->x + c->w/2 - self->w/2 + cos(self->t)*30;
   self->y = c->y + c->h/2 - self->w/2 + sin(self->t)*30 + c->f;
}

static void draw(entity_t *self)
{
   blit((int)self->x, (int)self->y + (int)self->f, self->w, self->h, self->tw, self->th, self->image, (int)(at/10)*self->w, 0);
}

entity_t flame_new(entity_t *center)
{
   c = center;

   unsigned width, height = 0;
   rpng_load_image_argb("/usr/share/obake/blueflame.png", &img_flame, &width, &height);

   entity_t self;

   self.w = 16;
   self.h = 16;
   self.tw = 16*3;
   self.th = 16;
   self.image = img_flame;
   self.update = &update;
   self.draw = &draw;

   return self;
}
