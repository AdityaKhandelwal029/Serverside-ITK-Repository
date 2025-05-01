#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int c;
		tag_t item_tag = NULL;
		tag_t rev_tag = NULL;
		ITEM_find_item("100", &item_tag);
		ITEM_set_id(item_tag, "1000");
		ITEM_set_name(item_tag, "APPLE_11");
		ITEM_save_item(item_tag);
		ITK_exit_module(true);
	}
	return(0);
}