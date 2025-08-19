/****************************************************************************************************
*  File            :   tm_Mail_Approval.c
*  Created By      :   Anvesh Bujule
*  Created On      :   18 Jul 2025
*  Purpose         :   Mail Approval
*
*  
*****************************************************************************************************/


#define _CRT_SECURE_NO_DEPRECATE
#define NUM_ENTRIES 1
#define TE_MAXLINELEN  128
#include <epm/epm.h>
#include <ae/dataset_msg.h>
//#include <tccore/iman_msg.h>
#include <ps/ps.h>
#include <ps/ps_errors.h>
#include <time.h>
#include <ai/sample_err.h>
//#include <tc/tc.h>
#include <tccore/grm_msg.h>
#include <tccore/workspaceobject.h>
//#include <bom/bom.h>
#include <ae/dataset.h>
#include <ps/ps_errors.h>
#include <sa/sa.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fclasses/tc_string.h>
#include <tccore/item.h>
#include <tccore/item_errors.h>
#include <sa/tcfile.h>
//#include <epm/releasestatus.h>
#include <tcinit/tcinit.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <res/reservation.h>
#include <tccore/aom.h>
#include <tccore/custom.h>
#include <tc/emh.h>
#include <ict/ict_userservice.h>
//#include <tc/iman.h>
//#include <tccore/imantype.h>
//#include <sa/imanfile.h>
#include <lov/lov.h>
#include <lov/lov_msg.h>
//#include <itk/mem.h>
#include <ss/ss_errors.h>
#include <sa/user.h>
#include <sa/tcfile.h>
#include <sa/tcfile_cache.h>
#include <tccore/grm.h>
#include <tc/folder.h>//new
#include <string.h>
#include <fclasses/tc_date.h>
#include <ae/datasettype.h>
#include <cfm/cfm.h>
#include <tc/preferences.h>
//#include "participant.h"
//#include <epm/cr_effectivity.h>
#include <user_exits/epm_toolkit_utils.h>
#include <pie/pie.h>
#include <time.h>
#define ITK_err 919002
#define ITK_errStore1 (EMH_USER_error_base + 5)
#define ITK_Prjerr (EMH_USER_error_base + 8)
#define PLM_error6 (EMH_USER_error_base+27)
/*--------------------------------------------------*/

#define Debug TRUE
#define ITK_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
#define CALLAPI(expr)ITKCALL(ifail = expr); if(ifail != ITK_ok) { PrintErrorStack(); return ifail;}

int DFQApplCntflag=0;
int falseflagg=0;
int DFQpdfCount=0;
int DFQexcelsCount=0;
int MeetexpectationCnt=0;

static int report_error( char *file, int line, char *function, int return_code)
{
	if (return_code != ITK_ok)
	{
		int index = 0;
		int n_ifails = 0;
		const int* severities = 0;
		const int* ifails = 0;
		const char** texts = NULL;

		EMH_ask_errors( &n_ifails, &severities, &ifails, &texts);
		printf("%3d error(s)\n", n_ifails);
		for( index=0; index<n_ifails; index++)
		{
			printf("ERROR: %d\tERROR MSG: %s\n", ifails[index], texts[index]);
			TC_write_syslog("ERROR: %d\tERROR MSG: %s\n", ifails[index], texts[index]);
			printf ("FUNCTION: %s\nFILE: %s LINE: %d\n\n", function, file, line);
			TC_write_syslog("FUNCTION: %s\nFILE: %s LINE: %d\n", function, file, line);
		}
		EMH_clear_errors();

	}
	return return_code;
}
#define CHECK_FAIL if (ifail != 0) { printf ("line %d (ifail %d)\n", __LINE__, ifail); exit (0);}

void removeNewLineChar(char *ptr)
{
    while((ptr != NULL) && (*ptr != '\n'))
    {
        ++ptr;
    }
    *ptr = '\0';
}


