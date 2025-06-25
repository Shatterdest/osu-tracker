#if defined(_WIN32)
extern "C" int ui_main(void);
#elif defined(__linux__)
extern "C" int ui_main(void);
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
