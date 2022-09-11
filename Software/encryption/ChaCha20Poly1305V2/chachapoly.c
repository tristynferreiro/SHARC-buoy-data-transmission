#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

struct chachapolyaead_ctx aead_ctx;
            uint32_t seqnr = 0;
            uint32_t seqnr_aad = 0;
            int pos_aad = 0;
            uint8_t aead_k_1[64] = {
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
            uint8_t aead_k_2[64] = {
                0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
    const uint8_t one[8] = {1, 0, 0, 0, 0, 0, 0, 0}; /* NB little-endian */
    uint64_t aad_chacha_nonce_hdr = 0;
    uint8_t expected_tag[POLY1305_TAGLEN], poly_key[POLY1305_KEYLEN];
    int r = -1;
    int aad_pos = 0;

/*
chacha-merged.c version 20080118
D. J. Bernstein
Public domain.
*/

/* $OpenBSD: chacha.c,v 1.1 2013/11/21 00:45:44 djm Exp $ */

typedef unsigned char u8;
typedef unsigned int u32;

typedef struct chacha_ctx chacha_ctx;

#define U8C(v) (v##U)
#define U32C(v) (v##U)

#define U8V(v) ((u8)(v)&U8C(0xFF))
#define U32V(v) ((u32)(v)&U32C(0xFFFFFFFF))

#define ROTL32(v, n) (U32V((v) << (n)) | ((v) >> (32 - (n))))

#define U8TO32_LITTLE(p)                                                       \
  (((u32)((p)[0])) | ((u32)((p)[1]) << 8) | ((u32)((p)[2]) << 16) |            \
   ((u32)((p)[3]) << 24))

#define U32TO8_LITTLE(p, v)                                                    \
  do {                                                                         \
    (p)[0] = U8V((v));                                                         \
    (p)[1] = U8V((v) >> 8);                                                    \
    (p)[2] = U8V((v) >> 16);                                                   \
    (p)[3] = U8V((v) >> 24);                                                   \
  } while (0)

#define ROTATE(v, c) (ROTL32(v, c))
#define XOR(v, w) ((v) ^ (w))
#define PLUS(v, w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v), 1))

#define QUARTERROUND(a, b, c, d)                                               \
  a = PLUS(a, b);                                                              \
  d = ROTATE(XOR(d, a), 16);                                                   \
  c = PLUS(c, d);                                                              \
  b = ROTATE(XOR(b, c), 12);                                                   \
  a = PLUS(a, b);                                                              \
  d = ROTATE(XOR(d, a), 8);                                                    \
  c = PLUS(c, d);                                                              \
  b = ROTATE(XOR(b, c), 7);

static const char sigma[16] = "expand 32-byte k";
static const char tau[16] = "expand 16-byte k";

void chacha_keysetup(chacha_ctx *x, const u8 *k, u32 kbits) {
  const char *constants;

  x->input[4] = U8TO32_LITTLE(k + 0);
  x->input[5] = U8TO32_LITTLE(k + 4);
  x->input[6] = U8TO32_LITTLE(k + 8);
  x->input[7] = U8TO32_LITTLE(k + 12);
  if (kbits == 256) { /* recommended */
    k += 16;
    constants = sigma;
  } else { /* kbits == 128 */
    constants = tau;
  }
  x->input[8] = U8TO32_LITTLE(k + 0);
  x->input[9] = U8TO32_LITTLE(k + 4);
  x->input[10] = U8TO32_LITTLE(k + 8);
  x->input[11] = U8TO32_LITTLE(k + 12);
  x->input[0] = U8TO32_LITTLE(constants + 0);
  x->input[1] = U8TO32_LITTLE(constants + 4);
  x->input[2] = U8TO32_LITTLE(constants + 8);
  x->input[3] = U8TO32_LITTLE(constants + 12);
}

void chacha_ivsetup(chacha_ctx *x, const u8 *iv, const u8 *counter) {
  x->input[12] = counter == NULL ? 0 : U8TO32_LITTLE(counter + 0);
  x->input[13] = counter == NULL ? 0 : U8TO32_LITTLE(counter + 4);
  x->input[14] = U8TO32_LITTLE(iv + 0);
  x->input[15] = U8TO32_LITTLE(iv + 4);
}

void chacha_encrypt_bytes(chacha_ctx *x, const u8 *m, u8 *c, u32 bytes) {
  u32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
  u32 j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15;
  u8 *ctarget = NULL;
  u8 tmp[64];
  uint32_t i;

  if (!bytes)
    return;

  j0 = x->input[0];
  j1 = x->input[1];
  j2 = x->input[2];
  j3 = x->input[3];
  j4 = x->input[4];
  j5 = x->input[5];
  j6 = x->input[6];
  j7 = x->input[7];
  j8 = x->input[8];
  j9 = x->input[9];
  j10 = x->input[10];
  j11 = x->input[11];
  j12 = x->input[12];
  j13 = x->input[13];
  j14 = x->input[14];
  j15 = x->input[15];

  for (;;) {
    if (bytes < 64) {
      if (m != NULL) {
        for (i = 0; i < bytes; ++i) {
          tmp[i] = m[i];
        }
        m = tmp;
      }
      ctarget = c;
      c = tmp;
    }
    x0 = j0;
    x1 = j1;
    x2 = j2;
    x3 = j3;
    x4 = j4;
    x5 = j5;
    x6 = j6;
    x7 = j7;
    x8 = j8;
    x9 = j9;
    x10 = j10;
    x11 = j11;
    x12 = j12;
    x13 = j13;
    x14 = j14;
    x15 = j15;
    for (i = 20; i > 0; i -= 2) {
      QUARTERROUND(x0, x4, x8, x12)
      QUARTERROUND(x1, x5, x9, x13)
      QUARTERROUND(x2, x6, x10, x14)
      QUARTERROUND(x3, x7, x11, x15)
      QUARTERROUND(x0, x5, x10, x15)
      QUARTERROUND(x1, x6, x11, x12)
      QUARTERROUND(x2, x7, x8, x13)
      QUARTERROUND(x3, x4, x9, x14)
    }
    x0 = PLUS(x0, j0);
    x1 = PLUS(x1, j1);
    x2 = PLUS(x2, j2);
    x3 = PLUS(x3, j3);
    x4 = PLUS(x4, j4);
    x5 = PLUS(x5, j5);
    x6 = PLUS(x6, j6);
    x7 = PLUS(x7, j7);
    x8 = PLUS(x8, j8);
    x9 = PLUS(x9, j9);
    x10 = PLUS(x10, j10);
    x11 = PLUS(x11, j11);
    x12 = PLUS(x12, j12);
    x13 = PLUS(x13, j13);
    x14 = PLUS(x14, j14);
    x15 = PLUS(x15, j15);

    if (m != NULL) {
      x0 = XOR(x0, U8TO32_LITTLE(m + 0));
      x1 = XOR(x1, U8TO32_LITTLE(m + 4));
      x2 = XOR(x2, U8TO32_LITTLE(m + 8));
      x3 = XOR(x3, U8TO32_LITTLE(m + 12));
      x4 = XOR(x4, U8TO32_LITTLE(m + 16));
      x5 = XOR(x5, U8TO32_LITTLE(m + 20));
      x6 = XOR(x6, U8TO32_LITTLE(m + 24));
      x7 = XOR(x7, U8TO32_LITTLE(m + 28));
      x8 = XOR(x8, U8TO32_LITTLE(m + 32));
      x9 = XOR(x9, U8TO32_LITTLE(m + 36));
      x10 = XOR(x10, U8TO32_LITTLE(m + 40));
      x11 = XOR(x11, U8TO32_LITTLE(m + 44));
      x12 = XOR(x12, U8TO32_LITTLE(m + 48));
      x13 = XOR(x13, U8TO32_LITTLE(m + 52));
      x14 = XOR(x14, U8TO32_LITTLE(m + 56));
      x15 = XOR(x15, U8TO32_LITTLE(m + 60));
    }
    j12 = PLUSONE(j12);
    if (!j12) {
      j13 = PLUSONE(j13);
      /* stopping at 2^70 bytes per nonce is user's responsibility */
    }

    U32TO8_LITTLE(c + 0, x0);
    U32TO8_LITTLE(c + 4, x1);
    U32TO8_LITTLE(c + 8, x2);
    U32TO8_LITTLE(c + 12, x3);
    U32TO8_LITTLE(c + 16, x4);
    U32TO8_LITTLE(c + 20, x5);
    U32TO8_LITTLE(c + 24, x6);
    U32TO8_LITTLE(c + 28, x7);
    U32TO8_LITTLE(c + 32, x8);
    U32TO8_LITTLE(c + 36, x9);
    U32TO8_LITTLE(c + 40, x10);
    U32TO8_LITTLE(c + 44, x11);
    U32TO8_LITTLE(c + 48, x12);
    U32TO8_LITTLE(c + 52, x13);
    U32TO8_LITTLE(c + 56, x14);
    U32TO8_LITTLE(c + 60, x15);

    if (bytes <= 64) {
      if (bytes < 64) {
        for (i = 0; i < bytes; ++i)
          ctarget[i] = c[i];
      }
      x->input[12] = j12;
      x->input[13] = j13;
      return;
    }
    bytes -= 64;
    c += 64;
    if (m != NULL) {
      m += 64;
    }
  }
}

/*
 * Public Domain poly1305 from Andrew Moon
 * poly1305-donna-unrolled.c from https://github.com/floodyberry/poly1305-donna
 */

/* $OpenBSD: poly1305.c,v 1.3 2013/12/19 22:57:13 djm Exp $ */


#define mul32x32_64(a, b) ((uint64_t)(a) * (b))

#define U8TO32_LE(p)                                                           \
  (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) |                          \
   ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))

