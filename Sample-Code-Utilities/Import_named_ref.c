#include "Header.h"

//Assignment - To import the dataset named reference.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int Rev_Count;
	int Sec_obj_count;
	int i, j, k;
	int Ref;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* Rev_name = NULL;
	char* cName_reg = NULL;
	tag_t tItem = NULLTAG;
	tag_t* Rev = NULLTAG;
	tag_t Rel_type = NULLTAG;
	tag_t *Sec_obj = NULLTAG;
	tag_t* tName_ref = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Item_id = ITK_ask_cli_argument("-id="); //Command line arguments for getting Item ID
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	Error_check(ITEM_find_item(Item_id, &tItem)); //API to find Item
	Error_check(ITEM_list_all_revs(tItem, &Rev_Count, &Rev));// API to get all Item Revision
	for (i = 0; i < Rev_Count; i++) // Using for loop to fetch revisions one by one
	{
		Error_check(ITEM_ask_rev_name2(Rev[i], &Rev_name)); //API to get name of Item Revision
		Error_check(GRM_find_relation_type("IMAN_specification", &Rel_type));
		Error_check(GRM_list_secondary_objects_only(Rev[i], Rel_type, &Sec_obj_count, &Sec_obj));
		printf("No. of object found= %d", Sec_obj_count);
		for (j = 0; j < Sec_obj_count; j++)
		{
			AOM_refresh(Sec_obj[j], TRUE);
			Error_check(AE_import_named_ref(Sec_obj[j], "Text", "D:\\vs2017\\New Text Document.txt", "", SS_TEXT));
			AOM_save(Sec_obj[j]);
			AOM_refresh(Sec_obj[j], FALSE);
		}

	}

}