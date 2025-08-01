// Objective : Client Code to Assign Project to user.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <tccore/item.h>
#include <form/form.h>
#include <tccore/aom.h>
#include <form/formtype.h>
#include <qry/qry.h>
#include <tccore/workspaceobject.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <ae/ae.h>
#include <property/prop.h>
#include <fclasses/tc_string.h>
#include <tc/folder.h>
#include <objio/objio.h>
#include <sa/site.h>
#include <ss/ss_const.h>
#include <unidefs.h>
#include <tccore/item.h>
#include <cfm/cfm.h>
#include <ps/ps_errors.h>
#include <fclasses/tc_date.h>
#include <ps/ps.h>
#include <epm/cr.h>
#include <epm/epm.h>
#include <tccore/uom.h>
#include <sa/user.h>
#include <tc/folder.h>
#include <tccore/project.h>
#include <tcinit/tcinit.h>

static int ReadFile( FILE *input);
static int AssignProjtoUser(int elmt1);

#define NUMBER  10000
char store[10000];

struct
 {  
	//DML detail..
	 
	char UserID[1000];
	char GroupName[1000];	
	char ProjectName[1000];
	char DesignGrp[1000];
	char RoleName[1000];

 } cadload[10000];

int ITK_user_main(int argc, char* argv[])
{
	FILE *input;
	int iStatus = 0; 
	char	*message = NULL;
	char    *loggedInUser       =   NULL;

	char *usr = ITK_ask_cli_argument("-u=");
	char *pwd = ITK_ask_cli_argument("-pf=");
	char *grp = ITK_ask_cli_argument("-g=");
	char *argstring = ITK_ask_cli_argument( "-i=");
	
	tag_t loggedInUserTag    =  NULLTAG;
	
	printf("argstring ---> %s\n",argstring);
	
	printf("\n**********************************  -u       = %s ",usr);
	printf("\n**********************************  -p or pf = %s ",pwd);
	printf("\n**********************************  -g       = %s ",grp);
	printf("\n**********************************  -i       = %s ",argstring);

	//iStatus = ITK_auto_login();
	
	iStatus = ITK_init_module(usr,pwd,grp);
	
	 if(iStatus == ITK_ok)
	 {
		printf("\n\n\t Login Successful\n"); 
		POM_get_user(&loggedInUser,&loggedInUserTag);
		printf("\n ********************************** Name of the logged in user = %s",loggedInUser);	
		if (argstring != 0)
		{
			if((input = fopen(argstring, "r")) == NULL)
			{
				printf("Cannot open input file %s\n",argstring);
				exit( EXIT_FAILURE );
			} 
			else
			{
				printf("\n Input File Opened..");

	 			// Calling Function to read input File..
				 
				iStatus = ReadFile(input);

				if(iStatus == 0)
				{
				    printf("\n Successful.."); 
				}
				else
				{
					printf("\n UnSuccessful");
				}
			 }
		 }				 		 
	 }	 
	 else
	 {
		printf("\n\n\t Login UnSuccessful\n"); 
		EMH_ask_error_text(iStatus,&message);
		printf("\n\t ERROR MESSAGE : [%s]",message);
	}
	//fclose(input);
	return 0;
}

static int ReadFile(FILE*  input)
{
	int ifail = 0;
	int i, c1, c2 = 0;
	int			elmt1				= 0;

	printf("\n ReadFile...\n"); 

	while(fgets(store, NUMBER, input) != NULL)
	{
		for (i = 0; i < strlen(store); i++)
		{
			 if ( store[i] == '\n' || store[i] == '\r' )
				 store[i] = '\0';
		}

		c1 = 0; 
		for(i=1; i<=50 && store[c1]!='\0'; i++)
		{ 
			c2=0;
			switch(i)
			{ 	
				case 1:					
					for(c2=0;store[c1]!='~'; c1++)
					{	
						cadload[elmt1].UserID[c2]=store[c1];
						c2++;
					}
					cadload[elmt1].UserID[c2]='\0';					
					//printf("\nUserID  :  %s",cadload[elmt1].UserID);fflush(stdout);
					break;				

				case 2:					
					for(c2=0;store[c1]!='~'; c1++)
					{
						cadload[elmt1].GroupName[c2]=store[c1];
						c2++;
					}
					cadload[elmt1].GroupName[c2]='\0';					
					//printf("\nGroupName  :  %s",cadload[elmt1].GroupName);fflush(stdout);
					break;
           
				case 3:					
					for(c2=0;store[c1]!='~'; c1++)
					{
						cadload[elmt1].ProjectName[c2]=store[c1];
						c2++;
					}
					cadload[elmt1].ProjectName[c2]='\0';					
					//printf("\nProjectName  :  %s",cadload[elmt1].ProjectName);fflush(stdout);
					break;

				case 4:					
					for(c2=0;store[c1]!='~'; c1++)
					{
						cadload[elmt1].DesignGrp[c2]=store[c1];
						c2++;
					}
					cadload[elmt1].DesignGrp[c2]='\0';					
					//printf("\nDesign Grp  :  %s",cadload[elmt1].DesignGrp);fflush(stdout);
					break;

				case 5:					
					for(c2=0;store[c1]!='~'; c1++)
					{
						cadload[elmt1].RoleName[c2]=store[c1];
						c2++;
					}
					cadload[elmt1].RoleName[c2]='\0';					
					//printf("\nRoleName  :  %s",cadload[elmt1].RoleName);fflush(stdout);
					break;

			} // End of Switch case.

            c1++;

        }  // End of For Loop.

		elmt1++;
	}
	ifail = AssignProjtoUser(elmt1);
	return ifail;
}


