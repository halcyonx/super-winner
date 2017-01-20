#pragma once

class Config {
public:
	static int get(const std::string& name);

private:
	static std::map<std::string, int> _params;
	static int _init;
};

//Singleton<Config> single;
//SingletonHolder<Config> config;