#define U32TO8_LE(p, v)                                                        \
  do {                                                                         \
    (p)[0] = (uint8_t)((v));                                                   \
    (p)[1] = (uint8_t)((v) >> 8);                                              \
    (p)[2] = (uint8_t)((v) >> 16);                                             \
    (p)[3] = (uint8_t)((v) >> 24);                                             \
  } while (0)

void poly1305_auth(unsigned char out[POLY1305_TAGLEN], const unsigned char *m,
                   size_t inlen, const unsigned char key[POLY1305_KEYLEN]) {
  uint32_t t0, t1, t2, t3;
  uint32_t h0, h1, h2, h3, h4;
  uint32_t r0, r1, r2, r3, r4;
  uint32_t s1, s2, s3, s4;
  uint32_t b, nb;
  size_t j;
  uint64_t t[5];
  uint64_t f0, f1, f2, f3;
  uint32_t g0, g1, g2, g3, g4;
  uint64_t c;
  unsigned char mp[16];

  /* clamp key */
  t0 = U8TO32_LE(key + 0);
  t1 = U8TO32_LE(key + 4);
  t2 = U8TO32_LE(key + 8);
  t3 = U8TO32_LE(key + 12);

  /* precompute multipliers */
  r0 = t0 & 0x3ffffff;
  t0 >>= 26;
  t0 |= t1 << 6;
  r1 = t0 & 0x3ffff03;
  t1 >>= 20;
  t1 |= t2 << 12;
  r2 = t1 & 0x3ffc0ff;
  t2 >>= 14;
  t2 |= t3 << 18;
  r3 = t2 & 0x3f03fff;
  t3 >>= 8;
  r4 = t3 & 0x00fffff;

  s1 = r1 * 5;
  s2 = r2 * 5;
  s3 = r3 * 5;
  s4 = r4 * 5;

  /* init state */
  h0 = 0;
  h1 = 0;
  h2 = 0;
  h3 = 0;
  h4 = 0;

  /* full blocks */
  if (inlen < 16)
    goto poly1305_donna_atmost15bytes;
poly1305_donna_16bytes:
  m += 16;
  inlen -= 16;

  t0 = U8TO32_LE(m - 16);
  t1 = U8TO32_LE(m - 12);
  t2 = U8TO32_LE(m - 8);
  t3 = U8TO32_LE(m - 4);

  h0 += t0 & 0x3ffffff;
  h1 += ((((uint64_t)t1 << 32) | t0) >> 26) & 0x3ffffff;
  h2 += ((((uint64_t)t2 << 32) | t1) >> 20) & 0x3ffffff;
  h3 += ((((uint64_t)t3 << 32) | t2) >> 14) & 0x3ffffff;
  h4 += (t3 >> 8) | (1 << 24);

poly1305_donna_mul:
  t[0] = mul32x32_64(h0, r0) + mul32x32_64(h1, s4) + mul32x32_64(h2, s3) +
         mul32x32_64(h3, s2) + mul32x32_64(h4, s1);
  t[1] = mul32x32_64(h0, r1) + mul32x32_64(h1, r0) + mul32x32_64(h2, s4) +
         mul32x32_64(h3, s3) + mul32x32_64(h4, s2);
  t[2] = mul32x32_64(h0, r2) + mul32x32_64(h1, r1) + mul32x32_64(h2, r0) +
         mul32x32_64(h3, s4) + mul32x32_64(h4, s3);
  t[3] = mul32x32_64(h0, r3) + mul32x32_64(h1, r2) + mul32x32_64(h2, r1) +
         mul32x32_64(h3, r0) + mul32x32_64(h4, s4);
  t[4] = mul32x32_64(h0, r4) + mul32x32_64(h1, r3) + mul32x32_64(h2, r2) +
         mul32x32_64(h3, r1) + mul32x32_64(h4, r0);

  h0 = (uint32_t)t[0] & 0x3ffffff;
  c = (t[0] >> 26);
  t[1] += c;
  h1 = (uint32_t)t[1] & 0x3ffffff;
  b = (uint32_t)(t[1] >> 26);
  t[2] += b;
  h2 = (uint32_t)t[2] & 0x3ffffff;
  b = (uint32_t)(t[2] >> 26);
  t[3] += b;
  h3 = (uint32_t)t[3] & 0x3ffffff;
  b = (uint32_t)(t[3] >> 26);
  t[4] += b;
  h4 = (uint32_t)t[4] & 0x3ffffff;
  b = (uint32_t)(t[4] >> 26);
  h0 += b * 5;

  if (inlen >= 16)
    goto poly1305_donna_16bytes;

/* final bytes */
poly1305_donna_atmost15bytes:
  if (!inlen)
    goto poly1305_donna_finish;

  for (j = 0; j < inlen; j++)
    mp[j] = m[j];
  mp[j++] = 1;
  for (; j < 16; j++)
    mp[j] = 0;
  inlen = 0;

  t0 = U8TO32_LE(mp + 0);
  t1 = U8TO32_LE(mp + 4);
  t2 = U8TO32_LE(mp + 8);
  t3 = U8TO32_LE(mp + 12);

  h0 += t0 & 0x3ffffff;
  h1 += ((((uint64_t)t1 << 32) | t0) >> 26) & 0x3ffffff;
  h2 += ((((uint64_t)t2 << 32) | t1) >> 20) & 0x3ffffff;
  h3 += ((((uint64_t)t3 << 32) | t2) >> 14) & 0x3ffffff;
  h4 += (t3 >> 8);

  goto poly1305_donna_mul;

poly1305_donna_finish:
  b = h0 >> 26;
  h0 = h0 & 0x3ffffff;
  h1 += b;
  b = h1 >> 26;
  h1 = h1 & 0x3ffffff;
  h2 += b;
  b = h2 >> 26;
  h2 = h2 & 0x3ffffff;
  h3 += b;
  b = h3 >> 26;
  h3 = h3 & 0x3ffffff;
  h4 += b;
  b = h4 >> 26;
  h4 = h4 & 0x3ffffff;
  h0 += b * 5;
  b = h0 >> 26;
  h0 = h0 & 0x3ffffff;
  h1 += b;

  g0 = h0 + 5;
  b = g0 >> 26;
  g0 &= 0x3ffffff;
  g1 = h1 + b;
  b = g1 >> 26;
  g1 &= 0x3ffffff;
  g2 = h2 + b;
  b = g2 >> 26;
  g2 &= 0x3ffffff;
  g3 = h3 + b;
  b = g3 >> 26;
  g3 &= 0x3ffffff;
  g4 = h4 + b - (1 << 26);

  b = (g4 >> 31) - 1;
  nb = ~b;
  h0 = (h0 & nb) | (g0 & b);
  h1 = (h1 & nb) | (g1 & b);
  h2 = (h2 & nb) | (g2 & b);
  h3 = (h3 & nb) | (g3 & b);
  h4 = (h4 & nb) | (g4 & b);

  f0 = ((h0) | (h1 << 26)) + (uint64_t)U8TO32_LE(&key[16]);
  f1 = ((h1 >> 6) | (h2 << 20)) + (uint64_t)U8TO32_LE(&key[20]);
  f2 = ((h2 >> 12) | (h3 << 14)) + (uint64_t)U8TO32_LE(&key[24]);
  f3 = ((h3 >> 18) | (h4 << 8)) + (uint64_t)U8TO32_LE(&key[28]);

  U32TO8_LE(&out[0], f0);
  f1 += (f0 >> 32);
  U32TO8_LE(&out[4], f1);
  f2 += (f1 >> 32);
  U32TO8_LE(&out[8], f2);
  f3 += (f2 >> 32);
  U32TO8_LE(&out[12], f3);
}

