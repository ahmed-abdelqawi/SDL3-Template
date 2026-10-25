#include <iostream>
#include <cstdlib>
#include <stdint.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> /* INCLUDE ONLEY ONCE! */

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef uint32_t u32;
typedef uint64_t u64;

#define FB_WIDTH 320
#define FB_HEIGHT 200
u32 FrameBuffer[FB_WIDTH * FB_HEIGHT];

/* frame buffer manipulation */
void putPixel(int x_pos, int y_pos, u32 color);
void clearBg(u32 color);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    /* initialize SDL3 */
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL_Init() Failed! " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    /* === create window, renderer, and texture === */

    SDL_Window *window = SDL_CreateWindow("SDL3-Project", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow() Failed! " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer() Failed! " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, FB_WIDTH, FB_HEIGHT);
    if (texture == nullptr)
    {
        std::cout << "SDL_CreateTexture() Failed! " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST))
    {
        std::cout << "SDL_SetTextureScaleMode() Failed! " << SDL_GetError() << std::endl;
    }

    /* enable vsync */
    if (!SDL_SetRenderVSync(renderer, 1))
    {
        std::cout << "SDL_SetRenderVSync() Failed! " << SDL_GetError() << std::endl;
    }

    /* === GAME LOOP === */

    SDL_Event event;
    int running = 1;

    /* for delta time calculations */
    const double targetFPS = 1.0 / 60.0f;
    u64 start = 0, end = 0;
    double deltaTime = 0.0f;

    int x = 0, y = FB_HEIGHT / 2;

    while (running)
    {
        start = SDL_GetPerformanceCounter(); /* get the performance until the last frame */

        /* === WINDOW EVENTS === */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = 0;
                break;

            default:
                break;
            }
        }

        /* === DRAWING STUFF === */

        /* set the pixels */
        clearBg(0x323232);
        putPixel(x++ % FB_WIDTH, y, 0x00ff00);

        /* update texture */
        SDL_UpdateTexture(texture, nullptr, (void *)FrameBuffer, sizeof(u32) * FB_WIDTH);

        /* render the current frame */
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        end = SDL_GetPerformanceCounter(); /* get performance for the current frame */
        deltaTime = (double)(end - start) / (double)SDL_GetPerformanceFrequency();
        /* delay until we hit the targetFPS (60) */
        if (deltaTime < targetFPS)
            SDL_Delay((targetFPS - deltaTime) * 1000.0f);
    }

    /* === HOUSEKEEPING === */
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

/* putPixel(): set a pixel @ (x_pos, y_pos) to color */
void putPixel(int x_pos, int y_pos, u32 color)
{
    if (x_pos >= FB_WIDTH || x_pos < 0)
        return;
    if (y_pos >= FB_HEIGHT || y_pos < 0)
        return;

    int pixelIndex = (y_pos * FB_WIDTH) + x_pos;
    FrameBuffer[pixelIndex] = color;
}

/* clearBG(): clear the entire texture with color */
void clearBg(u32 color)
{
    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; ++i)
        FrameBuffer[i] = color;
}
