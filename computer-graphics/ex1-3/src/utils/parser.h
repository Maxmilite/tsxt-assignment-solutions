#pragma once

#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <exception>

class ArgParser {
public:
    ArgParser(int argc, char* argv[]) {
        parser.add_argument("-v", "--verbose")
            .help("Enable verbose logging")
            .default_value(false)
            .flag();
        parser.add_argument("-t", "--trace")
            .help("Enable trace logging")
            .default_value(false)
            .flag();
        parser.add_argument("-c", "--config")
            .help("Path to config file")
            .default_value(std::string("./assets/config.yml"));
        try {
            parser.parse_args(argc, argv);
        } catch (const std::exception& e) {
            spdlog::error("{}", e.what());
            exit(-1);
        }
    }

    template<typename T>
    T get(std::string_view arg_name) {
        return parser.get<T>(arg_name);
    }

private:
    argparse::ArgumentParser parser;
};