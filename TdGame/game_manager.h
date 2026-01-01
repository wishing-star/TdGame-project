#pragma once

#include "manager.h"
#include "config_manager.h"
#include "resources_manager.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class GameManager : public Manager<GameManager> {
	friend class Manager<GameManager>;

public:
	int run(int argc, char** argv) {

		Uint64 last_counter = SDL_GetPerformanceCounter();
		const Uint64 counter_freq = SDL_GetPerformanceFrequency();

		while (!is_quit) {
			while (SDL_PollEvent(&event))
				on_input();

			Uint64 current_counter = SDL_GetPerformanceCounter();
			double delta = (double)(current_counter - last_counter) / counter_freq;
			last_counter = current_counter;
			if (delta * 1000 < 1000.0 / 60)
				SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));

			on_update(delta);

			on_render();
		}
		
		return 0;
	}

protected:
	GameManager() {
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL2 ³õÊ¼»¯Ê§°Ü£¡");
		init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"SDL_image ³õÊ¼»¯Ê§°Ü£¡");
		init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer ³õÊ¼»¯Ê§°Ü£¡");
		init_assert(!TTF_Init(), u8"SDL_ttf ³õÊ¼»¯Ê§°Ü£¡");

		ConfigManager* config = ConfigManager::instance();

		init_assert(config->map.load("map.csv"), u8"¼ÓÔØÓÎÏ·µØÍ¼Ê§°Ü£¡");
		init_assert(config->load_level_config("level.csv"), u8"¼ÓÔØ¹Ø¿¨ÅäÖÃÊ§°Ü£¡");
		init_assert(config->load_game_config("config.json"), u8"¼ÓÔØÓÎÏ·ÅäÖÃÊ§°Ü£¡");

		window = SDL_CreateWindow(config->basic_template.window_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			config->basic_template.window_width, config->basic_template.window_height, SDL_WINDOW_SHOWN);
		init_assert(window, u8"´´½¨ÓÎÏ·´°¿ÚÊ§°Ü");

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		init_assert(renderer, u8"´´½¨ÓÎÏ·äÖÈ¾Æ÷Ê§°Ü");

		init_assert(ResourcesManager::instance()->load_from_file(renderer), u8"¼ÓÔØÓÎÏ·×ÊÔ´Ê§°Ü£¡");

		init_assert(generate_tile_map_texture, u8"Éú³ÉÍßÆ¬µØÍ¼Ê§°Ü£¡");
	}

	~GameManager() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Event event;
	bool is_quit = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

private:
	void init_assert(bool flag, const char* err_msg) {
		if (flag) return;

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"ÓÎÏ·Æô¶¯Ê§°Ü", err_msg, window);
		exit(-1);
	}

	void on_input() {

	}

	void on_update(double delta) {

	}

	void on_render() {

	}

	bool generate_tile_map_texture() {
		const Map& map = ConfigManager::instance()->map;
		const TileMap& tile_map = map.get_tile_map();
		SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		SDL_Texture* tex_tile_set = ResourcesManager::instance()->get_texture_pool().find(ResID::Tex_Tileset)->second;

		int width_tex_tile_set, height_tex_tile_set;
		SDL_QueryTexture(tex_tile_set, nullptr, nullptr, &width_tex_tile_set, & height_tex_tile_set);
	}
};
