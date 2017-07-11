#ifndef HALLOWELT_WINDOW_H
#define HALLOWELT_WINDOW_H

#include <stdafx.h>
#include <Entity.h>

// Wrapps a SDL_Window and gives it the ability to renderEntity Entities on its surface.
class Window {
public:
    // creates a new Window with a title and a width and a height.
    Window(const char *title, int width, int height);

    // renders a entity on the surface of the window.
    template <typename T>
    void renderEntity(T entity) {
        SDL_Surface* texture = entity->getTile()->getTexture()->getSurface();
        SDL_Rect_ptr textureRect = entity->getTile()->getRekt();
        SDL_Rect_ptr entityRect = entity->getRekt();

        SDL_BlitSurface(texture, textureRect.get(), sdlSurface, entityRect.get());
    }

    //renders text on the surface of the windows
    void renderText(std::string);

    // updates the window to display the rendered surface.
    void update();

    // gets the width of the window.
    int getWidth();

    // gets the height of the window.
    int getHeight();

private:
    const char* title;
    int width;
    int height;

    SDL_Window  *sdlWindow;
    SDL_Surface *sdlSurface;
};


#endif
