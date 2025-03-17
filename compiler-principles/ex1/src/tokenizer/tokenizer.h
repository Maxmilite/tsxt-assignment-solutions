#pragma once

#include "../common.h"

class Tokenizer {
public:
    Tokenizer(const std::string& input) : input(input), pos(0) {}
    std::string nextToken();
    std::string getType(const std::string& token);
    bool hasNext();

private:
    std::string input;
    int pos;
};