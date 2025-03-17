#include "./common.h"
#include "./tokenizer/tokenizer.h"

int main(int argc, char* argv[]) {
    std::string input;
    while (std::cin) {
        std::string line;
        std::getline(std::cin, line);
        input += line + '\n';
    }

    try {
        Tokenizer tokenizer(input);
        std::vector<std::pair<std::string, std::string>> tokens;
        while (tokenizer.hasNext()) {
            std::string token = tokenizer.nextToken();
            if (token.empty()) {
                break;
            }
            tokens.emplace_back(token, tokenizer.getType(token));
        }

        for (const auto& [token, type] : tokens) {
            std::cout << token << " " << type << '\n';
        }
        
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << '\n';
    }

    return 0;

}