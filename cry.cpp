// Local Headers
#include "cry.h"

CRY::CRY() {
}

bool CRY::run() {
    switch (cmd) {
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

void CRY::set_cmd(CMD c) {
    if (c >= CMD::GEN_RSA && c <= CMD::ENC) {
        cmd = c;
    } else {
        cmd = CMD::INVALID;
    }
}

void CRY::gen_rsa() {
    int ret = 0;
    int bits = 2048;
    unsigned long e = RSA_F4;
    RSA *r = NULL;
    BIGNUM *bn = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;

    bn = BN_new();
    ret = BN_set_word(bn, e);
    if (ret != 1) goto free_all;

    r = RSA_new();
    ret = RSA_generate_key_ex(r, bits, bn, NULL);
    if (ret != 1) goto free_all;

    // 2. save public key
	bp_public = BIO_new_file("public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if(ret != 1){
		goto free_all;
	}

	// 3. save private key
	bp_private = BIO_new_file("private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

	// 4. free
free_all:

	BIO_free_all(bp_public);
	BIO_free_all(bp_private);
	RSA_free(r);
	BN_free(bn);

	return;
}