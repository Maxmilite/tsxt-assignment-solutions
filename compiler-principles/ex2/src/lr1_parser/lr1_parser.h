#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <functional>

#define SYMINT 0
#define SYMDOUBLE 1

namespace LR1 {

const std::string TYPES[] = {
	"START",
	"ONE",
	"INTSYM",
	"DOUBLESYM",
	"SCANFSYM",
	"PRINTFSYM",
	"IFSYM",
	"THENSYM",
	"WHILESYM",
	"DOSYM",
	"EQU",
	"GREATER",
	"GE",
	"LESS",
	"LE",
	"OR",
	"AND",
	"NOT",
	"NOTEQU",
	"PLUS",
	"MINUS",
	"TIMES",
	"DIVISION",
	"COMMA",
	"LBRACE1",
	"RBRACE1",
	"LBRACE2",
	"RBRACE2",
	"SEMICOLON",
	"ID",
	"IDENT",
	"UDOUBLE",
	"UINT",
	"EQUEQU",
	"PROG",
	"SUBPROG",
	"M",
	"N",
	"VARIABLES",
	"STATEMENT",
	"VARIABLE",
	"T",
	"ASSIGN",
	"SCANF",
	"PRINTF",
	"L",
	"B",
	"EXPR",
	"ORITEM",
	"ANDITEM",
	"RELITEM",
	"NOITEM",
	"ITEM",
	"FACTOR",
	"BORTERM",
	"BANDTERM",
	"BFACTOR",
	"PLUS_MINUS",
	"MUL_DIV",
	"REL",
	"SCANF_BEGIN",
	"PRINTF_BEGIN",
	"END",
};

const std::set<std::string> TERMINALS = {
	"INTSYM",
	"DOUBLESYM",
	"SCANFSYM",
	"PRINTFSYM",
	"IFSYM",
	"THENSYM",
	"WHILESYM",
	"DOSYM",
	"EQU",
	"EQUEQU",
	"GREATER",
	"GE",
	"LESS",
	"LE",
	"OR",
	"AND",
	"NOT",
	"NOTEQU",
	"PLUS",
	"MINUS",
	"TIMES",
	"DIVISION",
	"COMMA",
	"LBRACE1",
	"RBRACE1",
	"LBRACE2",
	"RBRACE2",
	"SEMICOLON",
	"UDOUBLE",
	"UINT",
	"IDENT",
};

struct Production {
    std::string lhs;
    std::vector<std::string> rhs;
    std::function<void()> action;
    
    Production(
        const std::string& lhs, 
        const std::vector<std::string>& rhs,
        const std::function<void()>& action = []() {} 
    ) : lhs(lhs), rhs(rhs), action(action) {}
};

struct LR1Item {
	int prodIndex;
    int dotPos;
    std::set<std::string> lookahead;
    
    LR1Item() = default;
	LR1Item(int prodIndex, int dotPos, const std::set<std::string>& lookahead)
		: prodIndex(prodIndex), dotPos(dotPos), lookahead(lookahead) {}
    
    bool operator==(const LR1Item &other) const;
    
    bool operator<(const LR1Item &other) const;
};

typedef std::set<LR1Item> I;

struct CCompare {
	bool operator()(const std::pair<I, int> &a, const std::pair<I, int> &b) const {
		return a.first < b.first;
	}
};

struct Symbol {
	std::string rtype;
	std::string name;
	std::string quad;
	std::string nextList;
	std::string trueList;
	std::string falseList;
	int type;
	int width;
	std::string op;
	std::string place;

	Symbol() {
		rtype = "ONE";
	}
	Symbol(const std::string& rtype)
		: rtype(rtype) {}
	Symbol(const std::string& rtype, const std::string& name, const std::string& quad, const std::string& nextList, const std::string& trueList, const std::string& falseList, int type, int width, const std::string& op, const std::string& place)
		: rtype(rtype), name(name), quad(quad), nextList(nextList), trueList(trueList), falseList(falseList), type(type), width(width), op(op), place(place) {}
};

struct TSymbol { // T for table. Symbol in table.
    std::string name;
    int type;
    std::string value;
    int offset;
    bool hasValue;

	TSymbol() = default;
    TSymbol(const std::string& name, int type, const std::string& value, int offset, bool hasValue = false)
        : name(name), type(type), value(value), offset(offset), hasValue(hasValue) {}
};

struct Quadruple {
	std::string op;
	std::string arg1;
	std::string arg2;
	std::string result;

	Quadruple() = default;
	Quadruple(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result)
		: op(op), arg1(arg1), arg2(arg2), result(result) {}
};

struct Trans {
	int oldState;
	std::string symbol;
	int newState;

	Trans() = default;
	Trans(int oldState, const std::string& symbol, int newState)
		: oldState(oldState), symbol(symbol), newState(newState) {}

	bool operator< (const Trans& other) const {
		if (oldState != other.oldState) {
			return oldState < other.oldState;
		}
		if (symbol != other.symbol) {
			return symbol < other.symbol;
		}
		return newState < other.newState;
	}
};

class LR1Parser {
public:
    LR1Parser();
	void setInput(const std::vector<std::string>& input);
	void parse();
	void printQuadruples() const;
	std::vector<std::string> numAndId;

private:
	int offset;
	int nxq;
	int tmpIndex;

	std::vector<Production> productions;

	std::vector<Symbol> symbolStack;
	std::vector<int> stateStack;
	std::vector<std::string> input;

	std::map<std::string, std::vector<int>> leftToRight;
	std::map<std::string, std::set<std::string>> first;
	std::map<std::string, std::set<std::string>> follow;

	std::set<std::pair<I, int>, CCompare> C;
	std::map<int, I> state2I;

	std::set<Trans> transSet;

	std::vector<TSymbol> symbolTable;
	std::map<std::string, int> symbolTableIndex;
	std::map<int, std::map<std::string, std::function<void()>>> analyticalTable;

	std::vector<Quadruple> quadruples;

	void loadProductions();

	bool isTerminal(const std::string& s) const;
	void getFirst();
	void getFollow();

	std::set<std::string> getFirstForCandidate(const std::vector<std::string>& candidate);

	I closure(LR1Item item);
	I go(const I& i, const std::string& X);

	void buildC();
	void buildActionTable();
	
	void backpatch(const std::string& p, std::string t);
	void gen(const std::string& op, const std::string& arg1, const std::string& arg2, const std::string& result);
	void enter(const std::string& name, int type, int offset);
	
	std::string newTemp(int type);
	int lookupType(const std::string& name);
	std::string lookup(const std::string& name);
	std::string merge(const std::string& p1, const std::string& p2);
	std::string makeList(int i);
	std::string makeList();

};

}
