#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
  CURL *curl;
  CURLcode res;
  string readBuffer;
  long responseCode = 0;
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://date.jsontest.com/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    curl_easy_cleanup(curl);


    cout << "Response Code: "<<responseCode << endl;
  }

  if (responseCode == 200){
    cout << "PageSource " << endl << readBuffer << endl;

    Json::Value jsonData;
    Json::Reader jsonReader;

    if (jsonReader.parse(readBuffer,jsonData)){
      cout << "Successfully parsed JSON data" << endl;
      cout << "\nJSON data received:" << endl;
      //cout << jsonData.toStyledString() << endl;
      const string dateString (jsonData["date"].asString());
      cout << "\t Date String: " << dateString << endl;

      const string timeString (jsonData["time"].asString());
      cout << "\t time String: " << timeString << endl;

      const long int unipochString (jsonData["milliseconds_since_epoch"].asUInt64());
      cout << "\t unixepoch String: " << unipochString << endl;
    }
  }
  return 0;
}
