#include "install.h"
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include "commons.h"
#include <sys/stat.h>
int install(char *argv[], int argc) {
  const std::string base_url =
      "http://127.0.0.1:66/ratp/download_latest?package_name=";
  const std::string check_url = "http://127.0.0.1:66/ratp/packageExist?package_name=";
  std::filesystem::path cur_dir = std::filesystem::current_path();



  for (size_t i = 2; i < argc; ++i) {
    std::string package_name = argv[i];
    std::string check_url_full = check_url + package_name;
    std::string url = base_url + package_name;
    std::string filename = package_name + ".tar.xz";
    std::filesystem::path destination_cache = cur_dir / ".cache" / filename;
    std::filesystem::path temp_dir = cur_dir / ".temp";
    int exists = package_exist(check_url_full);
    if(!exists){
      std::cout << "❌ Package doesn't exist ! Please check the name of the package." << std::endl;
      return 0;
    } 



  download(url,package_name);
  install(package_name,0);
  return 0;
}
}
