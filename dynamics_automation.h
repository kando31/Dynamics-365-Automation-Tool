#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include "csv.hpp"

using namespace csv;

std::string api_key = "FQOtBZ6XA7isAreOz1VDbMo6S05IP6ubYtcHZRdDAypeaGgrekykeyc0Wus5ulOBoekT7bptReqQvmUcyrTMe8RIZ6a0v7wwPo2XaziDMtEm4YCmRU8sv";

size_t WriteCallBack(void* contents, size_t size, size_t nmemb, void* userp){
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

std::string get_asset_id_by_serial(const std::string& serial_number, const std::string& api_key)
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl\n";
        return "";
    }

    std::string url = "https://eom-api.egate.net/v2/dynamics/asset/serial/" + serial_number;
    std::string response;

    struct curl_slist* headers = nullptr;
    std::string auth_header = "X-EGN-AUTH: " + api_key;
    headers = curl_slist_append(headers, auth_header.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        return "";
    }

    if (response_code != 200) {
        std::cerr << "Error: HTTP status " << response_code << "\n";
        std::cerr << "Response: " << response << "\n";
        return "";
    }

    try {
        auto json = nlohmann::json::parse(response);
        if (json.contains("new_assetid")) {
            return json["new_assetid"].get<std::string>();
        } else {
            std::cerr << "JSON does not contain 'new_assetid'\n";
            return "";
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return "";
    }
}

std::string get_site_id_by_prov_task(const std::string& site_id, const std::string& api_key){
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl\n";
        return "";
    }

    std::string url = "https://eom-api.egate.net/v3/dynamics/provisioning/task/" + site_id;
    std::string response;

    struct curl_slist* headers = nullptr;
    std::string auth_header = "X-EGN-AUTH: " + api_key;
    headers = curl_slist_append(headers, auth_header.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        return "";
    }

    if (response_code != 200) {
        std::cerr << "Error: HTTP status " << response_code << "\n";
        std::cerr << "Response: " << response << "\n";
        return "";
    }

    try {
        auto json = nlohmann::json::parse(response);
        if (json.contains("_new_customersiteid_value")) {
            return json["_new_customersiteid_value"].get<std::string>();
        } else {
            std::cerr << "JSON does not contain 'new_assetid'\n";
            return "";
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return "";
    }
}

void update_hardware_in_dynamics(const std::string& asset_id, const std::string& asset_tag, const std::string& site_ID){
    // std::string url = "https://eom-api.egate.net/v2/dynamics/assets/{asset_id}/{site_id}/{asset_tag}"
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl\n";
    }

    std::string url = "https://eom-api.egate.net/v2/dynamics/assets/" + asset_id + "/" + site_ID + "/" + asset_tag;
    std::cout << url << "\n";
    std::string response;

    struct curl_slist* headers = nullptr;
    std::string auth_header = "X-EGN-AUTH: " + api_key;
    headers = curl_slist_append(headers, auth_header.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); // Set the method to PUT
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
    }

    if (response_code != 200) {
        std::cerr << "Error: HTTP status " << response_code << "\n";
        std::cerr << "Response: " << response << "\n";
    }
}
void update_hardware_dispostion(const std::string& asset_id, const std::string& disposition){
    // std::string url = "https://eom-api.egate.net/v2/dynamics/assets/{asset_id}/{site_id}/{asset_tag}"
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl\n";
    }

    std::string url = "https://eom-api.egate.net/v3/dynamics/asset/" + asset_id + "/disposition/" + disposition;
    std::string response;

    struct curl_slist* headers = nullptr;
    std::string auth_header = "X-EGN-AUTH: " + api_key;
    headers = curl_slist_append(headers, auth_header.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); // Set the method to PUT
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
    }

    if (response_code != 200) {
        std::cerr << "Error: HTTP status " << response_code << "\n";
        std::cerr << "Response: " << response << "\n";
    }

}