#include<iostream>
#include<dir.h>
#include<conio.h>
#include<windows.h>
#include<iomanip>

#include"_guest.h"
#include"../user/user.h"
#include"../menu/menu_feauture.h"
#include"../input_data/normal_input/inp_unk_str.h"
#include"../input_data/search_sub_str.h"

using std::cout;
using std::endl;
using std::setw;
using std::ios;

extern short max_char;

guest_activity::guest_activity()
{
    //make dir for guest and order record
    int a=mkdir("record_file");

        for(int i=0; i<3; i++)
            file_name[i]=new char[25];

        file_name[0]="./record_file/guest.txt";
        file_name[1]="./record_file/account.txt";
        file_name[2]="./record_file/menu.txt";

    //create file in record dir
    for(int i=0; i<3; i++)
    {
        temp_file.open(file_name[i],ios::app);
        temp_file.close();
    }

    guest_head=NULL;

    order_name=NULL;
    guest_name=NULL;
    get_price=0;
    quantity=0;
    grand_total=0;
    guest_money=0;
}

guest_activity::~guest_activity()
{
    for(int i=0; i<3; i++)
        delete[] file_name[i];
}

void guest_activity::show_order()
{
    guest_data* help=guest_head;
    if(help==NULL)
    {
        cout<<"order is empty\n\n";
        system("pause");

    }

    while(help!=NULL)
    {
        cout<<"---------------------------------\n";
        cout<<help->guest_name<<endl;
        cout<<"---------------------------------\n";
        order* h_order=help->head_order;

        while(h_order!=NULL)
        {
            cout<<h_order->order_name<<"\t\tQty: "<<h_order->quantity<<"\t\tPrc: "<<h_order->price<<endl;
            h_order=h_order->next;
        }
        cout<<endl;
        delete h_order;

        help=help->next_guest;
    }

    getch();
}

void guest_activity::guest_order(user& _user)
{
    char repeat;

    cout<<"   +ORDER+   \n";
    cout<<"+++++++++++++\n\n";

    cout<<"guest name: ";
    my_input_str(this->guest_name);
    if(strlen(this->guest_name)==0)
    {
        cout<<"invalid data\n";
        return;
    }

    guest_data* help=guest_head;
    if(help==NULL)
    {
        guest_head=new guest_data;
        guest_head->guest_name=this->guest_name;
        guest_head->head_order=NULL;//order struct node
        guest_head->next_guest=NULL;
    }
    else//add first
    {
        guest_data* add_guest=new guest_data;
        add_guest->guest_name=this->guest_name;
        add_guest->head_order=NULL;//order struct node

        add_guest->next_guest=help;
        guest_head=add_guest;
    }

    //search menu name in data
    max_char+=1;
    char temp[max_char];
    char price[8];//to convert price to int
    bool found=false;

add_order:
    //input menu name
    fflush(stdin);
    operation_file.open("./menu/food_menu.txt", ios::in);
    cout<<"menu name: ";
    my_input_str(order_name);

    if(strlen(order_name)==0)
    {
        cout<<"invalid data\n";
        return;
    }

    while(operation_file.getline(temp,max_char))
    {
        if(operation_file.tellg()==-1)
            continue;

        if(search_str(temp,order_name) != -1)
        {
            int j=0;
            for(int i=strlen(order_name)+1; i<strlen(temp); i++,j++)
                price[j]=temp[i];
            price[j++]='\0';
            cout<<"quantity: ";
            std::cin>>quantity;
            found=true;
        }

    }
    operation_file.close();
    if(!found)
    {
        cout<<"menu name not found\n\n";
        getch();
        return;
    }
    found=false;

    get_price=atoi(price);

    //check if same menu
    short add_menu=1;
    order* check_order=guest_head->head_order;
    while(check_order!=NULL)
    {
        if(strcmp(check_order->order_name,this->order_name)==0)
        {
            check_order->quantity+=this->quantity;
            add_menu=0;
           // cout<<"same\n";
            //getch();
        }

        check_order=check_order->next;
    }
    delete check_order;

    //save menu to node if not same with previous input
    if(add_menu)
    {
        //cout<<"add menu\n";
        //getch();
        order* help_order=guest_head->head_order;
        if(help_order==NULL)
        {
            guest_head->head_order=new order;
            guest_head->head_order->order_name=this->order_name;
            guest_head->head_order->price=get_price;
            guest_head->head_order->quantity=this->quantity;

            guest_head->head_order->next=NULL;
        }
        else
        {
            order* add_order=new order;
            add_order->order_name=this->order_name;
            add_order->price=get_price;
            add_order->quantity=this->quantity;

            add_order->next=guest_head->head_order;
            guest_head->head_order=add_order;
        }
    }

    cout<<"add order? y/n: ";
    std::cin>>repeat;
    fflush(stdin);

    if(repeat=='y' || repeat=='Y')
    {
        cout<<endl<<endl;
        memset(temp,0,max_char);
        memset(price,0,8);
        goto add_order;
    }

    guest_payment(guest_head,_user);

}

