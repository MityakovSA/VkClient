#include <vk/client.hpp>


namespace Vk
{
    auto print_groups(json groups) -> bool
    {
        if (!groups.empty()) {
            size_t g_count = groups["count"];
            std::cout << "TOTAL GROUPS COUNT: " << g_count << std::endl;
            if (g_count != 0) {
                size_t counter = 0;
                std::cout << "GROUPS:" << std::endl;
                json items = groups["items"];
                for (json::iterator it = items.begin(); it != items.end(); ++it) {
                    std::cout << ++counter << "." << std::endl;
                    std::string buf1;
                    int buf2;
                    buf2 = it.value()["id"];
                    std::cout << "ID: " << buf2 << std::endl;
                    buf1 = it.value()["name"];
                    std::cout << "NAME: " << buf1 << std::endl;
                    buf1 = it.value()["type"];
                    std::cout << "TYPE: " << buf1 << std::endl;
                    buf2 = it.value()["is_closed"];
                    std::cout << "IS CLOSED: " << (buf2 == 1 ? "TRUE" : "FALSE") << std::endl;
                }
            }
            return true;
        }
        return false;
    }

    auto Client::check_connection() -> bool
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle)
        {
            std::string post_fields = "access_token=" + _settings["token"] + "&v=5.60";
            std::string internal_struct;
            curl_easy_setopt(easy_handle, CURLOPT_URL, "https://api.vk.com/method/account.getInfo?");
            curl_easy_setopt(easy_handle, CURLOPT_POST, 1);
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

    auto Client::get_groups(size_t count) -> json
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle)
        {
            std::string post_fields = "extended=1&offset=0&count=" + std::to_string(count) + "&access_token=" + _settings["token"] + "&v=5.60";
            std::string internal_struct;
            curl_easy_setopt(easy_handle, CURLOPT_URL, "https://api.vk.com/method/groups.get?");
            curl_easy_setopt(easy_handle, CURLOPT_POST, 1);
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_fields.c_str());
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_fields.length());
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, internal_struct);
            if (curl_easy_perform(easy_handle) == CURLE_OK)
            {
                json server_answer = json::parse(internal_struct);
                json response = server_answer["response"];
                if (!response.empty()) return response;
                else
                {
                    json error = server_answer["error"];
                    std::cout << "ERROR: " << error << std::endl;
                    curl_easy_cleanup(easy_handle);
                    return nullptr;
                }
            }
            std::cout << "Something wrong with easy performing!" << std::endl;
            curl_easy_cleanup(easy_handle);
            return nullptr;
        }
        std::cout << "Something wrong with initialization!" << std::endl;
        return nullptr;
    }

    auto Client::write_data(char* buffer, size_t size, size_t nmemb, std::string& userp) -> size_t
    {
        size_t real_size = size * nmemb;
        userp += buffer;
        return real_size;
    }
}
