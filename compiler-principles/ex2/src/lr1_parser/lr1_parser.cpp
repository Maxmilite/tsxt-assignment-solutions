#include "./lr1_parser.h"
#include <queue>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <cassert>
#include <list>

namespace LR1 {

bool LR1Item::operator==(const LR1Item &other) const {
    return prodIndex == other.prodIndex &&
           dotPos == other.dotPos;
}

bool LR1Item::operator<(const LR1Item &other) const {
    if (prodIndex != other.prodIndex) {
        return prodIndex < other.prodIndex;
    }
    if (dotPos != other.dotPos) {
        return dotPos < other.dotPos;
    }
    return false;
}

LR1Parser::LR1Parser() {
    offset = 0;
    nxq = 0;
    tmpIndex = 0;

    loadProductions();
    getFirst();
    buildC();
    buildActionTable();
}

bool LR1Parser::isTerminal(const std::string& s) const {
    return TERMINALS.find(s) != TERMINALS.end();
}

void LR1Parser::getFirst() {
    for (auto type : TYPES) {
        if (isTerminal(type)) {
            first[type].insert(type);
        } else {
            first[type] = {};
        }
    }

    for (; ; ) {
        bool updated = false;
        for (auto& p : productions) {
            auto& lhs = p.lhs;
            auto& rhs = p.rhs;

            auto result = getFirstForCandidate(rhs);
            if (first[lhs].size() == 0 && result.size() > 0) {
                first[lhs] = result;
                updated = true;
            } else {
                for (auto& s : result) {
                    if (first[lhs].find(s) == first[lhs].end()) {
                        first[lhs].insert(s);
                        updated = true;
                    }
                }
            }
        }

        if (!updated) {
            break;
        }
    }

    // std::cerr << first.size() << '\n';
    // for (auto& [type, set] : first) {
    //     std::cerr << type << ": ";
    //     for (auto& s : set) {
    //         std::cerr << s << ", ";
    //     }
    //     std::cerr << '\n';
    // }
}

void LR1Parser::getFollow() {
    for (auto type : TYPES) {
        if (isTerminal(type)) {
            continue;
        }
        follow[type] = {};
    }

    follow["START"] = {"END"};

    for (auto& p : productions) {
        auto& lhs = p.lhs;
        auto& rhs = p.rhs;

        int k = rhs.size();
        k -= 2;

        for (int i = 0; i < k; i++) {
            auto& s = rhs[i];
            if (isTerminal(s)) {
                continue;
            }

            auto result = getFirstForCandidate({rhs.begin() + i + 1, rhs.end()});
            result.erase("ONE");
            if (result.size() > 0) {
                for (auto& t : result) {
                    follow[s].insert(t);
                }
            }
        }
    }

    for (; ; ) {
        bool updated = false;
        for (auto& p : productions) {
            auto& lhs = p.lhs;
            auto& rhs = p.rhs;

            int k = rhs.size();
            k -= 1;

            if (isTerminal(rhs[k])) {
                continue;
            } 

            for (auto& type : follow[lhs]) {
                if (follow[rhs[k]].find(type) == follow[rhs[k]].end()) {
                    follow[rhs[k]].insert(type);
                    updated = true;
                }
            }

            for (int i = k - 1; i >= 0; i--) {
                if (isTerminal(rhs[i])) {
                    break;
                }

                if (first[rhs[i + 1]].find("ONE") == first[rhs[i + 1]].end()) {
                    break;
                }

                for (auto& type : follow[lhs]) {
                    if (follow[rhs[i]].find(type) == follow[rhs[i]].end()) {
                        follow[rhs[i]].insert(type);
                        updated = true;
                    }
                }
            }

        }

        if (!updated) {
            break;
        }
    }

}

std::set<std::string> LR1Parser::getFirstForCandidate(const std::vector<std::string>& candidate) {
    std::set<std::string> result;
    if (candidate.size() == 1 && candidate[0] == "ONE") {
        result.insert("ONE");
        return result;
    }

    result = first[candidate[0]];
    result.erase("ONE");

    bool isEmpty = true;
    for (int i = 1; i < (int) candidate.size(); i++) {
        if (first[candidate[i - 1]].find("ONE") == first[candidate[i - 1]].end()) {
            isEmpty = false;
            break;
        }

        result.insert(first[candidate[i]].begin(), first[candidate[i]].end());
        result.erase("ONE");
    }

    if (isEmpty && first[candidate.back()].find("ONE") != first[candidate.back()].end()) {
        result.insert("ONE");
    }

    return result;
}

I LR1Parser::closure(LR1Item item) {

    // std::cerr << "Closure " << item.prodIndex << " " << item.dotPos << std::endl;

    I result;
    std::queue<LR1Item> q;
    q.push(item);
    result.insert(item);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        if (current.dotPos >= productions[current.prodIndex].rhs.size()) {
            continue;
        }

        auto& nextSymbol = productions[current.prodIndex].rhs[current.dotPos];
        if (isTerminal(nextSymbol)) {
            continue;
        }

        for (auto& prodIndex : leftToRight[nextSymbol]) {
            LR1Item newItem(prodIndex, 0, {});
            if (current.dotPos + 1 == productions[current.prodIndex].rhs.size()) {
                newItem.lookahead.insert(current.lookahead.begin(), current.lookahead.end());
            } else {
                std::set<std::string> firstSet = first[productions[current.prodIndex].rhs[current.dotPos + 1]];
                newItem.lookahead.insert(firstSet.begin(), firstSet.end());
            }

            auto res = result.insert(newItem);
            if (res.second) {
                q.push(newItem);
                continue;
            } 
            if (res.first->lookahead != newItem.lookahead) {
                auto temp = *res.first;
                result.erase(res.first);
                bool updated = false;
                for (auto& s : newItem.lookahead) {
                    if (temp.lookahead.find(s) == temp.lookahead.end()) {
                        temp.lookahead.insert(s);
                        updated = true;
                    }
                }
                if (updated) {
                    q.push(temp);
                }
                result.insert(temp);
            }
        }
    }

