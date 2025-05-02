#include "Header.h"

//Assignment - To create a Folder with name given as command line input and add the folder to the User Home folder.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cFolder = NULL;

	tag_t tFolder = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tHome_folder = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	cFolder = ITK_ask_cli_argument("-f="); //Command line arguments for getting Folder name

	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	Error_check(FL_create2(cFolder, "Testing", &tFolder)); // API to create folder with given name
	printf("\nFolder created Successfull");
	Error_check(AOM_save_with_extensions(tFolder));// API to save folder
	Error_check(SA_find_user2("infodba", &tUser));//API to find user
	Error_check(SA_ask_user_home_folder(tUser, &tHome_folder));//API to find home folder of finded user
	Error_check(FL_insert(tHome_folder, tFolder, 999)); //Inserting created folder into home folder
	Error_check(AOM_save_with_extensions(tHome_folder));//Saving changes to home folder

	return Status;
}