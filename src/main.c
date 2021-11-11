/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                                                   */
/*                                                                            */
/*   By: yhetman <yhetman@student.unit.ua>                                    */
/*                                                                            */
/*   Created: 2021/11/08 15:19:53 by yhetman                                  */
/*   Updated: 2021/11/08 15:19:55 by yhetman                                  */
/*                                                                            */
/* ************************************************************************** */

#include "sha256.h"


static int
get_flags(int argc, char ** argv, FILE **input, FILE **output, bool *collision)
{
    int 	flag,
    		check_flags;

    check_flags = 0;
    
    while ((flag = getopt(argc, argv, "i:o:ch")) != -1)
    {
        switch (flag)
        {
            case 'i':
                *input = fopen(optarg, "r+");
                check_flags++;
                break;
            case 'o':
                *output = fopen(optarg, "w+");
                check_flags++;
                break;
            case 'c':
                *collision = true;
                check_flags = 2;
                break;
            case 'h':
                printf("Usage:  ./SHA256 -i ./input_file -o ./output_file\n");
                printf("Usage to test for collision:  ./SHA256 -c\n");
                
                return 1;
            case '?':
                fprintf(stderr, "%s unsuported flag.", optarg);
                return 1;
        }
    }
    if (check_flags < 2)
    {
        fprintf(stderr, "Needs to satisfy all parameters, use -h for help.\n");
        return 1;
    }
    return 0;
}


int main(int argc, char **argv)
{
	FILE    		*inputfile,
            		*outputfile;
    long int 		size,
    				bytes_read,
    				bytes_written;
	unsigned char 	*input;
    t_bytes     	sha256hash;
    bool            collision = false;


    if (get_flags(argc, argv, &inputfile, &outputfile, &collision) != 0)
       return 1;
    if (collision)
    {
        find_collision();
        return (0);
    }
	fseek(inputfile, 0, SEEK_END);
	size = ftell(inputfile);
	fseek(inputfile, 0, SEEK_SET);

	input = (unsigned char *) malloc(size);
	if ((bytes_read = fread(input, sizeof(unsigned char), size, inputfile)) < 0)
	{
		printf("Error while reading input file.\n");
		exit(1);
    }
   
    calculate_sha256(input, sizeof(input), &sha256hash);
	if ((bytes_written = fwrite(sha256hash.bytes, sizeof(unsigned char),\
		SHA256_HASH_SIZE, outputfile)) < 0)
	{
		printf("Error while writting output to file.\n");
		exit(1);
	}

	free(input);
	fclose(inputfile);
	fclose(outputfile);
    return 0;
}