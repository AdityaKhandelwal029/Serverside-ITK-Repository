#include "Header.h"

//Assignment -To generate a report of secondary objects attached to the IR.no. 

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int Sec_obj_count;
	int i, j;
	int Rev_Count;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* Rev_name = NULL;
	char* Obj_type = NULL;
	char* Obj_name = NULL;
	
	tag_t* Rev = NULLTAG;
	tag_t Rel_type = NULLTAG;
	tag_t* Sec_obj = NULLTAG;
	tag_t tItem = NULLTAG;
	
	FILE* File;
	
	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Item_id = ITK_ask_cli_argument("-id="); //Command line arguments for getting ItemID
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	
	File = fopen("Sec_obj_report.csv", "w+");// Opening file to create report
	fprintf(File, "\nRevision name,Dataset Type, Dataset Name");
	Error_check(ITEM_find_item(Item_id, &tItem));//API to find Item
	Error_check(ITEM_list_all_revs(tItem, &Rev_Count, &Rev));//API to get revision of item
	for (i = 0; i < Rev_Count; i++)
	{
		Error_check(ITEM_ask_rev_name2(Rev[i], &Rev_name));// API to get name of item revision
		Error_check(GRM_find_relation_type("*", &Rel_type));//API to find relation 
		Error_check(GRM_list_secondary_objects_only(Rev[i], Rel_type, &Sec_obj_count, &Sec_obj));//APi to get secondary object attach to IR
		printf("\nSecondary object found %d", Sec_obj_count);
		for (j = 0; j < Sec_obj_count; j++)
		{
			Error_check(AOM_UIF_ask_value(Sec_obj[j], "object_type", &Obj_type)); //Getting value of property of founded secondary object
			printf("\nObject type= %s", Obj_type);
			Error_check(AOM_UIF_ask_value(Sec_obj[j], "object_name", &Obj_name)); //Getting value of property of founded secondary object
			printf("\nObj_name= %s", Obj_name);
			fprintf(File, "\n%s,%s,%s", Rev_name, Obj_type, Obj_name);//Printing values in CSV file
		}
	}
	if (Rev)
	{
		MEM_free(Rev);
	}
	if (Sec_obj)
	{
		MEM_free(Sec_obj);
	}
	if (Obj_type)
	{
		MEM_free(Obj_type);
	}
	if (Obj_name)
	{
		MEM_free(Obj_name);
	}
	return Status;
}
