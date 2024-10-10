#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//Execute Brain_damage.exe

struct list{
    int val;
    struct list *next;
};

struct sym_list{
    char label[10];
    // val == -1 (NULL) 
    // val == 0 or more (Actual symbol value)
    int val;
    struct list *li;
    struct sym_list *next;
} *sym_found, *symtab = NULL;

int search_symtab(char label[10]){
    sym_found = symtab;
    while(sym_found != NULL){
        if(strcmp(sym_found->label,label) == 0)
            return sym_found->val;
        sym_found = sym_found->next;
    }
    return -2;
}

void insert(char label[10], int loc, int append){
    struct sym_list *ptr1 = symtab, *temp1 = (struct sym_list*)malloc(sizeof(struct sym_list));
    strcpy(temp1->label,label);
    temp1->li = NULL;
    temp1->next = NULL;
    if(ptr1 != NULL){
        while (ptr1->next != NULL)
            ptr1 = ptr1->next;
    }
    if(append){
        struct list *temp = (struct list*)malloc(sizeof(struct list));
        temp->val = loc;
        temp->next = NULL;
        temp1->val = -1;
        temp1->li = temp;
        if(sym_found == NULL){
            if(ptr1 == NULL)
                symtab = temp1;
            else
                ptr1->next = temp1;
        }
        else{
            struct list *ptr = sym_found->li;
            while(ptr->next != NULL)
                ptr = ptr->next;
            ptr->next = temp;
        }
    }
    else{
        temp1->val = loc;
        temp1->li = NULL;
        if(ptr1 == NULL)
            symtab = temp1;
        else
            ptr1->next = temp1;
    }
}

FILE *del_list(char label[10], int loc, FILE *objcode){
    char temp[10], str[200];
    strcpy(str,"");
    if(sym_found != NULL){
        struct list *ptr = sym_found->li;
        while(ptr != NULL){
            sprintf(temp,"%06x",ptr->val);
            strcat(str,temp);
            strcat(str,"^");
            ptr = ptr->next;
        }
        sym_found->val = loc;
        fprintf(objcode, "T^%s02^%04x^\n",str,loc);
        printf("T^%s02^%04x^\n\n",str,loc);
    }
    return objcode;
}
FILE *print_rec(FILE *objcode,int rec_len,int text_add,char text_rec[160]){
    fprintf(objcode, "T^%06x^%02x^%s\n",text_add, rec_len/2, text_rec);
    printf("T^%06x^%02x^%s\n\n",text_add, rec_len/2, text_rec);
    return objcode;
}

int main()
{
    FILE *input, *optab, *symtab1, *objcode;
    char label[10], opcode[10], operand[10];
    char code[10], mnemonic[5], *temp;
    char text_rec[160], obj[60], str[10];
    int length,flag,locctr,start;
    input = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab1 = fopen("symtab.txt", "w");
    objcode = fopen("objcode.txt", "w");
    if(input != NULL && optab != NULL){
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
        if (strcmp(opcode, "START") == 0) {
            //convert to hex and store it as int
            start = strtol(operand,&temp,16);                                     
            locctr = start;
            fscanf(input, "%s\t%s\t%s", label, opcode, operand); 
        } 
        else
            locctr = 0;
        strcpy(text_rec,"");
        int rec_len, curr_len = 0, text_add = start, next_add = start;
        printf("Text Records:\n\n");
        while (strcmp(opcode, "END") != 0) {
            strcpy(obj,"");
            if (strcmp(label, "-") != 0) {
                int search_res = search_symtab(label);
                if(search_res == -1){
                    if(curr_len != 0)
                        objcode = print_rec(objcode,curr_len,text_add,text_rec);
                    text_add = locctr;
                    curr_len = 0;
                    strcpy(text_rec,"");
                    objcode = del_list(label,locctr,objcode);
                }
                else if(search_res == -2)
                	insert(label,locctr,0);
                else
                    printf("ERROR! : Duplicate symbol\n");    
            }
            //Starting Address of next record
            next_add = locctr;
            fscanf(optab, "%s\t%s", mnemonic, code);
            flag = 0;
            while (strcmp(mnemonic, "END") != 0) {
                if (strcmp(opcode, mnemonic) == 0) {                       
                    locctr += 3;
                    strcat(obj, code);
                    flag = 1;
                    break;
                }
                fscanf(optab, "%s\t%s", mnemonic, code);
            }
            rewind(optab);
            if(flag == 1){
            	int search_res = search_symtab(operand);
                if(search_res >= 0){
                	sprintf(str, "%04x", search_res);
                	strcat(obj, str);
                }
                else{
                    sprintf(str, "%04x", 0000);
                	strcat(obj, str);
                    if(search_res == -2)
                        fprintf(symtab1, "%s\t%s\n", operand, "**");
                	insert(operand,locctr + 1,1);
                }
            }
            else if (strcmp(opcode, "WORD") == 0){
                sprintf(str, "%06d", atoi(operand));
                strcat(obj, str);
                locctr += 3;
            }
            else if (strcmp(opcode, "RESW") == 0)
                locctr += (3 * (atoi(operand)));
            else if (strcmp(opcode, "BYTE") == 0) {
                int len = strlen(operand);
                if(operand[0] == 'C'){
                    for (int i = 2; i < len - 1; i++){
                        int asci = operand[i];
                        //Int Ascii formated as hex (coverted to hex), stored as string
                        sprintf(str, "%x", asci);
                        strcat(obj, str);
                    }
                    locctr += strlen(operand) - 3;
                }
                else{
                    strncat(obj, operand + 2, len - 3);
                    locctr += (strlen(operand) - 3) / 2;
                }
            }
            else if (strcmp(opcode, "RESB") == 0)
                locctr += atoi(operand);
            else
                printf("ERROR! : Undefined opcode\n");
            fscanf(input, "%s\t%s\t%s", label, opcode, operand);
            rec_len = curr_len;
            curr_len += strlen(obj);
            if(curr_len <= 60){
                //If length less than 60 (30 bytes)
                if(strlen(obj) != 0){
                    //Add it text record
                    strcat(obj,"^");
                    strcat(text_rec,obj);
                }
            }
            if(curr_len > 60 || strcmp(opcode, "END") == 0){
                if(curr_len != 0)
                    objcode = print_rec(objcode,rec_len,text_add,text_rec);
                text_add = next_add;
                strcpy(text_rec,"");
                curr_len = strlen(obj);
                strcat(obj,"^");
                strcat(text_rec,obj);
            }
        }
        fclose(optab);
        fclose(input);
        length = locctr - start;
        fprintf(objcode, "E^%06x^\n", start);
        printf("End record :\n\nE^%06x^\n", start);
        printf("\nThe length of the code : %x\n", length);
        symtab1 = fopen("symtab1.txt", "w");
        while (symtab != NULL){
            fprintf(symtab1, "%s\t%04x\n", symtab->label, symtab->val);
            symtab = symtab->next;
        }
    }
    fclose(symtab1);
    fclose(objcode);
    return 0;
}