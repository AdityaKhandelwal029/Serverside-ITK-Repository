#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count;
		char *homef_name;
		tag_t *find_list,user_tag,homef_tag;
		SA_find_user2("infodba", &user_tag);
		SA_ask_user_home_folder(user_tag, &homef_tag);
		AOM_ask_name(homef_tag, &homef_name);
		WSOM_find2(homef_name, &count, &find_list);
		cout << " Total item found = " << count<<endl;
		for (int i = 0; i < count; i++)
		{
			string x1;
			char *item_pvalue,*class_name;
			tag_t class_tag;
			POM_class_of_instance(find_list[i],&class_tag);
			POM_name_of_class(class_tag, &class_name);
			cout << " class name = " << class_name<<endl;
			x1 = class_name;
			if (x1 == "Folder")
			{
				int secobj_count;
				tag_t relation_type;
				tag_t *secobj_list;
				GRM_find_relation_type("contents", &relation_type);
				GRM_list_secondary_objects_only(find_list[i],relation_type,&secobj_count,&secobj_list);
				cout << " secondary object count = " << secobj_count << endl;
			}
		}
	}
	return(0);
}

