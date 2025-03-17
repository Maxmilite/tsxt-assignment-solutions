#include "assmebler.h"
#include <set>
#include <algorithm>

// std::cout << "{" << __LINE__ << "} "

namespace util {

bool isTheta(const Quadruple &q) {
    return (q.op.val[0] == '+' || q.op.val[0] == '-' || q.op.val[0] == '*' || q.op.val[0] == '/' || q.op.val[0] == '=' || q.op.val[0] == '<' || q.op.val[0] == '>' || q.op.val[0] == '!' || q.op.val[0] == '&' || q.op.val[0] == '|') && (q.arg2.val != "-");
}

bool isJ(const Quadruple &q) {
    return q.op.val == "j";
}

bool isJTheta(const Quadruple &q) {
    return q.op.val[0] == 'j' && q.op.val != "j" && q.op.val != "jnz";
}

bool isJnz(const Quadruple &q) {
    return q.op.val == "jnz";
}

bool isRW(const Quadruple &q) {
    return q.op.val == "W" || q.op.val == "R";
}

bool isEnd(const Quadruple &q) {
    return q.op.val == "End";
}

bool isOnlyX(const Quadruple &q) {
    return (q.op.val == "=" || q.op.val == "!") && q.arg2.val == "-";
}

bool isNumber(const std::string &s) {
    return isdigit(s[0]);
}

}

Assembler::Assembler() {
    input();
}

void Assembler::run() {
    genBasicBlock();
    genUse();
    genCode();
}

void Assembler::input() {
    int n;
    std::string line;
	std::getline(std::cin, line);
	if (line == "Syntax Error") {
		std::cout << "halt" << std::endl;
		exit(0);
	}

    this->offset = 0;

    // symbol table
    // std::cin >> n;
    n = std::stoi(line);
    for (int i = 0; i < n; ++i) {
        std::string name;
        int type;
        std::string value;
        int offset;
        std::cin >> name >> type >> value >> offset;
        this->offset = std::max(this->offset, offset);
        symbolTable["TB" + std::to_string(i)] = {name, type, value, offset, false};
    }

    // temp var count
    std::cin >> tempVarCount;

    // quadruples
    std::cin >> n;
    std::cin.ignore();
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);
        // format: "num: (op,arg1,arg2,result)"
        line = line.substr(line.find(':') + 2);
        line = line.substr(1, line.find(')') - 1);
        std::stringstream ss(line);
        std::vector<std::string> tokens(4);
        for (int j = 0; j < 4; ++j) {
            std::getline(ss, tokens[j], ',');
        }
        quadruples.push_back({tokens[0], tokens[1], tokens[2], tokens[3]});
    }
}

void Assembler::genLabel(int index) {
    if (labels[index] == 0) {
        labels[index] = 1;
    }
}

void Assembler::genBasicBlock() {
    labels.resize(quadruples.size(), 0);
    
    std::set<std::vector<int>> blocks; // basic blocks
    std::vector<int> isEnter(quadruples.size(), 0);
    isEnter[0] = true;

    for (int i = 0; i < quadruples.size(); ++i) {
        const auto &q = quadruples[i];
        
        if (util::isJTheta(q) || util::isJnz(q)) {
            int index = std::stoi(q.left.val);
            isEnter[index] = true;
            if (i < quadruples.size() - 1) {
                isEnter[i + 1] = true;
            }
            genLabel(index);
            continue;
        }

        if (util::isJ(q)) {
            int index = std::stoi(q.left.val);
            isEnter[index] = true;
            genLabel(index);
            continue;
        }

        if (util::isEnd(q)) {
            isEnter[quadruples.size() - 1] = true;
            continue;
        }

        if (util::isRW(q)) {
            isEnter[i] = true;
        }
    }

    for (int i = 0; i < quadruples.size(); ) {
        if (!isEnter[i]) {
            ++i;
            continue;
        }
        if (i + 1 == quadruples.size()) {
            std::vector<int> temp;
            temp.push_back(i);
            blocks.insert(temp);
            break;
        }

        for (int j = i + 1; j < quadruples.size(); ++j) {
            if (isEnter[j]) {
                std::vector<int> temp;
                for (int k = i; k < j; ++k) {
                    temp.push_back(k);
                }
                blocks.insert(temp);
                i = j;
                break;
            } else {
                if (quadruples[j].op.val[0] == 'j' || quadruples[j].op.val == "ret" || quadruples[j].op.val == "End") {
                    std::vector<int> temp;
                    for (int k = i; k <= j; ++k) {
                        temp.push_back(k);
                    }
                    blocks.insert(temp);
                    i = j + 1;
                    break;
                }
            }
        }
    }

    basicBlocks = std::vector<std::vector<int>>(blocks.begin(), blocks.end());
}

