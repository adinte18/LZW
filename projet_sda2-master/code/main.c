#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "liste.h"
#include "tree.h"
#include "hashmap.h"

#define PATH_MAX 1024

int main()
{  
    char choice;
    printf("Compress or decompress? (c/d)");
    scanf("%c", &choice);

    char filename[PATH_MAX];
    printf("Give full filename : ");
    scanf("%s", filename);

    FILE *f = fopen(filename, "rb");
    if (errno != EBADF)
    {
        perror("fopen");
    }

    fseek(f, 0L, SEEK_END);
    int sz = ftell(f);
    fseek(f, 0L, SEEK_SET);

    time_t test1;
    srand((unsigned) time(&test1));

    switch (choice)
    {
        case 'c':
        {
            FILE *f2 = fopen(strcat(filename, ".lzw"), "w+b");
            FILE * fichierTime;

            char choice_structure;
            printf("Choose the structure you want (l/t/h)\n");
            scanf(" %c", &choice_structure);

            switch(choice_structure)
            {
                case 'l':
                    fichierTime = fopen("Liste.time", "a");
                    clock_t debut1 = clock();
                    compression_liste(f, f2);
                    clock_t fin1 = clock();

                    double time1 = (fin1 - debut1)  * 1000 / (double) CLOCKS_PER_SEC;

                    fprintf(fichierTime, "%d %f\n", sz, time1);

                    fclose(fichierTime);

                    break;

                case 't':
                    fichierTime = fopen("Trie.time", "a");
                    clock_t debut2 = clock();
                    compression_trie(f, f2);
                    clock_t fin2 = clock();

                    double time2 = (fin2 - debut2)  * 1000 / (double) CLOCKS_PER_SEC;

                    fprintf(fichierTime, "%d %f\n", sz, time2);

                    fclose(fichierTime);

                    break;

                case 'h':
                    fichierTime = fopen("Hashmap.time", "a");
                    clock_t debut3 = clock();
                    compression_hashmap(f, f2);
                    clock_t fin3 = clock();

                    double time3 = (fin3 - debut3)  * 1000 / (double) CLOCKS_PER_SEC;

                    fprintf(fichierTime, "%d %f\n", sz, time3);

                    fclose(fichierTime);

                    break;                   
            }
        }
        
        case 'd':
        {
            FILE * output = fopen("result.txt", "w");

            decompression_liste(f, output);

            break;
        }

        default:
            printf("Choose only 'c' or 'd'\n");
            exit(1);
    }


    return 1;
}