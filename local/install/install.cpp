#include "install.h"
#include "curl/curl.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
int install(char *argv[], int argc) {
  const std::string base_url =
      "http://127.0.0.1:66/ratp/download_latest?package_name=";
  std::filesystem::path cur_dir = std::filesystem::current_path();

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Erreur: impossible d'initialiser libcurl\n";
    return 1;
  }

  for (size_t i = 2; i < argc; ++i) {
    std::string package_name = argv[i];
    std::string url = base_url + package_name;
    std::string filename = package_name + ".tar.xz";

    FILE *fp = fopen(filename.c_str(), "wb");
    if (!fp) {
      std::cerr << "  ⚠️  Erreur: impossible d'ouvrir '" << filename
                << "' en écriture.\n";
      continue;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode res = curl_easy_perform(curl);
    fclose(fp);

    if (res != CURLE_OK)
      std::cerr << "  ❌ Erreur curl pour '" << package_name
                << "': " << curl_easy_strerror(res) << '\n';
    else
      std::cout << "  ✅ Téléchargement terminé → " << filename << "\n";

    try {
      std::filesystem::path downloaded_content = cur_dir / filename;
      std::filesystem::path destination_cache = cur_dir / ".cache" / filename;
      std::cout << destination_cache << std::endl;
      std::filesystem::rename(downloaded_content, destination_cache);
      std::cout << "  ✅  " << filename << " a bien été mis en cache" << "\n";

    } catch (const std::filesystem::filesystem_error &e) {
      std::cout << " ❌ " << ""
                << "n'a pas pût être mis en cache. Veuillez vérifier si le "
                   "dossier .cache existe bien"
                << std::endl;
    }

    curl_easy_cleanup(curl);
  }
  return 0;
}
