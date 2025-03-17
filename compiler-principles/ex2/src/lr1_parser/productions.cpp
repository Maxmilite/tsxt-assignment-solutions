#include "lr1_parser.h"

namespace LR1 {

void LR1Parser::loadProductions() {
    productions = {

        // prog.
        {
            "START", {"PROG"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                symbolStack.push_back(Symbol("START"));
            }
        },
        {
            "PROG", {"SUBPROG"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                symbolStack.push_back(Symbol("PROG"));
            }
        },
        {
            "SUBPROG", {"M", "VARIABLES", "STATEMENT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto statement = symbolStack.back();
                symbolStack.pop_back();
                auto variables = symbolStack.back();
                symbolStack.pop_back();
                auto m = symbolStack.back();
                symbolStack.pop_back();
                backpatch(statement.nextList, std::to_string(nxq));
                gen("End", "-", "-", "-");
                symbolStack.push_back(Symbol("SUBPROG"));
            }
        },
        {
            "M", {"ONE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                offset = 0;
                int rep = 1;
                while (rep--) {
                    stateStack.pop_back();
                }
                symbolStack.push_back(Symbol("M"));
            }
        },
        {
            "N", {"ONE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto symbol = Symbol("N");
                symbol.quad = std::to_string(nxq);
                symbolStack.push_back(symbol);
            }
        },

        // var. statement
        {
            "VARIABLES", {"VARIABLES", "VARIABLE", "SEMICOLON"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                    symbolStack.pop_back();
                }
                symbolStack.push_back(Symbol("VARIABLES"));
            }
        },
        {
            "VARIABLES", {"VARIABLE", "SEMICOLON"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                    symbolStack.pop_back();
                }
                symbolStack.push_back(Symbol("VARIABLES"));
            }
        },
        {
            "T", {"INTSYM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("T");
                symbol.type = SYMINT;
                symbol.width = 4;
                symbolStack.push_back(symbol);
            }
        },
        {
            "T", {"DOUBLESYM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("T");
                symbol.type = SYMDOUBLE;
                symbol.width = 8;
                symbolStack.push_back(symbol);
            }
        },
        {
            "ID", {"IDENT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("ID");
                symbol.name = numAndId.front();
                numAndId.erase(numAndId.begin());
                symbolStack.push_back(symbol);
            }
        },
        {
            "VARIABLE", {"T", "ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto id = symbolStack.back();
                symbolStack.pop_back();
                auto t = symbolStack.back();
                symbolStack.pop_back();
                enter(id.name, t.type, offset);
                offset += t.width;
                auto symbol = Symbol("VARIABLE");
                symbol.type = t.type;
                symbol.width = t.width;
                symbolStack.push_back(symbol);
            }
        },
        {
            "VARIABLE", {"VARIABLE", "COMMA", "ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto id = symbolStack.back();
                symbolStack.pop_back();
                // comma
                symbolStack.pop_back();
                auto variable = symbolStack.back();
                symbolStack.pop_back();
                enter(id.name, variable.type, offset);
                offset += variable.width;
                auto symbol = Symbol("VARIABLE");
                symbol.type = variable.type;
                symbol.width = variable.width;
                symbolStack.push_back(symbol);
            }
        },

