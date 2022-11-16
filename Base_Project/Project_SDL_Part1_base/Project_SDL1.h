// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1400; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel

//taille loup
constexpr unsigned loup_h = 30;
constexpr unsigned loup_w = 60;

//taille mouton
constexpr unsigned mouton_h = 60;
constexpr unsigned mouton_w = 60;

// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;


//charger img
const std::string mouton_img = "../media/sheep.png";
const std::string loup_img = "../media/wolf.png";

// Helper function to initialize SDL
void init();

class animal {
private:
  SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
  SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use
                           // load_surface_for
  // todo: Attribute(s) to define its position

public:
  int pos_x = 0;
  int pos_y = 0;
  SDL_Rect *pos_ptr;
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr);
  // todo: The constructor has to load the sdl_surface that corresponds to the
  // texture
  ~animal(){}; // todo: Use the destructor to release memory and "clean up
               // behind you"

  void draw(){}; // todo: Draw the animal on the screen <-> window_surface_ptr.
                 // Note that this function is not virtual, it does not depend
                 // on the static type of the instance

  virtual void move(){}; // todo: Animals move around, but in a different
                             // fashion depending on which type of animal
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal {
  // todo
  // Ctor
    sheep(SDL_Surface *window_surface_ptr, unsigned rad) : animal(mouton_img, window_surface_ptr){
      pos_ptr->h = mouton_h;
      pos_ptr->w = mouton_w;

      //mouvement
      srand(time(0) + rad);

      do
      {
        pos_y = -1 + rand() % 3;
        pos_x = -1 + rand() % 4;
      } while (0 == pos_y && 0 == pos_x);
    };

  // Dtor
  virtual ~sheep(){};
  // implement functions that are purely virtual in base class
  virtual void move() override;
};

// Insert here:
// class wolf, derived from animal
class wolf : public animal{
  public:
    //Ctor
    wolf(SDL_Surface *window_surface_ptr, unsigned rad) : animal(loup_img, window_surface_ptr){
      pos_ptr->h = loup_h;
      pos_ptr->w = loup_w;
      rad_ = rad;
    };
    //Dtor
    virtual ~wolf(){};
    //move
    virtual void move() override;
    private:
      unsigned rad_;
};
// Use only sheep at first. Once the application works
// for sheep you can add the wolves

// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
  // Attention, NON-OWNING ptr, again to the screen
  SDL_Surface* window_surface_ptr_;

  // Some attribute to store all the wolves and sheep
  // here

public:
  ground(SDL_Surface* window_surface_ptr); // todo: Ctor
  ~ground(){}; // todo: Dtor, again for clean up (if necessary)

  std::vector<animal*> liste_animaux; //liste des animaux
  void add_animal(animal *animal); // todo: Add an animal
  void update(); // todo: "refresh the screen": Move animals and draw them
  // Possibly other methods, depends on your implementation
};

// The application class, which is in charge of generating the window
class application {
private:
  // The following are OWNING ptrs
  SDL_Window* window_ptr_;
  SDL_Surface* window_surface_ptr_;
  SDL_Event window_event_;
  // Other attributes here, for example an instance of ground
  unsigned rad_it = 0;
  ground *g_ptr_;

public:
  application(unsigned n_sheep, unsigned n_wolf); // Ctor
  ~application();                                 // dtor

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};
