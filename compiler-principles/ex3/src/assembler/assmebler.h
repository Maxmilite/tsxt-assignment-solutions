#pragma once

#include "../common.h"
#include <map>
#include <set>

struct TSymbol { // T for table. Symbol in table.
    std::string name;
    int type;
    std::string value;
    int offset;

    int use;
    bool live;
    bool isTemp; // is temporary variable

	TSymbol() {
        isTemp = true;
        use = -1;
        live = false;
        offset = -1;
    }
    TSymbol(const std::string& name, int type, const std::string& value, int offset, bool isTemp = true)
        : name(name), type(type), value(value), offset(offset), isTemp(isTemp) {
            use = -1;
            live = false;
        }
    TSymbol(bool isTemp) : isTemp(isTemp) {
        use = -1;
        live = false;
        offset = -1;
    }
};

struct QItem {
    std::string val;
    int use = -1;
    bool live = false;

    QItem() = default;
    QItem(const std::string& val) : val(val) {}
};

struct Quadruple {
	QItem op;
	QItem arg1;
	QItem arg2;
	QItem left;

	Quadruple() = default;
	Quadruple(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result)
		: op(op), arg1(arg1), arg2(arg2), left(result) {}
};

struct AvalItem {
	std::set<std::string> reg;
	std::set<std::string> mem;
};

const std::map<std::string, std::string> ops = {
    {"+", "add"},
    {"-", "sub"},
    {"*", "mul"},
    {"/", "div"},
    {"=", "mov"},
    {"<", "cmp"},
    {">", "cmp"},
    {"<=", "cmp"},
    {">=", "cmp"},
    {"==", "cmp"},
    {"!=", "cmp"},
    {"&&", "and"},
    {"||", "or"},
    {"!", "not"},
};

const std::map<std::string, std::string> jumps = {
    {"j<", "jl"},
    {"j>", "jg"},
    {"j<=", "jle"},
    {"j>=", "jge"},
    {"j==", "je"},
    {"j!=", "jne"},
};

const std::map<std::string, std::string> cmps = {
    {"<", "setl"},
    {">", "setg"},
    {"<=", "setle"},
    {">=", "setge"},
    {"==", "sete"},
    {"!=", "setne"},
};


namespace util {
    bool isTheta(const Quadruple& q);
    bool isJ(const Quadruple& q);
    bool isJTheta(const Quadruple& q);
    bool isJnz(const Quadruple& q);
    bool isRW(const Quadruple& q); // if q is read or write
    bool isEnd(const Quadruple& q);
    bool isOnlyX(const Quadruple& q); // if q only contains left operand
    bool isNumber(const std::string& s);
}

class Assembler {
public:
    Assembler();
    void run();

private:
    std::map<std::string, TSymbol> symbolTable;
    std::vector<Quadruple> quadruples;
    
    int offset, initOffset;
    int tempVarCount;

    std::vector<int> labels;
    std::vector<std::vector<int>> basicBlocks;
    std::vector<std::set<std::string>> liveOut; // live variables in each basic block
    
    std::map<std::string, std::set<std::string>> Rval;
    std::vector<std::string> regs = {"R0", "R1", "R2"};
    std::map<std::string, AvalItem> Aval;

    std::map<std::string, QItem> history;

    void input();
    void genBasicBlock();
    void genLabel(int index);
    void genUse();
    std::set<std::string> getUseInfo(const std::vector<int>& block);
    void genCode();

    void genForOnlyX(int index, int blockIdx);
    void genForTheta(int index, int blockIdx);
    void genForRW(int index, int blockIdx);
    void genForEnd();

    std::string findReg(const std::vector<std::string>& RA, int index);
    std::string acquireReg(int index);
    void releaseReg(const std::string& var, const std::set<std::string>& live);

    std::string getAddress(const std::string& var);
};