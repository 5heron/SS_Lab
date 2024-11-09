#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char opcode[10], label[10], operand[100];
char args[10][10];
int expanding = 0; // 0 == FALSE
long int strt,end;

FILE *deftab, *argtab, *namtab, *out, *input;

void expand();
void define();
void build_arg_list(int def){
    //build arg list
    char arg[10];
    for(int i = 0,j = 0,p = 0; i < strlen(operand); i++){
        j = 0;
        if(operand[i] == '&' || !def){
            p++;
            strcpy(arg, "");
            while (operand[i] != ',' && operand[i] != '\'' && i < strlen(operand))
                arg[j++] = operand[i++];
            if(!def && i < strlen(operand) && operand[i] == '\'')
                i++;
            arg[j] = '\0';
            strcpy(args[p], arg);
        }
    }
}
void replace_args(){
    char new_op[100], arg[10];
    int k = 0,p;
    for (int i = 0, j = 0; i < strlen(operand); i++)
    {
        j = 0;
        if(operand[i] == '&')
        {
            strcpy(arg, "");
            while (operand[i] != ',' && operand[i] != '\'' && i < strlen(operand))
                arg[j++] = operand[i++];
            arg[j] = '\0';
            p = 0;
            while(p < 10){
                if(strcmp(args[p],arg) == 0){
                    new_op[k++] = '?';
                    new_op[k++] = p + 48;
                }
                p++;
            }
            if(i < strlen(operand))
                new_op[k++] = operand[i];
        }
        else if(operand[i] == '?'){
            int pos = operand[++i] - 48;
            while(j < strlen(args[pos]))
                new_op[k++] = args[pos][j++];
        }
        else
            new_op[k++] = operand[i];
    }
    new_op[k] = '\0';
    strcpy(operand,new_op);
}
void process_line(){
    int flag = 0;
    char m_name[10];
    fseek(namtab,0,SEEK_SET);
    while (fscanf(namtab, "%s\t%ld\t%ld", m_name, &strt, &end) != EOF) {
        if (strcmp(opcode, m_name) == 0) {                       
            flag = 1;
            break;
        }
    }
    if(flag)
        expand();
    else if(strcmp(opcode,"MACRO") == 0)
        define();
    else if(strcmp(opcode,"MEND") != 0)
        fprintf(out,"%s\t%s\t%s\n",label,opcode,operand);
}

void get_line(){
    if(expanding){
        fscanf(deftab,"%s\t%s\t%s\n",label,opcode,operand);
        replace_args();
    }
    else
        fscanf(input,"%s\t%s\t%s\n",label,opcode,operand);
}

void expand(){
    expanding = 1;
    fprintf(out,".%s\t%s\t%s\n",label,opcode,operand);
    build_arg_list(0);
    fseek(deftab,strt,SEEK_SET);
    get_line();
    while(strcmp(opcode,"MEND") != 0){
        get_line();
        process_line();
    }
    expanding = 0;
    fseek(deftab,0,SEEK_END);
}

void define(){
    int pos = 0, j;
    char arg[10], copylabel[10];
    strt = ftell(deftab);
    fprintf(deftab,"%s\t%s\t%s\n",label,opcode,operand);
    strcpy(copylabel,label);
    build_arg_list(1);
    int level = 1;
    //levels don't work
    while(level > 0){
        get_line();
        //substitute parameters with ?no
        replace_args();
        fprintf(deftab,"%s\t%s\t%s\n",label,opcode,operand);
        if(strcmp(opcode,"MACRO") == 0)
            level++;
        else if(strcmp(opcode,"MEND") == 0)
            level--;
    }
    end = ftell(deftab);
    fseek(namtab,0,SEEK_END);
    fprintf(namtab,"%s\t%ld\t%ld\n",copylabel,strt,end);
}

int main(){
    input = fopen("input.txt", "r");
    deftab = fopen("deftab.txt", "w+");
    namtab = fopen("namtab.txt", "w+");
    out = fopen("out.txt", "w");
    if(input != NULL){
        while (strcmp(opcode,"END") != 0){
            get_line();
            process_line();
        }
    }    
}