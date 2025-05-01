#include"POM_enq.h"
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int rows;
	int cols;
	void*** results; 
	char* Error_msg = NULL;
	const char * select_attr_list[] = { "puid" };
	const char * itemid[] = { "000473" };
	const char * itemtype[] = { "" };



	ITK_init_module("infodba", "infodba", "dba");
    POM_enquiry_create("Get_itemid");

	POM_enquiry_add_select_attrs("Get_itemid", "A5Mercedes", 1, select_attr_list);

	/*POM_enquiry_set_attr_expr("Get_itemid", "ExprId1", "Item", "item_id", POM_enquiry_equal, "valueId1");
	POM_enquiry_set_string_value("Get_itemid", "valueId1", 1, itemid, POM_enquiry_bind_value);*/

	/*POM_enquiry_set_attr_expr("Get_itemid", "ExprId2", "Item", "object_type", POM_enquiry_equal, "valueId2");
	POM_enquiry_set_string_value("Get_itemid", "valueId2", 1, itemtype, POM_enquiry_bind_value);*/



	POM_enquiry_set_expr("Get_itemid", "ExprId3", "ExprId1", POM_enquiry_and, "ExprId2");



	POM_enquiry_execute("Get_itemid", &rows, &cols, &results);



	if (rows > 0)
	{
		int reportrow = 0;
		tag_t *objs = NULLTAG;
		char* ObjName = NULL;
		(objs) = (tag_t *)MEM_alloc(rows * sizeof(tag_t));
		for (int i = 0; i < rows; i++)
		{
			(objs)[i] = (*(tag_t *)results[i][0]);
			AOM_ask_value_string((objs)[i], "object_name", &ObjName);
			cout << "Object Name is " << ObjName << endl;
		}
	}
	POM_enquiry_delete("Get_itemid");
	return Status;
}