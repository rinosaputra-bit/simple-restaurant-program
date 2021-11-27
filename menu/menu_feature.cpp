#include<iostream>
#include<cstring>
#include<conio.h>
#include<windows.h>
#include"menu_feauture.h"
#include"../input_data/normal_input/inp_unk_str.h"
#include"../input_data/search_sub_str.h"

using std::cout;
using std::endl;
using std::ios;

short max_char=13;//base on main course: string lenght is 13, when first time create
                  //file, this file just have string from operation file object
short max_table_string=3; //get max indeks in table reservation file defaul value is 3 base on 2 digit table number and null terminated
short max_table_num=-1;

menu_feature::menu_feature()
{
    menu_type=NULL;
    menu_name=NULL;


    file_menu=new char[strlen("./menu/food_menu.txt")+1];
    strcpy(file_menu,"./menu/food_menu.txt");

    //make file first
    operation_file.open(file_menu,ios::app | ios::ate | ios::in |ios::out);

    if(operation_file.tellg()==0)
        operation_file<<"APPERTIZER\nMAIN COURSE\nDESERT\n";

    operation_file.clear();
    operation_file.seekg(0,ios::beg);

    char c;
    short now=0;
    while(!operation_file.eof())
    {
        operation_file.get(c);

        if(c=='\n')
        {
            if(max_char<now)
                max_char=now;
            now=0;
        }

        now++;
    }

    operation_file.close();

    //create table reservation file use fstream temp file
    temp_file.open("./menu/table_reservation.txt",ios::in | ios::out | ios::ate | ios::app);
    if(temp_file.tellg()==0)
    {
        for(int i=0; i<20;i++)//assumtion that have 20 table
            temp_file<<i+1<<endl;
    }

    temp_file.clear();
    temp_file.seekg(0,ios::beg);

    now=0;
    while(!temp_file.eof())
    {
        temp_file.get(c);

        if(c=='\n')
        {
            if(max_table_string<now)
                max_table_string=now;
            now=0;
            max_table_num+=1;
        }

        now++;
    }
    temp_file.close();
}

