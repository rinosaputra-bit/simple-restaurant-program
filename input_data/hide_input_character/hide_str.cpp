#include<iostream>
#include<cstring>
#include<conio.h>
#include<windows.h>
#include"hide_str.h"


uint8_t my_custom_hide_input(char*& word, char* id_username)//cant input white space
{
    char c;
    int len=2;
    word=new char[len];

    int i=0;

    system("cls");
    while(1)
    {
        std::cout<<"ID   : "<<id_username<<"\n\n";
        std::cout<<"Pasword : ";
        for(int k=0; k<i; k++)
            std::cout<<"*";

        c=getche();

        Sleep(100);
        system("cls");

        if(c=='\n' || c==0x0D)
            break;
        else if(c==8)//backspace
        {
            i-=1;
            continue;
        }
       else if(c==' ' || c=='\t' || c=='\f')
            continue;

        if(i==len)
        {
            char temp[len];
            strcpy(temp,word);

            len+=4;
            delete word;
            word=new char[len];

            strcpy(word,temp);
            delete temp;
        }

        word[i]=c;

        i++;
    }
    //i++;
    word[i]='\0';

    //if first indeks is zero string is empty
    if(word[0]=='\0')
        return 1;

    return 0;
}

void my_hide_input(char*& word)//accept white space
{
    char c;
    int len=2;
    word=new char[len];

    int i=0;

    while(1)
    {
        for(int k=0; k<i; k++)
            std::cout<<"*";

        c=getche();

        Sleep(100);
        system("cls");

        if(c=='\n' || c==0x0D)
            break;

        else if(c==8)//backspace
        {
            i-=1;
            continue;
        }

        if(i==len)
        {
            char temp[len];
            strcpy(temp,word);

            len+=4;
            delete word;
            word=new char[len];

            strcpy(word,temp);
            delete temp;
        }

        word[i]=c;

        i++;
    }
    //i++;
    word[i]='\0';

}


