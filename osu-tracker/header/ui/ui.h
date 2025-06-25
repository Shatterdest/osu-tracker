#pragma once

struct userC {
    const char* username;
    const char* avatar;
};

struct dataEntryC {
    const char* key;
    const char* name;
    int sort;
    const char* init;
    const char* current;
    const char* change;
    int dataType;
    int formatType;
    bool noDiff;
    bool display;
    bool banchoSupport;
    bool titanicSupport;
};

dataEntryC userToDataEntryC(const config::user& u) {
    dataEntryC entry{};
    entry.key = "username";
    entry.name = u.username.c_str();
    entry.sort = 0;
    entry.init = "";
    entry.current = "";
    entry.change = "";
    entry.dataType = 0;    // e.g. _string
    entry.formatType = 0;  // e.g. f_string
    entry.noDiff = false;
    entry.display = true;
    entry.banchoSupport = false;
    entry.titanicSupport = false;
    return entry;
}

dataEntryC* convertEntriesToC(const std::vector<config::dataEntry>& vec, size_t& outCount) {
    outCount = vec.size();
    dataEntryC* out = new dataEntryC[outCount];
    const auto& app = config::application::instance();

    for (size_t i = 0; i < vec.size(); ++i) {
        out[i].key = vec[i].key.c_str();
        out[i].name = vec[i].name.c_str();
        out[i].sort = vec[i].sort;
        out[i].init = vec[i].init.c_str();
        out[i].current = vec[i].current.c_str();
        out[i].change = vec[i].change.c_str();
        out[i].dataType = static_cast<int>(vec[i].dataType);
        out[i].formatType = static_cast<int>(vec[i].formatType);
        out[i].noDiff = vec[i].noDiff;
        out[i].display = vec[i].display;
        out[i].banchoSupport = (app.server == config::server::bancho);
        out[i].titanicSupport = (app.server == config::server::titanic);
    }
    return out;
}

userC sharedUser;
dataEntryC* sharedEntries = nullptr;
size_t sharedCount = 0;


#if defined(_WIN32)
extern "C" int ui_main(const struct userC* user, const struct dataEntryC* entries, size_t count);
#elif defined(__linux__)
extern "C" int ui_main(const struct userC* user, const struct dataEntryC* entries, size_t count);
#endif
class ui {
    public:
    // Function to update shared data (must keep data alive, careful with lifetime)
    static void updateSharedData(const config::user& user, const std::vector<config::dataEntry>& entries) {
        sharedUser.username = user.username.c_str();
        sharedUser.avatar = user.avatar.c_str();

        delete[] sharedEntries;
        sharedEntries = convertEntriesToC(entries, sharedCount);
    }

    static int open(const config::user& u, const std::vector<config::dataEntry>& entries) {
        size_t count = 0;
        dataEntryC* cEntries = convertEntriesToC(entries, count);

        userC cUser{
            u.username.c_str(),
            u.avatar.c_str()
        };

        int result = ui_main(&cUser, cEntries, count);

        delete[] cEntries;
        return result;
    }

    int close() {
        // You would implement this depending on your platform logic
        return 0;
    }
};
