#ifndef _guest_h
#define _guest_h

#include<fstream>
#include"../user/user.h"
#include"guest_order_list.h"
#include"../user/user.h"

typedef unsigned long long money;
class guest_activity{

public:
    guest_activity();//make dir
    ~guest_activity();

    void guest_order(user& _user);
    void show_order();
    void guest_remove_order(char*);
    void guest_payment(guest_data*& node,user& _user);
    void guest_andOrder_record(guest_data*& node, user& _user);//order history
    void show_order_history();

    void search_guest(guest_data*& node,user& _user);

    void display(user&_user);

private:
    std::fstream operation_file;
    std::fstream temp_file;
    char* file_name[3];//guest history,account history,menu history,

    char* order_name;
    char* guest_name;
    money get_price;
    short quantity;
    money grand_total;
    money guest_money;
    money cashBack;

    guest_data* guest_head;
};


#endif // guest_h
