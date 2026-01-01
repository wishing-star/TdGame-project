#define SDL_MAIN_HANDLED

#include <iostream>
#include <fstream>
#include <sstream>

#include <cJSON.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

void test_json() {
	std::ifstream file("test.json");
	if (!file.good()) {
		std::cout << "file open failed" << std::endl;
		return;
	}

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	cJSON* json_root = cJSON_Parse(ss.str().c_str());

	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");

	std::cout << json_name->string << ":" << json_name->valuestring << std::endl;
	std::cout << json_age->string << ":" << json_age->valueint << std::endl;

	std::cout << json_pets->string << ":" << std::endl;
	cJSON* json_item = nullptr;

	cJSON_ArrayForEach(json_item, json_pets) {
		std::cout << json_item->valuestring << std::endl;
	}

}

void test_csv() {
	std::ifstream file("test.csv");
	if (!file.good()) {
		std::cout << "file open failed" << std::endl;
		return;
	}

	std::string str_line;

	while (std::getline(file, str_line)) {
		std::string str_grid;
		std::stringstream ss(str_line);

		while (std::getline(ss, str_grid, ',')) {
			std::cout << str_grid << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	//test_json();
	test_csv();

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window* window = SDL_CreateWindow(u8"你好，世界", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_Surface* suf_img = IMG_Load("1.png");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	TTF_Font* font = TTF_OpenFont("ipix.ttf", 20);
	SDL_Color color = { 255,255,255,255 };

	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"你好，世界!", color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);

	int FPS = 60;

	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_frq = SDL_GetPerformanceFrequency();

	bool is_quit = false;
	
	SDL_Event event;
	SDL_Point pos_cursor = { 0, 0 };
	SDL_Rect rect_dst, text_dst;

	rect_dst.w = suf_img->w * 2;
	rect_dst.h = suf_img->h * 2;

	text_dst.w = suf_text->w;
	text_dst.h = suf_text->h;

	while (!is_quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION) {
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / counter_frq;
		last_counter = current_counter;
		if (delta * 1000 < 1000.0 / 60)
			SDL_Delay((Uint32)(1000.0 / 60 - delta * 1000));

		rect_dst.x = pos_cursor.x, rect_dst.y = pos_cursor.y;
		text_dst.x = pos_cursor.x, text_dst.y = pos_cursor.y;

		SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); // 深蓝色背景
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, tex_img, nullptr, &rect_dst);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 300, 125, 0, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &text_dst);
		SDL_RenderPresent(renderer);
	}

	return 0;
}