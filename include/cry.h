#ifndef CRY_H
#define CRY_H

// External Headers
#include <vector>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <memory>

// Internal Headers
#include "common.h"
#include "parser.h"

using BN_ptr = unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = unique_ptr<BIO, decltype(&::BIO_free)>;

#define BIO_alloc(wToF, name) wToF ? BIO_new_file(name, "w+") : BIO_new(BIO_s_mem())

class CRY {
public:
    CRY();
    ~CRY();

    bool run();
    
    void init(const PARSER &parser);
private:
    void gen_rsa();
    void load_rsa();

    void dump_key(BIO_FILE_ptr &bp);
    
    EVP_CIPHER_CTX *rsaEncryptCtx;
    EVP_CIPHER_CTX *rsaDecryptCtx;

    RSA_ptr m_rsa;
    CMD m_cmd;
    STR_ARG_MAP m_configs;
};

#endif // CRY_H