    // std::cerr << result.size() << std::endl;
    // for (auto& item : result) {
    //     std::cerr << productions[item.prodIndex].lhs << " -> ";
    //     for (int i = 0; i < productions[item.prodIndex].rhs.size(); i++) {
    //         if (i == item.dotPos) {
    //             std::cerr << ". ";
    //         }
    //         std::cerr << productions[item.prodIndex].rhs[i] << " ";
    //     }
    //     if (item.dotPos == productions[item.prodIndex].rhs.size()) {
    //         std::cerr << ". ";
    //     }
    //     std::cerr << ", ";
    //     for (auto& type : item.lookahead) {
    //         std::cerr << type << " ";
    //     }
    //     std::cerr << "; \n";
    // }

    return result;
}

I LR1Parser::go(const I& i, const std::string& X) {

    I result;
    for (auto& item : i) {
        auto& prod = productions[item.prodIndex];
        if (item.dotPos >= prod.rhs.size()) {
            continue;
        }
        if (prod.rhs[item.dotPos] == X) {
            LR1Item newItem(item.prodIndex, item.dotPos + 1, item.lookahead);
            result.insert(newItem);
        }
    }

    I closureResult;
    for (auto& item : result) {
        auto closureResultItem = closure(item);
        closureResult.insert(closureResultItem.begin(), closureResultItem.end());
    }

    return closureResult;
}

