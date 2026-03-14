#include <filesystem>
#include <iostream>
#include <string>
// A RATP package should always have the following files : install.sh,
// uninstall.sh, .version. Those always file must not empty ! It should also
// have the follwing folder : docs, package, dependencies

// Wrapper used to clarify the code
int path_exists(std::filesystem::path path) {
  return std::filesystem::exists(path);
}
int file_is_empty(std::filesystem::path path) {
  return std::filesystem::file_size(path) == 0;
}

int check_arch(std::string folder) {
  std::filesystem::path folder_path = folder;
  // Checking if the file exists
  if (path_exists(folder_path)) {
    std::cout << "The path to access to the folder containing the package "
                 "cannot be found. Please check the spelling and try again."
              << std::endl;
    return -1;

  } else {
    std::cout << "✅The given path is correct." << std::endl;
    std::cout << "Checking if the architecture match the standard..."
              << std::endl;
    // Checking if install.sh is where it should be and not empty
    std::filesystem::path installsh_path = folder_path / "install.sh";
    if (!path_exists(installsh_path)) {
      std::cout << "❌install.sh does not exists. Please check if it exists "
                   "and it's position in the file architecture."
                << std::endl;
      return -1;
    }
    if (file_is_empty(installsh_path)) {
      std::cout << "❌install.sh does exists but it's empty. Your RATP package "
                   "will not work if install.sh is empty !"
                << std::endl;
      return -1;
      std::cout << "✅install.sh is correctly positioned and not empty."
                << std::endl;
    }
    // Checking if uninstall.sh is where it should be and not empty
    std::filesystem::path uninstallsh_path = folder_path / "uninstall.sh";
    if (!path_exists(uninstallsh_path)) {
      std::cout << "❌uninstall.sh does not exists. Please check if it exists "
                   "and it's position in the file architecture."
                << std::endl;
      return -1;
    }
    if (file_is_empty(uninstallsh_path)) {
      std::cout << "❌uninstall.sh does exists but it's empty. Your RATP "
                   "package will not work if uninstall.sh is empty !"
                << std::endl;
      return -1;
    }
    std::cout << "✅uninstall.sh is correctly positioned and not empty."
              << std::endl;
    // Checking if the .version file is where it should be and not empty
    std::filesystem::path dotversion_path = folder_path / ".version";
    if (!path_exists(dotversion_path)) {
      std::cout << "❌.version does not exists. Please check if it exists "
                   "and it's position in the file architecture."
                << std::endl;
      return -1;
    }
    if (file_is_empty(dotversion_path)) {
      std::cout << "❌.version does exists but it's empty. Your RATP "
                   "package will not work if uninstall.sh is empty !"
                << std::endl;
      return -1;
    }
    std::cout << "✅.version is correctly positioned and not empty."
              << std::endl;
  }

  return 0;
}
