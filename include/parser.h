#ifndef PARSER_H
#define PARSER_H

// External Headers
#include <unordered_map>
#include <vector>

// Local Headers
#include "common.h"

#define NUM_M_FLAGS 1
typedef vector<string> ARGS;

class PARSER {
private:
    static const array<string, NUM_M_FLAGS> mainFlags;
    static const unordered_map<string, int> mulArgs; 

    bool success;
    unordered_map<string, ARGS> flags;

public:
    PARSER(int argc, char *argv[]);

    void print_flags();
    bool is_successful() {
        return success;
    };
};

#endif // PARSER_H