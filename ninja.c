#include "game.h"

static entity_t self;
static anim_t anim_ninja_stand_left;
static anim_t anim_ninja_stand_right;
static anim_t anim_ninja_run_left;
static anim_t anim_ninja_run_right;
static anim_t anim_ninja_jump_left;
static anim_t anim_ninja_jump_right;
static anim_t anim_ninja_fall_left;
static anim_t anim_ninja_fall_right;
static anim_t anim_ninja_duck_left;
static anim_t anim_ninja_duck_right;
static float vy = 0;
static float ay = 0.2;
static float vx = 0;
static float ax = 0.1;

static bool on_the_ground(entity_t *self)
{
   return self->y == 240 - 48 - 32;
}

static void update(entity_t *self)
{
   // apply gravity if not on the ground
   if (self->y < 240 - 48 - 32)
   {
      vy += ay;
      self->y += vy;
   }

   // disable gravity if on the ground
   if (self->y >= 240 - 48 - 32)
   {
      vy = 0;
      self->y = 240 - 48 - 32;
   }

   // jumping
   if (ks.b && on_the_ground(self))
   {
      self->y -= 1;
      vy = - 6;
   }

   // moving
   if (ks.left) {
      vx -= ax;
      if (vx < -6)
         vx = -6;
      self->d = false;
   }
   if (ks.right) {
      vx += ax;
      if (vx > 6)
         vx = 6;
      self->d = true;
   }

   // apply speed
   self->x += floor(vx);

   // decelerating
   if (!(ks.right && vx > 0) && !(ks.left && vx < 0) && on_the_ground(self))
   {
      if (vx > 0)
      { 
         vx -= 0.5;
         if (vx < 0)
            vx = 0;
      }
      else if (vx < 0)
      { 
         vx += 0.5; 
         if (vx > 0)
            vx = 0;
      }
   }

   // run animation
   anim_ninja_run_left.p = 9 - abs(vx);
   anim_ninja_run_right.p = 9 - abs(vx);
   if (vx == 0 && on_the_ground(self))
      self->anim = self->d ? &anim_ninja_stand_right : &anim_ninja_stand_left;
   if (vx != 0 && on_the_ground(self))
      self->anim = self->d ? &anim_ninja_run_right : &anim_ninja_run_left;

   // jump animation
   if (vy < 0)
      self->anim = self->d ? &anim_ninja_jump_right : &anim_ninja_jump_left;
   if (vy > 0)
      self->anim = self->d ? &anim_ninja_fall_right : &anim_ninja_fall_left;

   // duck animation
   if (vx == 0 && on_the_ground(self) && ks.down)
      self->anim = self->d ? &anim_ninja_duck_right : &anim_ninja_duck_left;

   // camera
   camera.x = - self->x + SCREEN_WIDTH/2 - self->w/2;
   if (camera.x > 0)
      camera.x = 0;
}

static void draw(entity_t *self)
{
   draw_anim(
      (int)self->x,
      (int)self->y,
      self->anim);
}

entity_t* ninja_new()
{
   anim_ninja_stand_left.surface = surface_new("/usr/share/obake/ninja_stand_left.png");
   anim_ninja_stand_left.t = 0;
   anim_ninja_stand_left.p = 240;
   anim_ninja_stand_left.w = 48;
   anim_ninja_stand_left.h = 48;

   anim_ninja_stand_right.surface = surface_new("/usr/share/obake/ninja_stand_right.png");
   anim_ninja_stand_right.t = 0;
   anim_ninja_stand_right.p = 240;
   anim_ninja_stand_right.w = 48;
   anim_ninja_stand_right.h = 48;

   anim_ninja_run_left.surface = surface_new("/usr/share/obake/ninja_run_left.png");
   anim_ninja_run_left.t = 0;
   anim_ninja_run_left.p = 3;
   anim_ninja_run_left.w = 48;
   anim_ninja_run_left.h = 48;

   anim_ninja_run_right.surface = surface_new("/usr/share/obake/ninja_run_right.png");
   anim_ninja_run_right.t = 0;
   anim_ninja_run_right.p = 3;
   anim_ninja_run_right.w = 48;
   anim_ninja_run_right.h = 48;

   anim_ninja_jump_left.surface = surface_new("/usr/share/obake/ninja_jump_left.png");
   anim_ninja_jump_left.t = 0;
   anim_ninja_jump_left.p = 1;
   anim_ninja_jump_left.w = 48;
   anim_ninja_jump_left.h = 48;

   anim_ninja_jump_right.surface = surface_new("/usr/share/obake/ninja_jump_right.png");
   anim_ninja_jump_right.t = 0;
   anim_ninja_jump_right.p = 1;
   anim_ninja_jump_right.w = 48;
   anim_ninja_jump_right.h = 48;

   anim_ninja_fall_left.surface = surface_new("/usr/share/obake/ninja_fall_left.png");
   anim_ninja_fall_left.t = 0;
   anim_ninja_fall_left.p = 1;
   anim_ninja_fall_left.w = 48;
   anim_ninja_fall_left.h = 48;

   anim_ninja_fall_right.surface = surface_new("/usr/share/obake/ninja_fall_right.png");
   anim_ninja_fall_right.t = 0;
   anim_ninja_fall_right.p = 1;
   anim_ninja_fall_right.w = 48;
   anim_ninja_fall_right.h = 48;

   anim_ninja_duck_left.surface = surface_new("/usr/share/obake/ninja_duck_left.png");
   anim_ninja_duck_left.t = 0;
   anim_ninja_duck_left.p = 1;
   anim_ninja_duck_left.w = 48;
   anim_ninja_duck_left.h = 48;

   anim_ninja_duck_right.surface = surface_new("/usr/share/obake/ninja_duck_right.png");
   anim_ninja_duck_right.t = 0;
   anim_ninja_duck_right.p = 1;
   anim_ninja_duck_right.w = 48;
   anim_ninja_duck_right.h = 48;

   self.w = 48;
   self.h = 48;
   self.x = SCREEN_WIDTH/2  - self.w/2;
   self.y = SCREEN_HEIGHT/2 - self.h/2;
   self.v = 1.0;
   self.d = true;
   self.anim = &anim_ninja_stand_left;
   self.update = &update;
   self.draw = &draw;

   return &self;
}