#define WRONG_USAGE 100001
char* subString (char* mainStringf ,int fromCharf,int toCharf);
//
void dousage()
{
   printf("\nUSAGE: t5ListEmptyVisPdfDesRev -fromDate= -toDate= -sType= -outFile= \n");
   printf("\E.g: t5ListEmptyVisPdfDesRev -fromDate=01-May-2018 -toDate=02-May-2018 -sType=CMI2Part -outFile=out.txt \n");
   return;
}

void lower_string(char s[])
{
	int c = 0;

	while (s[c] != '\0')
	{
		if (s[c] >= 'A' && s[c] <= 'Z')
		{
			s[c] = s[c] + 32;
		}
		c++;
	}
}

char* subString (char* mainStringf ,int fromCharf,int toCharf)
{
	int i;
	char *retStringf;
	retStringf = (char*) malloc(3);
	for(i=0; i < toCharf; i++ )
    *(retStringf+i) = *(mainStringf+i+fromCharf);
	*(retStringf+i) = '\0';
	return retStringf;
}

static int PrintErrorStack( void )
/*
*
* PURPOSE : Function to dump the ITK error stack
*
* RETURN : causes program termination. If you made it here
*          you're not coming back modified for cust.c to not call exit()
*          but to just print the error stack
*
* NOTES : This version will always return ITK_ok, which is quite strange
*           actually. But if the error reporting was "OK" then that makes
*           sense
*
*/
{
    int iNumErrs = 0;
    const int *pSevLst = NULL;
    const int *pErrCdeLst = NULL;
    const char **pMsgLst = NULL;
    register int i = 0;

    EMH_ask_errors( &iNumErrs, &pSevLst, &pErrCdeLst, &pMsgLst );
    fprintf( stderr, "Error(PrintErrorStack): \n");
    for ( i = 0; i < iNumErrs; i++ )
    {
        fprintf( stderr, "\t%6d: %s\n", pErrCdeLst[i], pMsgLst[i] );
    }
    return ITK_ok;
}

