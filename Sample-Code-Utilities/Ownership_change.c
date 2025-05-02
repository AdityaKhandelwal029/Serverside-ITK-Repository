#include "Header.h"

//Assignment - To change the ownership.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int Rev_count;
	int i, j;
	int Sec_obj_count;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;

	tag_t tItem = NULLTAG;
	tag_t* Rev = NULLTAG;
	tag_t Rel_type = NULLTAG;
	tag_t* Sec_obj = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tGroup = NULLTAG;
	
	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session


	Error_check(SA_find_user2("inz", &tUser)); //Finding new user
	Error_check(SA_find_group("Test", &tGroup)); //Finding new user group
	ITK_set_bypass(TRUE);
	Error_check(ITEM_find_item("000048", &tItem));//Finding Item
	printf("\n Item found successfull");
	Error_check(AOM_set_ownership(tItem, tUser, tGroup)); //Changing ownership of item
	Error_check(ITEM_list_all_revs(tItem, &Rev_count, &Rev));//Finding revisions of item
	printf("\nTotal revfound is %d", Rev_count);
		for (i = 0; i < Rev_count; i++)
		{
			Error_check(AOM_set_ownership(Rev[i], tUser, tGroup));//Changing ownership of item
			Error_check(GRM_find_relation_type("IMAN_specification", &Rel_type));//Finding relation type
			Error_check(GRM_list_secondary_objects_only(Rev[i], Rel_type, &Sec_obj_count, &Sec_obj));//Using relation type getting secondary object attached to IR
			printf("\nSec Obj Count is %d", Sec_obj_count);
			for (j = 0; j < Sec_obj_count; j++)
			{
				Error_check(AOM_set_ownership(Sec_obj[j], tUser, tGroup));//Changing ownership of secondary objects of IR
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
	return Status;
}