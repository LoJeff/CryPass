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
    ~CRY();

    bool run();
    
    void init(const PARSER &parser);
private:
    void gen_rsa();
    void load_rsa();

    void errorHandler();

    void createRSA(unsigned char * key, int pub);
    void createRSAWithFilename(char * filename, int pub);

    RSA *m_pri_rsa;
    RSA *m_pub_rsa;
    CMD m_cmd;
    STR_ARG_MAP m_configs;
};

#endif // CRY_H