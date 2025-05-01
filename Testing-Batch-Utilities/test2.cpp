#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int iretcode = ITK_ok;
		int n = 0;
		tag_t item = NULLTAG;
		tag_t* items = NULL;
		const char *names[1] = { "item_id" }, *values[1] = { "1412901" };
		ITEM_find_items_by_key_attributes(1, names, values, &n, &items);
		cout << " n = " << n << endl;

	}
	return(0);
}