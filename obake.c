#include "game.h"

static uint32_t *img_obake_left;
static uint32_t *img_obake_right;

static void update(entity_t *self)
{
   if (ks.up)    { self->y -= self->v; }
   if (ks.down)  { self->y += self->v; }
   if (ks.left)  { self->x -= self->v; self->d = false; }
   if (ks.right) { self->x += self->v; self->d = true; }

   self->t += 0.05;
   self->f = cos(self->t)*3;
   self->image = self->d ? img_obake_right : img_obake_left;
}

static void draw(entity_t *self)
{
   draw_image((int)self->x, (int)self->y + (int)self->f, self->w, self->h, self->image);
}

entity_t obake_new()
{
   unsigned width, height = 0;
   rpng_load_image_argb("/usr/share/obake/obake_left.png", &img_obake_left, &width, &height);
   rpng_load_image_argb("/usr/share/obake/obake_right.png", &img_obake_right, &width, &height);

   entity_t self;

   self.w = 48;
   self.h = 48;
   self.x = SCREEN_WIDTH/2  - self.w/2;
   self.y = SCREEN_HEIGHT/2 - self.h/2;
   self.v = 1.0;
   self.image = img_obake_left;
   self.update = &update;
   self.draw = &draw;

   return self;
}
