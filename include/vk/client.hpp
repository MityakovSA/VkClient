#include <string>
#include <map>
#include <string>
#include <iostream>
#include <vk/json.hpp>
#include <curl/curl.h>
#include <exception>


namespace Vk
{
    using json = nlohmann::json;

    auto print_groups(json groups) -> bool;

    class Client
    {
    public:
        using dict_t = std::map<std::string, std::string>;
        Client(dict_t settings) : _settings(settings) {};
        auto check_connection() -> bool;
        auto get_groups(size_t count) -> json;
        class Client_except : public std::runtime_error
        {
        public:
            explicit Client_except(const std::string& what_arg) : std::runtime_error(what_arg) {}
            explicit Client_except(const char* what_arg) : std::runtime_error(what_arg) {}
        };
        class server_error : public Client_except
        {
        public:
            explicit server_error(const std::string& what_arg, const json& error) : Client_except(what_arg), _error(error) {}
            explicit server_error(const char* what_arg, const json& error) : Client_except(what_arg), _error(error) {}
            json _error;
        };
    private:
        dict_t _settings;
        static auto write_data(char* buffer, size_t size, size_t nmemb, std::string& userp) -> size_t;
    };
}