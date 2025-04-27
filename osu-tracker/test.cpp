#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include <iostream> 
#include <cpr/cpr.h>

TEST_CASE("GET request to Google", "[network]") {

    auto response = cpr::Get(cpr::Url{"https://api.kirino.sh/inspector/users/stats/13817114?skipDailyData=true&skipOsuData=true&skipExtras=true"});
    std::cout << response.text << "\n";
    std::cout << response.raw_header << "\n";
    std::cout << response.reason << "\n";
    std::cout << response.redirect_count << "\n";
    // Check that the HTTP status code is 200 (OK)
    REQUIRE(response.status_code == 200);

    // Check that the response body is not empty
    REQUIRE_FALSE(response.text.empty());
}