#define __STDC_WANT_LIB_EXT1__ 1

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && \
    !defined(__WINDOWS__)
#define __WINDOWS__
#endif

#if defined(__linux__) || defined(__CYGWIN__)
#include <endian.h>

#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define htole32(x) OSSwapHostToLittleInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#elif defined(__OpenBSD__)
#include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
#include <sys/endian.h>
#define le32toh(x) letoh32(x)
#define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)
#include <sys/param.h>
#include <winsock2.h>

#if BYTE_ORDER == LITTLE_ENDIAN
#define htole32(x) (x)
#define le32toh(x) (x)

#define htole64(x) (x)
#define le64toh(x) (x)

#elif BYTE_ORDER == BIG_ENDIAN
#define htole32(x) __builtin_bswap32(x)
#define le32toh(x) __builtin_bswap32(x)

#define htole64(x) __builtin_bswap64(x)
#define le64toh(x) __builtin_bswap64(x)

#else
#error byte order not supported
#endif /* endif byteorder */
#else

#error platform not supported

#endif /* endif platform */

#ifndef HAVE_TIMINGSAFE_BCMP

int timingsafe_bcmp(const void* b1, const void* b2, size_t n)
{
    const unsigned char *p1 = b1, *p2 = b2;
    int ret = 0;

    for (; n > 0; n--)
        ret |= *p1++ ^ *p2++;
    return (ret != 0);
}

