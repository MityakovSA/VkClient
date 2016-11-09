#include <vk/client.hpp>
#include <curl/curl.h>
#include <vk/json.hpp>

namespace Vk
{
    using json = nlohmann::json;
    auto Client::check_connection() -> bool
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle)
        {
            std::string post_fields = "access_token=" + _settings["token"] + "&v=5.60";
            std::string internal_struct;
            curl_easy_setopt(easy_handle, CURLOPT_URL, "https://api.vk.com/method/account.getInfo?");
            curl_easy_setopt(easy_handle, CURLOPT_POST, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_fields.c_str());
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_fields.length());
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, internal_struct);
            if (curl_easy_perform(easy_handle) == CURLE_OK)
            {
                json server_answer = json::parse(internal_struct);
                json response = server_answer["response"];
                if (!response.empty())
                {
                    std::cout << "RESPONSE: " << response << std::endl;
                    curl_easy_cleanup(easy_handle);
                    return true;
                }
                else
                {
                    json error = server_answer["error"];
                    std::cout << "ERROR: " << error << std::endl;
                    curl_easy_cleanup(easy_handle);
                    return false;
                }
            }
            std::cout << "Something wrong with easy performing!" << std::endl;
            curl_easy_cleanup(easy_handle);
            return false;
        }
        std::cout << "Something wrong with initialization!" << std::endl;
        return false;
    }

    static auto write_data(char* buffer, size_t size, size_t nmemb, std::string& userp) -> size_t
    {
        size_t real_size = size * nmemb;
        userp += buffer;
        return real_size;
    }
}
