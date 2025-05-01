#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		char *dvalue;
		int count;
		tag_t item_t, relation_type_tag, revision_tag;
		tag_t *secondaryobj_t;
		ITEM_find_item("000298",&item_t);
		ITEM_find_rev("000298", "A", &revision_tag);
		cout << " Item found successfuly " << endl;
		AOM_UIF_ask_value(item_t, "object_name", &dvalue);
		cout << " The name of item = " << dvalue << endl;
		GRM_find_relation_type("pseudo_folder", &relation_type_tag);
		GRM_list_secondary_objects_only(revision_tag, relation_type_tag, &count, &secondaryobj_t);
		cout << " Total secondary obj = " << count<<endl; 
		for (int i = 0; i < count; i++)
		{
			char *value1, *value2; 
			tag_t class_t;
			AOM_UIF_ask_value(secondaryobj_t[i], "object_name",&value1); 
			POM_class_of_instance(secondaryobj_t[i], &class_t);
			POM_name_of_class(class_t, &value2);
			cout << " object name = " << value1<<endl;
			cout << " class name = " << value2 << endl;
		} 
		cout << " ************************* " << endl; 
		int count2;
		char *dvalue2,*class_name;
		tag_t *sec2_t,class_t;
		GRM_list_all_related_objects_only(item_t,&count2,&sec2_t);
		cout << " Count 2 = " << count2 << endl;
		for (int j = 0; j < count2; j++)
		{
			AOM_UIF_ask_value(sec2_t[j],"object_name",&dvalue2); 
			POM_class_of_instance(sec2_t[j], &class_t);
			POM_name_of_class(class_t, &class_name);
			cout << " object name = " << dvalue2;
			cout << " class name = " << class_name<<endl;
		}
	}
	return(0);
}
