#include <iostream>
#include<string>
#include "help/help.h"


int main(int argc, char *argv[]) {
    if(argc ==1) {
        std::cout << "Welcome to RATP, package manager.\n" << help() << std::endl;
    } else {
    }
    return 0;
}