#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		ITK_set_bypass(TRUE);
		tag_t part_t;
		char *dvalue;
		long int er;
		int iRef = 0;
		int* iLevel;
		tag_t* tRef = NULL;
		char** cRel = NULL;
		cout<<ITEM_find_item("1409171", &part_t)<<endl; 
		cout<<AOM_UIF_ask_value(part_t,"object_name",&dvalue)<<endl;
		cout << " Name = " << dvalue<<endl;
		cout<<" Aom lock for delete = "<<AOM_lock_for_delete(part_t)<<endl;
		er=AOM_delete(part_t); 
		cout << " Error code = " << er<<endl; 
		if (er > 0)
		{
			char *error_code;
			EMH_ask_error_text(er, &error_code); 
			cout << " Error message = " << error_code;
		}
		ITK_set_bypass(FALSE);
	}
	return(0);
}