#include "help/help.h"
#include "install/install.h"
#include <cstring>
#include <iostream>
int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout
        << "\x1B[31m[Error] No arguments were specified.\x1B[0m\n\nWelcome to RATP, package manager.\n\n"
        << help() << std::endl;
  } else {
    if (std::strcmp(argv[1], "help") == 0) {
      std::cout << help() << std::endl;
    } if(std::strcmp(argv[1], "install") == 0){


      int status = install(argv);
      return status;
    }

  }
  return 0;
}
