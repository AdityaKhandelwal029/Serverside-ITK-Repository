

//   ITEM_copy_rev()  + ITEM_perform_deepcopy() 



#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		tag_t itemt;
		tag_t lstitemrevt;
		tag_t newrevt;
		int count=0, count2=0;
		string objname = "NewCustomObject1";
		string relation1 = "IMAN_specification";
		string operation = "ITEM_revise_operation";
		char *operation1 = (char*)operation.c_str();

		tag_t *deepcopiedobjects;
		tag_t *deepcopyobjs;
		cout << ITEM_find_item("1412901", &itemt) << endl;
		cout << ITEM_ask_latest_rev(itemt, &lstitemrevt) << endl;

		ITEM_deepcopy_info_t structobj;
		structobj.component = lstitemrevt;
		structobj.action = 4;
		structobj.objName = (char*)objname.c_str();
		structobj.relation = (char*)relation1.c_str();

		structobj.is_target_primary = 1;
		structobj.is_required = 1;
		structobj.copy_relations = 1;


		cout << " lstitemrevt = " << lstitemrevt << " component = " << structobj.component << endl;
		cout << " Action value = " << structobj.action << endl;
		cout << " objname = " << structobj.objName << endl;
		cout << " Relation = " << structobj.relation << endl;
		cout << " is_target_priamry = " << structobj.is_target_primary << endl;
		cout << " is_required = " << structobj.is_required << endl;
		cout << " copy_relations = " << structobj.copy_relations << endl;

		//cout << ITEM_copy_rev_using(lstitemrevt, NULL, 1, &structobj, &newrevt, &count, &deepcopyobjs) << endl;

		cout << ITEM_copy_rev(lstitemrevt,NULL, &newrevt);

		cout << AOM_save_with_extensions(newrevt) << endl;
		cout << " Count of deepcopied objects = " << count << endl;

		//for (int i = 1; i <= count; i++)
		//{
		//	tag_t classtag;
		//	char *classname, *objectname;
		//	POM_class_of_instance(deepcopyobjs[i], &classtag);
		//	POM_name_of_class(classtag, &classname);
		//	AOM_ask_value_string(deepcopyobjs[i], "object_name", &objectname);
		//	cout << " i = " << i << " class name = " << classname << endl;
		//}
		cout << " **************************** "<<endl;

		int error_code;
		char *error_text;
		error_code= ITEM_perform_deepcopy(newrevt, operation1, lstitemrevt, &count2, &deepcopiedobjects);
		if (error_code > 0)
		{
			EMH_ask_error_text(error_code, &error_text);
			cout << " Error_code = " << error_code << endl;
			cout << " Error  =  " << error_text << endl;
		}
		else
		{
			cout << " Count2 = " << count2 << endl;
			for (int i = 1; i <= count2; i++)
			{
				tag_t classtag;
				char *classname, *objectname;
				POM_class_of_instance(deepcopiedobjects[i], &classtag);
				POM_name_of_class(classtag, &classname);
				AOM_ask_value_string(deepcopiedobjects[i], "object_name", &objectname);
				cout << " i = " << i << " class name = " << classname;
			}
		}

	}
	return(0);
}
