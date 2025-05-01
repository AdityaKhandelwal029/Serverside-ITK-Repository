#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t item_tag,itemr_tag;
		char *item_name,*item_name2,*rev_name;
		ITEM_find_item("000135",&item_tag);
		AOM_ask_name(item_tag, &item_name);
		cout << "ITEM NAME = " << item_name<<endl;
		ITEM_ask_name2(item_tag, &item_name2);
		cout << " item_name2 = " << item_name2<<endl;
		ITEM_ask_latest_rev(item_tag, &itemr_tag);
		AOM_ask_name(itemr_tag, &rev_name);
		cout << " rev name = " << rev_name << endl;
	}
	return(0);
}