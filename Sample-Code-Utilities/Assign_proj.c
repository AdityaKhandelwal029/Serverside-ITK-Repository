#include "Header.h"

//Assignment - To assign the project to the members.


//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* cUser=NULL;
	char* Proj_name = NULL;
	char* User_name=NULL;

	tag_t tProj = NULLTAG;
	tag_t tUser = NULLTAG;
	



	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	cUser = ITK_ask_cli_argument("-k="); //Command line arguments for getting user name
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	
	Error_check(SA_find_user2(cUser, &tUser));
	Error_check(SA_ask_user_person_name2(tUser, &User_name));
	printf("\nUser name is %s", User_name);
	PROJ_find("13579", &tProj);
	PROJ_ask_name2(tProj, &Proj_name);
	printf("\nProject name is %s", Proj_name);
	int Proj_memb_count;
	int Admin_count;
	int Prvlg_user_count;
	tag_t* tMembers = NULLTAG;
	tag_t* tAdmin = NULLTAG;
	tag_t* tPrivileged_user = NULLTAG;
	PROJ_ask_team(tProj, &Proj_memb_count, &tMembers, &Admin_count, &tAdmin, &Prvlg_user_count, &tPrivileged_user);
	printf("\nMember count = %d", Proj_memb_count);
	printf("\nAdmin count = %d", Admin_count);
	printf("\nPrivileged count = %d", Prvlg_user_count);
	Error_check(PROJ_add_members(tProj, 1, &tUser));
	Error_check(PROJ_add_author_members(tProj, 1, &tUser));
	PROJ_ask_team(tProj, &Proj_memb_count, &tMembers, &Admin_count, &tAdmin, &Prvlg_user_count, &tPrivileged_user);
	printf("\nMember count = %d", Proj_memb_count);
	printf("\nAdmin count = %d", Admin_count);
	printf("\nPrivileged count = %d", Prvlg_user_count);
	return Status;
}