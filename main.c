#include "lib/framebuf.h"
#include "lib/raytracer.h"
#include "lib/scene.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	if (!SDL_CreateWindowAndRenderer("Software Renderer", FB_WIDTH * W_SCALE,
	                                 FB_HEIGHT * W_SCALE, SDL_WINDOW_RESIZABLE,
	                                 &window, &renderer)) {
		SDL_Log("Create window/renderer failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_SetRenderVSync(renderer, 1);

	SDL_SetRenderLogicalPresentation(renderer, FB_WIDTH, FB_HEIGHT,
	                                 SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

	SDL_Texture *screen =
	    SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888,
	                      SDL_TEXTUREACCESS_STREAMING, FB_WIDTH, FB_HEIGHT);

	if (!screen) {
		SDL_Log("SDL_CreateTexture faiöed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_LINEAR);

	framebuf *fb;
	if (!(fb = fb_make(FB_WIDTH, FB_HEIGHT))) {
		SDL_Log("Framebuffer creation failed");
		return EXIT_FAILURE;
	}

	float mouse_x = FB_WIDTH / 2.0f, mouse_y = FB_HEIGHT / 2.0f;
	bool running = true;

    // rren setup
    Scene *scene = scene_example();

	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			SDL_ConvertEventToRenderCoordinates(renderer, &e);
			if (e.type == SDL_EVENT_QUIT ||
			    (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)) {
				running = false;
			} else if (e.type == SDL_EVENT_MOUSE_MOTION) {
				mouse_x = e.motion.x;
				mouse_y = e.motion.y;
			}
		}

		// rren
        render(fb, scene);
		// rren off

		SDL_UpdateTexture(screen, NULL, fb->buf,
		                  fb->width * (int)sizeof(Uint32));
		SDL_SetRenderDrawColor(renderer, 0u, 0u, 0u, 255u);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	fb_free(fb);
	SDL_DestroyTexture(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}