#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

const unsigned char basepoint[32] = {9};

void curve25519_clamp(unsigned char secret[32])
{
	secret[0] &= 248;
	secret[31] &= 127;
	secret[31] |= 64;
}

PHP_FUNCTION(curve25519_public)
{
	unsigned char *secret;
	int secret_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &secret, &secret_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (secret_len != 32) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Secret must be 32 bytes");
		RETURN_FALSE;
	}

	char *clamped = estrdup(secret);
	curve25519_clamp(clamped);

	unsigned char public[33];
	curve25519_donna(public, clamped, basepoint);

	efree(clamped);

	RETURN_STRINGL(public, 32, 1);
}

PHP_FUNCTION(curve25519_shared)
{
	unsigned char *secret;
	int secret_len;

	unsigned char *public;
	int public_len;


	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &secret, &secret_len, &public, &public_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (secret_len != 32) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Secret must be 32 bytes");
		RETURN_FALSE;
	}

	if (public_len != 32) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Public must be 32 bytes");
		RETURN_FALSE;
	}

	char *clamped = estrdup(secret);
	curve25519_clamp(clamped);

	unsigned char shared[33];
	curve25519_donna(shared, secret, public);

	efree(clamped);

	RETURN_STRINGL(shared, 32, 1);
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_curve25519_public, 0, 0, 1)
	ZEND_ARG_INFO(0, secret)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_curve25519_shared, 0, 0, 1)
	ZEND_ARG_INFO(0, secret)
	ZEND_ARG_INFO(0, public)
ZEND_END_ARG_INFO()

const zend_function_entry curve25519_functions[] = {
	PHP_FE(curve25519_public, arginfo_curve25519_public)
	PHP_FE(curve25519_shared, arginfo_curve25519_shared)
	PHP_FE_END
};

PHP_MINFO_FUNCTION(curve25519)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "curve25519 support", "enabled");
	php_info_print_table_end();
}

zend_module_entry curve25519_module_entry = {
	STANDARD_MODULE_HEADER,
	"curve25519",
	curve25519_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(curve25519),
	NO_VERSION_YET,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CURVE25519
ZEND_GET_MODULE(curve25519)
#endif
