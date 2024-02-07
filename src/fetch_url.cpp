#include "fetch_url.hpp"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int url2file(std::string url, std::string filename) {
  CURL *curl_handle;
  FILE *pagefile;
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
  pagefile = fopen(filename.c_str(), "wb");
  if (pagefile) {
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
    curl_easy_perform(curl_handle);
    fclose(pagefile);
  }
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
  return 0;
}
