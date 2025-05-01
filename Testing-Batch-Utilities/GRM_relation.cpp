#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		long long int x,a,b;
		int iCount;
		char *dvalue = NULL;
		char* cObject_name = NULL;
		tag_t item_t, relation_t;
		tag_t tOwningUser = NULLTAG;
		tag_t tOwningGroup = NULLTAG;
		tag_t tHomeFolder = NULLTAG;
		tag_t* TValueTag = NULL;
		logical lLock = true; 
		int count; 
		tag_t *sec_obj;
		cout << ITEM_find_item("1409595", &item_t) << endl;
		cout << AOM_UIF_ask_value(item_t, "object_string", &dvalue) << endl;
		cout << " Name = " << dvalue << endl;
		cout << AOM_UIF_ask_value(item_t, "item_id", &dvalue) << endl;
		cout << " Item_id = " << dvalue << endl;
		GRM_find_relation_type("IMAN_reference",&relation_t); 
		GRM_list_secondary_objects_only(item_t, relation_t, &count, &sec_obj);  
		cout << " Count = " << count << endl; 
	    a=GRM_delete_relation(relation_t);  
		cout << " a = " <<a <<endl; 
		if (a != 0)
		{
			char *error;
			EMH_ask_error_text(a, &error); 
			cout << " Error message = " << error << endl; 
		}  
		cout << AOM_save(item_t);

	}
	return(0);
}
