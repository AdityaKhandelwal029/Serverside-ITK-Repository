#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		for (int i = 1; i <= 10; i++)
		{
			tag_t item_t, itemr_t,user_t,homef_t;
			cout << ITEM_create_item(" ", " ", "A4customitem2", "A", &item_t, &itemr_t) << endl;
			ITEM_save_item(item_t); 
			SA_find_user2("infodba", &user_t);
			SA_ask_user_home_folder(user_t, &homef_t);
			FL_insert(homef_t, item_t, i + 25);
			AOM_save_with_extensions(homef_t);
		}
	}
	return(0);
}
