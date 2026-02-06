#include "upgrade.h"
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <filesystem>
int upgrade() {
  std::filesystem::path cur_dir = std::filesystem::current_path();
  namespace fs = std::filesystem;
  const fs::path to_upgrade {cur_dir / ".to_upgrade"}; 
  if(fs::is_empty(to_upgrade)) {
    std::cout << "Nothing to upgrade." << std::endl;
    return -1;
  }
  for (const fs::directory_entry& entry : fs::directory_iterator(to_upgrade)) {

    if (!entry.is_regular_file())
        continue;

    std::string filename = entry.path().filename().string();
    std::string package_name = entry.path().stem().string();

    std::filesystem::path downloaded_content = entry.path();
    std::filesystem::path destination_cache = cur_dir / ".cache" / filename;
    std::filesystem::path cache_dir = cur_dir / ".cache";
    std::filesystem::path temp_dir = cur_dir / ".temp";

    auto packet_size = fs::file_size(downloaded_content);
    if (packet_size == 0) {
        std::cout << "The package " << filename << " doesn't exist." << std::endl;
        continue;
    }

    try {
        fs::rename(downloaded_content, destination_cache);
        std::cout << "✅  " << filename
                  << " was successfully added to cache.\n";
    } catch (const fs::filesystem_error&) {
        std::cout << "❌ Couldn't move " << filename
                  << " to .cache directory." << std::endl;
        continue;
    }


    std::cout << "Decompressing " << filename << " ..." << std::endl;
    std::cout << destination_cache << "\n";
    std::cout << "Uncompressing the following files : " << std::endl;
    std::string command =
        "tar -xvf " + destination_cache.string() + " -C " + cache_dir.string() + "/";
    std::system(command.c_str());

    fs::path path_to_install = cache_dir / "install.sh";
    std::string path_to_install_ =
        path_to_install.string();

    if (fs::exists(path_to_install)) {
        chmod(path_to_install.c_str(), 0755);
    } else {
        std::cerr << "❌ install.sh not found in " << package_name << "\n";
        continue;
    }

    fs::path path_to_version = cache_dir / ".version";

    std::cout << "Installing " << package_name << std::endl;
    std::ifstream version(path_to_version);
    if (!version.is_open()) {
        std::cout << "❌ Couldn't check version. Installation aborted.\n";
        continue;
    }

    int status_install = std::system(path_to_install_.c_str());
    if (status_install == -1) {
        std::cout << "❌ Installation failed for " << package_name << std::endl;
        continue;
    }

    std::cout << "✅ " << package_name << " installed successfully.\n";

    std::string version_str;
    std::getline(version, version_str);

    std::ofstream registry("registry", std::ios::app);
    registry << package_name << "|" << version_str << ";\n";

    registry.close();
    version.close();

    std::cout << "Clearing temp...\n";
    fs::remove_all(cache_dir);
    fs::create_directory(cache_dir);

    std::cout << package_name
              << " was successfully installed.\n";
}

  return 0;
}
