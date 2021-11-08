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
	t_bytes				hash;
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


t_hashes *
init_t_hash()
{	t_hashes 		*hash;

    hash = (t_hashes*)malloc(sizeof(t_hashes));
	hash->generated_key = (char *)malloc(sizeof(char) * 32);
	hash->generated_key = random_string(hash->generated_key, 32);
	calculate_sha256(hash->generated_key, sizeof(hash->generated_key), &hash->hash);
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
			return -1;htop
		tmp = tmp->next_hash;
	}
	return 1;
}

void
find_collision()
{
	t_hashes 	*hashed_keys;
	t_hashes    *new_hash;
	t_hashes	*tmp;
	int 		success_counter = 0;

	hashed_keys = init_t_hash();
	while (true)
	{
		new_hash = init_t_hash();
		if (check_hash_list(hashed_keys, new_hash) != -1)
		{
			tmp = new_hash;
			tmp->next_hash = new_hash->next_hash;
			hashed_keys->next_hash = tmp;
			success_counter++;
		}
		else
			break;

		if (success_counter % 100000 == 0)
			printf("Collision test in progree: current amount of succesfull tries : %i .\n", success_counter);

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
