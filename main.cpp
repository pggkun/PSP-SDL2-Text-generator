#ifdef __psp__
    #include <pspkernel.h>
    #include <pspdebug.h>
    #include <pspctrl.h>
    #define SDL_MAIN_HANDLED
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "simple_text.h"
#include <iostream>

#ifdef __psp__
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
#endif

void on_finish_draw()
{
    return;
}

const char atlas_characters[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{¦}~⌂ÇçáéíóúãõüâêôñÑÁÉÍÓÚÃÕÜÂÊÔªº¿";

int main()
{
    std::string multiline_text = "Multiline test\nÃÕêíó[]\n\"()\"\n1234567890@#%¨&*~´`";

    SDL_Joystick *joystick = NULL;
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    SDL_Window *window = SDL_CreateWindow("Simple Text Sample", 0, 0, 480, 272, NULL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    joystick = SDL_JoystickOpen(0);

    SDL_Surface *bg = IMG_Load("gfx/background.png");

    SDL_Texture *bg_tex = SDL_CreateTextureFromSurface(renderer, bg);
    SDL_FreeSurface(bg);

    SDL_Rect rect = {16, 40, 480 - 16, 272 - 16};

    FontAtlas atlas("gfx/atlas.png", renderer, atlas_characters);
    CombinedTexture comb1;
    comb1.texture = nullptr;
    comb1.finished = false;
    TypeStats stats(0, 16, 0.0f, 0.003f);
    Uint32 start_time = SDL_GetTicks();
    double delta_time = 0.0;

    bool done = false;

#ifdef __psp__
    init_joystick();
    SceCtrlData ctrlData;
#endif

    while (!done)
    {

#ifdef __psp__
        update_joystick(ctrlData);
        if(ctrlData.Buttons & PSP_CTRL_HOME) break;
#endif

        SDL_SetRenderDrawColor(renderer, 100, 50, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bg_tex, NULL, NULL);

        // draw_text("Single line test with target texture", atlas, renderer, 16, 16, 16, 57, &comb1);
        // draw_text("Single line test without target test", atlas, renderer, 16, 16 + 16, 16, 57);
        // draw_text_multiline(multiline_text, atlas, renderer, rect, 16, 57, 70, &comb1);
        // draw_typewriter_simple(multiline_text, atlas, renderer, rect, stats, 16, &on_finish_draw, 57, 70, &comb1);
        draw_typewriter(multiline_text, atlas, renderer, rect, &comb1, stats, 18, &on_finish_draw, 57, 100);

        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
        Uint32 end_time = SDL_GetTicks();
        delta_time = (end_time - start_time) / 1000.0f;
        stats.timer += delta_time;
        start_time = end_time;
 
    }
    SDL_DestroyTexture(bg_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
