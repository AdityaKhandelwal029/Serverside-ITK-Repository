#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{ 
		cout << " WAIT "<<getchar();
		int n_items;
		tag_t *items_t; 
		const char *attribute_name[2] = {"item_id"};
		const char *attribute_value[2] = {"000460"};
		cout<<ITEM_find_items_by_key_attributes(1,attribute_name,attribute_value,&n_items,&items_t);
		for (int i = 0; i < n_items; i++)
		{
			cout << " Inside for loop " << endl;
			char *d_value;
			AOM_UIF_ask_value(items_t[i],"object_name",&d_value); 
			cout << " Object-name = " << d_value << endl;
		}
	}
	return(0);
}