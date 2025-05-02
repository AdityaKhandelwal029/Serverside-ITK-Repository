#include "Header.h"

//Assignment - To create a Folder with name given as command line input and add the folder to the User Home folder.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int nHit;
	int i,j;
	int Count;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cFolder = NULL;
	char* Child_name = NULL;

	tag_t tFolder = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tHome_folder = NULLTAG;
	tag_t* tList = NULLTAG;
	tag_t* Child_list = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	cFolder = ITK_ask_cli_argument("-f="); //Command line arguments for getting Folder name

	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	WSO_search_criteria_t criteria; 
	WSOM_clear_search_criteria(&criteria);
	strcpy(criteria.class_name, "Folder");
	strcpy(criteria.name, cFolder);
	WSOM_search(criteria, &nHit, &tList);//API to find out specific folder
	for (i = 0; i < nHit; i++)
	{
		FL_ask_references(tList[i], FL_fsc_as_ordered, &Count, &Child_list);//APLI to get stuff inside folder
		printf("\nChild found= %d", Count);
		for (j = 0; j < Count; j++)
		{
			AOM_UIF_ask_value(Child_list[j], "object_name", &Child_name);//Using this api to get name of object inside folder
			printf("\n%s", Child_name);
		}
	}
	if(tList)
	{
		MEM_free(tList);
	}
	if (Child_list)
	{
		MEM_free(Child_list);
	}
	if (Child_name)
	{
		MEM_free(Child_name);
	}
	return Status;
}