int Webservice_Mail_approve( EPM_action_message_t msg)
{
 
   
   tag_t* attachments            = NULLTAG;
   tag_t* Webservice_Particpant  = NULLTAG;
   tag_t* subtask                = NULLTAG;
   tag_t* rps                    = NULLTAG;
   tag_t* Memberstag             = NULLTAG;
   tag_t *Web_Particpant         = NULLTAG;
   tag_t *Mail_object            = NULLTAG;


 
   tag_t rootTask                = NULLTAG;
   tag_t tasktag                 = NULLTAG;
   tag_t Web_Particpant_rel_type = NULLTAG;
   tag_t attachments1            = NULLTAG;
   tag_t Usertag            = NULLTAG;
   tag_t persontag            = NULLTAG;
   tag_t Mailqry_tg            = NULLTAG;

   char *CurrentTask        = NULL;
   char *Web_ParticpantNamedup        = NULL;
   char *businessUnit        = NULL;
   char *firsttok        = NULL;
   char *filename        = NULL;
   char *Secondtok        = NULL;
   char *taskStatus        = NULL;
   char *parent_name        = NULL;
   char *objecttype         = NULL;
   char *taskname           = NULL;
   char *objectname         = NULL;
   char *Web_ParticpantName = NULL;
   char *WebserviceShellpath = NULL;
   char *childParentTaskNam = NULL;
   char *workFlowName = NULL;
   char *personmail = NULL;
   char * command    = NULL;
   // char * doublete    = {' " '};
   char *doublete = NULL;
   command = (char *)MEM_alloc(500);
   doublete = (char *)MEM_alloc(500);
   firsttok = (char *)MEM_alloc(50);
   Secondtok = (char *)MEM_alloc(50);



   int participantcount = 0;
   int tskcount         = 0;
   int noAttachment     = 0;
   
   int PrtCount   = 0;
   int Web_count  = 0;
   int rp_count   = 0;
   int Mailobj_count   = 0;

   int i = 0;
   int l = 0;
   int p = 0;
   int j = 0;
   int h = 0;
   int BreakFlag = 0;
  
   tc_strcpy(doublete,"");
   tc_strcat(doublete,"\"");
  // tc_strcat(doublete,"\"");
  

    printf("\n after cat555 %s\n",doublete);fflush(stdout);
    printf("\n Inside the Webservice Mail Approved Function ...\n");fflush(stdout);

    ITK_CALL(EPM_ask_root_task(msg.task,&rootTask));
    
		     
    ITK_CALL(AOM_ask_value_string(msg.task,"object_name",&CurrentTask));
    printf("\n Webservice- CurrentTask: %s \n",CurrentTask);fflush(stdout);

	ITK_CALL(AOM_ask_value_string(msg.task,"parent_name",&workFlowName));
    printf("\n Webservice- workFlowName: %s \n",workFlowName);fflush(stdout);
		     
    ITK_CALL(AOM_ask_value_string(rootTask,"object_name",&parent_name));
    printf("\n Webservice- parent Name: %s \n",parent_name);fflush(stdout);


	//ITK_CALL(EPM_ask_root_task(job_Tag,&rootTask));               					    
    ITK_CALL(EPM_ask_sub_tasks(rootTask,&tskcount,&subtask));
	printf("\n Webservice- tskcount : %d \n",tskcount);fflush(stdout);
	if(QRY_find2("Control Objects...", &Mailqry_tg));

	char *Mail_entry1[2] = {"SYSCD","SUBSYSCD"};
	char **Mail_RelVal = (char **) MEM_alloc(50 * sizeof(char *));
	Mail_RelVal[0] = "MailApprovalBusinessUnit";
	Mail_RelVal[1] = "ON";

	ITK_CALL(QRY_execute(Mailqry_tg, 2, Mail_entry1, Mail_RelVal, &Mailobj_count, &Mail_object));

     printf("\n Webservice- Mailobj_count : %d \n",Mailobj_count);fflush(stdout);

	if(Mailobj_count>0)
	{
       ITK_CALL(AOM_ask_value_string(Mail_object[p],"t5_Userinfo1",&businessUnit));
	    printf("\n Webservice- t5_Userinfo1 :%s \n",businessUnit);fflush(stdout);
     

	for(i=0; i<tskcount;i++)
	{


		tasktag=subtask[i];

	 
	   ITK_CALL(AOM_ask_value_string(tasktag,"fnd0Status",&taskStatus));
     
       printf("\n Webservice- taskStatus :%s \n",taskStatus);fflush(stdout);
       printf("\n Webservice- BreakFlag :%d \n",BreakFlag);fflush(stdout);
	   if(BreakFlag==0)
	   {

	   if(tc_strcmp(taskStatus,"Pending")==0)
		{

            ITK_CALL(AOM_ask_value_string(tasktag,"object_name",&taskname));
			printf("\n Webservice- taskname :%s \n",taskname);fflush(stdout);
		     
		    ITK_CALL(EPM_ask_attachments(rootTask,EPM_target_attachment,&noAttachment,&attachments));
		    printf("\n Webservice- Target Attachment:%d \n",noAttachment);fflush(stdout);

			ITK_CALL(AOM_ask_value_tags(tasktag,"fnd0FirstTargetParticipants",&Web_count,&Web_Particpant));
			printf("\n Webservice-  Web_count:%d \n",Web_count);fflush(stdout);




			for(h=0;h<Web_count;h++)
			{
			
			   ITK_CALL(AOM_UIF_ask_value(Web_Particpant[h],"fnd0AssigneeUser",&Web_ParticpantName));
			   printf("\n Webservice--- Web_ParticpantName :%s \n",Web_ParticpantName);fflush(stdout);	
			   tc_strdup(Web_ParticpantName,&Web_ParticpantNamedup);
			   printf("\n Webservice- Web_ParticpantNamedup :%s \n",Web_ParticpantNamedup);fflush(stdout);


			   firsttok=tc_strtok(Web_ParticpantNamedup,"(");    ///Govind Betkar (gbb916241)
			   Secondtok=tc_strtok(NULL,")");
				printf("\n Webservice- firsttok--:------ :%s \n",firsttok);fflush(stdout);
				printf("\n Webservice- Secondtok :-----:%s \n",Secondtok);fflush(stdout);
				ITK_CALL(SA_find_user2(Secondtok,&Usertag));
				ITK_CALL(SA_ask_user_person(Usertag,&persontag));
				ITK_CALL(SA_ask_person_email_address(persontag,&personmail));
				printf("\n Webservice- personmail ---:-------:%s \n",personmail);fflush(stdout);

				///ITK_CALL(SA_find_person2(firsttok,&persontag));
				if(persontag)
				{

				     printf("\n persontag Tag Found \n");

				}
				else
				{
				    printf("\n persontag NOT Tag Found \n");
				
				}

				if(Usertag)
				{
				
				printf("\n User Tag Found \n");
				
				}
				else
				{
				 

				 printf("\n User Tag Not Found \n");
				
				}


				ITK_CALL(PREF_ask_char_value("Mail Apporval Webservice Path", 0 , &WebserviceShellpath ));
				printf("\n Webservice---- WebserviceShellpath %s \n",WebserviceShellpath);fflush(stdout);
			
			 
				for(j=0; j<noAttachment;j++)
				{
				
					attachments1=attachments[j];
						 
					ITK_CALL(AOM_ask_value_string(attachments1,"object_type",&objecttype));
					printf("\n  Webservice object_type %s \n",objecttype);fflush(stdout);  //T5_ParRequestRevision

					  ITK_CALL(AOM_ask_value_string(attachments1,"object_name",&objectname));
					  printf("\n  Webservice objectname %s \n",objectname);fflush(stdout);


					 tc_strcpy(command,"");
					 printf("\n inside pFirst_webservice condition \n");
					 tc_strcat(command,WebserviceShellpath);
					 tc_strcat(command,"pFirst_webservice.sh");
					 tc_strcat(command," ");
					 tc_strcat(command,doublete);
					 tc_strcat(command,objectname);
					 tc_strcat(command,doublete);
					 tc_strcat(command," ");
					 tc_strcat(command,doublete);
					 tc_strcat(command,businessUnit);
					 tc_strcat(command,doublete);
					 tc_strcat(command," ");
					  tc_strcat(command,doublete);
					 tc_strcat(command,workFlowName);
					 tc_strcat(command,doublete);
					 tc_strcat(command," ");
					 tc_strcat(command,doublete);
					 tc_strcat(command,Web_ParticpantName);
					 tc_strcat(command,doublete);
					 tc_strcat(command," ");
					 tc_strcat(command,doublete);
					 tc_strcat(command,taskname);
					 tc_strcat(command,doublete);
					 tc_strcat(command," ");
					 tc_strcat(command,doublete);
					 tc_strcat(command,"/home/ercdev/devgroups/Aditya_ak927337/Shell/sample.xlsx");
					 tc_strcat(command,doublete);
					 tc_strcat(command," ");
					 tc_strcat(command,doublete);
					 tc_strcat(command,personmail);
					 tc_strcat(command,doublete);
					 printf("\n Berfor exicute Command ----------: %s", command);
	                 system(command);


					 BreakFlag=1;
					 
					 

				}

			}
			 printf("\n Break Exected \n");
			//break;
		}
		}

	}

	}
	else
	{
	
	  printf("\n Control Object count is Zero\n");fflush(stdout);
	
	}

	printf("\n END of Web service Handler\n");fflush(stdout);
	

	return ITK_ok;


}


extern int register_Mail_Approval(int *decision, va_list args)
{
	

    *decision = ALL_CUSTOMIZATIONS;
	int	ifail = ITK_ok;
	


   if( ITK_ok == EPM_register_action_handler("Webservice_Mail_approve", "Webservice Mail approve", Webservice_Mail_approve))
   {
	     printf("\n Registered Action Handler Webservice_Mail_approve \n");fflush(stdout);
   }
   else
   {
		printf("\n FAILED to register Action Handler Webservice_Mail_approve\n");fflush(stdout);
   }


	return ifail;

}

extern DLLAPI int tm_Mail_Approval_register_callbacks()
{
     CUSTOM_register_exit("tm_Mail_Approval", "USER_init_module", (CUSTOM_EXIT_ftn_t)register_Mail_Approval);

      printf("\n tm_Mail_Approval Registered \n");

	 return ( ITK_ok );
}