static int AssignProjtoUser(int totalelmt)
{
	
	int		ifail			= ITK_ok;
	int		i				= 0;
	int		number_found	= 0;

	char	*UserID			= NULL;
	char	*GroupName		= NULL;
	char	*ProjectName	= NULL;
	char	*UsrName		= NULL;
	char	*projectid		= NULL;
	char	*DesignGrp		= NULL;
	char	*RoleName		= NULL;
	char	*Role			= NULL;
	
	logical  is_privileged  = FALSE;
	logical  active			= FALSE;
	
	tag_t	user_tag		= NULLTAG;
	tag_t	group_tag		= NULLTAG;
	tag_t	project_tag		= NULLTAG;
	tag_t	role_tag 		= NULLTAG;
	tag_t	*user_tag_objs	= NULLTAG;
	tag_t	*role_tag_objs	= NULLTAG;
	
	tag_t	*members		= NULLTAG;

	for(i=0; i<totalelmt; i++)
	{
		//// Query USER
		UserID = (char	*)MEM_alloc(100);
		tc_strcpy(UserID,cadload[i].UserID);
		ifail = SA_find_user2(UserID, &user_tag);
		
		if(user_tag != NULLTAG)
		{
			AOM_ask_value_string(user_tag,"user_id",&UsrName);
			printf("\n UsrName > [%s]", UsrName);fflush(stdout);

			//AOM_ask_value_string(user_tag,"user_id",&UsrName);
			//printf("\n UsrName > [%s]", UsrName);fflush(stdout);
		}
		else
		{
			printf("\nUser %s NOT Available", UsrName);fflush(stdout);
			return ifail;
		}

		//// Query GROUP
		GroupName = (char	*)MEM_alloc(100);
		tc_strcpy(GroupName,cadload[i].GroupName);
		printf("\n GroupName > [%s]", GroupName); fflush(stdout);
		
		SA_find_group(GroupName, &group_tag);

		//// Query PROJECT
		ProjectName = (char	*)MEM_alloc(100);
		tc_strcpy(ProjectName,cadload[i].ProjectName);
		printf("\n ProjectName --***> %s\n", ProjectName); fflush(stdout);
		ifail = PROJ_find(ProjectName, &project_tag);
		
		if(project_tag != NULLTAG)
		{
			AOM_ask_value_string(project_tag,"project_id",&projectid);
			printf("\n projectid > [%s]", projectid);fflush(stdout);
		}
		else
		{
			printf("\nPROJECT %s is NOT LIVE",ProjectName); fflush(stdout);
			return ifail;
		}

		//// check PROJECT is ACTIVE
		PROJ_is_project_active	(project_tag,&active);	
		//printf("\n active ====>[%d]", active);fflush(stdout);

		
		DesignGrp = (char	*)MEM_alloc(100);
		tc_strcpy(DesignGrp,cadload[i].DesignGrp);
		//printf("\n DesignGrp ====>[%s]", DesignGrp);fflush(stdout);

		RoleName = (char	*)MEM_alloc(100);
		tc_strcpy(RoleName,cadload[i].RoleName);
		//printf("\n RoleName ====>[%s]", RoleName);fflush(stdout);

		if(tc_strcmp(RoleName,"D")==0)
			tc_strcpy(RoleName,"_Designers");
		if(tc_strcmp(RoleName,"M")==0)
			tc_strcpy(RoleName,"_Managers");
		//printf("\n RoleName ====>[%s]", RoleName);fflush(stdout);
		
		Role=MEM_alloc(500);
		tc_strcpy(Role,DesignGrp);
		tc_strcat(Role,RoleName);
		printf("\n Role > [%s]", Role);fflush(stdout);
	
		ifail = SA_find_role2(Role,&role_tag);
		if(role_tag != NULLTAG)
		{			
			//printf("\nRole Tag Found.. "); fflush(stdout);
		}
		else
		{
			printf("\n%s ROLE NOT FOUND",Role); fflush(stdout);
			return ifail;
		}

		number_found = 0;
		SA_find_groupmember_by_rolename(Role, GroupName, UsrName, &number_found, &members);
		printf("\n\n SA_find_groupmember_by_rolename:  number_found is %d ",number_found) ;fflush(stdout);
		
		if(number_found>0)
		{
			user_tag_objs = (tag_t*) MEM_alloc(sizeof(tag_t) * 1);
			user_tag_objs[0] = members[0];  //user_tag;
			
			//// USER added in PROJECT
			PROJ_add_members(project_tag, 1, user_tag_objs);
			printf("\nUSER Added in Project. "); fflush(stdout);

			//// set USER PRIVILEDGES
			PROJ_add_author_members(project_tag,1, user_tag_objs);
			printf("\nPRIVILEDGE Granted to USER \n"); fflush(stdout);
		}
		else
		{
			printf("\n User %s NOT Found in Role %s",UsrName,Role); fflush(stdout);
			return ifail;
		}			
	}
	
	MEM_free(members);
	return ifail;
}