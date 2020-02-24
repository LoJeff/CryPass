#ifndef PARSER_H
#define PARSER_H

// External Headers
#include <vector>
#include <unordered_map>

// Local Headers
#include "common.h"

#define NUM_M_FLAGS 2
typedef vector<string> ARGS;
typedef unordered_map<string, ARGS> STR_ARG_MAP;
typedef unordered_map<string, CMD> CMD_ENUM_MAP;

class PARSER {
private:
    static const array<string, NUM_M_FLAGS> m_mainFlags;
    static const unordered_map<string, int> m_mulArgs; 
    static const CMD_ENUM_MAP m_cmdToEnum;

    STR_ARG_MAP m_flags;
public:
    PARSER();
    bool parse(const string& s);

    void print_flags() const;
    void list_cmds() const;

    string get_cmd_str() const;
    CMD get_cmd(string s = "") const;
    void get_flag(STR_ARG_MAP &flags, const string &s) const;
private:
    void sep_string(const string &s, vector<string> &vec);
};

#endif // PARSER_H