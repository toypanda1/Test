#include "NSP_TDYTH_NAAAS_sha256.h"
#include "memory.h"

#ifndef lint
static const char rcsid[] =
        "$Id: sha256.c 680 2003-07-25 21:57:49Z asaddi $";
#endif /* !lint */

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define Ch(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define SIGMA0(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))
#define SIGMA1(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))
#define sigma0(x) (ROTR((x), 7) ^ ROTR((x), 18) ^ ((x) >> 3))
#define sigma1(x) (ROTR((x), 17) ^ ROTR((x), 19) ^ ((x) >> 10))

#define DO_ROUND() { \
	t1 = h + SIGMA1(e) + Ch(e, f, g) + *(Kp++) + *(W++); \
	t2 = SIGMA0(a) + Maj(a, b, c); \
	h = g; \
	g = f; \
	f = e; \
	e = d + t1; \
	d = c; \
	c = b; \
	b = a; \
	a = t1 + t2; \
}

static const uint32_t K[64] = {
        0x428a2f98L, 0x71374491L, 0xb5c0fbcfL, 0xe9b5dba5L,
        0x3956c25bL, 0x59f111f1L, 0x923f82a4L, 0xab1c5ed5L,
        0xd807aa98L, 0x12835b01L, 0x243185beL, 0x550c7dc3L,
        0x72be5d74L, 0x80deb1feL, 0x9bdc06a7L, 0xc19bf174L,
        0xe49b69c1L, 0xefbe4786L, 0x0fc19dc6L, 0x240ca1ccL,
        0x2de92c6fL, 0x4a7484aaL, 0x5cb0a9dcL, 0x76f988daL,
        0x983e5152L, 0xa831c66dL, 0xb00327c8L, 0xbf597fc7L,
        0xc6e00bf3L, 0xd5a79147L, 0x06ca6351L, 0x14292967L,
        0x27b70a85L, 0x2e1b2138L, 0x4d2c6dfcL, 0x53380d13L,
        0x650a7354L, 0x766a0abbL, 0x81c2c92eL, 0x92722c85L,
        0xa2bfe8a1L, 0xa81a664bL, 0xc24b8b70L, 0xc76c51a3L,
        0xd192e819L, 0xd6990624L, 0xf40e3585L, 0x106aa070L,
        0x19a4c116L, 0x1e376c08L, 0x2748774cL, 0x34b0bcb5L,
        0x391c0cb3L, 0x4ed8aa4aL, 0x5b9cca4fL, 0x682e6ff3L,
        0x748f82eeL, 0x78a5636fL, 0x84c87814L, 0x8cc70208L,
        0x90befffaL, 0xa4506cebL, 0xbef9a3f7L, 0xc67178f2L
};


#ifndef RUNTIME_ENDIAN

#ifdef WORDS_BIGENDIAN

#define BYTESWAP(x) (x)
#define BYTESWAP64(x) (x)

#else /* WORDS_BIGENDIAN */

#define BYTESWAP(x) ((ROTR((x), 8) & 0xff00ff00L) | \
	(ROTL((x), 8) & 0x00ff00ffL))
#define BYTESWAP64(x) _byteswap64(x)

//static inline uint64_t _byteswap64(uint64_t x)
uint64_t _byteswap64(uint64_t x)
{
    uint32_t a = x >> 32;
    uint32_t b = (uint32_t) x;
    return ((uint64_t) BYTESWAP(b) << 32) | (uint64_t) BYTESWAP(a);
}

#endif /* WORDS_BIGENDIAN */

#endif /* !RUNTIME_ENDIAN */

