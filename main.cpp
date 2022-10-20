
#include <pspkernel.h>
#include <pspdebug.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

PSP_MODULE_INFO("SDL TTF Test", 0, 1, 1);

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
                       TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

int main()
{
    SDL_Window *window = SDL_CreateWindow("SDL TTF Test", 0, 0, 480, 272, NULL);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("cambriai.ttf", 24);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, NULL);

    SDL_Texture *texture1, *texture2;

    SDL_Rect rect1, rect2;

    get_text_and_rect(renderer, 0, 0, "SDL2 + SDL TTF EXAMPLE", font, &texture1, &rect1);
    get_text_and_rect(renderer, 0, rect1.y + rect1.h, "Using Cambria Italic, size:24", font, &texture2, &rect2);

    SDL_Rect my_rect;
    my_rect.x = (480 - 256) / 2;
    my_rect.y = (272 - 256) / 2;
    my_rect.w = 256;
    my_rect.h = 256;

    while(true)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }
    return 0;
}
