#include"Header.h"
int ITK_user_main(int argc, char *argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t item_t;
		char **prop_m;
		ITEM_find_item("000057", &item_t);
		AOM_ask_prop_names(item_t, &count, &prop_m);
		for (int i = 0; i < count; i++)
		{
			char *d_value;
			AOM_UIF_ask_value(item_t, prop_m[i], &d_value);
			cout << " property name = " << prop_m[i];
			cout << " value = " << d_value << endl;
		}
	}
	return(0);
}