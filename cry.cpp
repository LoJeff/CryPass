// Local Headers
#include "cry.h"

CRY::CRY():
    m_rsa(RSA_new(), ::RSA_free) {
}

CRY::~CRY() {
}

bool CRY::run() {
    switch (m_cmd) {
        case CMD::GEN_RSA:
            gen_rsa();
            break;
        case CMD::LOAD_RSA:
            load_rsa();
            break;
        case CMD::ENC_RSA:
            break;
        default:
            assert(false);
    }
    return true;
}

void CRY::init(const PARSER &parser) {
    CMD c = parser.get_cmd();
    if (c >= CMD::GEN_RSA && c <= CMD::ENC_RSA) {
        m_configs.clear();
        m_cmd = c;
        switch (c) {
            case CMD::GEN_RSA:
                parser.get_flag(m_configs, "-o");
                break;
            case CMD::LOAD_RSA:
                parser.get_flag(m_configs, "input");
                break;
        };
    } else {
        m_cmd = CMD::INVALID;
    }
}

void CRY::gen_rsa() {
    bool writeToFile = m_configs.count("-o");

    int ret = 0;
    BN_ptr bne(BN_new(), BN_free);
    size_t pri_len, pub_len;

    ret = BN_set_word(bne.get(), RSA_F4);
    assert(ret);

    ret = RSA_generate_key_ex(m_rsa.get(), (int) 2048, bne.get(), NULL);
    assert(ret);

    // 2. create public key
    BIO_FILE_ptr bp_public(BIO_alloc(writeToFile, "public.pem"), BIO_free);
    ret = PEM_write_bio_RSAPublicKey(bp_public.get(), m_rsa.get());
    assert(ret);

	// 3. create private key
    BIO_FILE_ptr bp_private(BIO_alloc(writeToFile, "private.pem"), BIO_free);
    ret = PEM_write_bio_RSAPrivateKey(bp_private.get(), m_rsa.get(), NULL, NULL, 0, NULL, NULL);
    assert(ret);

    // 4. Reading PEM
    if (!writeToFile) {
        dump_key(bp_public);
        dump_key(bp_private);
    }
}

void CRY::dump_key(BIO_FILE_ptr &bp) {
    size_t len;
    len = BIO_pending(bp.get());
    unsigned char key[len+1];
    assert(BIO_read(bp.get(), key, len));
    key[len] = '\0';
    cout << key << endl;
}

void CRY::load_rsa() {
    if (m_configs.count("input")) {
        string file_name = m_configs["input"][0];
        FILE * fp = fopen(file_name.c_str(),"rb");
        
        if(fp == NULL) {
            cout << "Unable to open file " << file_name << endl;
            return;
        }
        char file_header[33];
        fgets(file_header, 33, fp);

        cout << file_header << endl;        
    } else {
        cout << "Please provide a key" << endl;
    }
}