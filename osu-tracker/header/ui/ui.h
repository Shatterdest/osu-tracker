#if defined(_WIN32)
extern "C" int ui_main(void); // Declare the C function from ui_windows.h
#elif defined(__linux__)
extern "C" int ui_main(void); // Declare the Linux version if applicable
#endif
class ui {
    public:
    static int open() {
        return ui_main(); // Run the platform-specific UI loop
    }

    int close() {
        // You would implement this depending on your platform logic
        return 0;
    }
};
