#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		char *item_name,**item_prop;
		tag_t item_tag;
		ITEM_find_item("000136", &item_tag);
		AOM_ask_name(item_tag, &item_name);
		cout << " **************Itemr name = " << item_name << endl;
		AOM_ask_prop_names(item_tag, &count, &item_prop);
		cout << " Total number of properties = " << count;
			for (int j = 0; j < count; j++)
			{
				char *propd_name, *propd_value;
				AOM_UIF_ask_name(item_tag, item_prop[j], &propd_name);
				cout << " property display _name = " << propd_name;
				AOM_UIF_ask_value(item_tag, item_prop[j], &propd_value);
				cout << " ****************property display value = " << propd_value << endl;
			}
	}
	return(0);
}

