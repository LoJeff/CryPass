// Local Headers
#include "common.h"
#include "parser.h"
#include "cry.h"

int main(int argc, char *argv[]) {

    string input;
    PARSER parser;
    CRY crypt;
    
    while (true) {
        cout << ">> ";
        getline(cin, input);
        cout << endl;
        parser.parse(input);

        string cmdStr = parser.get_cmd_str();
        CMD cmd = parser.get_cmd(cmdStr);
        switch (cmd) {
            case CMD::GEN_RSA:
            case CMD::LOAD_RSA:
            case CMD::ENC_RSA:
            cout << "Running " << cmdStr << endl;
                crypt.init(parser);
                crypt.run();
                break;
            case CMD::HELP:
                parser.list_cmds();
                break;
            case CMD::Q:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "To exit type Q" << endl;
        }
    }
}