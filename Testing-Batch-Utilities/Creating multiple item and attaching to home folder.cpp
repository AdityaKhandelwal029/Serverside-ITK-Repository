#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		for (int i = 200; i <= 210; i++)
		 {
			cout << " Inside loop ";
			string a = "101";
			string b;
			int x = i;
			while (x != 0)
			{
				int y;
				y = x % 10;
				x = x / 10;
				b = b + char(48 + y);
			}
			a = a + b;
			printf(" a = %s", a);
			const char *id = a.c_str();
			printf(" id = %s", id);
			tag_t item_tag, itemr_tag, user_tag, homef_tag;
			ITEM_create_item(id, "test_item_10", "Item", "A", &item_tag, &itemr_tag);
			ITEM_save_item(item_tag);
			SA_find_user2("infodba", &user_tag);
			SA_ask_user_home_folder(user_tag, &homef_tag);
			FL_insert(homef_tag, item_tag,i-110);
			AOM_save_without_extensions(homef_tag);
		 }
	}
	return(0);
}
