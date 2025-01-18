#define CROW_STATIC_DIRECTORY "www/static/"
#include "crow.h"
#include <filesystem>
crow::SimpleApp app;

class CustomLogger : public crow::ILogHandler {
public:
    CustomLogger() {}
    void log(std::string message, crow::LogLevel /*level*/) {
        // "message" doesn't contain the timestamp and loglevel
        // prefix the default logger does and it doesn't end
        // in a newline.
        //std::cerr << message << std::endl;
        
        // taken from crow documentation -> disable log
    }
};

auto webserver_start()
{
    CustomLogger logger;
    crow::logger::setHandler(&logger);
    // Get the path to the "www" directory relative to the executable
    std::string www_path = std::filesystem::current_path().string() + "/www/";
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
        .multithreaded();

    return app.run_async();

}