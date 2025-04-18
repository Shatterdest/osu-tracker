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
	void log(std::string message, crow::LogLevel level) {

		// Get the current system time
		auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		// Convert to a time structure
		std::tm timeInfo;
		localtime_s(&timeInfo, &currentTime);

		// Format the date and time as strings
		char dateBuffer[20];
		strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);

		char timeBuffer[9];
		strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);
		setColorRGB_f(100, 100, 100);

		std::cout << (std::string)dateBuffer + " " + timeBuffer << " ";
		resetColor();
		switch (level) {
			#if OSU_TRACKER_DEBUG_BUILD == true
				case crow::LogLevel::Debug:
					std::cout << "[";
					setColor(conCol::b_defaultColor, conCol::f_white);
					std::cout << "Debug";
					resetColor();
					std::cout << "] ";
					setColorRGB_f(255, 255, 255);
					std::cout << message << "\n";
					resetColor();
					break;
				case crow::LogLevel::Info:
					std::cout << "[";
					setColor(conCol::b_defaultColor, conCol::f_cyan);
					std::cout << "Info";
					resetColor();
					std::cout << "] ";
					setColorRGB_f(255, 255, 255);
					std::cout << message << "\n";
					resetColor();
					break;
			#endif
			case crow::LogLevel::Warning: 
				std::cout << "[";
				setColor(conCol::b_defaultColor, conCol::f_yellow);
				std::cout << "Warning";
				resetColor();
				std::cout << "] ";
				setColorRGB_f(255, 255, 255);
				std::cout << message << "\n";
				resetColor();
				break;
			case crow::LogLevel::Error: 
				std::cout << "[";
				setColor(conCol::b_defaultColor, conCol::f_red);
				std::cout << "Error";
				resetColor();
				std::cout << "] ";
				setColorRGB_f(255, 255, 255);
				std::cout << message << "\n";
				resetColor();
				break;
			case crow::LogLevel::Critical: 
				setColor(conCol::b_red, conCol::f_white);
				std::cout << "[Critical]";
				std::cout << "Info";
				std::cout << "]";
				setColorRGB_f(255, 255, 255);
				std::cout << " " << message << "\n";
				resetColor();
				break;
		}
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

void webserver_start()
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

	// Page routing
	CROW_ROUTE(app, "/")([]() {
		crow::mustache::context ctx;
		ctx["valuei"] = std::to_string(valuei);
		auto page = crow::mustache::load("index.html").render(ctx);
		return page;
	});
	CROW_ROUTE(app, "/tracker")([]() {
		crow::mustache::context ctx;

		auto page = crow::mustache::load("tracker.html").render(ctx);
		return page;
	});
	CROW_ROUTE(app, "/settings")([]() {
		crow::mustache::context ctx;

		auto page = crow::mustache::load("settings.html").render(ctx);
		return page;
	});
	app.add_static_dir();

	app.bindaddr(OSU_TRACKER_WEBSERVER_IP)
		.port(OSU_TRACKER_WEBSERVER_PORT);
	app.run();
}