void guest_activity::guest_remove_order(char* gst_name)
{
    cout<<"+CANCEL ORDER+   \n";
    cout<<"+++++++++++++++\n\n";

    guest_data* help=guest_head;

    if(help==NULL)
    {
        cout<<"data is empty\n\n";
        system("pause");
        return;
    }

    if(gst_name==NULL)
    {
        cout<<"Guest Name: ";
        my_input_str(guest_name);
    }
    else
        strcpy(guest_name,gst_name);

    guest_data* before=NULL;

    while(help!=NULL)
    {
        if(strcmp(help->guest_name,this->guest_name)==0)
            break;

        before=help;
        help=help->next_guest;
    }

    if(help==NULL)
    {
        cout<<"\n\nguest name not found\n\n";
        system("pause");
        return;
    }

    //delete data in current node
    order* h_order=help->head_order;
    while(h_order!=NULL)
    {
        order* remove_order=h_order;
        h_order=h_order->next;

        delete[] remove_order->order_name;
        delete remove_order;
    }
    delete[] help->guest_name;

    //if help node same with head node
    if(help==guest_head)
    {
        guest_head=help->next_guest;
        delete help;
    }

    //if node after head node
    else if(help==guest_head->next_guest)
    {
        guest_head->next_guest=help->next_guest;
        delete help;
    }

    //if node in last indeks
    else if(help->next_guest==NULL)
    {
        before->next_guest=NULL;
        delete help;
    }

    //if node in midle
    else
    {
        before->next_guest=help->next_guest;
        delete help;
    }
}

void guest_activity::search_guest(guest_data*& node,user& _user)
{
    fflush(stdin);
    guest_data* help=node;

    if(help==NULL)
    {
        cout<<"data is empty\n\n";
        system("pause");
        return;
    }

    cout<<"Guest Name: ";
    my_input_str(guest_name);

    while(help!=NULL)
    {
        if(strcmp(help->guest_name,this->guest_name)==0)
            break;
        help=help->next_guest;
    }
    if(help==NULL)
    {
        cout<<"data not found\n\n";
        system("pause");
        return;
    }

    guest_payment(help, _user);
}

