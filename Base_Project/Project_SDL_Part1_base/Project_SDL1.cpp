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
  SDL_Surface* img_surf = IMG_Load(path.c_str());

    // Convert created surface to windows surface format.
    SDL_Surface* final_surf = SDL_ConvertSurface(img_surf, window_surface_ptr->format, 0);
    // Free temporary image surface.
    SDL_FreeSurface(img_surf);
    return final_surf;
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
    while(g < n_sheep)
    {
        g_ptr_->add_animal(new sheep(window_surface_ptr_, rad_it));
        rad_it += 50;
        g++;
    }
    g = 0;
    while(g < n_wolf)
    {
        g_ptr_->add_animal(new wolf(window_surface_ptr_, rad_it));
        rad_it += 50;
        g++;
    }
}


int application::loop(unsigned period) {
    while (period > 0)
    {
    
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

//fonction pour que le mouton reste dans les limites de la fenetre
int reste_limites(int pos, int max) {
    if (frame_boundary > pos)
    {
        return frame_boundary;
    }
    if ((max - frame_boundary) < pos)
    {
        return max - frame_boundary;
    }
    return pos;
}


animal::animal(const std::string &file_path, SDL_Surface* window_surface_ptr) {
    std::cout << file_path + "\n";
    window_surface_ptr_ = window_surface_ptr;
    image_ptr_ = load_surface_for(file_path, window_surface_ptr);
 

    pos_ptr = new SDL_Rect();
    pos_ptr->x = reste_limites((rand() % frame_width) + frame_boundary, frame_width);
    pos_ptr->y = reste_limites(frame_boundary + (rand() % frame_height), frame_height);

    draw();
}





