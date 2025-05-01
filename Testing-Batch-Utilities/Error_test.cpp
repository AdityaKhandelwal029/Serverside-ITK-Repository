#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int x;
		tag_t item_t,itemr_t;
		x=ITEM_create_item("000675", "Test-item-2", "Item", "A", &item_t, &itemr_t); 
		cout << " X = " << x;
		x=ITEM_save_item(item_t); 
		cout << " X = " << x << endl;
	}
	return(0);
}