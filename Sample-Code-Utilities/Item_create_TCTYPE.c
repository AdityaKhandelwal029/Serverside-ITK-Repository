#include "Header.h"

//Assignment - To fill the property of ItemMasterForm at the creation of Item.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* form_name[1] = { "123456/A" };
	char* user_data_1[1] = { "User_data_1_value" };
	char* object_desc[1] = { "Description related to IR" };
	char* item_id[1] = { "123456" };
	char* object_name[1] = { "Use_case_1" };


	tag_t IM = NULLTAG;
	tag_t IM_crateinput = NULLTAG;
	tag_t IM_form = NULLTAG;
	tag_t IR_tag = NULLTAG;
	tag_t Item = NULLTAG;
	tag_t Item_type = NULLTAG;
	tag_t Rev_createinput = NULLTAG;
	tag_t Item_createinput = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group

	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	ITK_set_bypass(TRUE);
	Error_check(TCTYPE_find_type("A2TestMaster", "Form", &IM));
	Error_check(TCTYPE_construct_create_input(IM, &IM_crateinput));
	Error_check(TCTYPE_set_create_display_value(IM_crateinput, "object_name", 1, form_name));
	Error_check(TCTYPE_set_create_display_value(IM_crateinput, "user_data_1", 1, user_data_1));
	Error_check(TCTYPE_create_object(IM_crateinput, &IM_form));
	AOM_save_with_extensions(IM_form);
	Error_check(TCTYPE_ask_type("A2Test",&Item_type));
	Error_check(TCTYPE_construct_create_input(Item_type, &Item_createinput));
	Error_check(TCTYPE_set_create_display_value(Item_createinput, "item_id", 1, item_id));
	Error_check(TCTYPE_set_create_display_value(Item_createinput, "object_name", 1, object_name));
	Error_check(AOM_set_value_tag(Item_createinput,"IMAN_master_form", &IM_form));
	Error_check(TCTYPE_create_object(Item_createinput, &Item));
	Error_check(ITEM_save_item(Item));
	return Status;
}


