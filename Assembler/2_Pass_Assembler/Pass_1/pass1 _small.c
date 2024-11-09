#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int search(char label[10],FILE *f1){
    char name[10],val[10];
    rewind(f1);
    while(fscanf(f1,"%s\t%s",name,val) != EOF){
        if(strcmp(label,name) == 0)
            return 1;
    }
    return 0;
}
int main(){
    FILE *input, *inter, *optab, *symtab, *len;
    char operand[10],opcode[10],label[10],*temp;
    input = fopen("input.txt", "r");
    inter = fopen("inter.txt", "w");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "w+");
    len = fopen("length.txt","w");
    if(input != NULL && optab != NULL){
        fscanf(input,"%s\t%s\t%s",label,opcode,operand);
        int start = 0,locctr;
        if(strcmp(opcode,"START") == 0){
            start = strtol(operand,&temp,16);
            fprintf(inter,"\t%s\t%s\t%s\n",label,opcode,operand);
            fscanf(input,"%s\t%s\t%s",label,opcode,operand);
        }
        locctr = start;
        while(strcmp(opcode,"END") != 0){
            fprintf(inter,"%04x\t%s\t%s\t%s\n",locctr,label,opcode,operand);
            if(strcmp(label,"**") != 0){
                if(search(label,symtab))
                    printf("Duplicate symbol !\n");
                else 
                    fprintf(symtab, "%s\t%04x\n",label,locctr);
            }
            if(search(opcode,optab) || strcmp(opcode,"WORD") == 0)
                locctr += 3;
            else if(strcmp(opcode,"BYTE") == 0){
                if(operand[0] == 'X')
                    locctr += (strlen(operand) - 3)/2;
                else
                    locctr += strlen(operand) - 3;
            }
            else if(strcmp(opcode,"RESB") == 0)
                locctr += atoi(operand);
            else if(strcmp(opcode,"RESW") == 0)
                locctr += atoi(operand) * 3;
            else
                printf("Undefined opcode !\n");
            fscanf(input,"%s\t%s\t%s",label,opcode,operand);
        }
        fprintf(inter,"%04x\t%s\t%s\t%s\n",locctr,label,opcode,operand);
        int length = locctr - start;
        fprintf(len,"%04x",length);
        printf("Length = %04x\n",length);
    }
    fclose(optab);
    fclose(symtab);
    fclose(inter);
    fclose(input);
}