std::set<std::string> Assembler::getUseInfo(const std::vector<int> &block) {
    std::set<std::string> res;

    for (const auto& i : block) {
        auto&& [_, x, y, z] = quadruples[i];

        if (x.val[0] == 'T') {
            symbolTable[x.val].use = -1;
            if (!symbolTable[x.val].isTemp) {
                symbolTable[x.val].live = true;
                res.insert(x.val);
            }
        }
        if (y.val[0] == 'T') {
            symbolTable[y.val].use = -1;
            if (!symbolTable[y.val].isTemp) {
                symbolTable[y.val].live = true;
                res.insert(y.val);
            }
        }
        if (z.val[0] == 'T') {
            symbolTable[z.val].use = -1;
            if (!symbolTable[z.val].isTemp) {
                symbolTable[z.val].live = true;
                res.insert(z.val);
            }
        }
    }

    for (int i = block.size() - 1; i >= 0; --i) {
        int index = block[i];
        auto&& [_, x, y, z] = quadruples[index];

        if (x.val[0] == 'T') {
            x.use = symbolTable[x.val].use;
            x.live = symbolTable[x.val].live;
            symbolTable[x.val].live = true;
            symbolTable[x.val].use = index;
        }

        if (y.val[0] == 'T') {
            y.use = symbolTable[y.val].use;
            y.live = symbolTable[y.val].live;
            symbolTable[y.val].live = true;
            symbolTable[y.val].use = index;
        }

        if (z.val[0] == 'T') {
            z.use = symbolTable[z.val].use;
            z.live = symbolTable[z.val].live;
            symbolTable[z.val].live = false;
            symbolTable[z.val].use = -1;
        }
    }

    return res;
}

void Assembler::genUse() {
    liveOut.resize(basicBlocks.size());
    for (int i = 0; i < basicBlocks.size(); ++i) {
        const auto &block = basicBlocks[i];
        liveOut[i] = getUseInfo(block);
    }

    // std::cerr << "liveOut: " << std::endl;
    // for (int i = 0; i < liveOut.size(); ++i) {
    //     std::cerr << "block " << i << ": ";
    //     for (const auto& a : liveOut[i]) {
    //         std::cerr << a << " ";
    //     }
    //     std::cerr << std::endl;
    // }
}