void LR1Parser::buildC() {
    int state = 0;
    LR1Item startItem = {
        *leftToRight["START"].begin(),
        0,
        {"END"}
    };

    I startI = closure(startItem);
    C.insert({startI, state});
    state2I[state] = startI;
    state++;
    stateStack.push_back(0);

    std::list<std::set<std::pair<I, int>>::iterator> q;
    q.push_back(C.begin());

    while (!q.empty()) {
        auto [i, curState] = *q.front();
        q.pop_front();
        // std::cerr << curState << " " << q.size() << std::endl;

        for (auto& item : i) {

            // std::cerr << &item << " " << q.size() << std::endl;

            auto& prod = productions[item.prodIndex];
            if (item.dotPos >= prod.rhs.size()) {
                continue;
            }

            auto X = prod.rhs[item.dotPos];
            I nextI = go(i, X);
            // std::cerr << X << " " << nextI.size() << std::endl;

            // std::cerr << "go " << item.prodIndex << " " << item.dotPos << " " << X << std::endl;

            auto res = C.insert({nextI, state});

            if (res.second) {
                state2I[state] = nextI;
                q.push_back(res.first);
                transSet.insert({curState, X, state});
                state++;
                continue;
            }

            // std::cerr << "insert " << curState << " " << X << " " << res.first->second << std::endl;

            auto temp = *res.first;
            auto tempI = temp.first;
            bool g_updated = false;

            for (auto& it : nextI) {
                auto res = tempI.find(it);
                if (res == tempI.end()) continue;

                auto tempItem = *res;

                bool updated = false;
                for (auto& s : it.lookahead) {
                    if (tempItem.lookahead.find(s) == tempItem.lookahead.end()) {
                        // std::cerr << "insert " << s << std::endl;
                        tempItem.lookahead.insert(s);
                        updated = true;
                        g_updated = true;
                    }
                }
                if (updated) {
                    tempI.erase(res);
                    tempI.insert(tempItem);
                }
            }

            // std::cerr << g_updated << std::endl;
        
            temp.first = tempI;
            if (g_updated) {
                C.erase(res.first);
                // std::cerr << "erase " << res.first->second << std::endl;
                auto del = std::find(q.begin(), q.end(), res.first);
                if (del != q.end()) {
                    q.erase(del);
                }
                C.insert(temp);
                q.push_back(C.find(temp));
            }
            transSet.insert({curState, X, temp.second});

            // std::cerr << &item << " " << q.size() << std::endl;
        }
    }

    // std::cerr << C.size() << std::endl;
	// for (auto &state : C) {
	// 	std::cerr << "State " << state.second << ": ";
	// 	for (auto &item : state.first) {
	// 		std::cerr << productions[item.prodIndex].lhs << " -> ";
	// 		for (int i = 0; i < productions[item.prodIndex].rhs.size(); i++) {
	// 			if (i == item.dotPos) {
	// 				std::cerr << ". ";
	// 			}
	// 			std::cerr << productions[item.prodIndex].rhs[i] << " ";
	// 		}
	// 		if (item.dotPos == productions[item.prodIndex].rhs.size()) {
	// 			std::cerr << ". ";
	// 		}
	// 		std::cerr << ", ";
	// 		for (auto &type : item.lookahead) {
	// 			std::cerr << type << " ";
	// 		}
	// 		std::cerr << "; ";
	// 	}
	// 	std::cerr << std::endl;
	// }
}

void LR1Parser::buildActionTable() {
    for (auto& trans : transSet) {
        if (isTerminal(trans.symbol)) {
            analyticalTable[trans.oldState][trans.symbol] = [&]() {
                Symbol symbol(input.front());
                input.erase(input.begin());
                symbolStack.push_back(symbol);
                stateStack.push_back(trans.newState);
            };
        } else {
            analyticalTable[trans.oldState][trans.symbol] = [&]() {
                stateStack.push_back(trans.newState);
            };
        }
    }

    for (auto i = C.begin(); i != C.end(); ++i) {
        for (auto& item : i->first) {
            auto& prod = productions[item.prodIndex];
            if (item.dotPos != prod.rhs.size()) {
                continue;
            }
            if (prod.lhs == "START") {
                analyticalTable[i->second]["END"] = [&]() {
                    input.erase(input.begin());
                };
                continue;
            }

            for (auto& type : item.lookahead) {
                analyticalTable[i->second][type] = [&]() {
                    productions[item.prodIndex].action();
                    auto action = analyticalTable[stateStack.back()][productions[item.prodIndex].lhs];
                    if (action) {
                        action();
                    } else {
                        throw std::runtime_error("No action at line " + std::to_string(__LINE__));
                    }
                };
            }
        }
    }

    // std::cerr << analyticalTable.size() << '\n';
    // for (auto& [state, actions] : analyticalTable) {
    //     std::cerr << state << ": ";
    //     for (auto& [symbol, action] : actions) {
    //         std::cerr << symbol << ", ";
    //     }
    //     std::cerr << '\n';
    // }
}

