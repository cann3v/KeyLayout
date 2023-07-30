#include "main.h"

int main(int argc, char** argv) {
    plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::info, &consoleAppender);

    int key;

    cxxopts::Options options("KeyLayout", "A brief description");

    options.add_options()
        ("k,key", "Key to switch layout", cxxopts::value<int>())
        ("i,interactive", "Enter to the interactive mode")
        ("d,daemon", "Run in daemon mode (no window)")
        ("v,verbose", "Verbose output")
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("verbose"))
        plog::get()->setMaxSeverity(plog::verbose);

    if (result.count("key"))
        key = result["key"].as<int>();
    else {
        PLOGF << "No key specified. Please see usage (-h/--help)";
        exit(1);
    }

    if (result.count("daemon")) {
        PLOGV << "Entering daemon mode";
        daemon_mode(key);
    }

    if (result.count("interactive")) {
        PLOGV << "Entering interactive mode";
        interactive_mode(key);
    }

    SetHook(key);

    return 0;
}
