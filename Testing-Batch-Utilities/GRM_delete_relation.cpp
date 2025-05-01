#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		long int x;
		tag_t item_t;
		cout << ITEM_find_item("1409151", &item_t) << endl;
		if (item_t != NULLTAG)
		{
			x = GRM_delete_relation(item_t);
			cout << x << endl;
		}
	}
	return(0);
}