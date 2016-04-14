int curve25519_donna(unsigned char *mypublic, const unsigned char *secret, const unsigned char *basepoint);

extern zend_module_entry curve25519_module_entry;

#define curve25519_module_ptr &curve25519_module_entry
#define phpext_curve25519_ptr curve25519_module_ptr

