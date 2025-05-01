#include"Header.h"



int pro7()
{
	int status = 0;
	void ***report;
	int row, p_rows;
	int column, p_columns;





	char* select_attr_list[] = { "object_name","object_type" };





	status = POM_enquiry_create("findItem");
	status = POM_enquiry_add_select_attrs("findItem", "Item", 2, select_attr_list);





	status = POM_enquiry_set_attr_expr("findItem", "expre1", "Item", "object_name", POM_enquiry_is_not_null, "");
	status = POM_enquiry_set_where_expr("findItem", "expre1");
	status = POM_enquiry_execute("findItem", &p_rows, &p_columns, &report);





	for (row = 0; row < p_rows; row++)
	{
		for (column = 0; column < p_columns; column++)
		{
			printf("%s\t", report[row][column]);
		}
		printf("\n");
	}
	return 0;
}