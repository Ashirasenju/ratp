#include "update.h"
#include "curl/curl.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *out) {
  out->append((char *)contents, size * nmemb);
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
      return 1;
    }
  }
  int return_value = extractHasToUpdate(response);
  std::cout << return_value << std::endl;
  if (return_value == 0 || return_value == 1) {
    return return_value;
  } else {
    return -4;
  }
}

void download(std::string package_name) {}
int update() {
  std::filesystem::path cur_dir = std::filesystem::current_path();
  std::ifstream registry("registry");
  std::string line;
  std::string package_name = "";
  std::string version = "";
  while (getline(registry, line)) {
    package_name = line.substr(0, line.find("|"));
    version = line.substr(line.find("|") + 2, 5);
    int status_update = isLatest(package_name, version);
    std::cout << status_update;
    if (status_update == -4) {
      std::cout << package_name
                << " couldn't be found in the distant server ..." << std::endl;
    } else if (status_update) {
      std::cout << package_name << " has to update" << std::endl;
    }
  }
  return 0;
}
