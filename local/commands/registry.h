#pragma once
#include <string>
#include <vector>
void show_package();
void modify_version(std::string package_name, std::string new_version);
int package_installed(std::string package_name);
void add_package(std::string package_name, std::string version);
void remove_package(std::string package_name);
std::string get_version(std::string package_name);
std::vector<std::string> get_versions_by_name(std::string version);