void Assembler::genCode() {
    for (int blockIdx = 0; blockIdx < basicBlocks.size(); ++blockIdx) {
        const auto& block = basicBlocks[blockIdx];
        if (labels[block[0]] == 1) {
            std::cout << "?" + std::to_string(block[0]) << ":" << std::endl;
        }

        for (const auto& quad : block) {
            if (util::isTheta(quadruples[quad])) {
                genForTheta(quad, blockIdx);
            } else if (util::isRW(quadruples[quad])) {
                genForRW(quad, blockIdx);
            } else if (util::isOnlyX(quadruples[quad])) {
                genForOnlyX(quad, blockIdx);
            }
        }

        // std::cerr << "liveOut: ";
        // for (const auto& a : liveOut[blockIdx]) {
        //     std::cerr << a << " ";
        // }
        // std::cerr << std::endl;

        for (auto a : liveOut[blockIdx]) {
            if (Aval[a].mem.find(a) != Aval[a].mem.end()) {
                continue;
            }
            if (Aval[a].reg.empty()) {
                continue;
            }
            for (auto reg : Aval[a].reg) {
                if (history[a].live) {
                    std::cout << "mov " << getAddress(a) << ", " << reg << std::endl;
                }
            }
        }

        auto quad = quadruples[block.back()];
        if (util::isJ(quad)) {
            std::cout << "jmp ?" + quad.left.val << std::endl;
        } else if (util::isJTheta(quad)) {
            auto x = quad.arg1.val;
            auto y = quad.arg2.val;
            auto q = quad.left.val;
            std::string X;
            if (!Aval[x].reg.empty()) {
                X = *Aval[x].reg.begin();
            } else {
                X = x;
            }
            std::string Y;
            if (!Aval[y].reg.empty()) {
                Y = *Aval[y].reg.begin();
            } else {
                Y = getAddress(y);
            }
            if (X == x) {
                X = acquireReg(block.back());
                std::cout << "mov " << X << ", " << getAddress(x) << std::endl;
            }
            std::cout << "cmp " << X << ", " << Y << std::endl;
            std::cout << jumps.at(quad.op.val) << " ?" << q << std::endl;
        } else if (util::isJnz(quad)) {
            auto x = quad.arg1.val;
            auto q = quad.left.val;
            std::string X;
            if (!Aval[x].reg.empty()) {
                X = *Aval[x].reg.begin();
            } else {
                X = x;
            }
            if (X == x) {
                X = acquireReg(block.back());
                std::cout << "mov " << X << ", " << getAddress(x) << std::endl;
            }
            std::cout << "cmp " << X << ", 0" << std::endl;
            std::cout << "jne ?" << q << std::endl;
        } else if (util::isEnd(quad)) {
            genForEnd();
        }

        Rval.clear();
        Aval.clear();

    }
}

std::string Assembler::findReg(const std::vector<std::string>& RA, int index) {
    std::string res;
    int maxUse = -1;
    int blockIdx = -1;
    for (int i = 0; i < basicBlocks.size(); ++i) {
        const auto& block = basicBlocks[i];
        if (std::find(block.begin(), block.end(), index) != block.end()) {
            blockIdx = i;
            break;
        }
    }

    const auto& block = basicBlocks[blockIdx];

    for (auto & R : RA) {
        bool found = false;
        for (int i = index + 1; i <= block.back(); ++i) {
            if (
                Rval[R].find(quadruples[i].arg1.val) != Rval[R].end()
                || Rval[R].find(quadruples[i].arg2.val) != Rval[R].end()
            ) {
                found = true;
                if (i > maxUse) {
                    maxUse = i;
                    res = R;
                }
                break;
            }
        }

        if (!found) {
            res = R;
            break;
        }
    }
    return res;
}

std::string Assembler::getAddress(const std::string& var) {

    if (var[0] == '[') {
        return var;
    }

    if (symbolTable.find(var) != symbolTable.end() && symbolTable[var].offset != -1) {
        return "[ebp-" + std::to_string(symbolTable[var].offset) + "]";
    }
    if (var.back() == 'i') {
        offset += 4;
        symbolTable[var].offset = offset;
    } else if (var.back() == 'd') {
        offset += 8;
        symbolTable[var].offset = offset;
    }
    return "[ebp-" + std::to_string(symbolTable[var].offset) + "]";
}

std::string Assembler::acquireReg(int index) {
    const auto& q = quadruples[index];
    auto x = q.arg1.val;
    auto y = q.arg2.val;
    auto z = q.left.val;

    if (!util::isNumber(x) && x != "-") {
        for (const auto& Ri : Aval[x].reg) {
            if (Rval[Ri] == std::set<std::string>{x} 
                && (x == z || !q.arg1.live)
            ) {
                return Ri;
            }
        }
    }

    for (const auto& Ri : regs) {
        if (Rval[Ri].empty()) {
            return Ri;
        }
    }

    std::vector<std::string> RA;
    for (const auto& Ri : regs) {
        if (!Rval[Ri].empty()) {
            RA.push_back(Ri);
        }
    }

    if (RA.empty()) {
        RA = regs;
    }

    std::string Ri;
    bool found = true;
    for (const auto& Rj : RA) {
        found = true;
        for (const auto& a : Rval[Rj]) {
            if (Aval[a].mem.find(a) == Aval[a].mem.end()) {
                found = false;
                break;
            }
        }
        if (found) {
            Ri = Rj;
            break;
        }
    }

    if (!found) {
        Ri = findReg(RA, index);
    }

    for (const auto& a : Rval[Ri]) {
        if (Aval[a].mem.find(a) == Aval[a].mem.end() && a != z) {
            std::cout << "mov " << getAddress(a) << ", " << Ri << std::endl;
        }
        if (a == x || (a == y && Rval[Ri].find(x) != Rval[Ri].end())) {
            Aval[a].mem = {a};
            Aval[a].reg = {Ri};
        } else {
            Aval[a].mem = {a};
            Aval[a].reg = {};
        }
    }

    Rval[Ri].clear();
    return Ri;
}

