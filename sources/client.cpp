#include <vk/client.hpp>


std::mutex lock;
std::vector<bool> notified;
std::queue<nlohmann::json> q_items;
std::condition_variable check;


namespace Vk
{
    auto Client::print_groups(const json& groups, bool f) -> bool
    {
        if (!groups.empty())
        {
            size_t g_count = groups["count"];
            std::cout << "TOTAL GROUPS COUNT: " << g_count << std::endl;
            if (g_count == 0) return true;
            json items = groups["items"];
            for (json::iterator it = items.begin(); it != items.end(); ++it) q_items.push(*it);
            int _n = 0;
            while (!_n)
            {
                std::cout << "Threads number: ";
                if ((std::cin >> _n) && ((_n < 1) || (_n > std::thread::hardware_concurrency())))
                {
                    std::cout << "Wrong threads number!" << std::endl;
                    _n = 0;
                }
            }
            notified.push_back(true);
            for(int i = 0; i < (_n-1); ++i)
            {
                notified.push_back(false);
            }
            std::vector<std::thread> threads;
            for(int i = 0; i < _n; ++i)
            {
                threads.push_back(std::thread(thread_safe_print, i, _n, f));
            }
            for(auto& t: threads)
                if (t.joinable()) t.join();
            return true;
        }
        return false;
    }

    auto Client::thread_safe_print(int ind, int n, bool f) -> void
    {
        unsigned int start_time;
        if (f) start_time = clock();
        while (1)
        {
            std::unique_lock<std::mutex> locker(lock);
            while (!notified.at(ind))
            {
                check.wait(locker);
            }
            if (q_items.empty())
            {
                if (f)
                {
                    std::cout << std::endl;
                    std::cout << std::this_thread::get_id() << " :" << std::endl;
                    std::cout << "Starting time: " << start_time << std::endl;
                    unsigned int end_time = clock();
                    std::cout << "Ending time: " << end_time << std::endl;
                }
                return;
            }
            while ((!q_items.empty()) && (notified.at(ind)))
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << std::endl;
                if (f)
                {
                    std::cout << "THREAD_ID " << std::this_thread::get_id() << " :" << std::endl;
                    std::cout << std::endl;
                }
                std::string buf1;
                int buf2;
                buf2 = q_items.front()["id"];
                std::cout << "ID: " << buf2 << std::endl;
                buf1 = q_items.front()["name"];
                std::cout << "NAME: " << buf1 << std::endl;
                buf1 = q_items.front()["type"];
                std::cout << "TYPE: " << buf1 << std::endl;
                buf2 = q_items.front()["is_closed"];
                std::cout << "IS CLOSED: " << (buf2 == 1 ? "TRUE" : "FALSE") << std::endl;
                q_items.pop();
                notified.at(ind) = false;
                if (ind == (n-1)) notified.at(0) = true;
                else notified.at(ind+1) = true;
                if (q_items.empty())
                    for (int i = 0; i < n; ++i) notified.at(i) = true;
                check.notify_all();
            }
        }
    }

    auto Client::check_connection() -> bool
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle)
        {
            std::string post_fields = "access_token=" + _settings["token"] + "&v=5.60";
            std::string external_data;
            curl_easy_setopt(easy_handle, CURLOPT_URL, "https://api.vk.com/method/account.getInfo?");
            curl_easy_setopt(easy_handle, CURLOPT_POST, 1);
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_fields.c_str());
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_fields.length());
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &external_data);
            if (curl_easy_perform(easy_handle) == CURLE_OK)
            {
                json server_answer = json::parse(external_data);
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
                    curl_easy_cleanup(easy_handle);
                    throw server_error("Server answered with error: ", error);
                }
            }
            curl_easy_cleanup(easy_handle);
            throw Client_except("Something wrong with easy performing!");
        }
        throw Client_except("Something wrong with initialization!");
    }

    auto Client::get_groups(size_t count) -> json
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle)
        {
            std::string post_fields = "extended=1&offset=0&count=" + std::to_string(count) + "&access_token=" + _settings["token"] + "&v=5.60";
            std::string external_data;
            curl_easy_setopt(easy_handle, CURLOPT_URL, "https://api.vk.com/method/groups.get?");
            curl_easy_setopt(easy_handle, CURLOPT_POST, 1);
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_fields.c_str());
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_fields.length());
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &external_data);
            if (curl_easy_perform(easy_handle) == CURLE_OK)
            {
                json server_answer = json::parse(external_data);
                json response = server_answer["response"];
                if (!response.empty()) return response;
                else
                {
                    json error = server_answer["error"];
                    curl_easy_cleanup(easy_handle);
                    throw server_error("Server answered with error!", error);
                }
            }
            curl_easy_cleanup(easy_handle);
            throw Client_except("Something wrong with easy performing!");
        }
        throw Client_except("Something wrong with initialization!");
    }

    auto Client::write_data(char* buffer, size_t size, size_t nmemb, std::string* userp) -> size_t
    {
        size_t real_size = size * nmemb;
        *userp += buffer;
        return real_size;
    }
}
