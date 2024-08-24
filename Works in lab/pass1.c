#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Sreerag's code

int check_symtab_duplicate(FILE *symtab, const char *label)
{
    char sym_label[25];
    int address;
    rewind(symtab);
    while (fscanf(symtab, "%s\t%d", sym_label, &address) != EOF)
    {
        if (strcmp(sym_label, label) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    FILE *optab, *input, *symtab, *inter, *len;
    int locctr, start = 0;
    char mnemonic[15], opcode[25], operand[25], label[25], code[20];

    optab = fopen("OPTAB.txt", "r");
    input = fopen("INPUT.txt", "r");
    symtab = fopen("SYMTAB.txt", "w+");
    inter = fopen("INTER.txt", "w");
    len = fopen("LEN.txt", "w");

    if (optab == NULL || input == NULL || symtab == NULL || inter == NULL || len == NULL)
    {
        printf("Error: Unable to open one or more files.\n");
        return 1;
    }

    fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0)
    {
        start = atoi(operand);
        locctr = start;
        printf("START is %d\n", start);
        fprintf(inter, "%s\t%s\t%s\n", label, opcode, operand);
    }
    else
    {
        locctr = 0;
    }

    fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    while (strcmp(opcode, "END") != 0)
    {
        fprintf(inter, "%d\t", locctr);
        if (strcmp(label, "**") != 0)
        {
            if (check_symtab_duplicate(symtab, label))
            {
                printf("Error: Duplicate label '%s' found in SYMTAB.\n", label);
                fclose(len);
                fclose(optab);
                fclose(inter);
                fclose(symtab);
                fclose(input);
                return 1;
            }
            else
            {
                if (fprintf(symtab, "%s\t%d\n", label, locctr) < 0)
                {
                    printf("Error: Failed to write to SYMTAB.\n");
                    fclose(len);
                    fclose(optab);
                    fclose(inter);
                    fclose(symtab);
                    fclose(input);
                    return 1;
                }
            }
        }

        int found_opcode = 0;
        rewind(optab);
        while (fscanf(optab, "%s\t%s", code, mnemonic) != EOF)
        {
            if (strcmp(code, opcode) == 0)
            {
                locctr += 3;
                found_opcode = 1;
                break;
            }
        }

        if (!found_opcode)
        {
            if (strcmp(opcode, "WORD") == 0)
            {
                locctr += 3;
            }
            else if (strcmp(opcode, "RESW") == 0)
            {
                locctr += 3 * atoi(operand);
            }
            else if (strcmp(opcode, "RESB") == 0)
            {
                locctr += atoi(operand);
            }
            else if (strcmp(opcode, "BYTE") == 0)
            {
                if (operand[0] == 'C')
                {
                    locctr += strlen(operand) - 3;
                }
                else if (operand[0] == 'X')
                {
                    locctr += (strlen(operand) - 3) / 2;
                }
            }
        }

        fprintf(inter, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    }

    fprintf(inter, "%s\t%s\t%s\n", label, opcode, operand);
    fprintf(len, "%d", locctr - start);
    printf("\nLength of program is %d\n", locctr - start);

    fclose(len);
    fclose(optab);
    fclose(inter);
    fclose(symtab);
    fclose(input);

    return 0;
}