// External Headers
#include <openssl/rsa.h>

// Local Headers
#include "common.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    cout << "ARGS" << endl;
    for (int i = 0; i < argc; ++i) {
        cout << "   " << argv[i] << endl;
    }

    PARSER* parser = new PARSER(argc, argv);

    if (parser->is_successful()) {
        cout << "PASS" << endl;
        parser->print_flags();
    } else {
        cout << "FAIL" << endl;
    }
    delete parser;
    parser = NULL;
    return 0;
}