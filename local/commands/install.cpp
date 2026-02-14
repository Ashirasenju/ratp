#include "install.h"
#include "curl/curl.h"
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/stat.h>
int install(char *argv[], int argc) {
  const std::string base_url =
      "http://127.0.0.1:66/ratp/download_latest?package_name=";
  std::filesystem::path cur_dir = std::filesystem::current_path();

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Cannot init liburl.\n";
    return 1;
  }

  for (size_t i = 2; i < argc; ++i) {
    std::string package_name = argv[i];
    std::string url = base_url + package_name;
    std::string filename = package_name + ".tar.xz";

    FILE *fp = fopen(filename.c_str(), "wb");
    if (!fp) {
      std::cerr << "  ⚠️  Cannot open a new buffer for the downloaded file. '"
                << filename << "\n";
      continue;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode res = curl_easy_perform(curl);
    fclose(fp);

    if (res != CURLE_OK)
      std::cerr << "  ❌ Curl Error '" << package_name
                << "': " << curl_easy_strerror(res) << '\n';
    else
      std::cout << "✅ Download finished → " << filename << "\n";
    std::filesystem::path downloaded_content = cur_dir / filename;
    std::filesystem::path destination_cache = cur_dir / ".cache" / filename;
    std::filesystem::path temp_dir = cur_dir / ".temp";

    auto packet_size = std::filesystem::file_size(downloaded_content);
    if (packet_size == 0) {
      std::cout << "The package doesn't exist." << std::endl;
      return -1;
    }
    try {
      std::filesystem::rename(downloaded_content, destination_cache);
      std::cout << "✅  " << filename << " was successfully added to cache."
                << "\n";

    } catch (const std::filesystem::filesystem_error &e) {
      std::cout << "❌ " << ""
                << "Couldn't be moved to .cache directory. Please check if "
                   "the .cache directory exists."
                << std::endl;
    }
    curl_easy_cleanup(curl);
    std::cout << "Decompressing " << filename << " ..." << std::endl;
    std::string command = "tar -xvf " + destination_cache.string() + " -C " +
                          temp_dir.string() + "/" + " > /dev/null 2>&1";
    std::system(command.c_str());

    std::filesystem::path path_to_install = temp_dir / "install.sh";
    std::string path_to_install_ =
        path_to_install.string() + " > /dev/null 2>&1";
    if (std::filesystem::exists(path_to_install)) {
      chmod(path_to_install.c_str(), 0755);
    } else {
      std::cerr << "❌ install.sh not found in " << package_name << "\n";
      continue;
    }
    std::filesystem::path path_to_version = temp_dir / ".version";

    std::cout << "Installing " << package_name << std::endl;
    std::ifstream version(path_to_version.c_str());
    if (!version.is_open()) {
      std::cout << "❌Couldn't check the version of the package. Installation "
                   "Aborted."
                << std::endl;
    }

    int status_install = std::system(path_to_install_.c_str());

    if (status_install == -1) {
      std::cout << "❌Something went wrong with the installation of "
                << package_name
                << "please try to contact the editor of the package"
                << std::endl;
      return -1;
    }
    std::cout << "✅ " << package_name
              << " installation procedure is successfull." << std::endl;
    std::cout << "Adding " << package_name << " to the registry..."
              << std::endl;
    std::string version_str;
    std::getline(version, version_str);
    std::ofstream registry("registry");
    std::string new_package = package_name + "|" + version_str + ";";
    registry << new_package << "\n";
    registry.close();
    version.close();
    std::cout << "✅ " << package_name << " succesfully added to the registry"
              << std::endl;
    std::cout << "Clearing..." << std::endl;
    std::filesystem::remove_all(temp_dir);
    std::filesystem::create_directory(temp_dir);
    std::cout << "✅ " << package_name
              << " was successfully installed on your computer." << std::endl;
  }
  return 0;
}
