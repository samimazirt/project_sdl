// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

namespace {
// Defining a namespace without a name -> Anonymous workspace
// Its purpose is to indicate to the compiler that everything
// inside of it is UNIQUELY used within this source file.

SDL_Surface* load_surface_for(const std::string& path,
                              SDL_Surface* window_surface_ptr) {

  // Helper function to load a png for a specific surface
  // See SDL_ConvertSurface
  SDL_Surface* img_surface = IMG_Load(path.c_str());

    // Convert created surface to windows surface format.
    SDL_Surface* converted_surface = SDL_ConvertSurface(img_surface, window_surface_ptr->format, 0);
    // Free temporary image surface.
    SDL_FreeSurface(img_surface);
    return converted_surface;
}
} // namespace

application::application(unsigned int n_sheep, unsigned int n_wolf) {
    // Create window surface.
    window_ptr_ = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   frame_width, frame_height, 0);
    window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);

    // todo: Use correct color.
    SDL_FillRect(window_surface_ptr_, NULL, SDL_MapRGB(window_surface_ptr_->format, 255, 255, 255));

    // Create ground with animals.
    g_ptr_ = new ground(window_surface_ptr_);
    unsigned int g = 0;
    while(g < n_sheep){
        g_ptr_->add_animal(new sheep(window_surface_ptr_, rad_it));
        rad_it += 50;
        g++;
    }
    g = 0;
    while(g < n_wolf){
        g_ptr_->add_animal(new wolf(window_surface_ptr_, rad_it));
        rad_it += 50;
        g++;
    }
}

//destructeur application - fermer et detruire la fenetre
application::~application() {
    SDL_FreeSurface(window_surface_ptr_);
    SDL_DestroyWindow(window_ptr_);
}

int application::loop(unsigned period) {
    while (period > 0) {
    
        for (animal *i : g_ptr_->liste_animaux)
        {
            i->move();
        }
        SDL_UpdateWindowSurface(window_ptr_);
        
        SDL_Delay(frame_time * 1000);
        SDL_FillRect(window_surface_ptr_, NULL, SDL_MapRGB(window_surface_ptr_->format, 255, 255, 255));
        period = period - 1;
    }

    return 0;
}

ground::ground(SDL_Surface* window_surface_ptr) {
    window_surface_ptr_ = window_surface_ptr;
}


void ground::add_animal(animal *animal) {
    liste_animaux.push_back(animal);
}






