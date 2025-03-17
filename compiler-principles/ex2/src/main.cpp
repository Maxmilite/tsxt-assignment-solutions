#include "./common.h"
#include "./tokenizer/tokenizer.h"
#include "./lr1_parser/lr1_parser.h"

int main(int argc, char* argv[]) {

    LR1::LR1Parser parser;
    
    std::string input;
    while (std::cin) {
        std::string line;
        std::getline(std::cin, line);
        input += line + '\n';
    }

    try {
        Tokenizer tokenizer(input);
        std::vector<std::string> tokens;

        std::vector<std::string> numAndId;

        while (tokenizer.hasNext()) {
            std::string token = tokenizer.nextToken();
            if (token.empty()) {
                break;
            }
            auto type = tokenizer.getType(token);
            tokens.push_back(type);

            // std::cerr << token << ' ' << type << '\n';

            if (type == "IDENT") {
                numAndId.push_back(token);
            } else if (type == "UINT") {
                numAndId.push_back(std::to_string(std::stoi(token)));
            } else if (type == "UDOUBLE") {
                numAndId.push_back(std::to_string(std::stod(token)));
            }
            
        }

        // std::cerr << tokens.size() << '\n';
        // std::cerr << '[';
        // for (auto& token : tokens) {
        //     std::cerr << token << ", ";
        // }
        // std::cerr << ']' << '\n';

        parser.setInput(tokens);
        parser.numAndId = numAndId;
        parser.parse();

    } catch (const std::runtime_error& e) {
        std::cout << "Syntax Error" << '\n';
        exit(0);
        // std::cerr << e.what() << '\n';
    }

    parser.printQuadruples();

    return 0;
}