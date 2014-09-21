#include "game.h"

static anim_t anim_pl_stand_left;
static anim_t anim_pl_stand_right;
static anim_t anim_pl_run_left;
static anim_t anim_pl_run_right;
static anim_t anim_pl_walk_left;
static anim_t anim_pl_walk_right;
static anim_t anim_pl_jump_left;
static anim_t anim_pl_jump_right;
static anim_t anim_pl_fall_left;
static anim_t anim_pl_fall_right;
static anim_t anim_pl_duck_left;
static anim_t anim_pl_duck_right;
static anim_t anim_pl_slip_left;
static anim_t anim_pl_slip_right;
static float ay = 0.2;
static float ax = 0.1;
static float walk_max_v = 3;
static float run_max_v = 6;

static bool on_the_ground(entity_t *self)
{
   return solid_at(self->x+8, self->y+33);
}

static void update(entity_t *self)
{
   // apply gravity if not on the ground
   if (!on_the_ground(self))
   {
      self->vy += ay;
      self->y += self->vy;
   }

   // jumping
   if (abs(self->vx) > 2 && ! self->ks->down && self->ks->b && on_the_ground(self))
   {
      self->y -= 1;
      self->vy = -4;
   }

   // moving
   if (!(self->ks->down && self->ks->b) && self->ks->left) {
      self->vx -= ax;
      if (self->vx < -run_max_v)
         self->vx = -run_max_v;
      self->d = false;
   }
   if (!(self->ks->down && self->ks->b) && self->ks->right) {
      self->vx += ax;
      if (self->vx > run_max_v)
         self->vx = run_max_v;
      self->d = true;
   }

   // apply speed
   self->x += self->vx;

   // decelerating
   if (!(self->ks->down && self->ks->b) && !(self->ks->right && self->vx > 0) && !(self->ks->left && self->vx < 0) && on_the_ground(self))
   {
      if (self->vx > 0)
      { 
         self->vx -= 0.5;
         if (self->vx < 0)
            self->vx = 0;
      }
      else if (self->vx < 0)
      { 
         self->vx += 0.5; 
         if (self->vx > 0)
            self->vx = 0;
      }
   }
   // decelerating when slipping
   if ((self->ks->down && self->ks->b) && !(self->ks->right && self->vx > 0) && !(self->ks->left && self->vx < 0) && on_the_ground(self))
   {
      if (self->vx > 0)
      { 
         self->vx -= 0.1;
         if (self->vx < 0)
            self->vx = 0;
      }
      else if (self->vx < 0)
      { 
         self->vx += 0.1; 
         if (self->vx > 0)
            self->vx = 0;
      }
   }

   // run animation
   anim_pl_run_left.p = 9 - abs(self->vx);
   anim_pl_run_right.p = 9 - abs(self->vx);
   if (self->vx == 0 && on_the_ground(self))
      self->anim = self->d ? &anim_pl_stand_right : &anim_pl_stand_left;
   if (self->vx != 0 && on_the_ground(self))
      self->anim = self->d ? &anim_pl_run_right : &anim_pl_run_left;

   // jump animation
   if (!on_the_ground(self) && self->vy < 0)
      self->anim = self->d ? &anim_pl_jump_right : &anim_pl_jump_left;
   if (!on_the_ground(self) && self->vy > 0)
      self->anim = self->d ? &anim_pl_fall_right : &anim_pl_fall_left;

   // duck animation
   if (self->vx == 0 && on_the_ground(self) && self->ks->down && self->ks->b)
      self->anim = self->d ? &anim_pl_duck_right : &anim_pl_duck_left;

   // duck animation
   if (self->vx != 0 && on_the_ground(self) && self->ks->down && self->ks->b)
      self->anim = self->d ? &anim_pl_slip_right : &anim_pl_slip_left;
}

static void draw(entity_t *self)
{
   draw_anim_colored(
      self->x - 16,
      self->y - 16,
      self->anim,
      self->c);
}

static void on_collide(entity_t *self, entity_t *other, int dx, int dy)
{
   if (abs(dy) < abs(dx) && dy < 0)
   {
      self->vy = 0;
      self->y += dy;
   }

   if (abs(dx) < abs(dy) && dx < 0)
   {
      self->vx = 0;
      self->x += dx;
   }

   if (abs(dx) < abs(dy) && dx > 0)
   {
      self->vx = 0;
      self->x += dx;
   }

   if (abs(dx) < abs(dy) && dx < 0)
   {
      self->vx = 0;
      self->x += dx;
   }
}

