// Tayanc Karakaya 241ADB158
// https://github.com/tayanckarakaya/calc-lab-home
// Compile with: gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c -lm
// Resources used: <every AI tool, website and book you used; details in comments where you used them>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[]){
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./calc -o OUTPUT_DIR INPUT_FILE\n");
        return 1;
    }
    if (strcmp(argv[1], "-o") != 0)
    {
        fprintf(stderr, "Expected -o option.\n");
        return 1;
    }
    
    if (mkdir(argv[2], 0777) != 0 && errno != EEXIST)
{
     fprintf(stderr, "Could not create output directory.\n");
     return 1;
}

    FILE *input = fopen(argv[3], "r");
 
    if (input == NULL){
        fprintf(stderr, "Couldnt open input file.\n");
        return 1;
    }
    char *fileName = strrchr(argv[3], '/');

    if (fileName == NULL){
    fileName = argv[3];
    }
    else{
    fileName++;
    }

printf("Input file name: %s\n", fileName);
    FILE *output = fopen("output.txt", "w");

        if (output == NULL){
            fprintf(stderr, "Couldnt open output file.\n");
            fclose(input);
                return 1;
}

    int character;
    character = fgetc(input);
    long long result = 0;
    char operation = '+';
    int hasNumber = 0;
    
    while (character != EOF){
        if (character >= '0' && character <= '9'){
             long long number =0;
             
             while (character >= '0' && character <= '9'){
                number = number * 10 + (character - '0');
                character = fgetc(input);
             }
            
             if (operation == '+'){
                 result += number;
                 hasNumber = 1;
                }
            
                else if (operation == '-'){
                result -= number;
                hasNumber = 1;
                }
        
            }else{
            if (character== '+'){
                operation= '+';
                
            }
            
            else if (character== '-'){
                operation = '-';
            }
                else if (character== '\n') 
                {
                    if (hasNumber== 1)
                    {
                        fprintf(output, "%lld\n", result);
                        result =0;
                        operation ='+';
                        hasNumber =0;
                    }
                    
                }
                
            character = fgetc(input);
        }
        
        
    }
    if (hasNumber == 1)
    {
        fprintf(output, "%lld\n", result);
    }

    fclose(input);

    fclose(output);

    return 0;
}
    

