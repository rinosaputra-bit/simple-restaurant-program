#include<cstring>
#include<iostream>
#include"inp_unk_str.h"

uint8_t my_input_str(char*& word)
{
    char c;
    int len=2;
    word=new char[len];

    int i=0;

    while(std::cin.get(c))
    {
        if(c=='\n')
            break;

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

    //if first index is null mean that string is empty
    if(word[0]=='\0')
        return 1;
    return 0;
}
