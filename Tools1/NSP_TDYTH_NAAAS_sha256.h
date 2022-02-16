//sha256.h
typedef unsigned long int uint64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;


#define NSP_TDYTH_4G_Kasme_S_LEN 14

#define SHA256_HASH_SIZE 32

/* Hash size in 32-bit words */
#define SHA256_HASH_WORDS 8

#ifndef SHA_DIGESTSIZE
#define SHA_DIGESTSIZE 20
#endif

#ifndef SHA_BLOCKSIZE
#define SHA_BLOCKSIZE 64
#endif

struct _SHA256Context {
    uint64_t totalLength;
    uint32_t hash[SHA256_HASH_WORDS];
    uint32_t bufferLength;
    union {
        uint32_t words[16];
        uint8_t bytes[64];
    } buffer;
#ifdef RUNTIME_ENDIAN
    int littleEndian;
#endif /* RUNTIME_ENDIAN */
};

typedef struct _SHA256Context SHA256Context;

#ifdef __cplusplus
extern "C" {
#endif

void SHA256Init (SHA256Context *sc);
void SHA256Update (SHA256Context *sc, const void *data, uint32_t len);
void SHA256Final (SHA256Context *sc, uint8_t hash[SHA256_HASH_SIZE]);
void truncate(char* d1, char* d2, int len );
void sha256(const char* d,int ld, char* out, int* t);
void hmac_sha256(const char* k, int lk, const char* d, int ld, char* out, int* t);
unsigned short BLEndianUshort(unsigned short value);

#ifdef __cplusplus
}
#endif