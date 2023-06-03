#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>

#define FONT_SIZE 24
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define UPDATE_INTERVAL 1000

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("CPS Counter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);
    TTF_Font* font = TTF_OpenFont("fonts/t.ttf", FONT_SIZE);
    SDL_Color textColor = { 255, 255, 255, 255 };

    const char* text = "Clicks per second: 0";
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    SDL_FreeSurface(textSurface);

    int textX = (WINDOW_WIDTH - textWidth) / 2;
    int textY = (WINDOW_HEIGHT - textHeight) / 2;

    SDL_Rect textRect = { textX, textY, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    int clickCount = 0;
    int cps = 0;
    Uint32 startTime = 0;
    Uint32 lastUpdate = 0;

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                clickCount++;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - lastUpdate;

        if (elapsedTime >= UPDATE_INTERVAL) {
            cps = (int)((float)clickCount / (elapsedTime / 1000.0f));
            lastUpdate = currentTime;
            clickCount = 0;

            char updatedText[64];
            snprintf(updatedText, sizeof(updatedText), "Clicks per second: %d", cps);
            SDL_DestroyTexture(textTexture);
            textSurface = TTF_RenderText_Solid(font, updatedText, textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            textWidth = textSurface->w;
            textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);
            textRect.w = textWidth;
            textRect.h = textHeight;

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
