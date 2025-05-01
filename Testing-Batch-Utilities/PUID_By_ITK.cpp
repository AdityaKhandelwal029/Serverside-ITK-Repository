/*ITK-Program to convert TAG to UID.*/

#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		for (int i = 1; i <= 10; i++)
		{
			string x;
			tag_t item_t; 
			char *uid;
			cout << " Please enter item id = "; 
			cin >> x; 
			const char *id = x.c_str();
			ITEM_find_item(id, &item_t); 
			ITK__convert_tag_to_uid(item_t, &uid);
			cout << " UID = " << uid << endl;
			cout << "****************************************************************************" << endl;
			char *name;
			const char *uid1 = uid;
			tag_t itemr_t;
			ITK__convert_uid_to_tag(uid1, &itemr_t);
			AOM_UIF_ask_value(itemr_t, "object_name", &name); 
			cout << " The name of the item = " << name<<endl;
		}
	}
	return(0);
}
