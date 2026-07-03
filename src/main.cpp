#include "nde/AppConfig.hpp"
#include "nde/Engine.hpp"

#include <filesystem>
#include <iostream>

int main(int argc, char** argv) {
    const std::filesystem::path configPath = argc > 1
        ? std::filesystem::path(argv[1])
        : std::filesystem::path("config/ndengine.local.json");

    try {
        auto config = nde::AppConfig::loadFromFile(configPath);
        nde::Engine engine(std::move(config));
        return engine.run();
    } catch (const std::exception& ex) {
        std::cerr << "Native Deployment Engine failed to start: " << ex.what() << '\n';
        return 1;
    }
}
