#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct inst
{
    char label[20];
    char operand[20];
    char opcode[20];
}inst;
typedef struct symbol
{
    char label[20];
    int loc;
    int arg_count;
    char args[10][10];
}symbol;

char *replaceWord(const char *s, const char *oldW, const char *newW)
{
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}
int main()
{
   int i_count=0,m_count=0;
   FILE *fp,*fp1,*fp2,*fp3,*fp4,*fp5,*fp6,*fp7,*fp8,*fp9;
   char line[256];
   fp = fopen("Input.txt", "r"); // read mode
   fp1= fopen("Input.txt", "r"); // read mode
   fp2= fopen("NAMETAB.txt","w");
   fp3= fopen("DEFTAB.txt","w");
   fp5= fopen("MacroOutPut.txt","w");
   fp6= fopen("output1.txt","w");
   fp7= fopen("output1.txt","r");
   fp4= fopen("output1.txt", "r"); // read mode
   fp8= fopen("OUTPUTFINAL.txt","w");
   char label[200],opcode[200],operand[200];
   inst DEFTAB[300];
   symbol NAMETAB[50];
   bool mstart=false;
   while (fgets(line, sizeof(line), fp))
   {
        int i=0;
        int t_count=1;
        while(i<strlen(line))
        {
            if(line[i]==' ')
                   t_count+=1;
            i++;
        }
        if(t_count==3)
        {
         fscanf(fp1, "%s %s %s", &label, &opcode, &operand);
        }
        else if(t_count==2)
        {
         fscanf(fp1, "%s %s", &opcode, &operand);
         strcpy(label,"");
        }
        else
        {
         fscanf(fp1, "%s", &opcode);
        strcpy(label,"");
         strcpy(operand,"");


        }
        if(mstart==false)
        {
           if(strcmp(opcode,"MACRO")==0)
           {
                strcpy(NAMETAB[m_count].label,label);
                NAMETAB[m_count].loc=i_count;
                strcpy(DEFTAB[i_count].label,label);
                strcpy(DEFTAB[i_count].opcode,opcode);
                strcpy(DEFTAB[i_count].operand,operand);
                int count=0,pos=0;
                for(int t=0;t<strlen(operand);t++)
                {
                    if(operand[t]=='&')
                    {
                         count+=1;
                         pos=0;
                         NAMETAB[m_count].args[count-1][pos]=operand[t];
                         pos+=1;

                    }
                     else
                     {
                        if(operand[t]!=',')
                        {
                           NAMETAB[m_count].args[count-1][pos]=operand[t];
                           pos+=1;
                        }
                     }
                }
                NAMETAB[m_count].arg_count=count;
                m_count+=1;
                i_count+=1;
                mstart=true;
                fprintf(fp2,"%s\t%d\n",label,i_count);
                fprintf(fp3,"%s",line);

           }
           else
           {
               fprintf(fp6,"%s",line);
           }

        }
        else
        {
            if(strcmp(opcode,"MEND")==0)
            {
                mstart=false;
            }
            fprintf(fp3,"%s",line);
            strcpy(DEFTAB[i_count].label,label);
            strcpy(DEFTAB[i_count].opcode,opcode);
            strcpy(DEFTAB[i_count].operand,operand);
            i_count+=1;

        }
   }
   printf("PASS ONE COMPLETED\n");
   printf("NAME TABLE AND DEFINITION TABLE GENERATED\n");

   for(int k=0;k<m_count;k++)
   {
       printf("MACRO NAME: %s\t MACRO POSITION :%d\t NUMBER OF ARGUMENTS: %d\n ARGUMENTS:\t",NAMETAB[k].label,NAMETAB[k].loc+1,NAMETAB[k].arg_count);
       for(int p=0;p<NAMETAB[k].arg_count;p++)
       {
           for(int i=1;i<strlen(NAMETAB[k].args[p]);i++)
               printf("%c",NAMETAB[k].args[p][i]);
           printf("\t");
       }
       printf("\n");

   }

   fclose(fp6);
   fclose(fp1);
   fclose(fp2);
   fclose(fp3);
   fclose(fp5);
   printf("\n\nPASS TWO\n");
   bool started=0;
   int ct=-1;
   int prev=0;
   while (fgets(line, sizeof(line), fp7))
   {
        int i=0;
        int t_count=1;
        while(i<strlen(line))
        {
            if(line[i]==' ')
                   t_count+=1;
            i++;
        }
        if(t_count==3)
        {
         fscanf(fp4, "%s %s %s", &label, &opcode, &operand);
        }
        else if(t_count==2)
        {
         fscanf(fp4, "%s %s", &opcode, &operand);
        }
        else
        {
         fscanf(fp4, "%s", &opcode);
        }
        int flag=0;
        int z;
        for(z=0;z<m_count;z++)
        {
            if(strcmp(NAMETAB[z].label,opcode)==0)
            {
                printf("MACRO CALL DETECTED\n");
                if(t_count==3)
                    fprintf(fp8,"%s ",label);
                flag=1;
                int w=NAMETAB[z].loc+1;
                int c=NAMETAB[z].arg_count;
                char arg[20][20];
                int pos=0;
                prev=ct+1;
                ct=ct+1;
                for(int s=0;s<strlen(operand);s++)
                {
                    if(operand[s]!=',')
                    {
                        arg[ct][pos]=operand[s];
                        pos+=1;
                    }
                    else
                    {
                        ct+=1;
                        pos=0;
                    }

                }
                fp9= fopen("DEFTAB.txt","r");
                int u=0;
                char l[50];
                while(u<NAMETAB[z].loc+1)
                {
                    fgets(l, sizeof(l), fp9);
                    u+=1;
                }
                while(strcmp(DEFTAB[w].opcode,"MEND")!=0 && fgets(l, sizeof(l), fp9))
                {
                     char *result[10];
                     for(int q=0;q<=c;q++)
                             result[q]=NULL;
                     result[0]=l;
                     int y;
                     for(y=0;y<c;y++)
                     {
                         result[y+1]=replaceWord(result[y],NAMETAB[z].args[y],arg[prev+y]);
                     }
                     fprintf(fp8,"%s",result[y]);
                     for(int q=0;q<=c;q++)
                         free(result[q]);
                     w+=1;
                }
                fclose(fp9);
                break;
            }
        }
        if(flag==0)
               fprintf(fp8,"%s",line);

   }
   printf("\nPASS TWO COMPLETE\nFINAL OUTPUT GENERATED IN OUTPUTFINAL.txt\n");

}
