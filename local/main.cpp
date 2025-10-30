#include "help/help.h"
#include "install/install.h"
#include "purge/purge.h"
#include "update/update.h"
#include "upgrade/upgrade.h"
#include <cstring>
#include <iostream>
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
      int status = install(argv);
      return status;
    }
    if (std::strcmp(argv[1], "purge") == 0) {
      int status = purge(argv);
      return status;
    }
    if (std::strcmp(argv[1], "upgrade") == 0) {
      int status = upgrade();
      return status;
    }
    if (std::strcmp(argv[1], "update") == 0) {
      int status = update();
      return status;
    }
    return -1;
  }
  return 0;
}