void menu_feature::update_menu()//delete, add and change menu
{
    int choose_option;


    cout<<"      +UPDATE MENU+\n";
    cout<<"  ++++++++++++++++++++++\n\n";

    cout<<"1.add menu\n\n2.delete menu\n\n3.change menu\n\n";
    cout<<"choose : ";
    std::cin>>choose_option;

    std::cin.ignore();
    //system("cls");

    const char* failed="invalid type menu type, menu name or menu price is incorect\n\n";
    cout<<"\nmenu type: ";
    try
    {
        if(unsigned (my_input_str(menu_type)))
            throw failed;

        //upper string to compare with data in file equal to toupper function
        for(int i=0; menu_type[i]!='\0';i++)
        {
            if( *(menu_type+i)>='a' && *(menu_type+i)<='z' )
                menu_type[i]=menu_type[i]-32;
        }

        if(strcmp(menu_type,"APPERTIZER")!=0 && (strcmp(menu_type,"MAIN COURSE") && strcmp(menu_type,"DESERT")))
            throw failed;

        cout<<"\nmenu name: ";
        if(unsigned(my_input_str(menu_name)))
            throw failed;

        if(choose_option==1)//if add menu input price
        {
            cout<<"\nprice: ";
            std::cin>>menu_price;
            if(std::cin.fail())//if user input except number
                throw failed;
        }
    }
    catch(const char*)
    {
        cout<<failed<<endl;
        system("cls");

        std::cin.clear();
        std::cin.ignore();
        return;
    }


    //update max char if lenght of menu_name is bigger than max char
    if(max_char<strlen(menu_name) || max_char-8 < strlen(menu_name))
        max_char=strlen(menu_name)+9;//include price max range is 1 milion or 9 char


    //write file to temp
    operation_file.open(file_menu, ios::in);
    temp_file.open("./user/temp.txt",ios::out);

    char* temp_str=new char[max_char];
    int status=0;
    while(operation_file.getline(temp_str,max_char))
    {
        if(operation_file.tellg()==-1)
            continue;


        if(strcmp(temp_str,menu_type)==0)//add menu if head of menu type string is found
        {
            if(choose_option==1)
            {
                temp_file<<menu_type<<endl;
                temp_file<<menu_name<<(char)223<<menu_price<<endl;//user ascci char code 223 to seperate menu name and menu price
                status=1;
                continue;
            }
        }

        int found=search_str(temp_str,menu_name);

        if(found != -1)
        {
            if(choose_option==2)//skip menu name to write in data /txt or delete it
            {
                status=1;
            }

            else if(choose_option==3)//skip write from menu data and fill new value to menu data
            {
                try
                {
                    cout<<"\nnew menu: ";
                    if(unsigned(my_input_str(menu_name)))
                        throw failed;

                    cout<<"\nnew price: ";
                    std::cin>>menu_price;

                    if(std::cin.fail())//if user input except number
                        throw failed;
                }
                catch(const char*)
                {
                    cout<<failed<<endl;
                    return;
                }


                if(max_char<strlen(menu_name))//update max char if lenght of menu_name is bigger than max char
                    max_char=strlen(menu_name);

                temp_file<<menu_name<<(char)223<<menu_price<<endl;
                status=1;
            }

            continue;
        }

        temp_file<<temp_str<<endl;//write data to temporary file
    }

    //delete temp_str;
    //temp_str=new char[max_char++];

    //write file to food file and update it, trunc food file first
    operation_file.close();
    temp_file.close();

    operation_file.open(file_menu, ios::out);//default is trunc
    temp_file.open("./user/temp.txt", ios::in);

    while(temp_file.getline(temp_str,max_char))
    {
        if(temp_file.tellg()==-1 || strlen(temp_str)==0)//to skip null value
            continue;

        operation_file<<temp_str<<endl;
    }

    operation_file.close();
    temp_file.close();

    if(status)
        cout<<"\n\nmenu succesfully update--\n\n";

    else
        cout<<"menu failed update, menu is not found\n\n";

    system("pause");
    system("cls");

}

void menu_feature::show_menu()
{
    operation_file.open(file_menu, ios::in);
    char temp[max_char++];

    while(operation_file.getline(temp,max_char))
    {
        if(strcmp(temp,"APPERTIZER")==0 || strcmp(temp,"MAIN COURSE")==0 || strcmp(temp,"DESERT")==0)
        {
            cout<<"\n\n----------------------\n"<<temp<<"\n----------------------\n";
            continue;
        }

        for(int i=0; i<=strlen(temp); i++)
        {
            if(temp[i]<0)
            {
                cout<<"\trp.";
                continue;
            }
            cout<<temp[i];

        }

        cout<<endl;
    }

    cout<<endl;
    operation_file.close();
    system("pause");
}

