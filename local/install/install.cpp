#include "install.h"
#include "curl/curl.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
int install(char *argv[]) {
  const std::string base_url =
      "http://127.0.0.1:66/ratp/download_latest?package_name=";

  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Erreur: impossible d'initialiser libcurl\n";
    return 1;
  }

  for (size_t i = 1; i < sizeof(argc); ++i) {
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
  }

  curl_easy_cleanup(curl);
  std::ofstream registry("../.cache/package_list");
  registry << "package_name";
  registry.close();
  return 0;
}
