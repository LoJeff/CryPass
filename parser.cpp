// Local Headers
#include "parser.h"

const array<string, NUM_M_FLAGS> PARSER::mainFlags({
    "output"
});

const unordered_map<string, int> PARSER::mulArgs({
    {"-enc", 1},
    {"-i", -1}
});

PARSER::PARSER(int argc, char *argv[]) {
    success = false;
    auto prevArg = flags.end();
    int mId = 0;
    int curArgsLeft = 0;
    string curStr;
    for (int i = 1; i < argc; ++i) {
        curStr = argv[i];
        
        if (curStr[0] == '-') { // Flag
            auto curArg = mulArgs.find(curStr);
            if (curArg == mulArgs.end()) {
                cout << "Flag " << curStr << " is not a valid flag" << endl;
                return;
            }

            // Insert flag with arguments separated by space
            auto res = flags.insert(make_pair(argv[i], ARGS()));
            if (!get<1>(res)) {
                cout << argv[i] << " flag occurs more than once" << endl;
                return;
            }
            auto flagIt = get<0>(res);

            if (get<1>(*curArg) >= 0) { // Fixed number of args
                for (i = i+1; i < get<1>(*curArg); ++i) {
                    if (i >= argc || argv[i][0] == '-') {
                        cout << "Missing args for flag " << get<0>(*flagIt) \
                            << ", expected " << get<1>(*curArg) << endl;
                        return;
                    }
                    get<1>(*flagIt).push_back(string(argv[i]));
                }
            } else { // Infinite number of args
                while (i+1 < argc && argv[i+1][0] != '-') {
                    get<1>(*flagIt).push_back(string(argv[++i]));
                }
            }
        } else { // Arg with no flag
            if (mId < NUM_M_FLAGS) { // if all main flags are taken
                flags.insert(make_pair(mainFlags[mId], ARGS({curStr})));
                ++mId;
            } else {
                cout << "All main flags filled" << endl;
                print_flags();
                return;
            }
        }
    }
    if (mId == NUM_M_FLAGS) success = true;
}

void PARSER::print_flags() {
    cout << "Parser Flags:" << endl; 
    for (auto it = flags.begin(); it != flags.end(); ++it) {
        cout << "   " << get<0>(*it) << " :";
        for (int i = 0; i < get<1>(*it).size(); ++i) {
            cout << " " << get<1>(*it)[i];
        }
        cout << endl;
    }
}