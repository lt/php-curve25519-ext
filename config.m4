PHP_ARG_ENABLE(curve25519, Whether to enable the "curve25519" extension,
	[  --enable-curve25519       Enable "php-curve25519" extension support])

if test $PHP_CURVE25519 != "no"; then
	PHP_NEW_EXTENSION(curve25519, curve25519.c, $ext_shared)
fi