void guest_activity::guest_payment(guest_data*& node, user& _user)
{
    if(node==NULL)
    {
        cout<<"data is empty\n\n";
        system("pause");
        return;
    }

    char c;
    cout<<"\npay now, y/n?: ";
    std::cin>>c;
    if(c!='y' && c!='Y')
    {
        cout<<"\n\norder save to list\n\n";
        system("pause");
        return;
    }

    system("cls");

    order* help=node->head_order;

    cout<<"\t\tEXAMPLE RESTAURANT\n\n";
    cout<<"UNKNOWN PLACE\t\t\t  "<<__DATE__<<endl;
    cout<<"GENERAL\t\t\t\t  "<<__TIME__<<endl<<endl;
    cout<<"-----------------------------------------------------\n";
    cout<<"Qty\tProduct Name\tPrice\tDiscount\tTotal\n";
    cout<<"-----------------------------------------------------\n";
    //show all product order
    while(help!=NULL)
    {
        cout<<help->quantity<<'\t'<<help->order_name<<setw(16)<<help->price<<setw(10)<<0;
        help->total=help->price * help->quantity;
        cout<<setw(10)<<help->total<<endl;

        grand_total+=help->total;

        help=help->next;
    }

    cout<<"\n-----------------------------------------------------\n";
    cout<<"\t\t\tTotal: "<<grand_total<<endl;
    cout<<"\t\t\tPayment: ";std::cin>>guest_money;

     if(guest_money<grand_total)
    {
        cout<<"money is not enought transaction failed\n\n";
        system("pause");
        return;
    }

    cashBack=guest_money-grand_total;
    cout<<"\t\t\tCashback :"<<cashBack<<endl;
    cout<<"-----------------------------------------------------\n";
    cout<<"CASHIER\t\t\t\t\t"<<_user.user_data.user_name;
    cout<<"\n-----------------------------------------------------\n";

    //save to data
    guest_andOrder_record(node,_user);

    //remove guest from node
    guest_remove_order(node->guest_name);

    grand_total=0;//reset value for another value
    cout<<"transaction success\n\n";
    getch();
}

void guest_activity::guest_andOrder_record(guest_data*& node, user& _user)
{
    operation_file.open(file_name[0],ios::out | ios::app);

    order* help=node->head_order;

    operation_file<<"\t\tEXAMPLE RESTAURANT\n\n";
    operation_file<<"UNKNOWN PLACE\t\t\t  "<<__DATE__<<endl;
    operation_file<<"GENERAL\t\t\t\t  "<<__TIME__<<endl<<endl;
    operation_file<<"-----------------------------------------------------\n";
    operation_file<<"Qty\tProduct Name\tPrice\tDiscount\tTotal\n";
    operation_file<<"-----------------------------------------------------\n";

    while(help!=NULL)
    {
        operation_file<<help->quantity<<'\t'<<help->order_name<<setw(16)<<help->price<<setw(10)<<0<<setw(10)<<help->total<<endl;

        help=help->next;
    }

    operation_file<<"\n-----------------------------------------------------\n";
    operation_file<<"\t\t\tTotal: "<<grand_total<<endl;
    operation_file<<"\t\t\tPayment: "<<guest_money<<endl;
    operation_file<<"\t\t\tCashback :"<<cashBack<<endl;
    operation_file<<"-----------------------------------------------------\n";
    operation_file<<"CASHIER\t\t\t\t\t"<<_user.user_data.user_name<<endl;
    operation_file<<"GUEST\t\t\t\t\t"<<node->guest_name;
    operation_file<<"\n-----------------------------------------------------\n\n\n";


    operation_file.close();


}

void guest_activity::show_order_history()
{
    operation_file.open(file_name[0],ios::in);

    char temp[200];

    while(operation_file.getline(temp,200))
        cout<<temp<<endl;

    getch();
}

void guest_activity::display(user& _user)
{
    int indeks=0;
    char mov;
    char to_display[5][20]={"order" ,"show order", "remove order", "pay order", "exit"};
    while(1)
    {
        if(indeks>4)
            indeks=0;
        else if(indeks<0)
            indeks=4;

        cout<<"++++++++++++++++++++++++++++\n";
        cout<<"----------ORDER-------------\n";
        cout<<"++++++++++++++++++++++++++++\n\n";

         for(int i=0;i<5;i++)
        {
            if(indeks==i)
                cout<<"\t->"<<to_display[i]<<"<-\n\n";
            else
                cout<<"\t"<<to_display[i]<<endl<<endl;
        }

        mov=getch();

        if(mov=='s')
            indeks+=1;
        else if(mov=='w')
            indeks--;

        else if(mov=0x0D)
        {
            system("cls");

            if(indeks==0)
                guest_order(_user);
            else if(indeks==1)
                show_order();
            else if(indeks==2)
                guest_remove_order(NULL);
            else if(indeks==3)
                search_guest(guest_head,_user);
            else if(indeks==4)
                return;
            //std::cin.get();

        }

        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore();
        }

        //delete[] guest_name;
        system("cls");
    }
}