void LR1Parser::backpatch(const std::string& p, std::string t) {
    std::string q;
    std::string P = p;
    // std::cerr << "calling: " << p << " | " << t << std::endl;
    while (P != "null" && P != "0") {
        q = quadruples[std::stoi(P)].result;
        quadruples[std::stoi(P)].result = t;
        P = q;
    }
}

std::string LR1Parser::merge(const std::string& a, const std::string& b) {
    if (a == "null") {
        return b;
    }
    if (b == "null") {
        return a;
    }
    std::string p = b;
    while (
        quadruples[std::stoi(p)].result != "null" && 
        quadruples[std::stoi(p)].result != "0"
    ) {
        p = quadruples[std::stoi(p)].result;
    }
    quadruples[std::stoi(p)].result = a;
    return b;
}

void LR1Parser::gen(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result) {
    // std::cerr << nxq << ": (" << op << ", " << arg1 << ", " << arg2 << ", " << result << ")" << std::endl;
    
    if ((int) quadruples.size() <= nxq) {
        quadruples.push_back({op, arg1, arg2, result});
    } else {
        quadruples[nxq] = {op, arg1, arg2, result};
    }
    nxq++;
}

void LR1Parser::enter(const std::string& name, int type, int offset) {
    if (symbolTableIndex.find(name) != symbolTableIndex.end()) {
        throw std::runtime_error("Symbol already exists");
    }
    symbolTableIndex[name] = symbolTable.size();
    symbolTable.push_back({name, type, "null", offset});
}

std::string LR1Parser::makeList(int i) {
    return std::to_string(i);
}

std::string LR1Parser::makeList() {
    return "null";
}

std::string LR1Parser::newTemp(int type) {
    std::string name = "T" + std::to_string(tmpIndex++);
    if (type == SYMINT) {
        name += "_i";
    } else if (type == SYMDOUBLE) {
        name += "_d";
    }
    return name;
}

int LR1Parser::lookupType(const std::string& name) {
    if (symbolTableIndex.find(name) == symbolTableIndex.end()) {
        return -1;
    }
    return symbolTable[symbolTableIndex[name]].type;
}

std::string LR1Parser::lookup(const std::string& name) {
    if (symbolTableIndex.find(name) == symbolTableIndex.end()) {
        throw std::runtime_error("Symbol not found");
    }
    return "TB" + std::to_string(symbolTableIndex[name]);
}

void LR1Parser::setInput(const std::vector<std::string>& input) {
    this->input = input;
    this->input.push_back("END");
}

void LR1Parser::parse() {
    while (!input.empty()) {
        auto state = stateStack.back();
        auto symbol = input.front();
        auto action = analyticalTable[state][symbol];

        // std::cerr << "State: " << state << " Symbol: " << symbol << std::endl;
        // std::cerr << "Stack: ";
        // for (auto& s : stateStack) {
        //     std::cerr << s << " ";
        // }
        // std::cerr << std::endl;

        if (action) {
            action();
        } else if (symbol != "ONE") {
            symbol = "ONE";
            action = analyticalTable[state][symbol];
            if (action) {
                action();
            } else {
                throw std::runtime_error("No action at line " + std::to_string(__LINE__));
            }
        } else {
            throw std::runtime_error("No action at line " + std::to_string(__LINE__));
        }
    }
}

void LR1Parser::printQuadruples() const {
    std::cout << symbolTable.size() << '\n';
    for (auto [name, type, value, offset, _] : symbolTable) {
        std::cout << name << ' ' << type << ' ' << value << ' ' << offset << '\n';
    }
    std::cout << tmpIndex << '\n';
    std::cout << quadruples.size() << '\n';
    for (int i = 0; i < (int) quadruples.size(); i++) {
        std::cout << i << ": (";
        std::cout << 
            quadruples[i].op << ',' << 
            quadruples[i].arg1 << ',' << 
            quadruples[i].arg2 << ',' << 
            quadruples[i].result << ')' << '\n';
    }
}


}