#pragma once
#include <random>
#include <string>
#include <sstream>
#include <vector>

namespace game_utils {

	float random_float() {
		auto rnd = [=]() {
			static std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(15, 20);
			return uni(rng);
		};
		auto f = std::string("0.") + std::to_string(rnd());
		return std::stof(f);
	}

	math::Vector3 random_vec(int min, int max) {

		auto rnd = [=]() {
			static std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_int_distribution<int> uni(min, max);
			auto a = uni(rng);
			if (a == 0)
				a += 1;
			return a;
		};

		return math::Vector3{ static_cast<float>(rnd()), static_cast<float>(rnd()), 0 };
	}

	std::vector<std::string> split(const std::string& s, char sep)
	{
		std::stringstream ss;
		ss.str(s);
		std::string item;
		std::vector<std::string> items;
		while (std::getline(ss, item, sep)) {
			items.push_back(item);
		}
		return items;
	}

} // namespace utils