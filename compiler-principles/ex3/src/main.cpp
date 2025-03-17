#include "./common.h"
#include "./assembler/assmebler.h"

#ifdef _WIN32
#include "./assembler/assembler.cpp"
#endif

#include <set>
#include <algorithm>


int main(int argc, char* argv[]) {
    Assembler assembler;
    assembler.run();
    return 0;
}