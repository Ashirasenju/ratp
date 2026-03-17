#include "pack.h"
#include <filesystem>
#include <iostream>
#include <string>
#include "commons.h"
// A RATP package should always have the following files : install.sh,
// uninstall.sh, .version. Those always file must not empty !
// Wrapper used to clarify the code
int path_exists(std::filesystem::path path) {
  return std::filesystem::exists(path);
}
int file_is_empty(std::filesystem::path path) {
  return std::filesystem::file_size(path) == 0;
}

int check_arch(std::string folder) {


  std::filesystem::path folder_path = folder;
  if (!path_exists(folder_path)) {
    std::cout << "The path to access to the folder containing the package "
                 "cannot be found. Please check the spelling and try again."
              << std::endl;
    return 0;

  } else {
    int number_of_files = 0;
    for (const auto & entry : std::filesystem::directory_iterator(folder))
        number_of_files++;
    if(number_of_files > 3){
      std::cout << "❌There is too much files. Please delete the files that do not matches the architecture." << std::endl;
      return 0;
    } if(number_of_files < 3){
      std::cout << "❌ There is too little files. Please create the files that matches the architecture." << std::endl;
      return 0;
    }
    std::cout << "✅The given path is correct." << std::endl;
    std::cout << "Checking if the architecture match the standard..."
              << std::endl;
    std::filesystem::path installsh_path = folder_path / "install.sh";
    if (!path_exists(installsh_path)) {
      std::cout << "❌install.sh does not exists. Please check if it exists "
                   "and it's position in the file architecture."
                << std::endl;
      return 0;
    }
    if (file_is_empty(installsh_path)) {
      std::cout << "❌install.sh does exists but it's empty. Your RATP package "
                   "will not work if install.sh is empty !"
                << std::endl;
      return 0;
      std::cout << "✅install.sh is correctly positioned and not empty."
                << std::endl;
    }
    std::filesystem::path uninstallsh_path = folder_path / "uninstall.sh";
    if (!path_exists(uninstallsh_path)) {
      std::cout << "❌uninstall.sh does not exists. Please check if it exists "
                   "and it's position in the file architecture."
                << std::endl;
      return 0;
    }
    if (file_is_empty(uninstallsh_path)) {
      std::cout << "❌uninstall.sh does exists but it's empty. Your RATP "
                   "package will not work if uninstall.sh is empty !"
                << std::endl;
      return 0;
    }
    std::cout << "✅uninstall.sh is correctly positioned and not empty."
              << std::endl;
    std::filesystem::path dotversion_path = folder_path / ".version";
    if (!path_exists(dotversion_path)) {
      std::cout << "❌.version does not exists. Please check if it exists "
                   "and it's position in the file architecture."
                << std::endl;
      return 0;
    }
    if (file_is_empty(dotversion_path)) {
      std::cout << "❌.version does exists but it's empty. Your RATP "
                   "package will not work if uninstall.sh is empty !"
                << std::endl;
      return 0;
    }
    std::cout << "✅.version is correctly positioned and not empty."
              << std::endl;
  }
  std::cout << "✅The architecture of this RATP package is correct !"
            << std::endl;

  return 1;
}

int pack(std::string folder,std::string package_name){
  int arch_check = check_arch(folder);
  if(arch_check){

    int stat = compress(folder,package_name);
    if(stat) {
      std::cout << "✅ Compression done ! Your package is ready to go !" << std::endl;
    } else{
      std::cout << "❌ Somthing went wrong with tar compression." << std::endl;
    }
    return 0;
  } else{
    return  -1;
  }
}

