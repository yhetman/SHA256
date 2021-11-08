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
	unsigned char 		*generated_key;
	unsigned char 		*current_hash;
	struct s_hashes 	*next_hash;
}						t_hashes;

unsigned char
*random_string(unsigned char *str, size_t size)
{
	unsigned char 	charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	size_t 	n;

	for (n = 0; n < size; n++)
	{
		int key = rand() % (int) (sizeof charset - 1);
		str[n] = charset[key];
	}
	
	str[size] = '\0';
  return str;
}


t_hashes *
init_t_hash()
{	t_hashes 		*hash;
    t_bytes     	sha256hash;

    hash = (s_hashes*)malloc(sizeof(s_hashes));
	hash->generated_key = (char *)malloc(sizeof(char) * 33);
	hash->generated_key = random_string(hash->generated_key, 33);
	printf("%s/n", hash->generated_key);
    calculate_sha256(hash->generated_key, sizeof(33), &sha256hash);
    hash->current_hash = sha256hash.bytes;
    hash->new_hash = NULL;
    return hash;
}

int
check_hash_list(t_hashes *h, unsigned char *current_hash)
{
	t_hashes *tmp;

	if (h == NULL)
		return 1;

	tmp = h;
	while (tmp != NULL)
	{
		if (strcmp(tmp->current_hash, current_hash) == 0)
			return -1;
		tmp = tmp->new_hash;
	}
	return 1;
}

void
find_collision()
{
	t_hashes 	*hashed_keys;
	t_hashes    *new_hash;
	int 		success_counter = 0;

	hashed_keys = NULL;
	new_hash = init_t_hashes();
	hashed_keys = new_hash;
	while(1)
	{
		new_hash = init_t_hashes();
		if ((check_hash_list(hashed_keys, new_hash->current_hash) < 0))
			break ;
		else
		{
			hashed_keys->next_hash = new_hash;
			success_counter++;
		}
	}
	printf("The Algorithm made %i\n succesfull hashings before collision.", success_counter);
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
