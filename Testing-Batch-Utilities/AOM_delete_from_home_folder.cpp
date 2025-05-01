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
		tag_t tOwningUser = NULLTAG;
		tag_t tOwningGroup = NULLTAG;
		tag_t tHomeFolder = NULLTAG;
		tag_t* TValueTag = NULL;
		logical lLock = true;
		cout << ITEM_find_item("1409565", &item_t) << endl;
		cout << AOM_UIF_ask_value(item_t, "object_string", &dvalue) << endl;
		cout << " Name = " << dvalue << endl;
		POM_ask_owner(item_t, &tOwningUser, &tOwningGroup);
		SA_ask_user_home_folder(tOwningUser, &tHomeFolder);
		AOM_ask_value_tags(tHomeFolder, "contents", &iCount, &TValueTag);
		for (int i = 0; i < iCount; i++)
		{
			AOM_ask_value_string(TValueTag[i], "object_string", &cObject_name);
			int c = tc_strcmp(dvalue, cObject_name);
			if (c == 0)
			{
				FL_remove(tHomeFolder, TValueTag[i]);
				AOM_save(tHomeFolder);
			}
		}
		cout << "AOM-lock-status = " << AOM_lock_for_delete(item_t) << endl;
		x = AOM_delete(item_t);
		cout << "AOM-delete-status = " << x << endl;
		if (x != 0)
		{
			char *error_code;
			EMH_ask_error_text(x, &error_code);
			cout << " Error message = " << error_code << endl;
		}
	}
	return(0);
}