void Assembler::releaseReg(const std::string& var, const std::set<std::string>& liveOut) {
    if (liveOut.find(var) == liveOut.end()) {
        for (const auto& reg : Aval[var].reg) {
            Rval[reg].erase(var);
        }
        Aval[var].reg.clear();
    }
}

void Assembler::genForOnlyX(int index, int blockIdx) {
    const auto& q = quadruples[index];
    auto x = q.arg1.val;
    auto z = q.left.val;
    auto r = acquireReg(index);

    std::string X;

    if (util::isNumber(x)) {
        X = x;
        std::cout << "mov " << r << ", " << X << std::endl;
    } else {
        if (Rval[r].find(x) == Rval[r].end()) {
            if (!Aval[x].reg.empty()) {
                X = *Aval[x].reg.begin();
            } else {
                X = getAddress(x);
            }
            std::cout << "mov " << r << ", " << X << std::endl;
        }

        if (q.op.val != "=") {
            std::cout << ops.at(q.op.val) << " " << r << std::endl;
        }

        if (!util::isNumber(x)) {
            releaseReg(x, liveOut[blockIdx]);
        }
    }

    Rval[r].insert(z);
    history[z] = q.left;
    Aval[z].reg.insert(r);
    Aval[z].mem.clear();
}

void Assembler::genForTheta(int index, int blockIdx) {
    const auto& q = quadruples[index];
    auto x = q.arg1.val;
    auto y = q.arg2.val;
    auto z = q.left.val;
    auto r = acquireReg(index);

    std::string X;
    if (util::isNumber(x) || x == "-") {
        X = x;
    } else {
        if (!Aval[x].reg.empty()) {
            X = *Aval[x].reg.begin();
        } else {
            X = getAddress(x);
        }
    }

    std::string Y;
    if (util::isNumber(y) || y == "-") {
        Y = y;
    } else {
        if (!Aval[y].reg.empty()) {
            Y = *Aval[y].reg.begin();
        } else {
            Y = getAddress(y);
        }
    }

    if (X == r) {
        std::cout << ops.at(q.op.val) << " " << r << ", " << Y << std::endl;
        if (ops.at(q.op.val) == "cmp") {
            std::cout << cmps.at(q.op.val) << " " << r << std::endl;
        }
        Aval[x].reg.erase(r);
    } else {
        std::cout << "mov " << r << ", " << X << std::endl;
        std::cout << ops.at(q.op.val) << " " << r << ", " << Y << std::endl;
        if (ops.at(q.op.val) == "cmp") {
            std::cout << cmps.at(q.op.val) << " " << r << std::endl;
        }
    }

    if (Y == r) {
        if (!util::isNumber(y)) {
            Aval[y].reg.erase(r);
        }
    }

    Rval[r] = {z};
    history[z] = q.left;
    Aval[z].reg = {r};
    Aval[z].mem.clear();

    if (!util::isNumber(x)) {
        releaseReg(x, liveOut[blockIdx]);
    }

    if (!util::isNumber(y)) {
        releaseReg(y, liveOut[blockIdx]);
    }
}

void Assembler::genForRW(int index, int blockIdx) {
    const auto& q = quadruples[index];
    if (q.op.val == "W") {
        std::cout << "jmp ?write";
    } else {
        std::cout << "jmp ?read";
    }

    std::cout << "(" << getAddress(q.left.val) << ")" << std::endl;

    if (!util::isNumber(q.left.val)) {
        releaseReg(q.left.val, liveOut[blockIdx]);
    }
}

void Assembler::genForEnd() {
    std::cout << "halt" << std::endl;
}