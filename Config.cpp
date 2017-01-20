#include "stdafx.h"
#include "Config.h"
#include "GameUtils.h"

int Config::_init = 0;
std::map<std::string, int> Config::_params;

int Config::get(const std::string& name) {
	if (_init) {
		return _params[name];
	}
	else {
		// инициализация параметров игры из input.txt
		auto stream = Core::fileSystem.OpenRead("input.txt");
		IO::TextReader reader(stream.get());
		std::string line;
		line = reader.ReadAsciiLine();
		while (line != "") {
			auto vec = game_utils::split(line, '=');
			_params[vec[0]] = std::stoi(vec[1]);
			line = reader.ReadAsciiLine();
		}
		++_init;
		return _params[name];
	}
}