#endif /* TIMINGSAFE_BCMP */

#ifndef HAVE_MEMSET_S
void memory_cleanse(void* p, size_t n)
{
#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
    memset(p, 0, n);
#endif
#endif
}

#else /* no memset_s available */
void memory_cleanse(void* p, size_t n) { (void)memset_s(p, n, 0, n); }
#endif

#define XOR(v, w) ((v) ^ (w))

int chacha20poly1305_init(struct chachapolyaead_ctx* ctx, const uint8_t* k_1, int k_1_len, const uint8_t* k_2, int k_2_len)
{
    if (k_1_len != CHACHA20_POLY1305_AEAD_KEY_LEN || k_2_len != CHACHA20_POLY1305_AEAD_KEY_LEN)
        return -1;
    chacha_keysetup(&ctx->main_ctx, k_1, 256);
    chacha_keysetup(&ctx->header_ctx, k_2, 256);
    ctx->cached_aad_seqnr = UINT64_MAX;
    return 0;
}

int chacha20poly1305_crypt(struct chachapolyaead_ctx* ctx, uint64_t seqnr, uint64_t seqnr_aad, int pos_aad, uint8_t* dest, size_t dest_len, const uint8_t* src, size_t src_len, int is_encrypt)
{

    if (
        // if we encrypt, make sure the source contains at least the expected AAD and the destination has at least space for the source + MAC
        (is_encrypt && (src_len < CHACHA20_POLY1305_AEAD_AAD_LEN || dest_len < src_len + POLY1305_TAGLEN)) ||
        // if we decrypt, make sure the source contains at least the expected AAD+MAC and the destination has at least space for the source - MAc
        (!is_encrypt && (src_len < CHACHA20_POLY1305_AEAD_AAD_LEN + POLY1305_TAGLEN || dest_len < src_len - POLY1305_TAGLEN))) {
        return r;
    }

    uint64_t chacha_iv = htole64(seqnr);
    memset(poly_key, 0, sizeof(poly_key));
    chacha_ivsetup(&ctx->main_ctx, (uint8_t*)&chacha_iv, NULL);
    chacha_encrypt_bytes(&ctx->main_ctx, poly_key, poly_key, sizeof(poly_key));

    if (!is_encrypt) {
        const uint8_t* tag = src + src_len - POLY1305_TAGLEN;

        poly1305_auth(expected_tag, src, src_len - POLY1305_TAGLEN, poly_key);
        if (timingsafe_bcmp(expected_tag, tag, POLY1305_TAGLEN) != 0) {
            goto out;
        }
        /* MAC has been successfully verified, make sure we don't covert it in decryption */
        src_len -= POLY1305_TAGLEN;
    }

    /* add AAD (encrypted length) */
    if (ctx->cached_aad_seqnr != seqnr_aad) {
        ctx->cached_aad_seqnr = seqnr_aad;
        aad_chacha_nonce_hdr = htole64(seqnr_aad);
        chacha_ivsetup(&ctx->header_ctx, (uint8_t*)&aad_chacha_nonce_hdr, NULL); // block counter 0
        chacha_encrypt_bytes(&ctx->header_ctx, NULL, ctx->aad_keystream_buffer, CHACHA20_ROUND_OUTPUT);
    }
    /* crypt the AAD (3 byte length) */
    dest[0] = XOR(src[0], ctx->aad_keystream_buffer[aad_pos + 0]);
    dest[1] = XOR(src[1], ctx->aad_keystream_buffer[aad_pos + 1]);
    dest[2] = XOR(src[2], ctx->aad_keystream_buffer[aad_pos + 2]);

    /* Set Chacha's block counter to 1 and encipher */
    chacha_ivsetup(&ctx->main_ctx, (uint8_t*)&chacha_iv, one);
    chacha_encrypt_bytes(&ctx->main_ctx, src + CHACHA20_POLY1305_AEAD_AAD_LEN, dest + CHACHA20_POLY1305_AEAD_AAD_LEN, src_len - CHACHA20_POLY1305_AEAD_AAD_LEN);

    /* If encrypting, calculate and append tag */
    if (is_encrypt) {
        poly1305_auth(dest + src_len, dest, src_len, poly_key);
    }
    r = 0;
out:
    memory_cleanse(expected_tag, sizeof(expected_tag));
    memory_cleanse(&chacha_iv, sizeof(chacha_iv));
    memory_cleanse(poly_key, sizeof(poly_key));
    return r;
}

