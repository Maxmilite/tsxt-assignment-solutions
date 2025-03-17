#include "./tokenizer.h"
#include <algorithm>

const std::string ERR_UNR_CHR = "Unrecognizable characters.";
const std::string ERR_DEC_BGN_END = "Malformed number: Decimal point at the beginning or end of a floating point number.";
const std::string ERR_DEC_MOR_ONE = "Malformed number: More than one decimal point in a floating point number.";
const std::string ERR_LEA_ZER = "Malformed number: Leading zeros in an integer.";

bool Tokenizer::hasNext() {
    return pos < input.size();
}

std::string Tokenizer::getType(const std::string& token) {

    if (!token.length()) {
        return "";
    }

    if (token == "int") {
        return "INTSYM";
    }
    if (token == "double") {
        return "DOUBLESYM";
    }
    if (token == "scanf") {
        return "SCANFSYM";
    }
    if (token == "printf") {
        return "PRINTFSYM";
    }
    if (token == "if") {
        return "IFSYM";
    }
    if (token == "then") {
        return "THENSYM";
    }
    if (token == "while") {
        return "WHILESYM";
    }
    if (token == "do") {
        return "DOSYM";
    }
    if (token == "+") {
        return "PLUS";
    }
    if (token == "-") {
        return "MINUS";
    }
    if (token == "*") {
        return "TIMES";
    }
    if (token == "/") {
        return "DIVISION";
    }
    if (token == ",") {
        return "COMMA";
    }
    if (token == "(") {
        return "LBRACE1";
    }
    if (token == ")") {
        return "RBRACE1";
    }
    if (token == "{") {
        return "LBRACE2";
    }
    if (token == "}") {
        return "RBRACE2";
    }
    if (token == ";") {
        return "SEMICOLON";
    }
    if (token == "&&") {
        return "AND";
    }
    if (token == "||") {
        return "OR";
    }
    if (token == "!=") {
        return "NOTEQU";
    }
    if (token == "==") {
        return "EQUEQU";
    }
    if (token == ">") {
        return "GREATER";
    }
    if (token == "<") {
        return "LESS";
    }
    if (token == ">=") {
        return "GE";
    }
    if (token == "<=") {
        return "LE";
    }
    if (token == "!") {
        return "NOT";
    }
    if (token == "=") {
        return "EQU";
    }

    // double
    if (token.find('.') != std::string::npos) {
        // check if there are more than one decimal point in the token
        if (std::count(token.begin(), token.end(), '.') > 1) {
            throw std::runtime_error(ERR_DEC_MOR_ONE);
        }
        
        // check if decimal point is at the beginning or end of the token
        if (token.front() == '.' || token.back() == '.') {
            throw std::runtime_error(ERR_DEC_BGN_END);
        }

        return "UDOUBLE";
    }

    // int
    if (std::all_of(token.begin(), token.end(), ::isdigit)) {
        // check if there are leading zeros in the token
        if (token.size() > 1 && token.front() == '0') {
            throw std::runtime_error(ERR_LEA_ZER);
        }

        return "UINT";
    }

    // identifier
    if (std::all_of(token.begin(), token.end(), [](char c) -> bool { return std::isalnum(c) || c == '_'; })
        && !std::isdigit(token.at(0))) {
        return "IDENT";
    }

    throw std::runtime_error(ERR_UNR_CHR);
}

std::string Tokenizer::nextToken() {

    while (pos < input.size() && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == '\r')) {
        pos++;
    }

    if (pos >= input.size()) {
        return "";
    }
    
    // comments
    if (input[pos] == '/') {
        if (pos + 1 < input.size() && input[pos + 1] == '/') {
            pos += 2;
            while (pos < input.size() && input[pos] != '\n') {
                pos++;
            }
            return nextToken();
        }
        if (pos + 1 < input.size() && input[pos + 1] == '*') {
            pos += 2;
            while (pos + 1 < input.size() && !(input[pos] == '*' && input[pos + 1] == '/')) {
                pos++;
            }
            pos += 2;
            return nextToken();
        }
    }

    // reserved: .
    if (input[pos] == '.') {
        throw std::runtime_error(ERR_DEC_BGN_END);
    }

    // symbols
    {
        if (input[pos] == '&') {
            if (pos + 1 < input.size() && input[pos + 1] == '&') {
                pos += 2;
                return "&&";
            }
            return "&";
        }
        if (input[pos] == '|') {
            if (pos + 1 < input.size() && input[pos + 1] == '|') {
                pos += 2;
                return "||";
            }
            return "|";
        }
        if (input[pos] == '!') {
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                pos += 2;
                return "!=";
            }
            pos++;
            return "!";
        }
        if (input[pos] == '=') {
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                pos += 2;
                return "==";
            }
            pos++;
            return "=";
        }
        if (input[pos] == '>') {
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                pos += 2;
                return ">=";
            }
            pos++;
            return ">";
        }
        if (input[pos] == '<') {
            if (pos + 1 < input.size() && input[pos + 1] == '=') {
                pos += 2;
                return "<=";
            }
            pos++;
            return "<";
        }
        if (input[pos] == '+') {
            pos++;
            return "+";
        }
        if (input[pos] == '-') {
            pos++;
            return "-";
        }
        if (input[pos] == '*') {
            pos++;
            return "*";
        }
        if (input[pos] == '/') {
            pos++;
            return "/";
        }
        if (input[pos] == ',') {
            pos++;
            return ",";
        }
        if (input[pos] == '(') {
            pos++;
            return "(";
        }
        if (input[pos] == ')') {
            pos++;
            return ")";
        }
        if (input[pos] == '{') {
            pos++;
            return "{";
        }
        if (input[pos] == '}') {
            pos++;
            return "}";
        }
        if (input[pos] == ';') {
            pos++;
            return ";";
        }
    }

    std::string token;

    // numbers
    if (isdigit(input[pos])) {
        while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) {
            token += input[pos];
            pos++;
        }
        return token;
    }

    // identifiers
    if (isalpha(input[pos]) || input[pos] == '_') {
        while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) {
            token += input[pos];
            pos++;
        }
        return token;
    }
    
    throw std::runtime_error(ERR_UNR_CHR);
}