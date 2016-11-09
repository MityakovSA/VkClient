#include <string>
#include <map>

namespace Vk
{
    class Client
    {
    public:
        using dict_t = std::map<std::string, std::string>;
        Client(dict_t settings) : _settings(settings) {};
        auto check_connection() -> bool;
        auto get_groups(size_t count) -> void;
    private:
        dict_t _settings;
        static auto write_data(char* buffer, size_t size, size_t nmemb, std::string& userp) -> size_t;
    };
}