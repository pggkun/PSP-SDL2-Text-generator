
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text_generator.h"

PSP_MODULE_INFO("SDL TTF Test", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

void init_joystick()
{
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void update_joystick(SceCtrlData &ctrlData)
{
    sceCtrlReadBufferPositive(&ctrlData, 1);
}

int main()
{
    SDL_Joystick *joystick = NULL;
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_Window *window = SDL_CreateWindow("SDL TTF Test", 0, 0, 480, 272, NULL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, NULL);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    joystick = SDL_JoystickOpen(0);

    SDL_Texture *texture1;
    SDL_Rect rect1;

    text_generator *tex_gen = new text_generator("cambriai.ttf", 24, *renderer);

    bool done = false;

    init_joystick();
    SceCtrlData ctrlData;
    while (!done)
    {
        update_joystick(ctrlData);
        if(ctrlData.Buttons & PSP_CTRL_HOME) break;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        tex_gen->gen_text_at_rect(0, 0, "SDL2 + SDL TTF EXAMPLE", &texture1, &rect1);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyTexture(texture1);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
