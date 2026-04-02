#include "install.h"
#include "commons.h"
#include "registry.h"
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/stat.h>
int install(char *argv[], int argc) {
  const std::string base_url =
      "http://127.0.0.1:66/ratp/download_latest?package_name=";
  const std::string check_url =
      "http://127.0.0.1:66/ratp/packageExist?package_name=";
  std::filesystem::path cur_dir = std::filesystem::current_path();

  for (size_t i = 2; i < argc; ++i) {
    std::string package_name = argv[i];
    if (package_name == "--local") {
      if (i + 1 == argc) {
        std::cout << "❌Please specify a path to the package." << std::endl;
        return -1;
      } else {
        std::filesystem::path local_package_path = argv[i + 1];

        std::string filename = local_package_path.filename();
        std::string package_name = local_package_path.filename()
                                       .replace_extension("")
                                       .replace_extension("");
        if (package_installed(package_name)) {
          std::cout << "❌ Package is already installed !" << std::endl;
          return EXIT_FAILURE;
        }

        std::filesystem::path path_to_cache = cur_dir / ".cache" / filename;

        if (std::filesystem::exists(path_to_cache)) {
          std::cout
              << filename
              << " is already in cache. Proceeding to install from the cache"
              << std::endl;
          install(package_name, 0);
          return 0;
        } else {
          std::filesystem::copy_file(local_package_path, path_to_cache);
          install(package_name, 0);
          return 0;
        }
        return -1;
      }
      return -1;
    } else {
      std::string check_url_full = check_url + package_name;
      std::string url = base_url + package_name;
      std::string filename = package_name + ".tar.xz";
      std::filesystem::path destination_cache = cur_dir / ".cache" / filename;
      std::filesystem::path temp_dir = cur_dir / ".temp";
      int exists = package_exist(check_url_full);
      if (!exists) {
        std::cout << "❌ Package doesn't exist ! Please check the name of the "
                     "package."
                  << std::endl;
        return 0;
      }

      download(url, package_name);
      install(package_name, 0);
      return 0;
    }
  }
  return 0;
}
