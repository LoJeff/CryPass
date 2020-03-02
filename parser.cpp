// Local Headers
#include "parser.h"

const CMD_INFO_MAP PARSER::s_cmdToInfo({
    {"INVALID", make_tuple(
        INVALID,
        FLAG_INFO({
        }),
        MAIN_ARGS({
        })
    )},
    {"HELP", make_tuple(
        HELP,
        FLAG_INFO({
        }),
        MAIN_ARGS({
        })
    )},
    {"GEN_RSA", make_tuple(
        GEN_RSA,
        FLAG_INFO({
            {"-o", 0},
            {"-v", 0}
        }),
        MAIN_ARGS({
        })
    )},
    {"LOAD_RSA", make_tuple(
        LOAD_RSA,
        FLAG_INFO({
            {"-p", 0}
        }),
        MAIN_ARGS({
            "input"
        })
    )},
    {"ENC_RSA", make_tuple(
        ENC_RSA,
        FLAG_INFO({
        }),
        MAIN_ARGS({
        })
    )},
    {"Q", make_tuple(
        Q,
        FLAG_INFO({}),
        MAIN_ARGS({})
    )}
});

PARSER::PARSER() {
}

bool PARSER::parse(const string& s) {
    m_flags.clear();

    auto prevArg = m_flags.end();
    int mId = 0;
    int curArgsLeft = 0;

    vector<string> argv;
    sep_string(s, argv);

    // Grabbing command
    if (argv.empty()) {
        cout << "Please provide a command" << endl;
        return false;
    }
    
    // Make case-insensitive
    for (int j = 0; j < argv[0].size(); ++j) {
        argv[0][j] = toupper(argv[0][j]);
    }

    if (!s_cmdToInfo.count(argv[0])) {
        cout << "Provided command " << argv[0] << " does not exist" << endl;
        return false;
    }
    m_flags.insert(make_pair("cmd", ARGS({argv[0]})));
    
    auto cmd_info_it = s_cmdToInfo.find(argv[0]);
    CMD cmd = get<0>(cmd_info_it->second);
    FLAG_INFO flag_info = get<1>(cmd_info_it->second);
    MAIN_ARGS arg_map = get<2>(cmd_info_it->second);

    for (int i = 1; i < argv.size(); ++i) {
        if (argv[i][0] == '-') { // Flag
            auto curArg = flag_info.find(argv[i]);
            if (curArg == flag_info.end()) {
                cout << "Flag " << argv[i] << " is not a valid flag" << endl;
                return false;
            }

            // Insert flag with arguments separated by space
            auto res = m_flags.insert(make_pair(argv[i], ARGS()));
            if (!get<1>(res)) {
                cout << argv[i] << " flag occurs more than once" << endl;
                return false;
            }
            auto flagIt = get<0>(res);

            if (get<1>(*curArg) >= 0) { // Fixed number of args
                for (i = i+1; i < get<1>(*curArg); ++i) {
                    if (i >= argv.size() || argv[i][0] == '-') {
                        cout << "Missing args for flag " << get<0>(*flagIt) \
                            << ", expected " << get<1>(*curArg) << endl;
                        return false;
                    }
                    get<1>(*flagIt).push_back(string(argv[i]));
                }
            } else { // Infinite number of args
                while (i+1 < argv.size() && argv[i+1][0] != '-') {
                    get<1>(*flagIt).push_back(string(argv[++i]));
                }
            }
        } else { // Arg with no flag
            if (mId < arg_map.size()) { // if all main flags are not taken
                m_flags.insert(make_pair(arg_map[mId], ARGS({argv[i]})));
                ++mId;
            } else {
                cout << "All main flags filled" << endl;
                print_flags();
                return false;
            }
        }
    }

    return true;
}

void PARSER::print_flags()  const {
    cout << "Parser Flags:" << endl; 
    for (auto it = m_flags.begin(); it != m_flags.end(); ++it) {
        cout << "   " << get<0>(*it) << " :";
        for (int i = 0; i < get<1>(*it).size(); ++i) {
            cout << " " << get<1>(*it)[i];
        }
        cout << endl;
    }
}

string PARSER::get_cmd_str() const {
    auto it = m_flags.find("cmd");
    return it == m_flags.end() ? "" : get<1>(*it)[0];
}

CMD PARSER::get_cmd(string s) const {
    if (s.empty()) {
        s = get_cmd_str();
    }
    CMD_INFO_MAP::const_iterator it = s_cmdToInfo.find(s);
    return it == s_cmdToInfo.end() ? CMD::INVALID : get<0>(get<1>(*it));
}

void PARSER::get_flag(STR_ARG_MAP &flags, const string &s) const {
    auto it = m_flags.find(s);
    if (it != m_flags.end()) {
        flags.insert(*it);
    }
}

void PARSER::sep_string(const string &s, vector<string> &vec) {
    vec.clear();
    int i = 0;
    while (i < s.size()) {
        vec.push_back("");
        while (i < s.size() && !isspace(s[i])) {
            vec.back().push_back(s[i]);
            ++i;
        }

        while (i < s.size() && isspace(s[i])) {
            ++i;
        }
    }
}

void PARSER::list_cmds() const {
    cout << "COMMAND LIST:" << endl;
    for (auto it = s_cmdToInfo.begin(); it != s_cmdToInfo.end(); ++it) {
        if (get<0>(get<1>(*it)) > CMD::INVALID) {
            cout << "   " << get<0>(*it) << endl;
        }
    }
}