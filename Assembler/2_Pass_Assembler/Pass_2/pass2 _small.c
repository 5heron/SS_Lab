#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int search(char label[10],FILE *f1){
    char name[10];
    int val;
    rewind(f1);
    while(fscanf(f1,"%s\t%x",name,&val) != EOF){
        if(strcmp(label,name) == 0)
            return val;
    }
    return -1;
}
int main(){
    FILE *inter,*out,*optab,*symtab;
    inter = fopen("inter.txt","r");
    out = fopen("out.txt","w");
    optab = fopen("optab.txt","r");
    symtab = fopen("symtab.txt","r");
    if(inter != NULL && optab != NULL && symtab != NULL){
        char label[10],opcode[10],operand[10],*temp,obj[20],cho[3],text[160];
        int locctr,start = 0,end,len;
        fseek(inter,-16,SEEK_END);
        fscanf(inter,"%x",&end);
        rewind(inter);
        fscanf(inter,"%s\t%s%s",label,opcode,operand);
        if(strcmp(opcode,"START") == 0){
            start = strtol(operand,&temp,16);
            len = end - start;
            fprintf(out,"H^%6s^%06x^%06x\n",label,start,len);
            fscanf(inter,"%x\t%s\t%s\t%s",&locctr,label,opcode,operand);          
        }
        int text_add = start,next_add = start,rec_len = 0,cur_len;
        strcpy(text,"");
        while(strcmp(opcode,"END") != 0){
            strcpy(obj,"");
            int code = search(opcode,optab);
            if(code != -1){
                int val = search(operand,symtab);
                if(val != -1)
                    sprintf(obj,"%02x%04x",code,val);
                else if(strcmp(operand,"**") == 0)
                    sprintf(obj,"%02x0000",code);
                else
                    printf("Symbol Not Found!\n");
            }
            else if(strcmp(opcode,"BYTE") == 0){
                if(operand[0] == 'X'){
                    strncpy(obj,operand + 2,strlen(operand) - 3);
                    obj[strlen(operand)-3] = '\0';
                }
                else{
                    for(int i = 2; i < strlen(operand) - 1; i++){
                        sprintf(cho,"%02x",operand[i]);     
                        strcat(obj,cho);
                    }
                    obj[((strlen(operand)-3)*2)] = '\0';
                }
            }
            else if(strcmp(opcode,"WORD") == 0)
                sprintf(obj,"%06x",atoi(operand));
            else if(!((strcmp(opcode,"RESW") == 0) || (strcmp(opcode,"RESB") == 0)))
                printf("Undefined opcode!\n");
            next_add = locctr;
            cur_len += strlen(obj);
            fscanf(inter,"%x\t%s\t%s%s",&locctr,label,opcode,operand);
            if(cur_len <= 60){
                if(strlen(obj) != 0){
                    strcat(obj,"^");
                    strcat(text,obj);
                    rec_len = cur_len;
                }
            }
            if(cur_len > 60){
                fprintf(out,"T^%04x^%02x^%s\n",text_add,rec_len/2 + (rec_len % 2),text);
                strcpy(text,"");
                rec_len = cur_len = strlen(obj);
                strcat(obj,"^");
                strcat(text,obj);
                text_add = next_add;
            }
        }
        if(strlen(text) != 0)
            fprintf(out,"T^%04x^%02x^%s\n",text_add,rec_len/2 + (rec_len % 2),text);
        fprintf(out, "E^%06x\n", start);
    }
    fclose(optab);
    fclose(out);
    fclose(inter);
    fclose(symtab);
}