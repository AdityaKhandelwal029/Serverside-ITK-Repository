#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		tag_t item_t, itemr_t;
		tag_t *rev_list;
		ITEM_find_item("000152", &item_t);
		ITEM_list_all_revs(item_t, &count, &rev_list); 
		cout << " Total revison = " << count<<endl;
		for (int i = 0; i < count; i++)
		{   
			char *value;
			AOM_UIF_ask_value(rev_list[i], "item_revision_id", &value);
			cout << " i " << i+1;
			cout << " value = " << value;
			cout << endl;
		}
	}
	return(0);
}