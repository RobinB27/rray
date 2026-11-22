#include "lib/framebuf.h"
#include "lib/raytracer.h"
#include "lib/scene.h"
#include "lib/util.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

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

	if (!SDL_SetWindowRelativeMouseMode(window, true))
		SDL_Log("Could not enable relative mouse mode: %s", SDL_GetError());

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

	FrameBuffer *fb;
	if (!(fb = fb_make(FB_WIDTH, FB_HEIGHT))) {
		SDL_Log("Framebuffer creation failed");
		return EXIT_FAILURE;
	}

	// rray setup
	Camera camera = cam_zero();
	Scene *scene = scene_example();

	struct timespec previous = {.tv_sec = 0LL, .tv_nsec = 0LL};
	const int64_t frame_ns = NS_PER_SECOND / 60;

	bool running = true;
	while (running) {
		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC, &start);

		SDL_Event e;
		Vec3 mouse_delta = vec3_zero();

		while (SDL_PollEvent(&e)) {
			SDL_ConvertEventToRenderCoordinates(renderer, &e);
			if (e.type == SDL_EVENT_QUIT ||
			    (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)) {
				running = false;
			} else if (e.type == SDL_EVENT_MOUSE_MOTION) {
				mouse_delta.x = e.motion.xrel;
				mouse_delta.y = e.motion.yrel;
			}
		}

		double dt = ns_between(previous, start);
		previous = start;

		float v = dt / NS_PER_SECOND;
		Vec3 V = vec3_zero();

		const bool *keys = SDL_GetKeyboardState(NULL);
		if (!keys) {
			SDL_Log("Get keyboard state failed: %s", SDL_GetError());
			return EXIT_FAILURE;
		}

		if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W])
			V.y = v;
		else if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
			V.y = -v;
		if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
			V.x = -v;
		else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
			V.x = v;

		camera.pos = vec3_add(camera.pos, V);

		Vec3 rot_acc = vec3_mulf(mouse_delta, 0.01);
		camera.rot.y += rot_acc.x;
		camera.rot.x += rot_acc.y;
		camera.rot = vec3_clamp(camera.rot, -PI, PI);

		// rray render
		render(fb, camera, scene);

		SDL_UpdateTexture(screen, NULL, fb->buf,
		                  fb->width * (int)sizeof(Uint32));
		SDL_SetRenderDrawColor(renderer, 0u, 0u, 0u, 255u);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);

		clock_gettime(CLOCK_MONOTONIC, &end);
		int64_t remaining_ns = frame_ns - ns_between(start, end);

		if (remaining_ns > 0) {
			struct timespec delay = {.tv_sec = remaining_ns / NS_PER_SECOND,
			                         .tv_nsec = remaining_ns % NS_PER_SECOND};
			nanosleep(&delay, NULL);
		}
	}

	scene_free(scene);
	fb_free(fb);
	SDL_DestroyTexture(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}