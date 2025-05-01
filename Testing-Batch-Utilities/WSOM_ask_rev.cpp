#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int rev_count,rev_count2,rev_limit,item_rev_c;
		tag_t item_t,*rev_list;
		ITEM_find_item("000485", &item_t);
		WSOM_ask_revision(item_t, &item_rev_c); 
		cout << " ITEM-REV-C = " << item_rev_c<<endl;

		ITEM_list_all_revs(item_t, &rev_count, &rev_list);
		cout << " rev_count = " << rev_count<<endl;
		for (int i = 0; i < rev_count; i++)
		{
			WSOM_ask_revision(rev_list[i], &rev_count2);
			cout << " REV_COUNT = " << rev_count2<<endl; 
		}

		WSOM_ask_revision_limit(item_t, &rev_limit); 
		cout << " Item-rev-limit = " << rev_limit;
	}
	return(0);
}