void menu_feature::table_update_reservation()
{
    char* failed="invalid input";
    int choose_option;

    cout<<"   +UPDATE RESERVATION+\n";
    cout<<"  ++++++++++++++++++++++\n\n";

    cout<<"1.add table reservation\n\n2.delete table reservation\n\n";
    cout<<"choose : ";
    try
    {
        std::cin>>choose_option;

        if(std::cin.fail())
            throw failed;

        cout<<"\nchoose table number: ";
        std::cin>>table_number;

        if(std::cin.fail())
            throw failed;

        std::cin.ignore();

    }
    catch(const char*)
    {
        cout<<failed<<endl;
        return;
    }


    //write file to temp
    operation_file.open("./menu/table_reservation.txt", ios::in);
    temp_file.open("./user/temp.txt",ios::out);


    char* get_table_data=new char[max_table_string++];
    short check_table=0; //increment value till same with table number from input
    bool succes=true;

    while(operation_file.getline (get_table_data,max_table_string) )
    {
        check_table++;
        if(operation_file.tellg()==-1)//to skip trash value
            continue;

        if(table_number==check_table)
        {
            if(choose_option==1)
            {
                if(strlen(get_table_data)<3)
                {
                    cout<<"\ncostumer name: ";
                    if(unsigned(my_input_str(costumer_name)))
                    {
                        cout<<endl<<failed<<endl;
                        succes=false;
                        continue;
                    }
                    temp_file<<table_number<<" "<<costumer_name<<endl;

                    if(max_table_string < strlen(costumer_name))
                        max_table_string=strlen(costumer_name)+3;
                    continue;
                }

                else
                {
                    cout<<"\ntable has been book by another costumer\n";
                    succes=false;
                }
            }

            else if(choose_option==2)
            {
                if(strlen(get_table_data)>3)
                {
                    temp_file<<table_number<<endl;
                    continue;
                }

                cout<<"\ncan not delete reservation at table "<<table_number<<" is empty\n";
                succes=false;
            }
        }


        temp_file<<get_table_data<<endl;//write file normally
    }

    operation_file.close();
    temp_file.close();
    delete get_table_data;

    if(!succes)//dont write to table file if update failed
        return;

    operation_file.open("./menu/table_reservation.txt", ios::out);
    temp_file.open("./user/temp.txt",ios::in);
    get_table_data=new char[max_table_string++];

    while(temp_file.getline(get_table_data,max_table_string))
    {
        if(temp_file.tellg()==-1)
            continue;

        if(strlen(get_table_data)==0)
            continue;

        operation_file<<get_table_data<<endl;
    }

    operation_file.close();
    temp_file.close();
    cout<<"\nsucces update table reservation\n\n";
}

void menu_feature::table_update()
{
    short choose_option;
    char* failed="invalid input or number table bigger than 99\n";

    cout<<"     +TABLE SETTING+\n";
    cout<<"  ++++++++++++++++++++++\n\n";

    cout<<"1.add table \n\n2.delete table\n\n";
    cout<<"choose : ";

    try
    {
        std::cin>>choose_option;
        if(std::cin.fail() || choose_option>2)
            throw failed;

        cout<<"\nchoose table number: ";
        std::cin>>table_number;

        if(std::cin.fail() || table_number>99)//if table numebr have 3 digit
            throw failed;
    }

    catch(const char*)
    {
        cout<<failed<<endl;
        return;
    }

    operation_file.open("./menu/table_reservation.txt", ios::in);
    temp_file.open("./user/temp.txt", ios::out);

    char* temp=new char[max_table_string++];
    char str_table_number[3];
    itoa(table_number,str_table_number,10);
    while(!operation_file.eof())
    {
        operation_file.getline(temp,max_table_string);

        if(operation_file.tellg() == -1)
            continue;

        if(search_str(temp,str_table_number) != -1)
        {
            if(choose_option==2)
                continue;

            else
            {
                cout<<"table already exist\n\n";
                return;//skip write file to table file
            }
        }
        temp_file<<temp<<endl;
    }

    if(choose_option==1)
        temp_file<<table_number<<endl; //write/add new table if input no table from user not same with data

    operation_file.close();
    temp_file.close();

    operation_file.open("./menu/table_reservation.txt",ios::out);
    temp_file.open("./user/temp.txt", ios::in);

    while(temp_file.getline(temp,max_table_string))
    {
        if(temp_file.tellg()== -1 || strlen(temp)==0)
            continue;

        operation_file<<temp<<endl;
    }

    operation_file.close();
    temp_file.close();

}

void menu_feature::show_table()
{
    operation_file.open("./menu/table_reservation.txt",ios::in);

    char to_show[max_table_string++];
    while(operation_file.getline(to_show,max_table_string))
    {
        if(strlen(to_show)>=3)
        {
            cout<<"table "<<to_show[0]<<to_show[1]<<"\t: [";
            for(int i=2; i<strlen(to_show); i++)
                cout<<to_show[i];

            cout<<"]\n";
            continue;
        }

        cout<<"table "<<to_show[0]<<to_show[1]<<"\t: "<<"empty\n";
    }

    operation_file.close();
    getch();
}































