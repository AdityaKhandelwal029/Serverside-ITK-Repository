#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		long long int x;
		int iCount;
		char *dvalue;
		char* cObject_name = NULL;
		tag_t item_t;
		tag_t* TValueTag = NULL;
		logical lLock = true;
		cout << ITEM_find_item("1409280", &item_t) << endl;
		cout << AOM_UIF_ask_value(item_t, "object_string", &dvalue) << endl;
		cout << " Object-string-property = " << dvalue << endl;
		cout << "Aom-lock-status = " << AOM_lock_for_delete(item_t) << endl;
		x=AOM_delete(item_t);
		cout << "AOM-delete-status = " << x << endl;
		if (x != 0)
		{
			char *error_code;
			EMH_ask_error_text(x, &error_code);
			cout << " Error message = " << error_code << endl;
			cout << " pom-check-status = " << POM_modifiable(item_t, &lLock) << endl;
			cout << " lock value = " << lLock << endl;
			if (lLock)
			{
				AOM_refresh(item_t, TRUE);
				AOM_save(item_t);
				cout << " Inside if block " << endl;
			}
			cout << "Aom-lock " << AOM_lock_for_delete(item_t) << endl;
			x = AOM_delete(item_t);
			cout << " Error code = " << x << endl;
			if (x != 0)
			{
				char *error_code;
				EMH_ask_error_text(x, &error_code);
				cout << " Error message = " << error_code << endl;
			}
		}
	}
	return(0);
}