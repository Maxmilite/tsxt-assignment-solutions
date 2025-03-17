#include <iostream>
#include <string>

#include "utils/parser.h"
#include "renderer/renderer.h"
#include "objects/objects.h"
#include "utils/yamlLoader.h"

int main(int argc, char* argv[]) {

	ArgParser parser(argc, argv);

	bool verbose = parser.get<bool>("-v");
	bool trace = parser.get<bool>("-t");
	if (trace) {
		spdlog::set_level(spdlog::level::trace);
	} else if (verbose) {
		spdlog::set_level(spdlog::level::debug);
	} else {
		spdlog::set_level(spdlog::level::info);
	}

	auto configPath = parser.get<std::string>("-c");

	spdlog::info("Loading Config...");
	spdlog::info("Config Path: {}", configPath);

	auto& globalConfig = getGlobalConfig();
	globalConfig.load(configPath);

	spdlog::info("Launching Renderer...");

	auto& renderer = getRenderer();
	renderer.init();
	
	while (!renderer.shouldClose()) {
		renderer.render();
	}
	
	return 0;
}