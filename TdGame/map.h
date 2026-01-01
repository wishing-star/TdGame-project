#pragma once

#include "tile.h"
#include "route.h"

#include <SDL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Map
{
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool;
public:
	Map() = default;
	~Map() = default;

	bool load(const std::string& path) {
		std::ifstream file(path);
		if (!file.good()) return false;

		TileMap tile_map_temp;

		int idx_x = -1, idx_y = -1;

		std::string str_line;
		while (std::getline(file, str_line)) {
			str_line = trim_str(str_line);

			if (str_line.empty()) continue;

			idx_x = -1, ++idx_y;
			tile_map_temp.emplace_back();

			std::string str_tile;
			std::stringstream str_stream(str_line);
			while (std::getline(str_stream, str_tile, ',')) {
				++idx_x;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();

				load_tile_from_string(tile, str_tile);
			}
		}

		file.close();

		if (tile_map_temp.empty() || tile_map_temp[0].empty()) return false;

		tile_map = tile_map_temp;

		generate_map_cache();

		return true;
	}

	size_t get_width() const {
		return tile_map.empty() ? 0 : tile_map[0].size();
	}

	size_t get_height() const {
		return tile_map.size();
	}

	const TileMap& get_tile_map() const {
		return tile_map;
	}

	const SDL_Point& get_idx_home() const {
		return idx_home;
	}

	const SpawnerRoutePool& get_spawner_route_pool() const {
		return spawner_route_pool;
	}

	void place_tower(const SDL_Point& idx_tile) {
		tile_map[idx_tile.y][idx_tile.x].has_tower = true;
	}

private:
	TileMap tile_map;
	SDL_Point idx_home = { 0 };

	SpawnerRoutePool spawner_route_pool;

private:
	std::string trim_str(const std::string& str) {
		size_t str_begin = str.find_first_not_of(" \t");
		if (str_begin == std::string::npos)
			return "";

		size_t str_end = str.find_last_not_of(" \t");
		return str.substr(str_begin, str_begin - str_end + 1);
	}

	void load_tile_from_string(Tile& tile, const std::string& str) {
		std::string str_tidy = trim_str(str);
		
		std::string str_value;
		std::vector<int> values;
		std::stringstream str_stream(str_tidy);
		while (std::getline(str_stream, str_value, '\\')) {
			int value;
			try
			{
				value = stoi(str_value);
			}
			catch (const std::invalid_argument&)
			{
				value = -1;
			}

			values.push_back(value);
		}

		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		tile.decoration = (values.size() < 2) ? -1 : values[1];
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
		tile.special_flag = (values.size() <= 3) ? -1 : values[3];
	}

	void generate_map_cache() {
		for (size_t y; y < get_height(); ++y) {
			for (size_t x; x < get_width(); ++x) {
				Tile& tile = tile_map[y][x];

				if (tile.special_flag < 0) continue;

				if (tile.special_flag == 0) {
					idx_home.x = x;
					idx_home.y = y;
				}
				else
					spawner_route_pool[tile.special_flag] = Route(tile_map, { (int)x, (int)y });
			}
		}
	}
};