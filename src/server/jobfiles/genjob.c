#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned int write_job(char* buf, unsigned int chars_to_write, FILE* out_file)
{
    // Write job type
    if (rand() % 2)
    {
        fwrite("O", 1, 1, out_file);
    }
    else
    {
        fwrite("E", 1, 1, out_file);
    }
    fwrite(&chars_to_write, sizeof(unsigned int), 1,
           out_file);  // Write job length
    unsigned int written =
        fwrite(buf, 1, chars_to_write, out_file);  // Write job text
    if (written != chars_to_write)
    {
        printf("%d %d\n", chars_to_write, written);
        perror("fwrite()");
        exit(1);
    }

    return written;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        printf("USAGE: %s <infile> <outfile> <short/long>\n", argv[0]);
        return 0;
    }

    FILE* in_file = fopen(argv[1], "r");
    FILE* out_file = fopen(argv[2], "w+");

    srand(time(NULL));

    char         buf[64000] = {0};
    unsigned int written_total = 0, loops = 0;
    for (;;)
    {
        unsigned int chars_to_read;
        if (argv[3][0] == 'l')
        {
            chars_to_read = (rand() % 54378) + 1;
        }
        else
        {
            chars_to_read = (rand() % 255) + 1;
        }

        // read chars_to_read lengths of characters from input file
        // and advance the input file pointer using the fread function
        unsigned int written;
        size_t       ret = fread(buf, 1, chars_to_read, in_file);
        if (ret != chars_to_read)
        {
            if (!feof(in_file))
            {
                printf("Not EOF! Error occured\n");
                perror("fread()");
                exit(1);
            }
            written = write_job(buf, ret, out_file);
            (void)written;
            break;
        }

        written = write_job(buf, chars_to_read, out_file);
        written_total += written;
        loops++;
        // printf("Status: %5d >> %7d (%4d)\n", written, written_total, loops);
    }

    fclose(out_file);
    fclose(in_file);
    return 0;
}
