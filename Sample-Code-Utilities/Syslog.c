#include "Header1_SYSLOG.h"

//Assignment - To create ITEM with specified item_id and name.

int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* Item_name = NULL;
	char *ItemName = NULL;


	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tItem1 = NULLTAG;
	FILE* File = fopen("C:\\Users\\11824\\source\\repos\\ITK_syslog\\ITK_syslog.txt", "w+");
	if (File != NULL)
	{
		fprintf(File, "******************************");
		fprintf(File, "\n----------ITK-SYSLOG----------");
		fprintf(File, "\n******************************");
		cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
		cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
		cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
		Item_id = ITK_ask_cli_argument("-id=");
		Item_name = ITK_ask_cli_argument("-item_name=");
		Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
		printf("\nLogin Successfull");
		fprintf(File, "\nTC Login Successfull");
		Error_check(ITEM_create_item(Item_id, Item_name, "Item", "A", &tItem, &tRev)); //API to create item with name & id provided
		fprintf(File, "\nItem Create Successfull");
		Error_check(ITEM_save_item(tItem));//Saving created item
		fprintf(File, "\nItem Save Successfull");
		Error_check(ITEM_save_rev(tRev));// Saving created itemn revision
		fprintf(File, "\nItem Revision Save Successfully");
		Error_check(ITEM_find_item(Item_id, &tItem1));//Checking item created or not(Finding Item)
		fprintf(File, "\nItem Find SuccessFull");
		Error_check(ITEM_ask_name2(tItem1, &ItemName));//API to get name of item from tag of item
		fprintf(File, "Item Name is %s", ItemName);
		printf("\nItem name is %s", ItemName);
	}
	else
	{
		printf("File cant open");
	}

	if (ItemName)
	{
		MEM_free(ItemName);
	}
	return Status;
}