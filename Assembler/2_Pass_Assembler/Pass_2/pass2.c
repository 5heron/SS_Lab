
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Touching grass very shortly !

int main()
{
    char address[5], name[6], opcode[6], label[6], operand[6], *temp, symbol[10], add[5], str[10], text_rec[160], obj[60];
    FILE *symtab, *inter, *objcode, *optab;
    symtab = fopen("symtab.txt", "r");
    inter = fopen("inter.txt", "r");
    objcode = fopen("out.txt", "w");
    optab = fopen("optab.txt", "r");
    if (symtab != NULL && inter != NULL)
    {
        //Get End address
        fseek(inter, -16, SEEK_END);
        fscanf(inter, "%s", address);
        int end = strtol(address, &temp, 16), start = 0;
        rewind(inter);
        fscanf(inter, "%s%s%s", name, opcode, address);
        if (strcmp(opcode, "START") == 0){
            start = strtol(address, &temp, 16);
            //length = End - start
            int length = end - start;
            fprintf(objcode, "H^%6s^%06x^%06x^\n", name, start, length);
            printf("Header record :\n\n H^%6s^%06x^%06x^\n\nText records :\n\n", name, start, length);
            fscanf(inter, "%s\t%s\t%s\t%s", address, label, opcode, operand);
        }
        strcpy(text_rec,"");
        int rec_len, curr_len = 0, text_add = start, next_add = start;
        while (strcmp(opcode, "END") != 0){
            char code[10], mnemonic[10];
            strcpy(obj,"");
            //Check if opcode is in optab
            rewind(optab);
            fscanf(optab, "%s\t%s", mnemonic, code);
            int flag = 0;
            while (strcmp(mnemonic, "END") != 0) {
                if (strcmp(opcode, mnemonic) == 0) {                       
                    flag = 1;
                    strcat(obj, code);
                    break;
                }
                fscanf(optab, "%s\t%s\n", mnemonic, code);
            }
            //If in optab
            if(flag == 1){
                if(strcmp(operand, "**") != 0){
                    //Check operand in symtab
                    rewind(symtab);
                    flag = 0;
                    while (fscanf(symtab, "%s\t%s", symbol, add) != EOF){
                        if (strcmp(operand, symbol) == 0){
                            strcat(obj, add);
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 0)
                        //If not in symtab ERROR
                        printf("ERROR : Symbol not found\n");
                }
                else
                    //If operand is "**" (RSUB)
                    strcat(obj,"0000");
            }
            else if (strcmp(opcode, "BYTE") == 0){
                //If BYTE
                int len = strlen(operand);
                if(operand[0] == 'X')
                    //Slice hex string from operand 
                    strncat(obj, operand + 2, len - 3);
                else {             
                    for (int i = 2; i < len - 1; i++){
                        int asci = operand[i];
                        //Int Ascii formated as hex (coverted to hex), stored as string
                        sprintf(str, "%x", asci);
                        strcat(obj, str);
                    }
                }
            }
            else if (strcmp(opcode, "WORD") == 0){
                //If WORD
                //String to int -> reformat it as 6 digit with leading zeroes -> stored back as string
                sprintf(str, "%06d", atoi(operand));
                strcat(obj, str);
            }
            else if (!(strcmp(opcode, "RESB") == 0 || strcmp(opcode, "RESW") == 0))
                //If None of the above
                printf("ERROR : Undefined opcode");
            //Length of record 
            curr_len += strlen(obj);
            if(curr_len <= 60){
                //If length less than 60 (30 bytes)
                if(strlen(obj) != 0){
                    //Add it text record
                    strcat(obj,"^");
                    strcat(text_rec,obj);
                    rec_len = curr_len;
                }
            }
            //Starting Address of next record
            next_add = strtol(address, &temp, 16);
            fscanf(inter, "%s\t%s\t%s\t%s", address, label, opcode, operand);
            if(curr_len > 60){
                //If adding the current instruction -> length > (30 bytes)
                //Print the old record
                fprintf(objcode, "T^%06x^%02x^%s\n",text_add, rec_len/2, text_rec);
                printf("T^%06x^%02x^%s\n\n",text_add, rec_len/2, text_rec);
                //Save address of current instruction as Starting adress of text record
                text_add = next_add;
                //Clear old text record and initialise new one concatenated with current instruction
                strcpy(text_rec,"");
                //Length of current instruction => text record length 
                rec_len = curr_len = strlen(obj);
                strcat(obj,"^");
                strcat(text_rec,obj);
            }
        }
        if(strlen(text_rec) != 0){
            fprintf(objcode, "T^%06x^%02x^%s\n",text_add, rec_len/2, text_rec);
            printf("T^%06x^%02x^%s\n\n",text_add, rec_len/2, text_rec);
        }
        fprintf(objcode, "E^%06x^\n", start);
        printf("End record :\n\nE^%06x^\n", start);
    }
    fclose(optab);
    fclose(objcode);
    fclose(inter);
    fclose(symtab);
    return 0;
}