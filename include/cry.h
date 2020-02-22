#ifndef CRY_H
#define CRY_H

// External Headers
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <unordered_map>

// Internal Headers
#include "common.h"

class CRY {
public:
    CRY();

    bool run();
    
    void set_cmd(CMD c);
private:
    void gen_rsa();

    CMD cmd;
};

#endif // CRY_H