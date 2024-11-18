#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>


struct File
{
    std::filesystem::path path;
	uint32_t type;
};


File openFileName(sf::WindowHandle handle, const char* filter, const std::string& defaultname = "");

File saveFileName(sf::WindowHandle handle, const char* filter, const std::string& defaultname = "");

