#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <sqlite3.h>

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
  //CURL set-up
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

  //Database set-up
  sqlite3 *db;
  char *err_msg = 0;

  int rc = sqlite3_open("data.db", &db);

  if (rc != SQLITE_OK) {

      fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);

      return 1;
  }

  //Parsing info if httpcode 200
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
      const string unipochString (jsonData["milliseconds_since_epoch"].asString());
      cout << "\t unixepoch String: " << unipochString << endl;
      const char sendFlag = '1';
      string sql = //"DROP TABLE IF EXISTS datetime;"
                  "CREATE TABLE IF NOT EXISTS datetime(time TEXT, unixepoch TEXT, date TEXT, flag TEXT);"
                  "INSERT INTO datetime VALUES ('" + timeString + "','" + unipochString + "','" + dateString + "','" + sendFlag +"');";
                  rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);
                  if (rc != SQLITE_OK ) {
                      fprintf(stderr, "SQL error: %s\n", err_msg);
                      sqlite3_free(err_msg);
                      sqlite3_close(db);
                      return 1;
                    }
    }
  }

  sqlite3_close(db);
  return 0;
}
