#include "Window.h"
#include "logging.h"

bool sdlInitialized = false;

Window::Window(const char *title, int width, int height) {
    this->title = title;
    this->width = width;
    this->height = height;

    if (!sdlInitialized) {
        INFO("Initializing SDL")
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);
        sdlInitialized = true;
    }

    INFO("Creating SDL Window")
    this->sdlWindow = SDL_CreateWindow(this->title, 200, 200, this->width, this->height, SDL_WINDOW_SHOWN);

    INFO("Getting Surface from Window")
    this->sdlSurface = SDL_GetWindowSurface(this->sdlWindow);
}

void Window::update() {
    SDL_UpdateWindowSurface(this->sdlWindow);
}

void Window::render(Entity *entity) {
    SDL_Surface* texture = entity->getTile()->getTexture()->getSurface();
    SDL_Rect* textureRect = entity->getTile()->getRekt();
    SDL_Rect* entityRect = entity->getRekt();

    SDL_BlitSurface(texture, textureRect, sdlSurface, entityRect);
}

int Window::getWidth() {
    return this->width;
}

int Window::getHeight() {
    return this->height;
}