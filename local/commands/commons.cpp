#include "commons.h"
#include <string>
#include <filesystem>
#include "curl/curl.h"
#include <cstdio>
#include <sys/stat.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <iostream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* output = static_cast<std::string*>(userp);
    output->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}
int extractHasToUpdate(const std::string &json) {
  const std::string key = "has_to_update";

  size_t pos = json.find(key);
  if (pos == std::string::npos)
    return -1;

  pos = json.find(":", pos);
  if (pos == std::string::npos)
    return -1;

  while (pos < json.size() && (json[pos] < '0' || json[pos] > '9'))
    pos++;

  if (pos >= json.size())
    return -1;

  return json[pos] - '0';
}

int isLatest(std::string package_name, std::string version) {
  CURL *curl;
  CURLcode res;
  std::string response;
  std::string url =
      "http://127.0.0.1:66/ratp/isLatest?package_name=" + package_name +
      "&version=" + version;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
      std::cerr << "Curl Error :" << curl_easy_strerror(res) << std::endl;
      return -2;
    }
  }
  int return_value = extractHasToUpdate(response);
  if (return_value == 0 || return_value == 1) {
    return return_value;
  } else {
    return -4;
  }
}

int download(const std::string url,std::string package_name){
      std::filesystem::path cur_dir = std::filesystem::current_path();

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Cannot init liburl.\n";
    return -1;
  }
      std::string filename = package_name + ".tar.xz";

      FILE *fp = fopen(filename.c_str(), "wb");
    if (!fp) {
      std::cerr << "  ⚠️  Cannot open a new buffer for the downloaded file. '"
                << filename << "\n";
      return -1;
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
      std::cout << "The package is empty. Please contact the maintainer of the package." << std::endl;
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
    return 0;
}


int package_exist(const std::string url){
{
   
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&response));
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
          

              if (response.find("1") != std::string::npos){
                      curl_easy_cleanup(curl);
                  return 1;
                }
              else{
                          curl_easy_cleanup(curl);
                  return 0;}
            }
        } else {
          std::cout << "somthing went wrong" << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return 69;
}

int install(std::string package_name,int is_update){
      std::filesystem::path cur_dir = std::filesystem::current_path();

    std::string filename = package_name + ".tar.xz";
    std::filesystem::path destination_cache = cur_dir / ".cache" / filename;
    std::filesystem::path temp_dir = cur_dir / ".temp";

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
      return 0;
    }
    std::filesystem::path path_to_version = temp_dir / ".version";

    std::cout << "Installing " << package_name << std::endl;
    std::ifstream version(path_to_version.c_str());
    if (!version.is_open()) {
      std::cout << "❌Couldn't check the version of the package. Installation "
                   "Aborted."
                << std::endl;
    }
    std::string version_str;
    std::getline(version,version_str);
    version.close();


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


    
    if(!is_update){
            std::ofstream registry("registry");
            std::string new_package = package_name + "|" + version_str + ";";
            registry << new_package << "\n";
            registry.close();
            std::cout << "✅ " << package_name << " succesfully added to the registry"
                      << std::endl;
            std::cout << "Clearing..." << std::endl;
            std::filesystem::remove_all(temp_dir);
            std::filesystem::create_directory(temp_dir);
            std::cout << "✅ " << package_name
                      << " was successfully installed on your computer." << std::endl;
            return 0;

    } else{
    std::string input_file = "registry"; // Fichier à traiter
    std::string output_file = "registry.temp";    // Fichier temporaire
        std::string line;
    std::string prefix = package_name + "|";
    std::ifstream infile(input_file);
    if (!infile.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << input_file << std::endl;
        return 1;
    }

    std::ofstream outfile(output_file);
    if (!outfile.is_open()) {
        std::cerr << "Impossible de créer le fichier " << output_file << std::endl;
        return 1;
    }
      while (std::getline(infile, line)) {
        if (line.rfind(prefix, 0) != 0) { // Vérifie que la ligne NE commence PAS par prefix
            outfile << line << "\n";
        } else{
            outfile << package_name << "|" << version_str << ";" << "\n";
        }
    }
        infile.close();
    outfile.close();
    if (std::remove(input_file.c_str()) != 0) {
        std::cerr << "Erreur lors de la suppression du fichier original\n";
        return 1;
    }

    if (std::rename(output_file.c_str(), input_file.c_str()) != 0) {
        std::cerr << "Erreur lors du renommage du fichier filtré\n";
        return 1;
    }
    }
                std::cout << "✅ " << package_name << " succesfully added to the registry"
                      << std::endl;
            std::cout << "Clearing..." << std::endl;
            std::filesystem::remove_all(temp_dir);
            std::filesystem::create_directory(temp_dir);
            std::cout << "✅ " << package_name
                      << " was successfully installed on your computer." << std::endl;
                      return 0;
    
}