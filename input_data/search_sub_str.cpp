#include<iostream>
#include<cstring>
#include<conio.h>
#include"search_sub_str.h"

using std::cout;
using std::endl;
using std::cin;

int search_str(char* str, char* sub_str)
{
    int len_sub_str=strlen(sub_str);
    int k;
    int found=0;
    int i;
    for(i=0; i<=strlen(str); i++)
    {
        if(i>=len_sub_str)
        {
            for(int j=i-len_sub_str,k=0; j<i; j++,k++)
            {
                if(str[j] != sub_str[k])
                {
                    found=0;
                    break;
                }
                found=1;
            }
        }
        if(found){
            //cout<<"data is found in indeks->"<<i-len_sub_str<<"-"<<i<<endl;
            break;
        }
    }

    if(strcmp(sub_str,"pasword: ")==0)
        return i-len_sub_str;
    else if(strcmp(sub_str,"username: ")==0)
        return i;

    if(found)
        return i;

    return -1;
}





