#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		ITK_set_bypass(TRUE);
		long long int x;
		char *dvalue;
		tag_t item_t;
		cout << ITEM_find_item("1409566", &item_t) << endl; 
		cout << AOM_UIF_ask_value(item_t, "object_name", &dvalue) << endl;
		cout << " Name = " << dvalue << endl;
		cout << "Aom-lock " << AOM_lock_for_delete(item_t) << endl;
		x = AOM_delete(item_t);
		cout << " Error code = " << x << endl;
		if (x != 0)
		{
			char *error_code;
			EMH_ask_error_text(x, &error_code);
			cout << " Error message = " << error_code << endl;
		}
		ITK_set_bypass(FALSE);
 	}
	return(0);
}