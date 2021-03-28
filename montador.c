//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Autor: Gabriell Barbedo Gomes Cordeiro -------------------------------------------------------------------------------------------------------------------------------------------------------
// Matricula: 14/0141251 --------------------------------------------------------------------------------------------------------------------------------
// Disciplina: Software Basico - CIC - UnB --------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The following lines are reserved for the declaration of functions that will be used throughout the program.

void zeroStep();
void firstPassage();
void reverseAtoi();
void secondPassage();
int findInFile();
void dataAfterCode();
int duplicateLabelCheck();













// Here comes the main function.

int main(void){

// First we have to open the file we wish to process. We'll assume it is on the same directory as this program.

    FILE *input, *auxiliary;
    input = fopen("bin.asm", "r");

// Let us check if it was opened correctly. If not, we'll display an error message and close the program.

if (input == NULL){
    printf("I could not open your file.\nCheck the spelling and if it is in the same directory as this program.\n");
    system("pause");
    return 0;
}

// Now, let us call the zeroStep function.
// It will format our input file, removing any unnecessary spaces, lines and tabulations.
// It will also remove comments and put all characters in lower case.
// The output will be a file called "tolower.txt". We'll work with this new file on the following steps.

zeroStep(input);
fclose(input);
printf("\nI've finished formatting your input file.\n");

// Now we'll call the dataAfterCode function.
// It will swap the order of the segments of the input, if necessary.
// We always want the data segment to come after the text segment.
// The output will be a file called "correctOrder.txt". We'll work with this new file on the following steps.

input = fopen("tolower.txt", "r");
dataAfterCode(input);
fclose(input);
printf("\nI've finished correcting the order of your input file.\n");

// Now, we'll call the firstPassage function.
// It will look for labels on the program and register their names and positions in a separate file.

input = fopen("correctOrder.txt", "r");
firstPassage(input);
fclose(input);
printf("\nI've finished collecting the labels of your input file.\n");

// Now we'll call the duplicateLabelCheck function.
// It will just check to see if our input file has duplicate labels.

input = fopen("labelsTable.txt", "r");
int ret;
ret = duplicateLabelCheck(input);
if (ret == 1){
    printf("There are duplicate labels in your input file. I'll continue the operation, but DO NOT consider the output files. They are NOT valid.\n");
}
fclose(input);

// Now, we'll call for the secondPassage function.
// As the name implies, it will actually build our object file.

input = fopen("correctOrder.txt", "r");
auxiliary = fopen("labelsTable.txt", "r");
secondPassage(input, auxiliary);
fclose(input);
fclose(auxiliary);






// Now, we'll close the program.
printf("\nI've finished everything I had to do. Cheers.\n");
system("pause");
return 0;
}




// This is the dataAfterCode function. This function will only switch the order of the sections of the input code.
// Data section must come after the text section.

void dataAfterCode(FILE *input){

// First, let us declare the variables we'll be using.

    char leitor[100];
    int exit = 0, ret, getOut = 0, exitCicle = 0;

    FILE *output;
    output = fopen("correctOrder.txt", "w");

// First, let us verify the order. After all, we might not even need to do anything.

    while (exit == 0){

        fgets(leitor, 100, input);

        ret = strncmp(leitor, "section text", 12);
        if (ret == 0){
            exit = 1;
        }

        ret = strncmp(leitor, "section data", 12);
        if (ret == 0){
            exit = 2;
        }

        if (leitor == NULL){
            exit = 3;
        }

    }

    rewind(input);

// exit = 1 means the section text comes first. We don't have to do anything.
// exit = 2 means the section data comes first and we'll need to change the order.
// exit = 3 means the file does not contain any section statement. In this case we need to abort the operation.

    if (exit == 3){
        printf("It seems your file does not contain any section statement. I'll abort the operation now.\n");
        system("pause");
    }

    if (exit == 2){

        char leitor[100];

        while (getOut != 1){

            fgets(leitor, 100, input);
            ret = strncmp("section text", leitor, 12);

            if (ret == 0){ // Here we found the section text

                fputs(leitor, output); // We print the 'section text' line on out output file.

                while ((fgets(leitor, 100, input)) != NULL){    // Now we copy the rest of the code from the input file to our output.

                    fputs(leitor, output);

                }

                fputc('\n', output);

                rewind(input);      // Now we go back to the beginning of the input file.

                while (exitCicle != 1){

                    fgets(leitor, 100, input);
                    ret = strncmp("section text", leitor, 12);

                    if (ret == 0){          // We search for the 'section text' line. If we find it, we're finished.

                        exitCicle = 1;      // This is out flag to get out of this cycle.

                    }else{                  // If it's not, then we're still in the section data segment.

                        fputs(leitor, output);

                    }

                }

                getOut = 1;                 // This is our flag to get out of this cycle.

            }


        }


    }


    if (exit == 1){

        while ((fgets(leitor, 100, input)) != NULL){
            fputs(leitor, output);
        }

    }

    fclose(output);
}



