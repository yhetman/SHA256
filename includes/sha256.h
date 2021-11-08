/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   sha256.h                                                                 */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/08 15:06:14 by yhetman                                  */
/*   Updated: 2021/11/08 15:06:15 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA256_H
# define SHA256_H


# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>

# define SHA256_HASH_SIZE	32
# define BLOCK_SIZE         64


typedef struct  s_sha256
{
    uint64_t    length;
    uint32_t    state[8];
    uint32_t    curlen;
    uint8_t     buf[64];
}               t_sha256;

typedef struct  s_bytes
{
    uint8_t     bytes[SHA256_HASH_SIZE];
}               t_bytes; 


extern uint32_t k[];


# define MIN(x, y) ((x < y) ? x : y)

# define ROTR(value, bits) ((value >> bits) | (value << (32 - bits)))
# define ROTL(value, bits) ((value & 0xFFFFFFFFUL) >> bits)

# define CH(x, y, z)     (z ^ (x & (y ^ z)))
# define MAJ(x, y, z)    (((x | y) & z) | (x & y))


# define SHA256_F1(x)       (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
# define SHA256_F2(x)       (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
# define SHA256_F3(x)       (ROTR(x, 7) ^ ROTR(x, 18) ^ ROTL(x, 3))
# define SHA256_F4(x)       (ROTR(x, 17) ^ ROTR(x, 19) ^ ROTL(x, 10))

# define SHA256_ROUND( a, b, c, d, e, f, g, h, i ) (    \
     t0 = h + SHA256_F2(e) + CH(e, f, g) + k[i] + w[i], \
     t1 = SHA256_F1(a) + MAJ(a, b, c),                  \
     d += t0,                                           \
     h  = t0 + t1)

# define SHA256_STEP(t, a, b, c, d, e, f, g, h) (	\
	t = h,											\
    h = g,											\
    g = f,											\
    f = e,											\
    e = d,											\
    d = c,											\
    c = b,											\
    b = a,											\
    a = t)

# define PUT_UINT32(x, y) { 	\
		(y)[0] =  (uint8_t)((x >> 24) & 255) ; 	\
		(y)[1] =  (uint8_t)((x >> 16) & 255); 	\
		(y)[2] =  (uint8_t)((x >>  8) & 255); 	\
		(y)[3] =  (uint8_t)(x & 255); }

# define GET_UINT32(y)(                      	\
   			((uint32_t)((y)[0] & 255) << 24) |  \
            ((uint32_t)((y)[1] & 255) << 16) |  \
            ((uint32_t)((y)[2] & 255) << 8 ) |  \
            ((uint32_t)((y)[3] & 255)))

# define STORE64H(x, y) {                   \
    (y)[0] = (uint8_t)((x >> 56) & 255),    \
    (y)[1] = (uint8_t)((x >> 48) & 255),    \
    (y)[2] = (uint8_t)((x >> 40) & 255),    \
    (y)[3] = (uint8_t)((x >> 32) & 255),    \
    (y)[4] = (uint8_t)((x >> 24) & 255),    \
    (y)[5] = (uint8_t)((x >> 16) & 255),    \
    (y)[6] = (uint8_t)((x >> 8) & 255),     \
    (y)[7] = (uint8_t)(x & 255); }

/***								***/
/***		SHA-256.c				***/
/***								***/

int 		compare_sha256(t_bytes *content1, t_bytes *content2);

void		init_sha256(t_sha256 *hash);
void		update_sha256(t_sha256 *hash, void *buffer, uint32_t buffsize);
void		transform_sha256(t_sha256 *hash, uint8_t *buffer);
void		final_sha256(t_sha256 *hash, t_bytes *content);

void		calculate_sha256(void *buffer, uint32_t buffsize, t_bytes *content);


void        count_execution_time();
void        find_collision();
#endif
