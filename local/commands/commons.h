#pragma once
#include <string>
#include <filesystem>


int download(const std::string url,std::string package_name);
int package_exist(const std::string url);
int isLatest(std::string package_name, std::string version);
int install(std::string package_name, int is_update);
int compress(std::filesystem::path folder_path,std::string package_name);
