#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include <iostream> 
#include <cpr/cpr.h>

// If this test fails
// 1. No Internet
// 2. SSL certificate issue
// 3. curl is built without SSL/HTTPS support
TEST_CASE("GET request to Google", "[network]") {
    
    auto response = cpr::Get(cpr::Url{"https://www.google.com"});

    // Check that the HTTP status code is 200 (OK)
    REQUIRE(response.status_code == 200);

    // Check that the response body is not empty
    REQUIRE_FALSE(response.text.empty());
}