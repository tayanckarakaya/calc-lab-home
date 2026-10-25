// Tayanc Karakaya 241ADB158
// https://github.com/tayanckarakaya/calc-lab-home
// Compile with: gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c -lm
// Resources used: https://share.gemini.google/5Jk0WP6yRlJr
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
void readNext(FILE *input, int *character, long long *position){
    *character = fgetc(input);
    (*position)++;
}
    long long readNumber(FILE *input, int *character, long long *position){
    long long number = 0;

    while (*character >= '0' && *character <= '9'){
        number = number * 10 + (*character - '0');
        readNext(input, character, position);
    }

    return number;
}

long long readExpression(FILE *input, int *character,//  I got help from ai
                         long long *position, long long *errorPosition);

long long readPower(FILE *input, int *character,//// Read powers before multiplication and division to give ** higher precedence.I got help from ai
                    long long *position, long long *errorPosition){
    long long number;

    while (*character == ' ' || *character == '\t'){
        readNext(input, character, position);
    }

    if (*character == '('){
        readNext(input, character, position);
        number = readExpression(input, character, position, errorPosition);

        if (*errorPosition != 0){
            return 0;
        }
        if (*character != ')'){
            *errorPosition = *position;
            return 0;
        }
        readNext(input, character, position);
    }
    else if (*character >= '0' && *character <= '9'){
        number = readNumber(input, character, position);
    }
    else{
        *errorPosition = *position;
        return 0;
    }

    while (*character == ' ' || *character == '\t'){
        readNext(input, character, position);
    }

    if (*character == '*'){
        int next = fgetc(input);
        if (next != EOF){
            ungetc(next, input);
        }

        if (next == '*'){
            readNext(input, character, position);
            readNext(input, character, position);

            while (*character == ' ' || *character == '\t'){
                readNext(input, character, position);
            }

            long long exponentStart = *position;
            long long exponent = readPower(input, character, position, errorPosition);//// Read the exponent recursively so 2**3**2 is calculated as 2**(3**2).

            if (*errorPosition != 0){
                return 0;
            }
            if (exponent < 0){
                *errorPosition = exponentStart;
                return 0;
            }

            long long answer = 1;
            while (exponent > 0){
                if (exponent % 2 == 1){
                    answer *= number;
                }
                exponent /= 2;
                if (exponent > 0){
                    number *= number;
                }
            }
            number = answer;
        }
    }

    return number;
}

long long readExpression(FILE *input, int *character,
                         long long *position, long long *errorPosition){
    long long result = 0;
    long long term = 0;
    char operation = '+';
    int expectNumber = 1;

    while (*character != EOF &&
           *character != '\n' &&
           *character != ')')
    {
        if (*character == ' ' || *character == '\t'){
            readNext(input, character, position);
            continue;
        }

        if ((*character >= '0' && *character <= '9') ||
            *character == '('){
            if (expectNumber == 0){
                *errorPosition = *position;
                return 0;
            }

            long long numberStart = *position;
            long long number;

            number = readPower(input, character, position, errorPosition);

            if (*errorPosition != 0){
         return 0;
            }

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
                if (number == 0)//// Division by zero is reported at the start of the right operand.
                {
                    *errorPosition = numberStart;
                    return 0;
                }

                term /= number;
            }

            expectNumber = 0;
            continue;
        }

        if (*character == '+' || *character == '-' ||
            *character == '*' || *character == '/'){
            if (expectNumber == 1){
                *errorPosition = *position;
                return 0;
            }

            operation = *character;
            expectNumber = 1;
            readNext(input, character, position);
        
        }else{
            *errorPosition = *position;
            return 0;
        }
    }

    if (expectNumber == 1){
        *errorPosition = *position;
        return 0;
    }

    return result + term;
}

int main(int argc, char *argv[]){
    if (argc != 4){
        fprintf(stderr, "Usage: ./calc -o OUTPUT_DIR INPUT_FILE\n");
        return 1;
    }
    if (strcmp(argv[1], "-o") != 0){
        fprintf(stderr, "Expected -o option.\n");
        return 1;
    }
    
    if (mkdir(argv[2], 0777) != 0 && errno != EEXIST){// Reuse an existing output directory; stop on other creation errors.

     fprintf(stderr, "Could not create output directory.\n");
     return 1;
}

    FILE *input = fopen(argv[3], "r");
 
    if (input == NULL){
        fprintf(stderr, "Couldnt open input file.\n");
        return 1;
    }
    char *fileName = strrchr(argv[3], '/');// Exclude the original directory when constructing the output filename.

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
// Check its return value to avoid opening a truncated path.
int pathLength = snprintf(
    outputPath,
    sizeof(outputPath),
    "%s/%s_tayanc_karakaya_241ADB158.txt",
    argv[2],
    baseName);

if (pathLength < 0 || pathLength >= (int)sizeof(outputPath)){
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

        int character = fgetc(input);
    long long position = 1;//Positions start at 1. readNext will count all characters, including spaces and newlines.
    long long errorPosition = 0;

    while (character != EOF)
    {
        // Skip blank lines and whitespace between expressions.
        if (character == ' ' || character == '\t' || character == '\n')
        {
            readNext(input, &character, &position);
            continue;
        }

        if (character == '#'){
    while (character != '\n' && character != EOF){
        readNext(input, &character, &position);
    }
    continue;
    }

        long long result = readExpression(input, &character, &position, &errorPosition);

        if (errorPosition != 0){
            break;
        }

        // An unmatched closing parenthesis remains after the expression.
        if (character == ')'){
            errorPosition = position;
            break;
        }

        fprintf(output, "%lld\n", result);

        if (character == '\n'){
            readNext(input, &character, &position);
        }
    }

    if (errorPosition != 0){
        // Remove earlier results so only the first error remains.
        output = freopen(outputPath, "w", output);

        if (output == NULL){
            fprintf(stderr, "Could not reopen output file.\n");
            fclose(input);
            return 1;
        }

        fprintf(output, "ERROR:%lld\n", errorPosition);
    }
    fclose(input);

    fclose(output);

    return 0;
}
    

