#define CROW_STATIC_DIRECTORY "www/static/"
#define CROW_ENFORCE_WS_SPEC

#include "crow.h"
#include <filesystem>
#include "json.hpp"
#include <unordered_set>
#include <mutex>

using json = nlohmann::json;
crow::SimpleApp app;

class CustomLogger : public crow::ILogHandler {
public:
	CustomLogger() {}
	void log(std::string message, crow::LogLevel) {
		// "message" doesn't contain the timestamp and loglevel
		// prefix the default logger does and it doesn't end
		// in a newline.
		std::cerr << message << std::endl;

		// taken from crow documentation -> disable log
	}
};

int valuei = 1000;

std::mutex ws_mutex;
std::unordered_set<crow::websocket::connection*> clients;

void update_mustache()
{
	std::lock_guard<std::mutex> lock(ws_mutex);

	json j;
	j["valuei"] = valuei;

	std::string msg = j.dump();

	for (auto client : clients)
	{
		client->send_text(msg);
	}
}

crow::SimpleApp webserver_start()
{
	CustomLogger logger;
	crow::logger::setHandler(&logger);
	// Get the path to the "www" directory relative to the executable
	std::string www_path = std::filesystem::current_path().string() + "/www/";
	crow::mustache::set_base("./www/");
	crow::mustache::set_global_base("./www/");

	CROW_WEBSOCKET_ROUTE(app, "/ws/mustache")
	.onopen([&](crow::websocket::connection& conn) {
		// Add & Open WS Connection
		std::lock_guard<std::mutex> _(ws_mutex);
		clients.insert(&conn);

		// Send initial state
		json j;
		j["valuei"] = std::to_string(valuei);
		conn.send_text(j.dump());
	})

	.onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t) {
		// Close Websocket Connection
		std::lock_guard<std::mutex> _(ws_mutex);
		clients.erase(&conn);
	});

	//define your endpoint at the root directory
	CROW_ROUTE(app, "/")([]() {
		crow::mustache::context ctx;
		ctx["valuei"] = std::to_string(valuei);
		auto page = crow::mustache::load("index.html").render(ctx);
		return page;
	});
	app.add_static_dir();

	app.bindaddr("127.0.0.1")
		.port(10727)
		.multithreaded();
	return app.run_async();
}
