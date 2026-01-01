#pragma once

#include "tile.h"

#include <SDL.h>
#include <vector>

class Route
{
public:
	typedef std::vector<SDL_Point> IdxList;

public:
	Route() = default;
	Route(const TileMap& map, const SDL_Point& idx_origin) {
		size_t width_map = map[0].size();
		size_t height_map = map.size();
		SDL_Point idx_next = idx_origin;

		while (true) {
			if (idx_next.x < 0 || idx_next.x >= width_map || idx_next.y < 0 || idx_next.y >= height_map)
				break;

			if (check_duplicate_idx(idx_next))
				break;
			else
				idx_list.push_back(idx_next);

			//bool is_next_dir_exist = true;

			const Tile& tile = map[idx_next.y][idx_next.x];

			if (tile.special_flag == 0) break;

			//寻找下一个路径下标
			int dif_y = dire[(int)tile.direction][0], dif_x = dire[(int)tile.direction][1];
			idx_next.y += dif_y, idx_next.x += dif_x;

			//下一个单元格不存在
			if (dif_y == dif_x) break;
		}
	}

	~Route() {

	}

	const IdxList& get_idx_list() const {
		return idx_list;
	}

private:
	IdxList idx_list;

	static constexpr int dire[5][2] = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

private:
	bool check_duplicate_idx(const SDL_Point& target_idx) {
		for (const auto& idx : idx_list) {
			if (target_idx.x == idx.x && target_idx.y == idx.y)
				return true;
		}

		return false;
	}
};