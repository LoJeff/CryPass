// Local Headers
#include "cry.h"

CRY::CRY() {
    m_pri_rsa = RSA_new();
    m_pub_rsa = RSA_new();
}

CRY::~CRY() {
    if (m_pri_rsa) {
        RSA_free(m_pri_rsa);
        m_pri_rsa = NULL;
    }

    if (m_pub_rsa) {
        RSA_free(m_pub_rsa);
        m_pub_rsa = NULL;
    }
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
    unsigned char *pri_key = NULL, *pub_key = NULL;

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

    pri_key = new unsigned char[pri_len+1];
    pub_key = new unsigned char[pub_len+1];

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

void CRY::errorHandler() {
    ERR_print_errors_fp(stderr);
    abort();
}

void CRY::createRSA(unsigned char * key, int pub) {
    if (pub ? m_pub_rsa != NULL : m_pri_rsa != NULL) {
        cout << "Overwriting " << (pub ? "Public" : "Private") << " Key" << endl;
    }

    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL) {
        printf( "Failed to create key BIO");
        return;
    }

    if (pub) {
        m_pub_rsa = PEM_read_bio_RSA_PUBKEY(keybio, &m_pub_rsa, NULL, NULL);
    } else {
        m_pri_rsa = PEM_read_bio_RSAPrivateKey(keybio, &m_pri_rsa, NULL, NULL);
    }
}

void CRY::createRSAWithFilename(char * filename, int pub) {
    FILE * fp = fopen(filename,"rb");

    if (fp == NULL) {
        cout << "Unable to open file " << filename << endl;
        return;
    }

    if(pub) {
        m_pub_rsa = PEM_read_RSA_PUBKEY(fp, &m_pub_rsa,NULL, NULL);
    } else {
        m_pri_rsa = PEM_read_RSAPrivateKey(fp, &m_pri_rsa,NULL, NULL);
    }
}