#define CROW_STATIC_DIRECTORY "www/static/"

#include "crow.h"
#include <filesystem>

void webserver_start()
{
    // Get the path to the "www" directory relative to the executable
    std::string www_path = std::filesystem::current_path().string() + "/www/";
    crow::SimpleApp app;
    crow::mustache::set_base("./www/");
    crow::mustache::set_global_base("./www/");
    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([]() {
        auto page = crow::mustache::load("index.html").render();
        return page;
    });
    app.add_static_dir();
    app.bindaddr("127.0.0.1")
    .port(10727)
    .multithreaded()
    .run();
}