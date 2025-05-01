#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		ITK_set_bypass(TRUE);
		tag_t item_t,user_t,rev_t; 
		cout<<ITEM_find_item("000461", &item_t)<<endl;  
		cout<<ITEM_ask_latest_rev(item_t,&rev_t) << endl;
		cout<<SA_find_user2("infodba", &user_t) << endl;
		cout<<AOM_refresh(item_t, TRUE) << endl;
		cout<<AOM_set_value_tag(item_t, "a5_USER_TAG", user_t) << endl;
		cout<<AOM_set_value_tag(item_t, "a5Tag_property", rev_t) << endl;
		cout<<AOM_save(item_t) << endl;
		cout<<AOM_refresh(item_t, FALSE) << endl; 
		ITK_set_bypass(FALSE);
	}
	return(0);
}