int chacha20poly1305_get_length(struct chachapolyaead_ctx* ctx,
    uint32_t* len_out,
    uint64_t seqnr,
    const uint8_t* ciphertext)
{
    uint8_t buf[3], seqbuf[8];

    int pos = seqnr % AAD_PACKAGES_PER_ROUND * CHACHA20_POLY1305_AEAD_AAD_LEN;
    seqnr = seqnr / (float)AAD_PACKAGES_PER_ROUND; /* 21 x 3byte length packages fits in a ChaCha20 round */
    if (ctx->cached_aad_seqnr != seqnr) {
        /* we need to calculate the 64 keystream bytes since we reached a new sequence number */
        ctx->cached_aad_seqnr = seqnr;
        seqnr = htole64(seqnr);                                   // use LE for the nonce
        chacha_ivsetup(&ctx->header_ctx, (uint8_t*)&seqnr, NULL); // block counter 0
        chacha_encrypt_bytes(&ctx->header_ctx, NULL, ctx->aad_keystream_buffer, CHACHA20_ROUND_OUTPUT);
    }

    /* decrypt the ciphertext length by XORing the right position of the 64byte keystream cache with the ciphertext */
    *len_out = 0;
    *len_out = XOR(ciphertext[0], ctx->aad_keystream_buffer[pos + 0]) |
               XOR(ciphertext[1], ctx->aad_keystream_buffer[pos + 1]) << 8 |
               XOR(ciphertext[2], ctx->aad_keystream_buffer[pos + 2]) << 16;

    /* convert to host endianness 32bit integer (only 24bit though) */
    *len_out = le32toh(*len_out);
    return 0;
}

