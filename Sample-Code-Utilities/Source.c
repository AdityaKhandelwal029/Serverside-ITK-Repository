#include "Header.h"


int ITK_user_main(int argc, char* argv[])
{
	char* cUserid = NULL;
	char* cPassword = NULL;
	char* cGroup = NULL;
	int Fail = ITK_ok;
	char* Error_msg = NULL;
	int Status = 0;

	cUserid = ITK_ask_cli_argument("-u=");
	cPassword = ITK_ask_cli_argument("-p=");
	cGroup = ITK_ask_cli_argument("-g=");

	tag_t tQuery2 = NULLTAG;
	int	No_of_Entry2;
	int i;
	int No_of_obj;
	tag_t* tObject = NULLTAG;
	tag_t tUser = NULLTAG;
	tag_t tGroup = NULLTAG;
	tag_t tUser1 = NULLTAG;
	tag_t tGroup1 = NULLTAG;
	char* Obj_name = NULL;
	char *User_name = NULL;
	char* Group_name = NULL;
	char* Obj_type = NULL;

	Error_check(ITK_init_module(cUserid, cPassword, cGroup));

	SA_find_user2("sam(sam)", &tUser);
	SA_find_group("Test", &tGroup);
	Error_check(QRY_find2("General...", &tQuery2));

	char   *cEntry_Names[2], *cValues[2];

	cEntry_Names[0] = "Type";
	cEntry_Names[1] = "Owning User";

	cValues[0] = "*";
	cValues[1] = "inz";

	Error_check(QRY_execute(tQuery2, 2, cEntry_Names, cValues, &No_of_obj, &tObject));
	for (i = 0; i < No_of_obj; i++)
	{


		Error_check(AOM_UIF_ask_value(tObject[i], "object_name", &Obj_name));
		printf("Object Name is %s", Obj_name);
		Error_check(AOM_UIF_ask_value(tObject[i], "object_type", &Obj_type));
		printf("Object type is %s", Obj_type);
		ITK_set_bypass(TRUE);

		//Error_check(POM_set_owners(No_of_obj,tObject[i],tUser,tGroup))
		if (Obj_type[i] == "Do Task")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type[i] == "Item")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type[i] == "Item Revision")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type[i] == "Text")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type[i] == "Inbox")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type == "Tasks To Track")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type == "Item Master")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type == "Item Revision Master")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type[i] == "My Worklist")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}
		if (Obj_type[i] == "Workflow Task")
		{
			Error_check(AOM_set_ownership(tObject[i], tUser, tGroup));
		}

		//Error_check(POM_ask_owner(tObject[i], tUser1, tGroup1));
		//Error_check(POM_ask_user_name(tUser1, &User_name));*/
	}

	return Fail;


}