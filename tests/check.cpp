#include <vk/client.hpp>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"


SCENARIO("Client must connect to server via access_token")
{
    GIVEN("Invalid token")
    {
        std::string token = "qwerty";
        Vk::Client client({{"token", token}});
        WHEN("Trying to connect")
        {
            THEN("check_connection() must throw exception")
            {
                REQUIRE_THROWS_AS(client.check_connection(), Vk::Client::server_error);
            }
        }
    }

    GIVEN("Valid token")
    {
        std::string token;
        std::cout << "access_token: ";
        std::cin >> token;
        Vk::Client client({{"token", token}});
        WHEN("Trying to connect")
        {
            THEN("check_connection() must return true")
            {
                REQUIRE(client.check_connection() == true);
            }
        }
    }
}


SCENARIO("get_groups() must throw exception something goes wrong")
{
    GIVEN("Invalid token")
    {
        std::string token = "qwerty";
        Vk::Client client({{"token", token}});
        WHEN("Trying to get groups list")
        {
            THEN("get_groups() must throw exception")
            {
                REQUIRE_THROWS_AS(client.get_groups(3), Vk::Client::server_error);
            }
        }
    }
}


SCENARIO("get_groups() must get correct list of groups with their specifications")
{
    GIVEN("Correct groups list")
    {
        Vk::json check = Vk::json::parse(R"({"count":11,"items":[{"id":125677403,"is_closed":0,"name":"SKILLET | UNLEASHED TOUR 2016 | RUSSIA 22/23.11","photo_100":"https://pp.vk.me/c637230/v637230114/15ebc/unaMAhpi4D8.jpg","photo_200":"https://pp.vk.me/c637230/v637230114/15ebb/s-bX_rtuhKs.jpg","photo_50":"https://pp.vk.me/c637230/v637230114/15ebd/HswUKzs8GXQ.jpg","screen_name":"skilletrussia2016","type":"event"},{"id":11204681,"is_closed":0,"name":"ASUS Россия","photo_100":"https://pp.vk.me/c631627/v631627255/46613/LCRh59fC62Q.jpg","photo_200":"https://pp.vk.me/c631627/v631627255/46612/pIKNZbTNM00.jpg","photo_50":"https://pp.vk.me/c631627/v631627255/46614/IoA4el8xPBM.jpg","screen_name":"asus","type":"group"},{"id":16647180,"is_closed":0,"name":"adidas Originals","photo_100":"https://pp.vk.me/c630625/v630625190/f5aa/qowr5ziLoU0.jpg","photo_200":"https://pp.vk.me/c630625/v630625190/f5a9/AgJuaQhKlFE.jpg","photo_50":"https://pp.vk.me/c630625/v630625190/f5ab/TlGsV4yTgCc.jpg","screen_name":"originals","type":"group"}]})");
        std::string token;
        std::cout << "access_token: ";
        std::cin >> token;
        Vk::Client client({{"token", token}});
        WHEN("Trying to get groups list")
        {
            Vk::json origin = client.get_groups(3);
            bool compare = check == origin;
            THEN("JSON objects must be equivalent")
            {
                REQUIRE(compare);
            }
        }
    }
}

