#include "purge.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
int purge(char *argv[], int argc) {
  std::filesystem::path cur_dir = std::filesystem::current_path();
  std::ifstream registry("registry");
  std::ofstream registry_out("registry.tmp");
  for (size_t i = 2; i < argc; ++i) {
    bool found = false;
    std::string currentline;
    std::filesystem::path temp_dir = cur_dir / ".cache";
    std::string package_name = std::string() + argv[i];
    while (std::getline(registry, currentline)) {
      found = true;
      if (currentline.find(package_name) != std::string::npos) {
        std::string delete_file = package_name + "_uninstall.sh";
        std::filesystem::path uninstall_file = cur_dir / ".cache" / delete_file;
        chmod(uninstall_file.c_str(), 0755);

        std::system(uninstall_file.c_str());
        std::cout << package_name << " has been succefully deleted."
                  << std::endl;
        std::filesystem::remove(uninstall_file);
      } else {
        registry_out << currentline << std::endl;
      }
    }
    if (!found)
      std::cout << package_name << " isnt installed." << std::endl;
  }
  std::filesystem::rename("registry.tmp", "registry");
  registry.close();
  registry_out.close();

  return 0;
}
