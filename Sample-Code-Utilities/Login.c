#include "Header.h"
//ITK utility to login into TC & printing values provided as arguments on console

//Assignment no. 1 & 2

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
		
	;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	
	printf("Command line arguments-\n1) %s\n2) %s\n3) %s", cUserID, cPassword, cGroup);
	

	return Status;
}