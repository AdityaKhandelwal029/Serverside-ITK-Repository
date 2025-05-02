#include "Header.h"

//Assignment - To print complete bill of material structure on console.

int BOM_ASK_CHILD(int Child_count, tag_t*tBOM_child)
{
	int i;
	int Sub_child_count;
	char* Item_id = NULL;
	char* Item_name = NULL;
	
	tag_t* Sub_child = NULLTAG;
	for (i = 0; i < Child_count; i++)
	{
		AOM_UIF_ask_value(tBOM_child[i], "bl_item_item_id", &Item_id);
		AOM_UIF_ask_value(tBOM_child[i], "bl_item_object_name", &Item_name);
		printf("\n%s  %s", Item_id, Item_name);
	    BOM_line_ask_all_child_lines(tBOM_child[i], &Sub_child_count, &Sub_child);
		if (Sub_child_count > 0)
		{
			BOM_ASK_CHILD(Sub_child_count, Sub_child);
		}
	}
	if (Item_id)
	{
		MEM_free(Item_id);
	}
	if (Item_name)
	{
		MEM_free(Item_name);
	}
	if (Sub_child)
	{
		MEM_free(Sub_child);
	}
}
//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int Child_count;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* cItem_id = NULL;
	char* cItem_name = NULL;

	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tBOM_line = NULLTAG;
	tag_t *tBOM_child = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Item_id = ITK_ask_cli_argument("-id="); //Command line arguments for getting Item ID
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	Error_check(ITEM_find_item(Item_id, &tItem));//API to find item from id
	Error_check(ITEM_ask_latest_rev(tItem, &tRev));//API to find Item's latest working revision
	Error_check(BOM_create_window(&tWindow));//API to create BOM window
	Error_check(BOM_save_window(tWindow));//API to save BOM window
	Error_check(BOM_set_window_top_line(tWindow, tItem, tRev, NULLTAG, &tBOM_line));//API to set top BOM line of window
	Error_check(AOM_UIF_ask_value(tBOM_line, "bl_item_item_id", &cItem_id));//API to get display value of property
	Error_check(AOM_UIF_ask_value(tBOM_line, "bl_item_object_name", &cItem_name));//API to get display value of property
	printf("\nItem_ID  Item_name");
	printf("\n%s  %s", cItem_id, cItem_name);
	Error_check(BOM_line_ask_all_child_lines(tBOM_line, &Child_count, &tBOM_child));//API to get child lines of BOM top line
	if(Child_count>0)
	{
		BOM_ASK_CHILD(Child_count, tBOM_child);//Calling function to get child lines of Bomline 
	}
	if (cItem_id)
	{
		MEM_free(cItem_id);
	}
	if (cItem_name)
	{
		MEM_free(cItem_name);
	}
	if (tBOM_child)
	{
		MEM_free(tBOM_child);
	}
	return Status;
}