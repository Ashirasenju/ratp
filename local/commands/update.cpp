#include "update.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "commons.h"

int update() {
    const std::string base_url =
      "http://127.0.0.1:66/ratp/download_latest?package_name=";
  std::filesystem::path cur_dir = std::filesystem::current_path();
  std::ifstream registry("registry");
  std::string line;
  std::string package_name = "";
  std::string version = "";
  while (getline(registry, line)) {
    package_name = line.substr(0, line.find("|"));
    version = line.substr(line.find("|") + 2, 5);
    std::string url = base_url + package_name;
    int status_update = isLatest(package_name, version);
    if(status_update == -2){
      return 0;
    }
    if(status_update == 0){
      std::cout << "✅ " << package_name << " is at the newest version." << std::endl;
    }
    if (status_update == -4) {
      std::cout << package_name
                << " couldn't be found in the distant server ..." << std::endl;
    } else if (status_update) {
      std::cout << package_name << " has to update, starting download of the latest version " << std::endl;
      download(url,package_name);
      install(package_name,1);
    }
  }
  return 0;
}
