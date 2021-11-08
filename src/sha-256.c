/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   sha256.c                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/08 13:06:14 by yhetman                                  */
/*   Updated: 2021/11/08 15:21:32 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "sha256.h"


uint32_t        k[64] = {
        0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
        0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
        0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
        0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
        0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
        0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
        0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
        0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
        0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
        0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
        0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
        0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
        0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
        0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
        0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
        0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};


int
compare_sha256(t_bytes *content1, t_bytes *content2)
{
    size_t  i;

    for (i = 0; i < 32; i++)
    {
        if (content1->bytes[i] < content2->bytes[i])
            return -1;
        else if (content1->bytes[i] > content2->bytes[i])
            return 1;
    }
    return 0;
}


void
transform_sha256(t_sha256 *hash, uint8_t *buffer)
{
    uint32_t    S[8],
                w[64],
                t0,
                t1,
                t;
    int         i;

    for( i = 0; i < 8; i++)
        S[i] = hash->state[i];

    for( i = 0; i < 16; i++)
        w[i] = GET_UINT32(buffer + 4 * i);

    for( i = 16; i < 64; i++)
        w[i] = SHA256_F4(w[i - 2]) + w[i - 7] + SHA256_F3(w[i - 15]) + w[i - 16];

    for( i = 0; i < 64; i++ )
    {
        SHA256_ROUND(S[0], S[1], S[2], S[3], \
            S[4], S[5], S[6], S[7], i);
        SHA256_STEP(t, S[0], S[1], S[2], S[3], \
            S[4], S[5], S[6], S[7]);
    }

    for( i = 0; i < 8; i++ )
        hash->state[i] = hash->state[i] + S[i];
}


void
init_sha256(t_sha256 *hash)
{
    hash->curlen = 0;
    hash->length = 0;
    hash->state[0] = 0x6A09E667UL;
    hash->state[1] = 0xBB67AE85UL;
    hash->state[2] = 0x3C6EF372UL;
    hash->state[3] = 0xA54FF53AUL;
    hash->state[4] = 0x510E527FUL;
    hash->state[5] = 0x9B05688CUL;
    hash->state[6] = 0x1F83D9ABUL;
    hash->state[7] = 0x5BE0CD19UL;
}


void
update_sha256(t_sha256 *hash, void *buffer, uint32_t buffsize)
{
    uint32_t n;

    if( hash->curlen > sizeof(hash->buf))
        return;

    while( buffsize > 0 )
    {
        if( hash->curlen == 0 && buffsize >= BLOCK_SIZE )
        {
            transform_sha256( hash, (uint8_t*)buffer );
            hash->length += BLOCK_SIZE * 8;
            buffer = (uint8_t*)buffer + BLOCK_SIZE;
            buffsize -= BLOCK_SIZE;
        }
        else
        {
            n = MIN( buffsize, (BLOCK_SIZE - hash->curlen) );
            memcpy( hash->buf + hash->curlen, buffer, (size_t)n );
            hash->curlen += n;
            buffer = (uint8_t*)buffer + n;
            buffsize -= n;
            if( hash->curlen == BLOCK_SIZE )
            {
                transform_sha256( hash, hash->buf );
                hash->length += 8 * BLOCK_SIZE;
                hash->curlen = 0;
            }
        }
    }
}


void
final_sha256(t_sha256 *hash, t_bytes *content)
{
    int i;

    if( hash->curlen >= sizeof(hash->buf) )
        return;

    hash->length += hash->curlen * 8;
    hash->buf[hash->curlen++] = (uint8_t)0x80;

    if( hash->curlen > 56 )
    {
        while( hash->curlen < 64 )
            hash->buf[hash->curlen++] = (uint8_t)0;
        transform_sha256(hash, hash->buf);
        hash->curlen = 0;
    }

    while( hash->curlen < 56 )
        hash->buf[hash->curlen++] = (uint8_t)0;

    STORE64H(hash->length, hash->buf + 56 );
    transform_sha256(hash, hash->buf );

    for( i = 0; i < 8; i++ )
        PUT_UINT32(hash->state[i], content->bytes + (4 * i));
}

void
calculate_sha256(void *buffer, uint32_t buffsize, t_bytes *content)
{
    t_sha256 hash;

    init_sha256(&hash);
    update_sha256(&hash, buffer, buffsize);
    final_sha256(&hash, content);
}
