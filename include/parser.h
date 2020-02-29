#ifndef PARSER_H
#define PARSER_H

// External Headers
#include <vector>
#include <unordered_map>
#include <tuple>

// Local Headers
#include "common.h"

typedef vector<string> ARGS;
typedef unordered_map<string, ARGS> STR_ARG_MAP;
typedef unordered_map<string, int> FLAG_INFO;
typedef vector<string> MAIN_ARGS;
typedef tuple<CMD, FLAG_INFO, MAIN_ARGS> CMD_INFO;
typedef unordered_map<string, CMD_INFO> CMD_INFO_MAP;

class PARSER {
private:
    static const CMD_INFO_MAP s_cmdToInfo;

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