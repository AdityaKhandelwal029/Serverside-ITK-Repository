#include "Header.h"

//Assignment - To generate report of all properties of IR (Take Item ID from console)

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int Rev_Count;
	int i, j;
	int Prop_count;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* Prop_value = NULL;
	char* Rev_name = NULL;
	char**Prop_name = NULL;
		
	tag_t tItem = NULLTAG;
	tag_t* Rev = NULLTAG;
	FILE* File;
	   
	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	Item_id = ITK_ask_cli_argument("-id="); //Command line arguments for getting Item ID
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	
	File = fopen("IR_Prop.csv", "w+"); //Creating CSV file for report
	if (File != NULL)
	{
		fprintf(File, "\nRevision name,Property Name, Propery Value ");
		Error_check(ITEM_find_item(Item_id, &tItem)); //API to find Item
		Error_check(ITEM_list_all_revs(tItem, &Rev_Count, &Rev));// API to get all Item Revision
		for (i = 0; i < Rev_Count; i++) // Using for loop to fetch revisions one by one
		{
			Error_check(ITEM_ask_rev_name2(Rev[i], &Rev_name)); //API to get name of Item Revision
			Error_check(AOM_ask_prop_names(Rev[i], &Prop_count, &Prop_name));// API to get all properties of Item Revision
			for (j = 0; j < Prop_count; j++)
			{
				Error_check(AOM_UIF_ask_value(Rev[i], Prop_name[j], &Prop_value)); //API to get value of property
				printf("\n%s= %s", Prop_name[j], Prop_value);
				fprintf(File, "\n%s,%s,%s", Rev_name, Prop_name[j], Prop_value); //printing values is csv file
			}
		}
	}
	else
	{
		printf("\nFile is not opening");
	}
	if (Rev)
	{
		MEM_free(Rev);
	}
	if (Rev_name)
	{
		MEM_free(Rev_name);
	}
	if (Prop_name)
	{
		MEM_free(Prop_name);
	}
	if (Prop_value)
	{
		MEM_free(Prop_value);
	}
}
