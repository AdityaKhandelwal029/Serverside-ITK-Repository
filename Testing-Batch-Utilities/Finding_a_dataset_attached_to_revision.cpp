#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int itemrevcount,totalobjectcount,relatedobject;
		tag_t item_t,relation_t;
		tag_t *itemrevlist_t,*totalobj_t,*relatedobj_t;
		ITEM_find_item("000471", &item_t); 
		ITEM_list_all_revs(item_t, &itemrevcount, &itemrevlist_t);
		cout << " Total revision count = " << itemrevcount << endl;
		for (int i = 0; i < itemrevcount; i++)
		{
			GRM_list_secondary_objects_only(itemrevlist_t[i], NULLTAG, &totalobjectcount, &totalobj_t); 
			cout << "The total count of secondary object =  " << totalobjectcount << endl; 
			for (int i = 0; i < totalobjectcount; i++)
			{
				char *name;
				AOM_UIF_ask_value(totalobj_t[i], "object_name", &name);
				cout << " Name  = " << name << endl;
			}

			GRM_find_relation_type("IMAN_specification", &relation_t); 
			GRM_list_secondary_objects_only(itemrevlist_t[i], relation_t, &relatedobject, &relatedobj_t); 
			cout << " The total count of dataset = " << relatedobject << endl; 
			for (int i = 0; i < relatedobject; i++)
			{
				char *name;
				AOM_UIF_ask_value(relatedobj_t[i], "object_name", &name);
				cout << " Name  = " << name << endl;
			}

		}
	}
	return(0);
}