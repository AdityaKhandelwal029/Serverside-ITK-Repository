#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t itemt;
		tag_t lstitemrevt;
		tag_t newrevt;
		int count, count2;

		//char operation[] = "ITEM_revise_operation";

		char objname1[] = "NewCustomObject1";
		char relation1[] = "IMAN_specification";

		//string objname = "NewCustomObject1";
		//string relation = "IMAN_specification";

		tag_t *deepcopiedobjects;
		tag_t *deepcopyobjs;
		cout << ITEM_find_item("1412901", &itemt) << endl;
		cout << ITEM_ask_latest_rev(itemt, &lstitemrevt) << endl;

		ITEM_deepcopy_info_t structobj;
		structobj.component = lstitemrevt;
		structobj.action = 4;
		structobj.objName = &objname1[0];
		structobj.relation = &relation1[0];
		structobj.is_target_primary = 1;
		structobj.is_required = 1;
		structobj.copy_relations = 1;
		cout << " component = " << structobj.component<<endl;
		cout << " Action = "<<structobj.action << endl;
		cout << " objname = " << structobj.objName << endl;
		cout << "  Relation " << structobj.relation << endl;
		cout << " is_target_primary = " << structobj.is_target_primary << endl;
		cout << " is_required = " << structobj.is_required << endl;
		cout << "copy_relations " << structobj.copy_relations << endl;

		cout << ITEM_copy_rev_using(lstitemrevt, NULL, 7, &structobj, &newrevt, &count, &deepcopyobjs) << endl;   //  1 = done 
		cout << AOM_save_with_extensions(newrevt) << endl;
		cout << " Count = " << count << endl;
		for (int i = 1; i <= count; i++)
		{
			tag_t classtag;
			char *classname, *objectname;
			POM_class_of_instance(deepcopyobjs[i], &classtag);
			POM_name_of_class(classtag, &classname);
			AOM_ask_value_string(deepcopyobjs[i], "object_name", &objectname);
			cout << " i = " << i << " class name = " << classname << endl;
		}
		cout << " **************************** ";
		//cout << ITEM_perform_deepcopy(newrevt, operation, lstitemrevt, &count2, &deepcopiedobjects);
		/*cout << " Count2 = " << count2 << endl;
		for (int i = 1; i <= count; i++)
		{
			tag_t classtag;
			char *classname, *objectname;
			POM_class_of_instance(deepcopiedobjects[i], &classtag);
			POM_name_of_class(classtag, &classname);
			AOM_ask_value_string(deepcopiedobjects[i], "object_name", &objectname);
			cout << " i = " << i << " class name = " << classname;
		}*/
	}
	return(0);
}
