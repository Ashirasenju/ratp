#pragma once
#include <filesystem>
#include <string>

int check_arch(std::string folder);

int path_exists(std::filesystem::path path);
int file_is_empty(std::filesystem::path path);
