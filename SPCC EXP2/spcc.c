#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct inst
{
    char label[20];
    char operand[20];
    char opcode[20];
    int count;
    int loc;
}inst;
typedef struct symbol
{
    char label[20];
    int loc;
}symbol;

int main()
{
   int label_count=0;
   char file_name[25];
   FILE *fp,*fp1,*fp2,*fp3,*fp4,*fp5,*fp6;
   char line[256];
   fp = fopen("Ass1.txt", "r"); // read mode
   fp1= fopen("Ass1.txt", "r"); // read mode
   fp2= fopen("symtab.txt", "w");
   fp3= fopen("optab.txt", "w");
   fp5= fopen("optab.txt", "r");
   fp6= fopen("output.txt","w");
   char label[20],opcode[20],operand[20];
   inst Instruct[300];
   symbol SYMTAB[50];
   fscanf(fp, "%s %s %s", label, opcode, operand);
   fscanf(fp1, "%s %s %s", label, opcode, operand);
   if(strcmp(opcode,"START")==0)
   {
     int loc=atoi(operand);
     fgets(line, sizeof(line), fp);
     int k=0;
    while (fgets(line, sizeof(line), fp))
    {
        int i=0;
        int t_count=1;
        while(line[i]=='\t')
                i+=1;
        while(i<strlen(line))
        {
            if(line[i]=='\t')
                   t_count+=1;
            i++;
        }
        if(t_count==3)
        {
         fscanf(fp1, "%s %s %s", &label, &opcode, &operand);
         fprintf(fp2, "%d\t%s\n",loc,label);

        }
        else if(t_count==2)
        {
         fscanf(fp1, "%s %s", &opcode, &operand);
        }
        else
        {
         fscanf(fp1, "%s", &opcode);
        }
        fprintf(fp3,"%d\t%s",loc,line);
        int flag=0;
        int o;
        char otp[20];
        Instruct[k].loc=loc;
        if(t_count!=1)
          strcpy(Instruct[k].operand,operand);
        strcpy(Instruct[k].opcode,opcode);
        Instruct[k].count=t_count;
        if(t_count==3)
        {
        strcpy(SYMTAB[label_count].label,label);
       SYMTAB[label_count].loc=loc;
            label_count+=1;
            strcpy(Instruct[k].label,label);
        }
        FILE *fp4;
        fp4= fopen("opcode.txt", "r");
        while(!feof(fp4))
        {
            fscanf(fp4,"%s %d",otp,&o);
        if(strcmp(opcode,otp)==0)
        {
           loc=loc+3;
           flag=1;
           break;
        }
        }
        if(flag==0)
        {
            if(strcmp(opcode,"WORD")==0)

            {
               loc=loc+3;
            }
            else if(strcmp(opcode,"RESW")==0)
            {
               int opr=atoi(operand);
               loc=loc+(3*opr);
            }
            else if(strcmp(opcode,"BYTE")==0)
            {
               if(operand[0]=='X')
               loc=loc+1;
               else
               {
                 int len=strlen(operand)-2;
                 loc=loc+len;
               }
            }
           else if(strcmp(opcode,"RESB")==0)
           {
               int opr=atoi(operand);
               loc=loc+opr;
           }
        }
        fclose(fp4);
        k++;

    }
    printf("PASS ONE IMPLEMENTED\n\n");
    printf("SYMBOL TABLE\n");
    for(int p=0;p<label_count;p++)
    {
        printf("%d\t%s\n",SYMTAB[p].loc,SYMTAB[p].label);
    }
    printf("\n");
    printf("PASS TWO\n");
    int z=0;
   while(z<k)
    {
        char out1[20];
        char out2[20];
        char o[5];
        char otp[10];
        fp4= fopen("opcode.txt", "r");
        int f=0;
        while(!feof(fp4))
        {
            fscanf(fp4,"%s %s",otp,o);
        if(strcmp(Instruct[z].opcode,otp)==0)
        {
           f=1;
           strcpy(out1,o);
           break;
        }
        }
        if(f==0)
        {
            if(strcmp(Instruct[z].opcode,"BYTE")==0)
            {
                int w=2,cn=0;
                while(Instruct[z].operand[w]!='\'')
                {
                    out1[cn]=Instruct[z].operand[w];
                    cn+=1;
                    w+=1;
                }

                if(strcmp(out1,"EOF")==0)
                         strcpy(out1,"454F46");
                strcpy(out2,"");


            }
            else{
            strcpy(out1,"--");
            strcpy(out2,"----");
            }

        }
        else{
        for(int t=0;t<label_count;t++)
        {
            if(strcmp(SYMTAB[t].label,Instruct[z].operand)==0)
            {
                itoa(SYMTAB[t].loc,out2,10);
                break;
            }
        }
        }

        printf("%s%s\n",out1,out2);
        fprintf(fp6, "%s%s\n",out1,out2);

        z++;

    }

   }

   else{
       printf("ERROR ! FIRST OPERAND SHOULD ALWAYS BE START");
   }
   fclose(fp);
   return 0;
}
