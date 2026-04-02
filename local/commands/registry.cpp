#include "registry.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// Functions to interact with the registry

void modify_version(std::string package_name, std::string new_version) {

  std::ifstream infile("registry");
  std::ofstream outfile("registry.temp");
  std::string line;
  std::string prefix = package_name + "|";

  while (std::getline(infile, line)) {
    if (line.rfind(prefix, 0) != 0) {

      outfile << line << "\n";
    } else {
      outfile << package_name << "|" << new_version << "\n";
    }
  }
  infile.close();
  outfile.close();
  std::filesystem::remove("registry");
  std::filesystem::rename("registry.tmp", "registry");
}

void show_package() {
  std::ifstream registry("registry");
  std::string line = "";
  std::cout << "Package List : " << std::endl;
  while (std::getline(registry, line)) {
    std::replace(line.begin(), line.end(), '|', ',');

    std::cout << "Name, Version : " << line << std::endl;
  }
  registry.close();
}
void add_package(std::string package_name, std::string version) {
  std::ofstream registry("registry");
  std::string new_package = package_name + "|" + version;
  registry << new_package << "\n";
  registry.close();
}
void remove_package(std::string package_name) {
  std::ifstream registry("registry");
  std::ofstream n_registry("registry.tmp");
  std::string line;
  while (getline(registry, line)) {
    if (line.find(package_name) == std::string::npos) {
      n_registry << line;
    }
  }
  registry.close();
  n_registry.close();
  std::filesystem::remove("registry");
  std::filesystem::rename("registry.tmp", "registry");
}
int package_installed(std::string package_name) {
  std::ifstream registry("registry");
  std::string line;
  while (getline(registry, line)) {
    if (line.find(package_name + "|")) {
      return 0;
    }
  }
  registry.close();
  return 1;
}
std::string get_version(std::string package_name);
std::vector<std::string> get_versions_by_name(std::string version);
