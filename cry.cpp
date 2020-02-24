// Local Headers
#include "cry.h"

CRY::CRY() {

}

bool CRY::run() {
    switch (m_cmd) {
        case CMD::GEN_RSA:
            gen_rsa();
            break;
        case CMD::ENC:
            break;
        default:
            assert(false);
    }
    return true;
}

void CRY::init(const PARSER &parser) {
    CMD c = parser.get_cmd();
    if (c >= CMD::GEN_RSA && c <= CMD::ENC) {
        m_configs.clear();
        m_cmd = c;
        switch (c) {
            case CMD::GEN_RSA:
                parser.get_flag(m_configs, "-o");
                break;
        };
    } else {
        m_cmd = CMD::INVALID;
    }
}

void CRY::gen_rsa() {
    bool writeToFile = m_configs.count("-o");

    int ret = 0;
    int bits = 2048;
    unsigned long e = RSA_F4;
    RSA *r = NULL;
    BIGNUM *bne = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;
    size_t pri_len, pub_len;
    char *pri_key = NULL, *pub_key = NULL;

    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) goto free_all;

    r = RSA_new();
    ret = RSA_generate_key_ex(r, bits, bne, NULL);
    if (ret != 1) goto free_all;

    // 2. create public key
    if (writeToFile) {
        bp_public = BIO_new_file("public.pem", "w+");
    } else {
        bp_public = BIO_new(BIO_s_mem());
    }
    ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if(ret != 1) goto free_all;

	// 3. create private key
    if (writeToFile) {
        bp_private = BIO_new_file("private.pem", "w+");
    } else {
        bp_private = BIO_new(BIO_s_mem());
    }
    ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    // 4. Reading PEM
    pri_len = BIO_pending(bp_private);
    pub_len = BIO_pending(bp_public);

    pri_key = new char[pri_len+1];
    pub_key = new char[pub_len+1];

    BIO_read(bp_private, pri_key, pri_len);
    BIO_read(bp_public, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    cout << pri_key << endl;
    cout << pub_key << endl;
free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(r);
	BN_free(bne);
    delete pri_key;
    delete pub_key;

	return;
}

void CRY::errorHandler()
{
    ERR_print_errors_fp(stderr);
    abort();
}

int CRY::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        errorHandler();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        errorHandler();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        errorHandler();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        errorHandler();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}