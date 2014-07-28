#include "Settings.hpp"
#include "Platform.hpp"
#include <fstream>
#include <sstream>

std::map<std::string, std::string> gt::Settings::settings;

bool gt::Settings::parse(const std::string &path)
{
	std::ifstream configFile(gt::Platform::getDefaultConfigPath() + path);
	if (!configFile)
		return true;
	while (!configFile.eof())
	{
		std::string key;
		std::string value;

		// split a line
		getline(configFile, key);
		unsigned sep = key.find('=');
		if (sep != std::string::npos)
		{
			value = key.substr(sep + 1);
			key = key.substr(0, sep);

			// remove whitespace
			while (isblank(key[key.length() - 1]))
				key.pop_back();
			while (isblank(value[0]))
				value = value.substr(1);

			settings[key] = value;
		}
	}
	configFile.close();
	return false;
}

bool gt::Settings::save(const std::string &path)
{
	std::ofstream configFile(gt::Platform::getDefaultConfigPath() + path, ios_base::trunc);// old configfile is destroyed
	if(!configFile)
		return true;
	for(auto i = settings.begin(); i != settings.end(); ++i)
		configFile << i->first << " = " << i->second << std::endl;
	configFile.close();
	return false;
}

template<typename T>
T gt::Settings::getOption(const std::string &key)
{
	auto i = settings.find(key);
	if(i == settings.end())
		throw std::exception("No such option.");
	std::stringstream opt(*i);
	T value;
	opt >> value;
	return value;
}

template<typename T>
void gt::Settings::setOption(const std::string &key, T value)
{
	std::stringstream opt;
	opt << value;
	settings[key] = opt.str();
}
