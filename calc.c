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


    char baseName[256];
    int nameLength = strlen(fileName);
    if (nameLength <= 4 || nameLength >= 256){
    fprintf(stderr, "Invalid input file name.\n");
    fclose(input);
    return 1;
}
    for (int i = 0; i < nameLength - 4; i++){
    baseName[i] = fileName[i];
    }
    baseName[nameLength - 4] = '\0';
    
   
    char outputPath[4096];

int pathLength = snprintf(
    outputPath,
    sizeof(outputPath),
    "%s/%s_tayanc_karakaya_241ADB158.txt",
    argv[2],
    baseName);

if (pathLength < 0 || pathLength >= (int)sizeof(outputPath))
{
    fprintf(stderr, "Output path is too long or could not be created.\n");
    fclose(input);
    return 1;
}

    FILE *output = fopen(outputPath, "w");

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
    long long position = 1;
    long long errorPosition = 0;
    int expectNumber = 1;
    long long term = 0;

    while (character != EOF){
    if (character >= '0' && character <= '9'){
        if (expectNumber == 0)
        {
            errorPosition = position;
            break;
        }

        long long number = 0;
        long long numberStart = position;
             
             while (character >= '0' && character <= '9'){
                number = number * 10 + (character - '0');
                character = fgetc(input);
                position++;}
            expectNumber = 0;

            if (operation == '+'){
    result += term;
    term = number;
        }
    else if (operation == '-'){
    result += term;
    term = -number;
        }
    else if (operation == '*'){
    term *= number;
        }
    else if (operation == '/'){
    if (number == 0) {
        errorPosition = numberStart;
        break;
            }

    term /= number;
}

hasNumber = 1;
        
          }else{
         if (character == '+' || character == '-'||
        character == '*' || character == '/'){
        if (expectNumber == 1)
        {
            errorPosition = position;
            break;
        }

        operation = character;
        expectNumber = 1;
    }
            
                else if (character == '\n')
{
    if (hasNumber == 1 && expectNumber == 1)
    {
        errorPosition = position;
        break;
    }

    if (hasNumber == 1)
                    {
                        fprintf(output, "%lld\n", result + term);
                        result =0;
                        operation ='+';
                        hasNumber =0;
                        expectNumber = 1;
                        term = 0;
                    }
                    
                }
                
                else if (character != ' ' && character != '\t'){
                     errorPosition = position;
                     break;}
            character = fgetc(input);
            position++;
        }
        
        
        }
        if (errorPosition == 0 && hasNumber == 1 && expectNumber == 1){
    errorPosition = position;
        }

        if (errorPosition != 0){
        output = freopen(outputPath, "w", output);

        if (output == NULL)
        {
            fprintf(stderr, "Could not reopen output file.\n");
            fclose(input);
            return 1;
        }

        fprintf(output, "ERROR:%lld\n", errorPosition);}
        else if (hasNumber == 1) {
        fprintf(output, "%lld\n", result + term);
        }
    fclose(input);

    fclose(output);

    return 0;
}
    