// Here ends the dataAfterCode function.



// This is the zeroStep function.

void zeroStep(FILE *file){

// First, let us declare the variables we'll use in this function.

    char leitor, leitoranterior;
    FILE *output;
    output = fopen("tolower.txt", "w");

// Now, we'll read the input file character by character.

    while ((leitor = fgetc(file)) != EOF){

// If any are on upper case, we'll lower them.

            if (leitor >= 65 && leitor <= 90){
                leitoranterior = leitor;
                fputc(leitor+32, output);
            }
            else{

// If there are two or more " " or "\n" in sequence, we'll remove the extras.
// We'll also remove " " and "\n" before and after ":".

                if ((leitor == ' ' && ((leitoranterior == ' ') || (leitoranterior == '\n'))) || (leitor == '\n' && leitoranterior == '\n') || ((leitor == '\n' || leitor == ' ') && leitoranterior == ':')){
                    leitoranterior = leitor;
                }else{

// We'll ignore all characters after ";" until we find a "\n".

                    if (leitor == ';'){
                        while ((leitor = fgetc(file)) != '\n'){
                            }
                        if (leitor == '\n'){
                                leitoranterior = leitor;
                                fputc(leitor, output);
                        }
                    }
                    else{
                    leitoranterior = leitor;
                    fputc(leitor, output);
                    }
                }
            }
        }
    fclose(output);
}

// Here ends the zeroStep function.

// This is the reverseAtoi function.

void reverseAtoi(int input, char* string){

int i, small, big;

char output[10], aux;
int exit = 0;

while(exit != 1){

    i = input % 10;

    if (i == 0){strncat(output, "0 ", 1);}
    if (i == 1){strncat(output, "1 ", 1);}
    if (i == 2){strncat(output, "2 ", 1);}
    if (i == 3){strncat(output, "3 ", 1);}
    if (i == 4){strncat(output, "4 ", 1);}
    if (i == 5){strncat(output, "5 ", 1);}
    if (i == 6){strncat(output, "6 ", 1);}
    if (i == 7){strncat(output, "7 ", 1);}
    if (i == 8){strncat(output, "8 ", 1);}
    if (i == 9){strncat(output, "9 ", 1);}

    input = input / 10;
    if (input == 0){
        exit = 1;
    }
}

exit = 0;
big = strlen(output) - 1;

while (exit != 1){

    aux = output[small];
    output[small] = output[big];
    output[big] = aux;

    small++;
    big--;

    if ((small > big) || (small == big)){
        exit = 1;
    }
}

strcpy(string, output);
}

// Here ends the reverseAtoi function.



















// This is the firstPassage function.

void firstPassage(FILE *input){

// First, let us declare the variables to be used in this function.

    char leitor[100], outputString[50], copyOfLeitor[100];
    FILE *output;
    output = fopen("labelsTable.txt", "w");

    const char colon[2] = ":";
    const char ch = ':';

    char *label, *ret;
    int counter = 0, increment;

// Now, we'll go over every line of the program, looking for labels.

    while ((fgets(leitor, 100, input)) != NULL){

        strcpy(copyOfLeitor, leitor);

// We want to register the position of the labels we find.
// Therefore, we need to count them as the program advances through the input file.

        if ((ret = strstr(leitor, "copy") != NULL)){
            increment = 3;
            }else{

                if ( (ret = strstr(leitor, "const")) || (ret = strstr(leitor, "space")) || (ret = strstr(leitor, "stop")) != NULL){
                        increment = 1;
                    }else{

                        if (((ret = strstr(leitor, "section text")) || (ret = strstr(leitor, "section data")) != NULL)){
                                increment = 0; // counter will not be updated in this case!

                        }else{
                            increment = 2;
                        }

                    }
            }

// When we find one, we'll copy it to our auxiliary file, "labelsTable.txt".
// We'll also copy the position of the label, as this will be important in the future.

        ret = strchr(leitor, ch);

        if(ret != NULL){

// On the following lines we'll build the string we want to print on our file.
// Like this: |label| + |;| + |position| + |;| + |type| + |\n|.

            label = strtok(leitor, colon);
            strcpy(outputString, label);
            strcat(outputString, ";");


// The next lines are basically a reverse Atoi function, also known as itoa.
// int NUMBER is the input. String AUXSTRING is the output.


        int i, small, big;
        int number = counter;

        char auxString[10], aux;
        int exit = 0;

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            strcat(outputString, auxString);

            for(i=0; auxString[i] != NULL; i++){
                auxString[i] = NULL;
            }

// Here is the pseudo itoa function
//--------------------------------------------------------------------------------------------------------------------

            strcat(outputString, ";");

            if (increment == 1){

                ret = strstr(copyOfLeitor, "const");

                if (ret != NULL){
                       strcat(outputString, "const");
                }

                ret = strstr(copyOfLeitor, "space");

                if (ret != NULL){
                       strcat(outputString, "space");
                }

                ret = strstr(copyOfLeitor, "stop");

                if (ret != NULL){
                       strcat(outputString, "label");
                }

            }


            else{
                strcat(outputString, "label");
            }




            strcat(outputString, "\n");
            fputs(outputString, output);

        }//close If

        counter = counter + increment;





    }//close while



    fclose(output);

    }//close mapLabels function

