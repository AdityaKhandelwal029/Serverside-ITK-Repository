#include "Header.h"

//Assignment - To search the Item.

int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* Item_name = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Item_id = ITK_ask_cli_argument("-id="); //Command line arguments for getting Item ID
	
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	Error_check(ITEM_find_item(Item_id, &tItem)); //API to find Item
	Error_check(ITEM_ask_name2(tItem, &Item_name));//API to get name of item
	printf("\nItem find successfull Successfull");
	if (Item_name)
	{
		MEM_free(Item_name);
	}
	return Status;
}