#define CROW_STATIC_DIRECTORY "www/static/"
#define CROW_ENFORCE_WS_SPEC

#include "crow.h"
#include <filesystem>
#include "json.hpp"
#include <unordered_set>
#include <mutex>
#include "config.h"

bool shutdown_webServer = false;

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
		#ifdef _WIN32
		localtime_s(&timeInfo, &currentTime);
		#elif __linux__
		localtime_r(&currentTime, &timeInfo);
		#endif
		// Format the date and time as strings
		char dateBuffer[20];
		strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);

		char timeBuffer[9];
		strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);
		setColorRGB_f(100, 100, 100);

		std::cout << (std::string)dateBuffer + " " + timeBuffer << " ";
		resetColor();
		switch (level) {
			#ifdef DEBUG_BUILD
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

std::mutex ws_mutex;
std::unordered_set<crow::websocket::connection*> clients;
std::unordered_set<crow::websocket::connection*> clients_settings;

void closeConnections() {
	std::lock_guard<std::mutex> lock(ws_mutex);
	for (auto client : clients) {
		client->close();
	}
	clients.clear();
	for (auto client : clients_settings) {
		client->close();
	}
	clients_settings.clear();
}

json settingsJson() {
	json _j;
	_j["ctx"] = "setting";

	_j["osu_id"] = vec_application[1][2];
	_j["client_id"] = vec_application[2][2];
	_j["client_secret"] = vec_application[3][2];
	_j["api_refreshInterval"] = vec_application[4][2];
	return _j;
}

/*
Mustache Updater
int: jsonInt
1 - mainJson
2 - settingsJson
*/
void update_mustache(int jsonInt, crow::websocket::connection& conn)
{
	std::lock_guard<std::mutex> lock(ws_mutex);
	json j;
	switch (jsonInt) {
		case 1:
			// no dynamic content
			return;
			break;
		case 2:
			conn.send_text(settingsJson().dump());
			break;
	}	
}

void restartWebServer(bool shutdown = false) {
	writeLog("Web Server termination initiated...", 255, 255, 0);
	shutdown_webServer = shutdown;
	app.stop();
}

/*
blocking function
*/
void webserver_start()
{
	CustomLogger logger;
	crow::logger::setHandler(&logger);

	// Get the path to the "www" directory relative to the executable
	std::string www_path = std::filesystem::current_path().string() + "/www/";
	crow::mustache::set_base("./www/");
	crow::mustache::set_global_base("./www/");

	CROW_WEBSOCKET_ROUTE(app, "/ws/main/")
	.onopen([&](crow::websocket::connection& conn) {
		// Add & Open WS Connection
		std::lock_guard<std::mutex> _(ws_mutex);
		clients.insert(&conn);
	})

	.onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t) {
		// Close Websocket Connection
		std::lock_guard<std::mutex> _(ws_mutex);
		clients.erase(&conn);
	})
	.onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
		std::lock_guard<std::mutex> _(ws_mutex);
		nlohmann::json j = json::parse(data);

		std::string cmd = j["cmd"];
		writeLog("Command received: " + cmd);
		
		if (cmd[0] == '#') {
			if(cmd == "#restart")
				restartWebServer();
			if(cmd == "#shutdown")
				restartWebServer(true);
		}

		/* Send back to all clients
		for (auto client : clients){
			client->send_text(data);
		}
		*/
	});

	CROW_WEBSOCKET_ROUTE(app, "/ws/settings/")
		.onopen([&](crow::websocket::connection& conn) {
			// Add & Open WS Connection
			std::lock_guard<std::mutex> _(ws_mutex);
			clients_settings.insert(&conn);

			// Send initial state
			json j = settingsJson();
			conn.send_text(j.dump());
		})

		.onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t) {
			// Close Websocket Connection
			std::lock_guard<std::mutex> _(ws_mutex);
			clients_settings.erase(&conn);
		})

		.onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
			std::lock_guard<std::mutex> _(ws_mutex);
			nlohmann::json j = json::parse(data);
			std::string cmd = j["cmd"];
			writeLog("Command received: " + cmd);

			if (cmd[0] == '#') {
				if (cmd == "#saveSettings") {
				}
				
				if (cmd == "#resetSettings") {
					rmConfig();
					nlohmann::json _j;
					_j["cmd"] = "resetSettings";
					conn.send_text(_j);
				}
			}

			/* Send back to all clients
			for (auto client : clients_settings){
				client->send_text(data);
			}
			*/
		});


	// Page routing
	CROW_ROUTE(app, "/")([]() {
		crow::mustache::context ctx;
		ctx["title"] = OSU_TRACKER_NAME;
		ctx["version"] = OSU_TRACKER_VERSION;
		ctx["hostname"] = OSU_TRACKER_WEBSERVER_IP;
		ctx["port"] = OSU_TRACKER_WEBSERVER_PORT;
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

		// navbar
		ctx["title"] = OSU_TRACKER_NAME;
		ctx["version"] = OSU_TRACKER_VERSION;
		
		// websocket connection
		ctx["hostname"] = OSU_TRACKER_WEBSERVER_IP;
		ctx["port"] = OSU_TRACKER_WEBSERVER_PORT;

		// config
		ctx["osu_id_name"] = vec_application[1][1];
		ctx["osu_id_val"] = vec_application[1][2];
		ctx["osu_id_desc"] = vec_application[1][3];

		ctx["client_id_name"] = vec_application[2][1];
		ctx["client_id_val"] = vec_application[2][2];
		ctx["client_id_desc"] = vec_application[2][3];

		ctx["client_secret_name"] = vec_application[3][1];
		ctx["client_secret_val"] = vec_application[3][2];
		ctx["client_secret_desc"] = vec_application[3][3];

		ctx["api_refreshInterval_name"] = vec_application[4][1];
		ctx["api_refreshInterval_val"] = vec_application[4][2];
		ctx["api_refreshInterval_desc"] = vec_application[4][3];

		auto page = crow::mustache::load("settings.html").render(ctx);
		return page;
	});
	
	// Info Page
	CROW_ROUTE(app, "/info")([]() {
		crow::mustache::context ctx;
		// PROJECT
		ctx["title"] = OSU_TRACKER_NAME;
		ctx["version"] = OSU_TRACKER_VERSION;
		ctx["hostname"] = OSU_TRACKER_WEBSERVER_IP;
		ctx["port"] = OSU_TRACKER_WEBSERVER_PORT;
		ctx["thread_count"] = std::to_string(app.concurrency());
		ctx["websocket_max_payload"] = std::to_string(app.websocket_max_payload());
		ctx["github"] = OSU_TRACKER_GITHUB;
		ctx["creator"] = OSU_TRACKER_CREATOR;
		ctx["profile"] = OSU_TRACKER_PROFILE;
		
		// BUILD INFO
		ctx["build_type"] = OSU_TRACKER_CMAKE_BUILD_TYPE;
		ctx["release_type"] = OSU_TRACKER_RELEASE_TYPE;

		// Libraries
		ctx["crow_version"] = crow::VERSION;
		ctx["crow_repo_url"] = CROW_REPOSITORY_URL;

		ctx["libcurl_version"] = LIBCURL_VERSION;
		ctx["libcurl_repo_url"] = LIBCURL_REPOSITORY_URL;

		ctx["asio_version"] = "10.30.02";
		ctx["asio_repo_url"] = ASIO_REPOSITORY_URL;

		ctx["cpr_version"] = CPR_VERSION;
		ctx["cpr_repo_url"] = CPR_REPOSITORY_URL;

		ctx["zlib_version"] = "Unknown";
		ctx["zlib_repo_url"] = ZLIB_REPOSITORY_URL;
		
		ctx["nlohmannJson_version"] = "3.11.2";
		ctx["nlohmannJson_repo_url"] = NLOHMANNJSON_REPOSITORY_URL;

		// CMAKE INFO
		ctx["OSU_TRACKER_CMAKE_GENERATOR"] = OSU_TRACKER_CMAKE_GENERATOR;
		ctx["OSU_TRACKER_CMAKE_CXX_STANDARD"] = OSU_TRACKER_CMAKE_CXX_STANDARD;
		ctx["OSU_TRACKER_CMAKE_VERSION"] = OSU_TRACKER_CMAKE_VERSION;
		ctx["OSU_TRACKER_CMAKE_MINIMUM_REQUIRED_VERSION"] = OSU_TRACKER_CMAKE_MINIMUM_REQUIRED_VERSION;
		ctx["OSU_TRACKER_CMAKE_CXX_COMPILER_ID"] = OSU_TRACKER_CMAKE_CXX_COMPILER_ID;
		ctx["OSU_TRACKER_CMAKE_CXX_COMPILER_VERSION"] = OSU_TRACKER_CMAKE_CXX_COMPILER_VERSION;
		ctx["OSU_TRACKER_CMAKE_CXX_COMPILER_ARCHITECTURE_ID"] = OSU_TRACKER_CMAKE_CXX_COMPILER_ARCHITECTURE_ID;
		ctx["OSU_TRACKER_CMAKE_CXX_COMPILER"] = OSU_TRACKER_CMAKE_CXX_COMPILER;
		ctx["OSU_TRACKER_CMAKE_SYSTEM_NAME"] = OSU_TRACKER_CMAKE_SYSTEM_NAME;

		
		auto page = crow::mustache::load("info.html").render(ctx);
		return page;
		});
	app.add_static_dir();

	app.bindaddr(OSU_TRACKER_WEBSERVER_IP)
		.port(OSU_TRACKER_WEBSERVER_PORT);

	while (!shutdown_webServer) {
		writeLog("Starting Web Server...");
		writeLog("Web Server should be accessible under:");
		writeLog("#####################", 255, 255, 0);
		std::string url = "http://" + (std::string)OSU_TRACKER_WEBSERVER_IP + ":" + std::to_string(OSU_TRACKER_WEBSERVER_PORT);
		writeLog(url, 0, 140, 255);
		writeLog("#####################", 255, 255, 0);
		app.multithreaded().run(); // blocking
		writeLog("Web Server Terminated" , 255, 0, 0);
		writeLog("Closing all connections...", 255, 255, 0);
		closeConnections();
		writeLog("All connections closed", 255, 255, 255);
	}
}

