#include"Header.h"
int ITK_user_main(int argc, char *argv[])
{
	 int x=login();
	 if (x == 1)
	 {
		 tag_t item_t, itemr_t;
		 cout<<ITEM_find_item("000124",&item_t);
		 cout << ITEM_ask_latest_rev(item_t, &itemr_t);
		 cout << ITEM_delete_rev(itemr_t);

	 }
	return (0);
}