entity_t* pl_new(key_state_t* ks, uint32_t c)
{
   anim_pl_stand_left.surface = surface_new("/usr/share/nogg/pl_stand_left.png");
   anim_pl_stand_left.t = 0;
   anim_pl_stand_left.p = 240;
   anim_pl_stand_left.w = 48;
   anim_pl_stand_left.h = 48;

   anim_pl_stand_right.surface = surface_new("/usr/share/nogg/pl_stand_right.png");
   anim_pl_stand_right.t = 0;
   anim_pl_stand_right.p = 240;
   anim_pl_stand_right.w = 48;
   anim_pl_stand_right.h = 48;

   anim_pl_run_left.surface = surface_new("/usr/share/nogg/pl_run_left.png");
   anim_pl_run_left.t = 0;
   anim_pl_run_left.p = 3;
   anim_pl_run_left.w = 48;
   anim_pl_run_left.h = 48;

   anim_pl_run_right.surface = surface_new("/usr/share/nogg/pl_run_right.png");
   anim_pl_run_right.t = 0;
   anim_pl_run_right.p = 3;
   anim_pl_run_right.w = 48;
   anim_pl_run_right.h = 48;

   anim_pl_walk_left.surface = surface_new("/usr/share/nogg/pl_walk_left.png");
   anim_pl_walk_left.t = 0;
   anim_pl_walk_left.p = 9;
   anim_pl_walk_left.w = 48;
   anim_pl_walk_left.h = 48;

   anim_pl_walk_right.surface = surface_new("/usr/share/nogg/pl_walk_right.png");
   anim_pl_walk_right.t = 0;
   anim_pl_walk_right.p = 9;
   anim_pl_walk_right.w = 48;
   anim_pl_walk_right.h = 48;

   anim_pl_jump_left.surface = surface_new("/usr/share/nogg/pl_jump_left.png");
   anim_pl_jump_left.t = 0;
   anim_pl_jump_left.p = 1;
   anim_pl_jump_left.w = 48;
   anim_pl_jump_left.h = 48;

   anim_pl_jump_right.surface = surface_new("/usr/share/nogg/pl_jump_right.png");
   anim_pl_jump_right.t = 0;
   anim_pl_jump_right.p = 1;
   anim_pl_jump_right.w = 48;
   anim_pl_jump_right.h = 48;

   anim_pl_fall_left.surface = surface_new("/usr/share/nogg/pl_fall_left.png");
   anim_pl_fall_left.t = 0;
   anim_pl_fall_left.p = 1;
   anim_pl_fall_left.w = 48;
   anim_pl_fall_left.h = 48;

   anim_pl_fall_right.surface = surface_new("/usr/share/nogg/pl_fall_right.png");
   anim_pl_fall_right.t = 0;
   anim_pl_fall_right.p = 1;
   anim_pl_fall_right.w = 48;
   anim_pl_fall_right.h = 48;

   anim_pl_duck_left.surface = surface_new("/usr/share/nogg/pl_duck_left.png");
   anim_pl_duck_left.t = 0;
   anim_pl_duck_left.p = 1;
   anim_pl_duck_left.w = 48;
   anim_pl_duck_left.h = 48;

   anim_pl_duck_right.surface = surface_new("/usr/share/nogg/pl_duck_right.png");
   anim_pl_duck_right.t = 0;
   anim_pl_duck_right.p = 1;
   anim_pl_duck_right.w = 48;
   anim_pl_duck_right.h = 48;

   anim_pl_slip_left.surface = surface_new("/usr/share/nogg/pl_slip_left.png");
   anim_pl_slip_left.t = 0;
   anim_pl_slip_left.p = 1;
   anim_pl_slip_left.w = 48;
   anim_pl_slip_left.h = 48;

   anim_pl_slip_right.surface = surface_new("/usr/share/nogg/pl_slip_right.png");
   anim_pl_slip_right.t = 0;
   anim_pl_slip_right.p = 1;
   anim_pl_slip_right.w = 48;
   anim_pl_slip_right.h = 48;

   entity_t *self = NULL;
   self = (entity_t*)realloc(self, sizeof(entity_t));

   self->type = 0;

   self->w = 16;
   self->h = 32;
   self->x = SCREEN_WIDTH/2  - self->w/2;
   self->y = SCREEN_HEIGHT/2 - self->h/2;
   self->vx = 0;
   self->vy = 0;
   self->d = true;
   self->anim = &anim_pl_stand_left;
   self->update = &update;
   self->draw = &draw;
   self->on_collide = &on_collide;

   self->ks = ks;
   self->c = c;

   num_entities++;
   entities = (entity_t**)realloc(entities, num_entities * sizeof(entity_t));
   entities[num_entities-1] = self;

   return self;
}
