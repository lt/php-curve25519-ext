#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_curve25519.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"

const unsigned char basepoint[32] = {9};

PHP_FUNCTION(curve25519_public)
{
	char *secret;
#if PHP_VERSION_ID >= 70000
    size_t secret_len;
#else
    int secret_len;
#endif	
	char public[32];
	char *unclamped;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &secret, &secret_len) == FAILURE) {
		RETURN_FALSE;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(secret, secret_len)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (secret_len != 32) {
		zend_throw_exception(spl_ce_InvalidArgumentException, "Secret must be 32 bytes", 0 TSRMLS_CC);
	}

	// curve25519_donna clamps, and would modify $secret
	unclamped = estrdup(secret);
	curve25519_donna(public, unclamped, basepoint);
	efree(unclamped);

#if PHP_VERSION_ID >= 70000
	RETURN_STRINGL(public, 32);
#else
	RETURN_STRINGL(public, 32, 1);
#endif
}

PHP_FUNCTION(curve25519_shared)
{
	char *secret;
	char *public;
#if PHP_VERSION_ID >= 70000
    size_t secret_len;
    size_t public_len;
#else
    int secret_len;
    int public_len;
#endif	
	char shared[32];
	char *unclamped;

#ifndef FAST_ZPP
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &secret, &secret_len, &public, &public_len) == FAILURE) {
		RETURN_FALSE;
	}
#else
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(secret, secret_len)
		Z_PARAM_STRING(public, public_len)
	ZEND_PARSE_PARAMETERS_END();
#endif

	if (secret_len != 32) {
		zend_throw_exception(spl_ce_InvalidArgumentException, "Secret must be 32 bytes", 0 TSRMLS_CC);
	}

	if (public_len != 32) {
		zend_throw_exception(spl_ce_InvalidArgumentException, "Public must be 32 bytes", 0 TSRMLS_CC);
	}

	unclamped = estrdup(secret);
	curve25519_donna(shared, unclamped, public);
	efree(unclamped);

#if PHP_VERSION_ID >= 70000
	RETURN_STRINGL(shared, 32);
#else
	RETURN_STRINGL(shared, 32, 1);
#endif
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_curve25519_public, 0, 0, 1)
	ZEND_ARG_INFO(0, secret)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_curve25519_shared, 0, 0, 2)
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
