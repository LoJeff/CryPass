#ifndef CRY_H
#define CRY_H

// External Headers
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Internal Headers
#include "common.h"
#include "parser.h"

class CRY {
public:
    CRY();

    bool run();
    
    void init(const PARSER &parser);
private:
    void gen_rsa();
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);

    void errorHandler();

    CMD m_cmd;
    STR_ARG_MAP m_configs;
};

#endif // CRY_H