#include "commands/help.h"
#include "commands/install.h"
#include "commands/pack.h"
#include "commands/purge.h"
#include "commands/registry.h"
#include "commands/update.h"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "\x1B[31m[Error] No arguments were "
                 "specified.\x1B[0m\n\nWelcome to RATP, package manager.\n\n"
              << help() << std::endl;
  } else {
    if (std::strcmp(argv[1], "help") == 0) {
      std::cout << help() << std::endl;
    }
    if (std::strcmp(argv[1], "install") == 0) {
      int status = install(argv, argc);
      return status;
    }
    if (std::strcmp(argv[1], "purge") == 0) {
      int status = purge(argv, argc);
      return status;
    }
    if (std::strcmp(argv[1], "show") == 0) {
      if (argc == 3) {
        show_package();
      } else {
        if (argc > 3) {

          if (std::strcmp(argv[2], "--package-name") == 0) {
            std::string version = get_version(argv[3]);
            if (std::strcmp(version.c_str(), "") == 0) {
              std::cout << "Package not found." << std::endl;
            } else {
              std::cout << version << std::endl;
            }
          } else if (std::strcmp(argv[2], "--version") == 0) {
            std::vector<std::string> package_names =
                get_versions_by_name(argv[3]);
            for (auto var : package_names) {
              std::cout << var << std::endl;
            }
          } else {
            std::cout << "wrong parameter" << std::endl;
          }
        }
      }
      return 0;
    }

    if (std::strcmp(argv[1], "update") == 0) {
      int status = update();
      return status;
    }
    if (std::strcmp(argv[1], "check-arch") == 0) {
      int status = -1;
      if (argc >= 3) {
        std::string path = argv[2];
        int status = check_arch(path);

      } else {
        std::cout << "❌Missing a argument." << std::endl;
      }

      return status;
    }
    if (std::strcmp(argv[1], "pack") == 0) {
      int status = -1;
      if (argc >= 4) {
        std::string path = argv[2];
        std::string package_name = argv[3];
        int status = pack(package_name, path);

      } else {
        std::cout << "❌Missing a argument." << std::endl;
      }

      return status;
    } else {
      std::cout << "❌ Command not found" << std::endl;
      return -1;
    }

    return -1;
  }
  return 0;
}