static const uint8_t padding[64] = {
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void
SHA256Init (SHA256Context *sc)
{
#ifdef RUNTIME_ENDIAN
    setEndian (&sc->littleEndian);
#endif /* RUNTIME_ENDIAN */

    sc->totalLength = 0LL;
    sc->hash[0] = 0x6a09e667L;
    sc->hash[1] = 0xbb67ae85L;
    sc->hash[2] = 0x3c6ef372L;
    sc->hash[3] = 0xa54ff53aL;
    sc->hash[4] = 0x510e527fL;
    sc->hash[5] = 0x9b05688cL;
    sc->hash[6] = 0x1f83d9abL;
    sc->hash[7] = 0x5be0cd19L;
    sc->bufferLength = 0L;
}

static void
burnStack (int size)
{
    char buf[128];

    memset (buf, 0, sizeof (buf));
    size -= sizeof (buf);
    if (size > 0)
        burnStack (size);
}

static void
SHA256Guts (SHA256Context *sc, const uint32_t *cbuf)
{
    uint32_t buf[64];
    uint32_t *W, *W2, *W7, *W15, *W16;
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t1, t2;
    const uint32_t *Kp;
    int i;

    W = buf;

    for (i = 15; i >= 0; i--) {
        *(W++) = BYTESWAP(*cbuf);
        cbuf++;
    }

    W16 = &buf[0];
    W15 = &buf[1];
    W7 = &buf[9];
    W2 = &buf[14];

    for (i = 47; i >= 0; i--) {
        *(W++) = sigma1(*W2) + *(W7++) + sigma0(*W15) + *(W16++);
        W2++;
        W15++;
    }

    a = sc->hash[0];
    b = sc->hash[1];
    c = sc->hash[2];
    d = sc->hash[3];
    e = sc->hash[4];
    f = sc->hash[5];
    g = sc->hash[6];
    h = sc->hash[7];

    Kp = K;
    W = buf;

#ifndef SHA256_UNROLL
#define SHA256_UNROLL 1
#endif /* !SHA256_UNROLL */

#if SHA256_UNROLL == 1
    for (i = 63; i >= 0; i--)
    DO_ROUND();
#elif SHA256_UNROLL == 2
    for (i = 31; i >= 0; i--) {
		DO_ROUND(); DO_ROUND();
	}
#elif SHA256_UNROLL == 4
	for (i = 15; i >= 0; i--) {
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	}
#elif SHA256_UNROLL == 8
	for (i = 7; i >= 0; i--) {
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	}
#elif SHA256_UNROLL == 16
	for (i = 3; i >= 0; i--) {
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	}
#elif SHA256_UNROLL == 32
	for (i = 1; i >= 0; i--) {
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
		DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	}
#elif SHA256_UNROLL == 64
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
#elif SHA256_UNROLL == 68
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
	DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
#else
#error "SHA256_UNROLL must be 1, 2, 4, 8, 16, 32, or 64!"
#endif

    sc->hash[0] += a;
    sc->hash[1] += b;
    sc->hash[2] += c;
    sc->hash[3] += d;
    sc->hash[4] += e;
    sc->hash[5] += f;
    sc->hash[6] += g;
    sc->hash[7] += h;
}

void
SHA256Update (SHA256Context *sc, const void *vdata, uint32_t len)
{
    const uint8_t *data = (const uint8_t *)vdata;
    uint32_t bufferBytesLeft;
    uint32_t bytesToCopy;
    int needBurn = 0;

#ifdef SHA256_FAST_COPY
#else /* SHA256_FAST_COPY */
    while (len) {
        bufferBytesLeft = 64L - sc->bufferLength;

        bytesToCopy = bufferBytesLeft;
        if (bytesToCopy > len)
            bytesToCopy = len;

        memcpy (&sc->buffer.bytes[sc->bufferLength], data, bytesToCopy);

        sc->totalLength += bytesToCopy * 8L;

        sc->bufferLength += bytesToCopy;
        data += bytesToCopy;
        len -= bytesToCopy;

        if (sc->bufferLength == 64L) {
            SHA256Guts (sc, sc->buffer.words);
            needBurn = 1;
            sc->bufferLength = 0L;
        }
    }
#endif /* SHA256_FAST_COPY */

    if (needBurn)
        burnStack (sizeof (uint32_t[74]) + sizeof (uint32_t *[6]) + sizeof (int));
}

void
SHA256Final (SHA256Context *sc, uint8_t hash[SHA256_HASH_SIZE])
{
    uint32_t bytesToPad;
    uint64_t lengthPad;
    int i;

    bytesToPad = 120L - sc->bufferLength;
    if (bytesToPad > 64L)
        bytesToPad -= 64L;

    lengthPad = BYTESWAP64(sc->totalLength);

    SHA256Update (sc, padding, bytesToPad);
    SHA256Update (sc, &lengthPad, 8L);

    if (hash) {
        for (i = 0; i < SHA256_HASH_WORDS; i++) {
#ifdef SHA256_FAST_COPY
            *((uint32_t *) hash) = BYTESWAP(sc->hash[i]);
#else /* SHA256_FAST_COPY */
            hash[0] = (uint8_t) (sc->hash[i] >> 24);
            hash[1] = (uint8_t) (sc->hash[i] >> 16);
            hash[2] = (uint8_t) (sc->hash[i] >> 8);
            hash[3] = (uint8_t) sc->hash[i];
#endif /* SHA256_FAST_COPY */
            hash += 4;
        }
    }
}

void truncate
        (
                char* d1, /* data to be truncated */
                char* d2, /* truncated data */
                int len /* length in bytes to keep */
        )
{
    int i ;
    for (i = 0 ; i < len ; i++)
    {
        d2[i] = d1[i];
    }
}

void sha256
        (
                const char* d, /* data */
                int ld, /* length of data in bytes */
                char* out, /* output buffer, at least "t" bytes */
                int* t
        )
{
    SHA256Context ictx;
    char isha[SHA256_HASH_SIZE];

    /**** Inner Digest ****/
    SHA256Init(&ictx) ;
    SHA256Update(&ictx, d, ld) ;
    SHA256Final(&ictx, isha) ;

    *t = *t > SHA256_HASH_SIZE ? SHA256_HASH_SIZE : *t;
    truncate(isha, out, *t) ;
}

void hmac_sha256
        (
                const char* k, /* secret key */
                int lk, /* length of the key in bytes */
                const char* d, /* data */
                int ld, /* length of data in bytes */
                char* out, /* output buffer, at least "t" bytes */
                int* t
        )
{
    SHA256Context ictx, octx ;
    char isha[SHA256_HASH_SIZE], osha[SHA256_HASH_SIZE] ;
    char key[SHA256_HASH_SIZE] ;
    char buf[SHA_BLOCKSIZE] ;
    int i ;

    if (lk > SHA_BLOCKSIZE) {

        SHA256Context tctx ;

        SHA256Init(&tctx) ;
        SHA256Update(&tctx, k, lk) ;
        SHA256Final(&tctx, key) ;

        k = key ;
        lk = SHA256_HASH_SIZE ;
    }

    /**** Inner Digest ****/

    SHA256Init(&ictx) ;

    /* Pad the key for inner digest */
    for (i = 0 ; i < lk ; ++i) buf[i] = k[i] ^ 0x36 ;
    for (i = lk ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x36 ;

    SHA256Update(&ictx, buf, SHA_BLOCKSIZE) ;
    SHA256Update(&ictx, d, ld) ;

    SHA256Final(&ictx, isha) ;

    /**** Outter Digest ****/

    SHA256Init(&octx) ;

    /* Pad the key for outter digest */

    for (i = 0 ; i < lk ; ++i) buf[i] = k[i] ^ 0x5C ;
    for (i = lk ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x5C ;

    SHA256Update(&octx, buf, SHA_BLOCKSIZE);
    SHA256Update(&octx, isha, SHA256_HASH_SIZE);
    SHA256Final(&octx, osha) ;

    /* truncate and print the results */
    *t = *t > SHA256_HASH_SIZE ? SHA256_HASH_SIZE : *t;
    truncate(osha, out, *t);
}

//short类型大小端转换函数
unsigned short BLEndianUshort(unsigned short value)
{
    return ((value & 0x00FF) << 8 ) | ((value & 0xFF00) >> 8);
}
