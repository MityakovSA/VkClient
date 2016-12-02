#include <vk/client.hpp>


int main()
{
    std::string token = "";
    std::string buf = "";
    //std::cout << "Enter access_token that you can get by following this URL:" << std::endl;
    //std::cout << "https://oauth.vk.com/authorize?client_id=5719838&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=friends&response_type=token&v=5.60" << std::endl;
    //std::cout << "access_token: ";
    if (std::cin >> buf) token = buf;
    Vk::Client client({{"token", token}});
    try
    {
        if (client.check_connection())
        {
            std::cout << "Connected!" << std::endl;
            std::string com = "";
            //std::cout << ">> ";
            while (std::getline(std::cin, com))
            {
                if (!(com == ""))
                {
                    if (com == "close") return 0;
                    else if (com == "get_groups") Vk::Client::print_groups(client.get_groups(5), false);
                    else if (com == "get_groups -v") Vk::Client::print_groups(client.get_groups(5), true);
                    else std::cout << "Wrong command!" << std::endl;
                    //std::cout << ">> ";
                }
            }
        }
        else std::cout << "Didn't connect!" << std::endl;
    }
    catch(std::invalid_argument& parse_except)
    {
        std::cout << parse_except.what() << std::endl;
    }
    catch(Vk::Client::server_error& error)
    {
        std::cout << error.what() << error._error << std::endl;
    }
    catch(Vk::Client::Client_except& client_except)
    {
        std::cout << client_except.what() << std::endl;
    }
    return 0;
}

