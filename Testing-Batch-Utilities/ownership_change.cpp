#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_tag, user_tag, group_tag;
		ITEM_find_item("000147", &item_tag);
		SA_find_user2("installuser", &user_tag);
		SA_find_group("dba", &group_tag);
		AOM_set_ownership(item_tag, user_tag, group_tag);
		cout << " Task done "<< endl;
	}
	return(0);
}