        // statement
        {
            "STATEMENT", {"ASSIGN"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("STATEMENT");
                symbol.nextList = makeList();
                symbolStack.push_back(symbol);
            }
        },
        {
            "STATEMENT", {"SCANF"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("STATEMENT");
                symbol.nextList = makeList();
                symbolStack.push_back(symbol);
            }
        },
        {
            "STATEMENT", {"PRINTF"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("STATEMENT");
                symbol.nextList = makeList();
                symbolStack.push_back(symbol);
            }
        },
        {
            "STATEMENT", {"ONE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto symbol = Symbol("STATEMENT");
                symbol.nextList = makeList();
                symbolStack.push_back(symbol);
            }
        },
        {
            "STATEMENT", {"LBRACE2", "L", "SEMICOLON", "RBRACE2"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 4;
                while (rep--) {
                    stateStack.pop_back();
                }
                symbolStack.pop_back();
                symbolStack.pop_back();
                auto l = symbolStack.back();
                symbolStack.pop_back();
                symbolStack.pop_back();
                auto symbol = Symbol("STATEMENT");
                symbol.nextList = l.nextList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "STATEMENT", {"WHILESYM", "N", "B", "DOSYM", "N", "STATEMENT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 6;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto statement = symbolStack.back();
                symbolStack.pop_back();
                auto n2 = symbolStack.back();
                symbolStack.pop_back();
                // do
                symbolStack.pop_back();
                auto b = symbolStack.back();
                symbolStack.pop_back();
                auto n1 = symbolStack.back();
                symbolStack.pop_back();
                // while
                symbolStack.pop_back();
                auto symbol = Symbol("STATEMENT");
                backpatch(statement.nextList, n1.quad);
                backpatch(b.trueList, n2.quad);
                symbol.nextList = b.falseList;
                gen("j", "-", "-", n1.quad);
                symbolStack.push_back(symbol);
            }
        },
        {
            "STATEMENT", {"IFSYM", "B", "THENSYM", "N", "STATEMENT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 5;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto statement = symbolStack.back();
                symbolStack.pop_back();
                auto n = symbolStack.back();
                symbolStack.pop_back();
                // then
                symbolStack.pop_back();
                auto b = symbolStack.back();
                symbolStack.pop_back();
                // if
                symbolStack.pop_back();
                auto symbol = Symbol("STATEMENT");
                backpatch(b.trueList, n.quad);
                symbol.nextList = merge(b.falseList, statement.nextList);
                symbolStack.push_back(symbol);
            }
        },
        {
            "ASSIGN", {"ID", "EQU", "EXPR"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto expr = symbolStack.back();
                symbolStack.pop_back();
                // =
                symbolStack.pop_back();
                auto id = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("ASSIGN");
                auto p = lookup(id.name);
                gen("=", expr.place, "-", p);
                symbolStack.push_back(symbol);
            }
        },
        {
            "L", {"L", "SEMICOLON", "N", "STATEMENT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 4;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto statement = symbolStack.back();
                symbolStack.pop_back();
                auto n = symbolStack.back();
                symbolStack.pop_back();
                // ;
                symbolStack.pop_back();
                auto l = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("L");
                backpatch(l.nextList, n.quad);
                symbol.nextList = statement.nextList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "L", {"STATEMENT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto statement = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("L");
                symbol.nextList = statement.nextList;
                symbolStack.push_back(symbol);
            }
        },

        // num. expression
        {
            "EXPR", {"EXPR", "OR", "ORITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto orItem = symbolStack.back();
                symbolStack.pop_back();
                // or
                symbolStack.pop_back();
                auto expr = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("EXPR");
                symbol.place = newTemp(SYMINT);
                symbol.type = SYMINT;
                gen("||", expr.place, orItem.place, symbol.place);
                symbolStack.push_back(symbol);
            }
        },
        {
            "EXPR", {"ORITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto orItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("EXPR");
                symbol.place = orItem.place;
                symbol.type = orItem.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "ORITEM", {"ORITEM", "AND", "ANDITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto andItem = symbolStack.back();
                symbolStack.pop_back();
                // and
                symbolStack.pop_back();
                auto orItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("ORITEM");
                symbol.place = newTemp(SYMINT);
                symbol.type = SYMINT;
                gen("&&", orItem.place, andItem.place, symbol.place);
                symbolStack.push_back(symbol);
            }
        },
        {
            "ORITEM", {"ANDITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto andItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("ORITEM");
                symbol.place = andItem.place;
                symbol.type = andItem.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "ANDITEM", {"NOITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto noItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("ANDITEM");
                symbol.place = noItem.place;
                symbol.type = noItem.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "ANDITEM", {"NOT", "NOITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto noItem = symbolStack.back();
                symbolStack.pop_back();
                // not
                symbolStack.pop_back();
                auto symbol = Symbol("ANDITEM");
                symbol.place = newTemp(SYMINT);
                symbol.type = SYMINT;
                gen("!", noItem.place, "-", symbol.place);
                symbolStack.push_back(symbol);
            }
        },
        {
            "NOITEM", {"NOITEM", "REL", "RELITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto relItem = symbolStack.back();
                symbolStack.pop_back();
                auto rel = symbolStack.back();
                symbolStack.pop_back();
                auto noItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("NOITEM");
                symbol.place = newTemp(SYMINT);
                symbol.type = SYMINT;
                gen(rel.op, noItem.place, relItem.place, symbol.place);
                symbolStack.push_back(symbol);
            }
        },
        {
            "NOITEM", {"RELITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto relItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("NOITEM");
                symbol.place = relItem.place;
                symbol.type = relItem.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "RELITEM", {"RELITEM", "PLUS_MINUS", "ITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto item = symbolStack.back();
                symbolStack.pop_back();
                auto plusMinus = symbolStack.back();
                symbolStack.pop_back();
                auto relItem = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("RELITEM");
                symbol.place = newTemp(relItem.type);
                symbol.type = relItem.type;
                gen(plusMinus.op, relItem.place, item.place, symbol.place);
                symbolStack.push_back(symbol);
            }
        },
        {
            "RELITEM", {"ITEM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto item = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("RELITEM");
                symbol.place = item.place;
                symbol.type = item.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "ITEM", {"FACTOR"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto factor = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("ITEM");
                symbol.place = factor.place;
                symbol.type = factor.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "ITEM", {"ITEM", "MUL_DIV", "FACTOR"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto factor = symbolStack.back();
                symbolStack.pop_back();
                auto mulDiv = symbolStack.back();
                symbolStack.pop_back();
                auto item = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("ITEM");
                symbol.place = newTemp(factor.type);
                symbol.type = item.type;
                gen(mulDiv.op, item.place, factor.place, symbol.place);
                symbolStack.push_back(symbol);
            }
        },
        {
            "FACTOR", {"ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto id = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("FACTOR");
                symbol.place = lookup(id.name);
                symbol.type = lookupType(id.name);
                symbolStack.push_back(symbol);
            }
        },
        {
            "FACTOR", {"UINT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                symbolStack.pop_back();
                auto symbol = Symbol("FACTOR");
                symbol.place = newTemp(SYMINT);
                symbol.type = SYMINT;
                gen("=", numAndId.front(), "-", symbol.place);
                numAndId.erase(numAndId.begin());
                symbolStack.push_back(symbol);
            }
        },
        {
            "FACTOR", {"UDOUBLE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                symbolStack.pop_back();
                auto symbol = Symbol("FACTOR");
                symbol.place = newTemp(SYMDOUBLE);
                symbol.type = SYMDOUBLE;
                gen("=", numAndId.front(), "-", symbol.place);
                numAndId.erase(numAndId.begin());
                symbolStack.push_back(symbol);
            }
        },
        {
            "FACTOR", {"LBRACE1", "EXPR", "RBRACE1"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                // )
                symbolStack.pop_back();
                auto expr = symbolStack.back();
                symbolStack.pop_back();
                // (
                symbolStack.pop_back();
                auto symbol = Symbol("FACTOR");
                symbol.place = expr.place;
                symbol.type = expr.type;
                symbolStack.push_back(symbol);
            }
        },
        {
            "FACTOR", {"PLUS_MINUS", "FACTOR"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto factor = symbolStack.back();
                symbolStack.pop_back();
                auto plusMinus = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("FACTOR");
                symbol.place = newTemp(factor.type);
                symbol.type = factor.type;
                gen(plusMinus.op, "0", factor.place, symbol.place);
                symbolStack.push_back(symbol);
            }
        },

        // cond. control expression
        {
            "B", {"B", "OR", "N", "BORTERM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 4;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto borTerm = symbolStack.back();
                symbolStack.pop_back();
                auto n = symbolStack.back();
                symbolStack.pop_back();
                // or
                symbolStack.pop_back();
                auto b = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("B");
                backpatch(b.falseList, n.quad);
                symbol.trueList = merge(b.trueList, borTerm.trueList);
                symbol.falseList = borTerm.falseList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "B", {"BORTERM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto borTerm = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("B");
                symbol.trueList = borTerm.trueList;
                symbol.falseList = borTerm.falseList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "BORTERM", {"BORTERM", "AND", "N", "BANDTERM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 4;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto bandTerm = symbolStack.back();
                symbolStack.pop_back();
                auto n = symbolStack.back();
                symbolStack.pop_back();
                // and
                symbolStack.pop_back();
                auto borTerm = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("BORTERM");
                backpatch(borTerm.trueList, n.quad);
                symbol.trueList = bandTerm.trueList;
                symbol.falseList = merge(borTerm.falseList, bandTerm.falseList);
                symbolStack.push_back(symbol);
            }
        },
        {
            "BORTERM", {"BANDTERM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto bandTerm = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("BORTERM");
                symbol.trueList = bandTerm.trueList;
                symbol.falseList = bandTerm.falseList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "BANDTERM", {"LBRACE1", "B", "RBRACE1"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                // )
                symbolStack.pop_back();
                auto b = symbolStack.back();
                symbolStack.pop_back();
                // (
                symbolStack.pop_back();
                auto symbol = Symbol("BANDTERM");
                symbol.trueList = b.trueList;
                symbol.falseList = b.falseList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "BANDTERM", {"NOT", "BANDTERM"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto bandTerm = symbolStack.back();
                symbolStack.pop_back();
                // not
                symbolStack.pop_back();
                auto symbol = Symbol("BANDTERM");
                symbol.trueList = bandTerm.falseList;
                symbol.falseList = bandTerm.trueList;
                symbolStack.push_back(symbol);
            }
        },
        {
            "BANDTERM", {"BFACTOR", "REL", "BFACTOR"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto bfactor2 = symbolStack.back();
                symbolStack.pop_back();
                auto rel = symbolStack.back();
                symbolStack.pop_back();
                auto bfactor1 = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("BANDTERM");
                symbol.trueList = makeList(nxq);
                symbol.falseList = makeList(nxq + 1);
                gen("j" + rel.op, bfactor1.place, bfactor2.place, "0");
                gen("j", "-", "-", "0");
                symbolStack.push_back(symbol);
            }
        },
        {
            "BANDTERM", {"BFACTOR"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto bfactor = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("BANDTERM");
                symbol.trueList = makeList(nxq);
                symbol.falseList = makeList(nxq + 1);
                gen("jnz", bfactor.place, "-", "0");
                gen("j", "-", "-", "0");
                symbolStack.push_back(symbol);
            }
        },
        {
            "BFACTOR", {"UINT"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                symbolStack.pop_back();
                auto symbol = Symbol("BFACTOR");
                symbol.place = newTemp(SYMINT);
                symbol.type = SYMINT;
                gen("=", numAndId.front(), "-", symbol.place);
                numAndId.erase(numAndId.begin());
                symbolStack.push_back(symbol);
            }
        },
        {
            "BFACTOR", {"UDOUBLE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                symbolStack.pop_back();
                auto symbol = Symbol("BFACTOR");
                symbol.place = newTemp(SYMDOUBLE);
                symbol.type = SYMDOUBLE;
                gen("=", numAndId.front(), "-", symbol.place);
                numAndId.erase(numAndId.begin());
                symbolStack.push_back(symbol);
            }
        },
        {
            "BFACTOR", {"ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                stateStack.pop_back();
                auto id = symbolStack.back();
                symbolStack.pop_back();
                auto symbol = Symbol("BFACTOR");
                symbol.place = lookup(id.name);
                symbol.type = lookupType(id.name);
                symbolStack.push_back(symbol);
            }
        },

        // operator
        {
            "PLUS_MINUS", {"PLUS"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("PLUS_MINUS");
                symbol.op = "+";
                symbolStack.push_back(symbol);
            }
        },
        {
            "PLUS_MINUS", {"MINUS"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("PLUS_MINUS");
                symbol.op = "-";
                symbolStack.push_back(symbol);
            }
        },
        {
            "MUL_DIV", {"TIMES"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("MUL_DIV");
                symbol.op = "*";
                symbolStack.push_back(symbol);
            }
        },
        {
            "MUL_DIV", {"DIVISION"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("MUL_DIV");
                symbol.op = "/";
                symbolStack.push_back(symbol);
            }
        },
        {
            "REL", {"EQUEQU"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("REL");
                symbol.op = "==";
                symbolStack.push_back(symbol);
            }
        },
        {
            "REL", {"NOTEQU"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("REL");
                symbol.op = "!=";
                symbolStack.push_back(symbol);
            }
        },
        {
            "REL", {"GREATER"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("REL");
                symbol.op = ">";
                symbolStack.push_back(symbol);
            }
        },
        {
            "REL", {"GE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("REL");
                symbol.op = ">=";
                symbolStack.push_back(symbol);
            }
        },
        {
            "REL", {"LESS"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("REL");
                symbol.op = "<";
                symbolStack.push_back(symbol);
            }
        },
        {
            "REL", {"LE"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 1;
                while (rep--) {
                    symbolStack.pop_back();
                    stateStack.pop_back();
                }
                auto symbol = Symbol("REL");
                symbol.op = "<=";
                symbolStack.push_back(symbol);
            }
        },

        // i/o
        {
            "SCANF", {"SCANF_BEGIN", "RBRACE1"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                }
                // )
                symbolStack.pop_back();
                // scanf
                symbolStack.pop_back();
                auto symbol = Symbol("SCANF");
                symbolStack.push_back(symbol);
            }
        },
        {
            "SCANF_BEGIN", {"SCANF_BEGIN", "COMMA", "ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto id = symbolStack.back();
                symbolStack.pop_back();
                // comma
                symbolStack.pop_back();
                auto scanfBegin = symbolStack.back();
                symbolStack.pop_back();
                auto p = lookup(id.name);
                gen("R", "-", "-", p);
                symbolStack.push_back(Symbol("SCANF_BEGIN"));
            }
        },
        {
            "SCANF_BEGIN", {"SCANFSYM", "LBRACE1", "ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto id = symbolStack.back();
                symbolStack.pop_back();
                // (
                symbolStack.pop_back();
                // scanf
                symbolStack.pop_back();
                auto p = lookup(id.name);
                gen("R", "-", "-", p);
                symbolStack.push_back(Symbol("SCANF_BEGIN"));
            }
        },
        {
            "PRINTF", {"PRINTF_BEGIN", "RBRACE1"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 2;
                while (rep--) {
                    stateStack.pop_back();
                }
                // )
                symbolStack.pop_back();
                // printf
                symbolStack.pop_back();
                auto symbol = Symbol("PRINTF");
                symbolStack.push_back(symbol);
            }
        },
        {
            "PRINTF_BEGIN", {"PRINTF_BEGIN", "COMMA", "ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto id = symbolStack.back();
                symbolStack.pop_back();
                // comma
                symbolStack.pop_back();
                auto printfBegin = symbolStack.back();
                symbolStack.pop_back();
                auto p = lookup(id.name);
                gen("W", "-", "-", p);
                symbolStack.push_back(Symbol("PRINTF_BEGIN"));
            }
        },
        {
            "PRINTF_BEGIN", {"PRINTFSYM", "LBRACE1", "ID"}, [&]() {
                // std::cerr << "Calling " << __LINE__ << std::endl;
                int rep = 3;
                while (rep--) {
                    stateStack.pop_back();
                }
                auto id = symbolStack.back();
                symbolStack.pop_back();
                // (
                symbolStack.pop_back();
                // printf
                symbolStack.pop_back();
                auto p = lookup(id.name);
                gen("W", "-", "-", p);
                symbolStack.push_back(Symbol("PRINTF_BEGIN"));
            }
        }
    };

    // std::cerr << productions.size() << '\n';
    // for (auto& production : productions) {
    //     std::cerr << production.lhs << ": ";
    //     for (auto& rhs : production.rhs) {
    //         std::cerr << rhs << ", ";
    //     }
    //     std::cerr << '\n';
    // }

    for (int i = 0; i < productions.size(); i++) {
        leftToRight[productions[i].lhs].push_back(i);
    }
}

}