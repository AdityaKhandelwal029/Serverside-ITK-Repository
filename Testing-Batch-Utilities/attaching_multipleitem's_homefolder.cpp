#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t user_tag, home_folder;
		SA_find_user2("installuser", &user_tag);
		SA_ask_user_home_folder(user_tag, &home_folder);
		for (int i = 15; i <= 25; i++)
		{
			tag_t item_tag, itemr_tag;
			string x="0000";
			int j,k;
			j = i % 10;
			k = i / 10;
			x = x + char(48 + k) + char(48 + j);
			printf("%s", x);
			const char *id = x.c_str();
			cout << " id = " << id<<endl;
			ITEM_create_item(id,"n1", "Item", "A", &item_tag, &itemr_tag);
			ITEM_save_item(item_tag);
			FL_insert(home_folder, item_tag,i+5);
			AOM_save(home_folder);
		}
	}
	return(0);
}