FILE *infile, *outfile;

void encrypt(void)
{
    char ch[256];
    int count = 0;

    while (1) {
    if(ch[count]==EOF)
		{
			break;
		}
		else
		{
            ch[count]=fgetc(infile);
            count++;

            uint8_t ciphertext_buf[255 + 16] = {0};
            chacha20poly1305_init(&aead_ctx, aead_k_1, 32, aead_k_2, 32);
            chacha20poly1305_crypt(&aead_ctx, seqnr, seqnr_aad, pos_aad, ciphertext_buf, 300, ch, 255, 1);
            for (int i = 0; i < count; i ++) {
                fprintf(outfile, "%d", ciphertext_buf[i]);

            }

            fclose(outfile);
            fclose(infile);

		}
    }
}

void decrypt(void)
{
    uint8_t ch[256];
    int count = 0;

    while (1) {
    if(ch[count]==EOF)
		{
			break;
		}
		else
		{
            ch[count]=fgetc(infile);
            count++;

            uint8_t ciphertext_buf[255 + 16];
            chacha20poly1305_init(&aead_ctx, aead_k_1, 32, aead_k_2, 32);
            chacha20poly1305_crypt(&aead_ctx, seqnr, seqnr_aad, pos_aad, ciphertext_buf, 300, ch, 255, 0);
            for (int i = 0; i < count; i ++) {
                fputc(ciphertext_buf[i], outfile);

            }
		}
    }
}
int main(int argc, char *argv[])
{
    int enc;
    char *s;

    if (argc != 4) {
        printf("Usage: lzss e/d infile outfile\n\te = encode\td = decode\n");
        return 1;
    }
    s = argv[1];
    if (s[1] == 0 && (*s == 'd' || *s == 'D' || *s == 'e' || *s == 'E'))
        enc = (*s == 'e' || *s == 'E');
    else {
        printf("? %s\n", s);  return 1;
    }
    if ((infile  = fopen(argv[2], "rb")) == NULL) {
        printf("? %s\n", argv[2]);  return 1;
    }
    if ((outfile = fopen(argv[3], "a")) == NULL) {
        printf("? %s\n", argv[3]);  return 1;
    }
    if (enc) encrypt();  else decrypt();
    fclose(infile);  fclose(outfile);
    return 0;
}