// Here ends the mapLabels function.
















// This is the secondPassage function.

void secondPassage(FILE *input){

// First, let us declare the variables that will be used.

    FILE *output;
    output = fopen("outputObjectFile.obj", "w");

    char leitor[50], *label, *line, *instruction, *operand1, *operand2, *extraOperand;
    char *ret;
    int i = 0, returnFromFile;

    char reverseAtoiStringSpace[10];

// These will be the flags used.

    int wrongInstruction = 1; // This will be used to identify unknown/miswritten instructions.

// Second, let us analyze our input file, line by line.

    while ((fgets(leitor, 50, input)) != NULL){

// Now, we're inside the main loop. Let us break every sentence into the regular expected structure:
// |label| + |:| + |mnemonic| + |space| + |operand| + |\n|

        ret = strchr(leitor, ':');

        if (ret != NULL){

            label = strtok(leitor, ":");
            line = strtok(NULL, ":");

        }else{
            line = strtok(leitor, ":");
        }

// LABEL has the label of the line, if it exists, and LINE has the rest of the line.

    instruction = strtok(line, " "); // INSTRUCTION will have the add, sub, copy, space or whatever mnemonic.

    if ((i = strcmp(instruction, "add")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(1, operand1);

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("01", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }




    }//close ADD if.

    if ((i = strcmp(instruction, "sub")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(1, operand1);

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("02", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }



    }// close SUB if.

    if ((i = strcmp(instruction, "mult")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(1, operand1);

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("03", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }




    }// close MULT if.

    if ((i = strcmp(instruction, "div")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(1, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("04", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close DIV if.

    if ((i = strcmp(instruction, "jmp")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(3, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("05", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }



    }// close JMP if.

    if ((i = strcmp(instruction, "jmpn")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(3, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("06", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close JMPN if.

    if ((i = strcmp(instruction, "jmpp")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(3, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("07", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close JMPP if.

    if ((i = strcmp(instruction, "jmpz")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(3, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("08", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close JMPZ if.

    if ((i = strcmp(instruction, "copy")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, ",");
        operand2 = strtok(NULL, ",");

        if (operand2 == NULL){
            wrongInstruction = 2;   // too few operands. Ex: zero operands for an ADD.
            printf("Too few operands!\n");

        }

        extraOperand = strtok(NULL, ",");

        if (extraOperand != NULL){
            wrongInstruction = 2;   // too many operands. Ex: zero operands for an ADD.
            printf("Too many operands!\n");

        }


        returnFromFile = variableTypeCheck(1, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand.
            printf("Wrong type of operand.\n");

        }

        returnFromFile = variableTypeCheck(2, operand2);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand.
            printf("Wrong type of operand.\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("09", output);
            fputc(' ', output);
            fputs(auxString, output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


        returnFromFile = findInFile(operand2);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }




    }// close COPY if.

    if ((i = strcmp(instruction, "load")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(1, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("10", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close LOAD if.

    if ((i = strcmp(instruction, "store")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(2, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("11", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close STORE if.

    if ((i = strcmp(instruction, "input")) == 0){

         wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(2, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("12", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close INPUT if.

    if ((i = strcmp(instruction, "output")) == 0){

         wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        returnFromFile = variableTypeCheck(2, operand1);
        // 1 is for ADD-like instructions. 2 is for STORE-like instructions. 3 is for JMP-like instructions.

        if (returnFromFile == 0){
            wrongInstruction = 3;   // wrong type of operand. Ex:
            printf("wrong type of operand\n");

        }

        returnFromFile = findInFile(operand1);

        if (returnFromFile == -1){
            wrongInstruction = 3;   // Could not find the operand in the labels table.
            printf("Undeclared operand\n");


        }else{

// Once again with the pseudo itoa function

        int i, small, big;
        int number = returnFromFile;

        char auxString[10], aux;
        int exit = 0;

        for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        while(exit != 1){

            i = number % 10;

            if (i == 0){strncat(auxString, "0 ", 1);}
            if (i == 1){strncat(auxString, "1 ", 1);}
            if (i == 2){strncat(auxString, "2 ", 1);}
            if (i == 3){strncat(auxString, "3 ", 1);}
            if (i == 4){strncat(auxString, "4 ", 1);}
            if (i == 5){strncat(auxString, "5 ", 1);}
            if (i == 6){strncat(auxString, "6 ", 1);}
            if (i == 7){strncat(auxString, "7 ", 1);}
            if (i == 8){strncat(auxString, "8 ", 1);}
            if (i == 9){strncat(auxString, "9 ", 1);}

            number = number / 10;
            if (number == 0){
                exit = 1;
            }
        }

        exit = 0;
        big = strlen(auxString) - 1;
        small = 0;

        while(exit != 1){

            aux = auxString[big];
            auxString[big] = auxString[small];
            auxString[small] = aux;

            aux++;
            big--;

            if (small >= big){
                exit = 1;
            }

        }

            fputs("13", output);
            fputc(' ', output);
            fputs(auxString, output);
            fputc(' ', output);

            for(i=0; auxString[i] != NULL; i++){auxString[i] = NULL;}

        }


    }// close OUTPUT if.

    if ((i = strncmp(instruction, "stop", 4)) == 0){

        wrongInstruction = 0;
        fputs("14", output);
        fputc(' ', output);


    }// close STOP if.

    if ((i = strcmp(instruction, "section")) == 0){

        wrongInstruction = 0;
        // In this case we don't want to print anything.

    }// close SECTION if.

    if ((i = strcmp(instruction, "space\n")) == 0){

        wrongInstruction = 0;
        fputs("00", output);
        fputc(' ', output);

    }// close SPACE if.

    if ((i = strcmp(instruction, "const")) == 0){

        wrongInstruction = 0;       // no lexical mistake in this line.

        operand1 = strtok(NULL, " ");

        if (operand1 == NULL){
            wrongInstruction = 2;   // too little operands. Ex: zero operands for an ADD.
            printf("too little operands\n");
        }

        operand2 = strtok(NULL, " ");

        if (operand2 != NULL){
            wrongInstruction = 2;   // too many operands. Ex: two operands for an ADD.
            printf("too many operands\n");

        }

        char auxiliaryString[10];

        strcpy(auxiliaryString, operand1);

        for(i=0; auxiliaryString[i] != '\n'; i++){}
        auxiliaryString[i] = '\0';

        fputs(auxiliaryString, output);
        fputc(' ', output);


    }// close CONST if.



// wrongInstruction = 1 means a lexical mistake, such as "copi" instead of "copy".
// wrongInstruction = 2 means a syntactic mistake. Ex: wrong number of arguments.
// wrongInstruction = 3 means a semantic mistake.

    switch (wrongInstruction){

        case 0:
            // there is no mistake!
        break;

        case 1:
            printf("There is a lexical mistake in this line:\n%s\n", line);
        break;

        case 2:
            printf("There is a syntactic mistake in this line:\n%s\n", line);
        break;

        case 3:
            printf("There is a semantic mistake in this line:\n%s\n", line);
        break;

    }

    wrongInstruction = 1;



    }// close while




    fclose(output);
}

// Here ends the secondPassage function.









// Here begins the findInFile function.
// This function will be responsible for searching a specific string in its database (the labelsTable file).

int findInFile(char* inputString){

// Let us declare the variables.

    FILE *lookUpTable;
    lookUpTable = fopen("labelsTable.txt", "r");

    char leitor[50], *label, *memoryPosition, *type;

    int comparison, result, size;

// Here is the main cicle.

    while ((fgets(leitor, 50, lookUpTable)) != NULL){

        label = strtok(leitor, ";");
        memoryPosition = strtok(NULL, ";");
        type = strtok(NULL, ";");

        size = strlen(label);

        comparison = strncmp(inputString, label, size);

// If we find a match between the input string and the label on any line,
// we'll return the value of the memory position from the file, converted to int.

        if (comparison == 0){
            result = atoi(memoryPosition);
            fclose(lookUpTable);
            return result;
        }

    }// close while.

// If we don't find a match, we'll return -1.
// This indicates a missing declaration or label.

fclose(lookUpTable);
return -1;
}

// Here ends the findInFile function.















// Here begins the duplicateLabelCheck function.
// This function checks if there are any duplicate labels in the labelsTable.txt file.

int duplicateLabelCheck(FILE *input){

char leitor[50], analysisLabel[50], comparisonLabel[50];

int exit = 0, ret = 0, innerExit = 0, counter = 0, i, lineCounter = 0;

while( fgets(leitor, 50, input) != NULL){
    lineCounter++;
}

rewind(input);

lineCounter--;

while (exit != 1){

        fgets(leitor, 50, input);

        if (leitor == NULL){
            exit = 1;
        }

        else{

        for(i=0; leitor[i] != ';'; i++){
            analysisLabel[i] = leitor[i];
        }
        analysisLabel[i] = NULL;

        while (fgets(leitor, 50, input) != NULL             ){

                for (i=0; leitor[i] != ';'; i++){
                    comparisonLabel[i] = leitor[i];
                }
                comparisonLabel[i] = NULL;

                ret = strcmp(analysisLabel, comparisonLabel);

                if (ret == 0){
                    exit = 1;
                    return 1;
                }




        }

        rewind(input);
        counter++;


        if (counter >= lineCounter){
            exit = 1;
        }

        for(i=0; i < counter; i++){
            fgets(leitor, 50, input);
            }

        }

}




return 0;
}



// Here ends the duplicateLabelCheck function.












// Here begins the variableTypeCheck function.
// This function will see if the operand of the operation in progress is befitting its nature.
// returns 1 if the variable type is correct, 0 if it's not. Returns -1 if it cannot find any correlation.

int variableTypeCheck(int inputVariableType, char* inputString){

// Let us declare the variables.

    FILE *lookUpTable;
    lookUpTable = fopen("labelsTable.txt", "r");

    char leitor[50], *label, *memoryPosition, *type;

    int ret;

// Here is the main cicle.

    while ((fgets(leitor, 50, lookUpTable)) != NULL){

        label = strtok(leitor, ";");
        memoryPosition = strtok(NULL, ";");
        type = strtok(NULL, ";");

// Let us define the following:
// inputVariableType = 1 expects an appropriate label for ADD, SUB, DIV, MUL: that is, a memory space that was reserved from either SPACE or CONST.
// inputVariableType = 2 expects an appropriate label for STORE OR COPY: that is, only a memory space that was reserved from a SPACE instruction.
// inputVariableType = 3 expects an appropriate label for JMP, JMPN, JMPZ, JMPP.

        ret = strcmp(label, inputString);

        if (ret == 0){

            switch (inputVariableType){

                case 1:

                    ret = strncmp(type, "label", 5);
                    if (ret == 0){
                        return 0;
                    }

                    else{
                        return 1;
                    }



                break;

                case 2:

                    ret = strncmp(type, "space", 5);
                    if (ret == 0){
                        return 1;
                    }

                    else{
                        return 0;
                    }

                break;

                case 3:

                    ret = strncmp(type, "label", 5);
                    if (ret == 0){
                        return 1;
                    }

                    else{
                        return 0;
                    }

                break;

                default:
                    printf("Abort operation!\n");
                    return -1;
                break;





            }







        }



    }// close while.

// If we don't find a match, we'll return -1.
// This indicates a missing declaration or label.

fclose(lookUpTable);
return -1;
}
















// Here ends the variableTypeCheck function.




// Here begins the spellCheck function.
// Receives a string and checks if it is in accordance with C's rules for naming variables.
// Returns 1 if everything's OK, Returns 0 otherwise.

int spellCheck(char *inputString){

// lines for variables.

    char copy[50];
    int i;

// code.

    strcpy(copy, inputString);

// first character can't be a number.

    if (copy[0] < 97 || copy[0] > 122){
        if (copy[0] != '_'){
            return 0;
        }
    }

// other characters must be wither letters, numbers or underscores.

    for (i=1; copy[i] != ('\n' || '\0'); i++){

        if (   copy[i] > 122 || (copy[i] < 97 && copy[i] > 57) || copy[i] < 48){
            if (copy[i] != '_'){
                return 0;
            }


        }


    }

return 1;

}




// Here ends the spellCheck function.
