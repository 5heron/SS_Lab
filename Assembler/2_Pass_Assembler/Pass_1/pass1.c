#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//If code doesn't work try code in SS_Lab/Works_in_lab/
//adaptation of https://github.com/ceadoor/System-Software-lab code
int check_symtab_duplicate(FILE *symtab, const char *label)
{
    char sym_label[25];
    int address;
    rewind(symtab);
    while (fscanf(symtab, "%s\t%d", sym_label, &address) != EOF){
        if (strcmp(sym_label, label) == 0)
            return 1;
    }
    return 0;
}

int main()
{
    char label[10], opcode[10], operand[10];
    char code[10], mnemonic[3], *temp;
    int locctr, start, length, flag;
    FILE *input, *optab, *symtab, *inter, *len;
    input = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "w+");
    inter = fopen("inter.txt", "w");
    len = fopen("length.txt", "w");
    if(input != NULL && optab != NULL){
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
        if (strcmp(opcode, "START") == 0) {
            //convert to hex and store it as int
            start = strtol(operand,&temp,16);                                     
            locctr = start;
            fprintf(inter, "\t%s\t%s\t%s\n", label, opcode, operand);     
            fscanf(input, "%s\t%s\t%s", label, opcode, operand); 
        } 
        else
            locctr = 0;
        while (strcmp(opcode, "END") != 0) {
            fprintf(inter, "%x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
            if (strcmp(label, "**") != 0) {
                if (check_symtab_duplicate(symtab, label))
                    printf("ERROR! : Duplicate symbol\n");
                else
                    fprintf(symtab, "%s\t%x\n", label, locctr);
            }
            fscanf(optab, "%s\t%s", code, mnemonic);
            flag = 0;
            while (strcmp(code, "END") != 0) {
                if (strcmp(opcode, code) == 0) {                       
                    locctr += 3;
                    flag = 1;
                    break;
                }
                fscanf(optab, "%s\t%s", code, mnemonic);
            }
            rewind(optab);
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            }
            else if (strcmp(opcode, "RESW") == 0) {
                locctr += (3 * (atoi(operand)));                      
            }
            else if (strcmp(opcode, "BYTE") == 0) {
                if(operand[0] == 'C')
                    locctr += strlen(operand) - 3;
                else    
                    locctr += (strlen(operand) - 3) / 2;
            }
            else if (strcmp(opcode, "RESB") == 0) {
                locctr += atoi(operand);
            }
            else if(flag == 0)
                printf("ERROR! : Undefined opcode\n");
            fscanf(input, "%s\t%s\t%s", label, opcode, operand);
        }
        fprintf(inter, "%x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        fclose(inter);
        fclose(symtab);
        fclose(optab);
        fclose(input);
        length = locctr - start;
        fprintf(len, "%x", length);
        fclose(len);
        printf("\nThe length of the code : %x\n", length);
    }
    return 0;
}