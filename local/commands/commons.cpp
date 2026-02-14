#include <string>
#include <filesystem>
#include "curl/curl.h"
#include <iostream>



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
    return 0;
}