#include"Header.h"
#include<boost/filesystem.hpp>
#include<boost/shared_array.hpp>
#include "boost/shared_ptr.hpp"
#include "boost/shared_array.hpp"
#include "boost/interprocess/smart_ptr/unique_ptr.hpp"
#include<boost/lexical_cast.hpp>
//template<typename T> struct SmartPointerDeallocator
//{
//	void operator()(T* p)
//	{
//		MEM_free(p);
//	}
//};
//template<typename T> boost::shared_array<T> make_shared_tcarray(int size)
//{
//	if (size <= 0)
//	{
//		/*throw std::runtime_error(
//			"Illegal size of " + boost::lexical_cast<std::string>(size) + " passed for memory allocation of shared smart pointer array");*/
//		cout << " Size < 0 " << endl;
//	}
//	size = size * sizeof(T);
//	return (boost::shared_array<T>((T*)MEM_alloc(size), SmartPointerDeallocator<T>()));
//}

int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int countofobject, countofobjects;
		tag_t itemt;
		tag_t lstitemrevt;
		tag_t newrevt;
		tag_t *secondaryobjlist;
		tag_t classtag;
		tag_t relationtag;
		char *classname;
		string classtype;
		tag_t *deepcopiedobjects;
		char operation[50] = "ITEM_revise_operation";
		char *op = &operation[0];

		cout << ITEM_find_item("1412901", &itemt) << endl;
		cout << ITEM_ask_latest_rev(itemt, &lstitemrevt) << endl;
		cout << GRM_find_relation_type("IMAN_specification", &relationtag) << endl;
		cout << GRM_list_secondary_objects_only(lstitemrevt, relationtag, &countofobject, &secondaryobjlist) << endl;
		cout << " Secondary objects found = " << countofobject << endl;
		for (int i = 0; i < countofobject; i++)
		{
			POM_class_of_instance(secondaryobjlist[i], &classtag);
			POM_name_of_class(classtag, &classname);
			classtype = classname;
			if (classtype == "Dataset")
			{
				cout << " i = 1 " << " class name = " << classname << endl;
			}
		}
		cout << ITEM_copy_rev(lstitemrevt, NULL, &newrevt) << endl;

		/*char **copyoptions = NULL;
		char **optionname = NULL;
		char **relationtype = NULL;

		copyoptions = (char **)MEM_alloc(1 * sizeof(char *));
		optionname = (char **)MEM_alloc(1 * sizeof(char *));
		relationtype = (char **)MEM_alloc(1 * sizeof(char *));

		copyoptions[0] = (char *)MEM_alloc(20);
		strcpy(copyoptions[0], "ITEM_no_copy");

		optionname[0] = (char *)MEM_alloc(20);
		strcpy(optionname[0], "MyItem1");

		relationtype[0] = (char *)MEM_alloc(20);
		strcpy(relationtype[0], "IMAN_specification");*/

		//vector<string> copyoptions;
		//copyoptions.push_back("ITEM_no_copy");
		//cout << " copyoptions.size() = " << copyoptions.size() << endl;

		//vector<string> optionname;
		//optionname.push_back("MyCustomItem1");
		//cout << " optionname.size() = " << optionname.size() << endl;

		//vector<string> relationtype;
		//relationtype.push_back("IMAN_specification");
		//cout << " relationtype.size() = " << relationtype.size() << endl;


		//boost::shared_array<const char*> p_attr_copyoptions1 = make_shared_tcarray<const char*>(copyoptions.size());
		//boost::shared_array<const char*> p_attr_optionname1 = make_shared_tcarray<const char*>(optionname.size());
		//boost::shared_array<const char*> p_attr_relationtype1 = make_shared_tcarray<const char*>(relationtype.size());

		//for (int i = 0; i < copyoptions.size(); i++) {
		//	p_attr_copyoptions1[i] = copyoptions[i].c_str();
		//}

		//for (int j = 0; j < optionname.size(); j++) {
		//	p_attr_optionname1[j] = optionname[j].c_str();
		//}
		//for (int k = 0; k < relationtype.size(); k++) {
		//	p_attr_relationtype1[k] = relationtype[k].c_str();
		//}
		 
		cout << " Point 1 " << endl;
		cout << " op = " << op << endl;
		try {

			const char *copyoption[1] = { "ITEM_no_copy" }, *optionname[1] = { "MyCustomname" };
			const char *relationtype[1] = { "IMAN_specification" };
			cout << " copyoption = " << *copyoption;
			cout << " optionname = " << *optionname << endl;
			cout << " relationtype = " << *relationtype << endl;

			cout << ITEM_perform_deepcopy_using(newrevt, op, lstitemrevt, 1, secondaryobjlist, copyoption, optionname, relationtype, &countofobjects, &deepcopiedobjects);

			cout << " Total objects deepcopied = " << countofobjects << endl;
		}
		catch (...)
		{
			cout << " Exception occured " << endl;
		}
	}
	return(0);
}
