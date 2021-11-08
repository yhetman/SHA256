/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   test_collision.c                                                         */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/08 20:05:03 by yhetman                                  */
/*   Updated: 2021/11/08 20:05:05 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "time.h"
#include "sha256.h"

typedef struct  		s_hashes
{
	char 				*generated_key;
	t_bytes				hash; //[SHA256_HASH_SIZE];
	// uint8_t				*current_hash;
	struct s_hashes 	*next_hash;
}						t_hashes;

char*
random_string(char *str, size_t size)
{
	char 	charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	size_t 	n;

	for (n = 0; n < size; n++)
	{
		int key = rand() % (int) (sizeof charset - 1);
		str[n] = charset[key];
	}
	
	str[size] = '\0';
  return str;
}

char
*convert(uint8_t *bytes)
{
    int 	buffer1[9],
    		buffer2[9];
    int 	i;
    char 	*buffer_pointer;

    buffer1[8]='\0';

    for(i = 0; i <= 7; i++)
        buffer1[7 - i] = (((*bytes) >> i) & (0x01));

    for(i = 0; i <= 7; i++)
        buffer2[i] = buffer1[i] + '0';

    buffer2[8] = '\0';

    // puts((char*)buffer2);

    buffer_pointer = (char*)buffer2;

    return buffer_pointer;
}


void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char*)s;
	i = -1;
	while (++i < n)
		str[i] = '\0';
}


void	ft_memdel(void **ap)
{
	if (ap == NULL)
		return ;
	free(*ap);
	*ap = NULL;
}

t_hashes *
init_t_hash()
{	t_hashes 		*hash;
    // t_bytes     	sha256hash;

    hash = (t_hashes*)malloc(sizeof(t_hashes));
	hash->generated_key = (char *)malloc(sizeof(char) * 32);
	hash->generated_key = random_string(hash->generated_key, 32);
	// hash->hash
	// printf("%s\n", hash->generated_key);
    // ft_bzero((void*)sha256hash.bytes, SHA256_HASH_SIZE);
    
    calculate_sha256(hash->generated_key, sizeof(hash->generated_key), &hash->hash);

    // hash->current_hash = hash->hash.bytes;

    for (size_t i = 0; i < 32; i++)
    	printf("%s ", convert(&(hash->hash.bytes[i])));
    printf("\n");
    // hash->next_hash = (t_hashes*)malloc(sizeof(t_hashes));
    return hash;
}



int
check_hash_list(t_hashes *h, t_hashes *new)
{
	t_hashes	*tmp;

	if (h == NULL)
		return 1;
	tmp = h;
	tmp->next_hash = h->next_hash;
	while (tmp->next_hash != NULL)
	{
		if (compare_sha256(&tmp->hash, &new->hash) == 0)
			return -1;
		tmp = tmp->next_hash;
		printf("%s\n", tmp->generated_key);
	}
	return 1;
}

void
find_collision()
{
	t_hashes 	*hashed_keys;
	t_hashes    *new_hash;
	// t_hashes	*tmp;
	int 		success_counter = 0;

	hashed_keys = init_t_hash();
	// hashed_keys = new_hash;
	while (true)
	{
		new_hash = init_t_hash();
		// printf("%s\t%s\n", hashed_keys->current_hash, (char*)new_hash->current_hash);

		if (check_hash_list(hashed_keys, new_hash) != -1)
		{
			// tmp = new_hash;
			hashed_keys->next_hash = new_hash;
			hashed_keys->next_hash->next_hash = new_hash->next_hash;

			// hashed_keys->next_hash = tmp->next_hash;
			success_counter++;
		}
		else
			break;
	}
	printf("The Algorithm made %i succesfull hashings before collision.\n", success_counter);
}


void
count_execution_time()
{
	clock_t t;
	t = clock();
	find_collision();
	t = clock() - t;

	double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Testing for collision took %f seconds of execution \n", time_taken);
}
