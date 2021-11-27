#ifndef menu_feature_h
#define menu_feature_h
#include<fstream>

class menu_feature {

public:
    menu_feature();

    void update_menu();//delete, add and change menu
    void show_menu();

    void table_update_reservation();
    void table_update();//to delete table or add table
    void show_table();


private:
    char* menu_type;
    char* menu_name;
    long long menu_price;

    short table_number;//for tabel number
    char* costumer_name;

    std::fstream operation_file;
    std::fstream temp_file;
    char* file_menu;
};


#endif // menu_feature_h
