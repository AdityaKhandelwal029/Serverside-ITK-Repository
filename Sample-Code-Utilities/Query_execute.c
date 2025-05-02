#include "Header.h"

//Assignment - To execute aquery to search the items.

//Main function start from here
int ITK_user_main(int argc, char*argv[])
{
	int Status = ITK_ok;
	int No_of_obj;
	int i;

	char* Error_msg = NULL;
	char* cUserID = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	char* Item_id = NULL;
	char* Item_name = NULL;
	char* User = NULL;
	char* Obj_name = NULL;
	char* Obj_type = NULL;

	tag_t tQuery = NULLTAG;
	tag_t Item_tag = NULLTAG;
	tag_t* tObject = NULLTAG;

	cUserID = ITK_ask_cli_argument("-u="); //Command line arguments for getting user name
	cPassword = ITK_ask_cli_argument("-p="); //Command line arguments for getting user password
	cGroup = ITK_ask_cli_argument("-g="); //Command line arguments for getting user group
	User = ITK_ask_cli_argument("-k="); //Command line arguments for getting user name
	Error_check(ITK_init_module(cUserID, cPassword, cGroup)); // API to start TC session
	printf("\nLogin Successfull");
	
	char *cEntry_Names[2], *cValues[2];
	cEntry_Names[0] = "Type";
	cEntry_Names[1] = "Owning User";
	cValues[0] = "*";
	cValues[1] = User;
	Error_check(QRY_find2("General...", &tQuery)); //API to find created query
	Error_check(QRY_execute(tQuery, 2, cEntry_Names, cValues, &No_of_obj, &tObject));
	for (i = 0; i < No_of_obj; i++)
	{
		Error_check(AOM_UIF_ask_value(tObject[i], "object_name", &Obj_name));
		printf("Object Name is %s", Obj_name);
		Error_check(AOM_UIF_ask_value(tObject[i], "object_type", &Obj_type));
		printf("Object type is %s", Obj_type);
	}
	if (tQuery)
	{
		MEM_free(tObject);
	}
	if (Obj_name)
	{
		MEM_free(Obj_name);
	}
	if (Obj_type)
	{
		MEM_free(Obj_type);
	}
	return Status;

}