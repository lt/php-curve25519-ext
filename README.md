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

It is not necessary for you to perform key clamping as detailed at http://cr.yp.to/ecdh.html as this is handled internally by the curve25519 implementation.

### How to install:

```
git clone git://github.com/lt/php-curve25519-ext.git
cd php-curve25519-ext
phpize
./configure
make
sudo make install
```
Finally add `extension=curve25519.so` to your /etc/php.ini

### Multi-party shared secrets:

When more than two parties are communicating, it is necessary to communicate intermediate values so that each party can compute a common shared secret

```
$alicePrivate = str_repeat('a', 32);
$bobPrivate   = str_repeat('b', 32);
$carolPrivate = str_repeat('c', 32);

$alicePublic = curve25519_public($alicePrivate); // Send to Bob
$bobPublic   = curve25519_public($bobPrivate);   // Send to Carol
$carolPublic = curve25519_public($carolPrivate); // Send to Alice

$aliceCarolShared = curve25519_shared($alicePrivate, $carolPublic); // Send to Bob
$bobAliceShared   = curve25519_shared($bobPrivate,   $alicePublic); // Send to Carol
$carolBobShared   = curve25519_shared($carolPrivate, $bobPublic);   // Send to Alice

$aliceShared = curve25519_shared($alicePrivate, $carolBobShared);
$bobShared   = curve25519_shared($bobPrivate,   $aliceCarolShared);
$carolShared = curve25519_shared($carolPrivate, $bobAliceShared);

// An adversary observed f(a), f(b), f(c), f(ab), f(ac), and f(bc), whereas each party solved for f(abc)
var_dump($aliceShared === $bobShared && $bobShared === $carolShared);
```
