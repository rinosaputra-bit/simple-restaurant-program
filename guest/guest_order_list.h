#ifndef guest_order_list_h
#define guest_order_list_h

struct order{
    char* order_name;

    short quantity;
    unsigned long long price;
    unsigned long long total;

    order* next;
};

struct guest_data{

    char* guest_name;

    order* head_order;
    guest_data* next_guest;
};

#endif // guest_order_list_h
