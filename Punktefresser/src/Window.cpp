#include "stdafx.h"

bool sdlInitialized = false;

Window::Window(const char *title, int width, int height) {
    this->title = title;
    this->width = width;
    this->height = height;

    if (!sdlInitialized) {
        INFO("Initializing SDL")
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);
        TTF_Init();
        sdlInitialized = true;
    }

    INFO("Creating SDL Window")
    this->sdlWindow = SDL_CreateWindow(this->title, 200, 200, this->width, this->height, SDL_WINDOW_SHOWN);

    INFO("Getting Surface from Window")
    this->sdlSurface = SDL_GetWindowSurface(this->sdlWindow);
}

Window::~Window() {
    SDL_DestroyWindow(sdlWindow);
}

void Window::update() {
    SDL_UpdateWindowSurface(this->sdlWindow);
}

void Window::renderText(std::string text) {

    TTF_Font* font = TTF_OpenFont("arial.ttf", 20);
    if(!font) {
        std::stringstream ss;
        ss << "Konnte Schriftart nicht laden! Fehler: " << TTF_GetError() << std::endl;
        ERROR(ss)
        return;
    }

    SDL_Color color = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(font, text.c_str(), color);

    SDL_Rect message_rect = {0, 0, 200, 200};
    SDL_BlitSurface(surfaceMessage, &message_rect, sdlSurface, &message_rect);

    TTF_CloseFont(font);
}

int Window::getWidth() {
    return this->width;
}

int Window::getHeight() {
    return this->height;
}

void Window::renderEntity(entity_ptr entity) {
    surface_ptr texture = entity->getTile()->getTexture()->getSurface();
    rect_ptr textureRect = entity->getTile()->getRekt();
    rect_ptr entityRect = entity->getRekt();

    SDL_BlitSurface(texture.get(), textureRect.get(), sdlSurface, entityRect.get());
}
