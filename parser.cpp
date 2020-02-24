// Local Headers
#include "parser.h"

const array<string, NUM_M_FLAGS> PARSER::m_mainFlags({
    "cmd",
    "output"
});

const unordered_map<string, int> PARSER::m_mulArgs({
    {"-o", 0},
    {"-i", -1}
});

const CMD_ENUM_MAP PARSER::m_cmdToEnum({
    {"INVALID", INVALID},
    {"HELP", HELP},
    {"GEN_RSA", GEN_RSA},
    {"ENC", ENC},
    {"EXIT", EXIT},
    {"Q", Q}
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

    for (int i = 0; i < argv.size(); ++i) {
        if (argv[i][0] == '-') { // Flag
            auto curArg = m_mulArgs.find(argv[i]);
            if (curArg == m_mulArgs.end()) {
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
            if (mId < NUM_M_FLAGS) { // if all main flags are not taken
                if (mId == 0) { // First main flag is always CMD
                    // Make case-insensitive
                    for (int j = 0; j < argv[i].size(); ++j) {
                        argv[i][j] = toupper(argv[i][j]);
                    }

                    if (!m_cmdToEnum.count(argv[i])) {
                        cout << "Command " << argv[i] << " does not exist" << endl;
                        return false;
                    }
                }
                m_flags.insert(make_pair(m_mainFlags[mId], ARGS({argv[i]})));
                ++mId;
            } else {
                cout << "All main flags filled" << endl;
                print_flags();
                return false;
            }
        }
    }

    if (m_flags.count("cmd")) {
        return true;
    } else {
        cout << "Please provide a command" << endl;
        return false;
    }
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
    CMD_ENUM_MAP::const_iterator it = m_cmdToEnum.find(s);
    return it == m_cmdToEnum.end() ? CMD::INVALID : get<1>(*it);
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
    for (auto it = m_cmdToEnum.begin(); it != m_cmdToEnum.end(); ++it) {
        if (get<1>(*it) > CMD::INVALID) {
            cout << "   " << get<0>(*it) << endl;
        }
    }
}