Curve25519 PHP extension
========================

This extension is a thin wrapper around [Adam Langley's curve25519-donna](https://github.com/agl/curve25519-donna) implementation

### Usage:

Start by generating 32 secret random bytes from a cryptographically safe source

Then generate the corresponding 32-byte public key by calling

```
$myPublic = curve25519_public($mySecret);
```

Given someone else's public key call

```
$shared = curve25519_shared($mySecret, $theirPublic);
```

The other party can compute the same secret by applying their secret key to your public key. Both of you can then use this shared secret for further communications.

It is not necessary for you to perform key clamping as detailed at http://cr.yp.to/ecdh.html as this is handled internally by the extension.

### How to install:

```
git clone git://github.com/lt/php-curve25519.git
cd php-curve25519
phpize
./configure
make
sudo make install
```
Finally add `extension=curve25519.so` to your /etc/php.ini
