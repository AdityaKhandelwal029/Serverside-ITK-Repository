/****************************************************************************************************
*  File            :   tm_ProjectAccessHandler.c
*  Created By      :   Aditya Khandelwal
*  Created On      :   17-10-2024
*  Purpose         :   Project Access extension request worflow 

Last Updated 

17-10-2024  Aditya Khandelwal - Updation of API for TC_14.
21-02-2025  Aditya Khandelwal - Updated Default Group Check. 

*****************************************************************************************************/
//#include <ps/ps.h>
//#include <ps/ps_errors.h>
//#include <ai/sample_err.h>
//#include <ae/dataset_msg.h>
//#include <tc/tc.h>
//#include <tccore/workspaceobject.h>
//#include <bom/bom.h>
//#include <ae/dataset.h>
//#include <ps/ps_errors.h>
//#include <sa/sa.h>
//#include <stdarg.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/dir.h>
//#include <sys/stat.h>
//#include <fclasses/tc_string.h>
//#include <tccore/item.h>
//#include <user_exits/user_exits.h>
//#include <tccore/item_errors.h>
//#include <sa/tcfile.h>
//#include <tcinit/tcinit.h>
//#include <tccore/tctype.h>
//#include <res/reservation.h>
//#include <tccore/aom.h>
//#include <tccore/custom.h>
//#include <tc/emh.h>
//#include <ict/ict_userservice.h>
//#include <tc/iman.h>
//#include <tccore/imantype.h>
//#include <tccore/project.h>
//#include <sa/imanfile.h>
//#include <lov/lov.h>
//#include <lov/lov_msg.h>
//#include <itk/mem.h>
//#include <ss/ss_errors.h>
//#include <tccore/item_msg.h>
//#include <tccore/iman_msg.h>
//#include <sa/user.h>
//#include <pie/pie.h>
//#define CONNECT_FAIL (EMH_USER_error_base + 2)
//#define ITK_err 919002
//#define ITK_errStore (EMH_USER_error_base + 4)
//#define PLM_error6 (EMH_USER_error_base+27)



/*--------------------------------------------------*/
#define _CRT_SECURE_NO_DEPRECATE
#define NUM_ENTRIES 1
#define TE_MAXLINELEN  128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <epm/epm.h>
#include <ae/dataset_msg.h>
#include <ps/ps.h>
#include <ps/ps_errors.h>
#include <time.h>
#include <ai/sample_err.h>
#include <tccore/grm_msg.h>
#include <tccore/workspaceobject.h>
#include <bom/bom.h>
#include <ae/dataset.h>
#include <ae/datasettype.h>
#include <ps/ps_errors.h>
#include <sa/sa.h>
#include <sys/stat.h>
#include <fclasses/tc_string.h>
#include <tccore/item.h>
#include <tccore/item_errors.h>
#include <sa/tcfile.h>
#include <tcinit/tcinit.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <res/reservation.h>
#include <tccore/aom.h>
#include <tccore/custom.h>
#include <tc/emh.h>
#include <ict/ict_userservice.h>
#include <lov/lov.h>
#include <lov/lov_msg.h>
#include <ss/ss_errors.h>
#include <sa/user.h>
#include <sa/tcfile.h>
#include <sa/tcfile_cache.h>
#include <tccore/grm.h>
#include <tc/folder.h>//new
#include <user_exits/epm_toolkit_utils.h>
#include <pie/pie.h>
#include <time.h>
#include <Fnd0Participant/participant.h>
#define ITK_err 919002
#define ITK_errStore1 (EMH_USER_error_base + 5)
#define ITK_Prjerr (EMH_USER_error_base + 8)
#define PLM_error6 (EMH_USER_error_base+27)
//#include <tccore/iman_msg.h>
//#include <tc/tc.h>
//#include <epm/releasestatus.h>
//#include <tc/iman.h>
//#include <tccore/imantype.h>
//#include <sa/imanfile.h>
//#include <itk/mem.h>
//#include <epm/cr_effectivity.h>

/*--------------------------------------------------*/

#define Debug TRUE
#define ITK_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
#define CALLAPI(expr)ITKCALL(ifail = expr); if(ifail != ITK_ok) { PrintErrorStack(); return ifail;}


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
int ITK_APICALL(int x,FILE *Flogfile)
{      
	int status=x;
    if(status!=ITK_ok)
    {
        int index = 0;                      
        int n_ifails = 0;           
        const int*  severities = 0;         
        const int*  ifails = 0;                 
        const char** texts = NULL;           
        EMH_ask_errors( &n_ifails, &severities, &ifails, &texts);               
        for( index=0; index<n_ifails; index++)                                     
        {                                                                          
            fprintf(Flogfile,"Error code - [%d]. Error message - %s\n",ifails[index],texts[index]);	
        } 
        EMH_clear_errors();
    }
    return status;
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
int TmProjectAccessHandler(EPM_action_message_t msg)
{
	int noOfAttachments=0,i=0,itkApiCallStatus=0,itr=0,found=0,itr2=0;
	
	char *timeStr            =   NULL;
	char *timeStrUpdated     =   NULL;
	char *itemId             =   NULL;
	char *t5GroupValue       =   NULL;
    char *t5ProjectCodeValue =   NULL;
    char *t5RoleValue        =   NULL;
    char *t5TmlManagerValue  =   NULL;
	char *fileName           =   NULL;
	char *originalFileName   =   NULL;
	char *secObjClass        =   NULL;
	char *fuserId            =   NULL;
	char *fuserName          =   NULL;
	char *roleObjName        =   NULL;
	char *nUserName          =   NULL;
	char *fGroupName         =   NULL;
	
	char *logPath = (char*) MEM_alloc(200*sizeof(char));
	
	int secObjectCount = 0;
	int refListcount   = 0;
	int roleCount      = 0;
	int noOfUsers      = 0;
	int assignSuccess  = 0;
	
	tag_t tRoot_task         = NULLTAG;
	tag_t datasetType        = NULLTAG;
	tag_t datasetTag         = NULLTAG;
	tag_t relationTypeTag    = NULLTAG;
	tag_t relationTag        = NULLTAG;
	tag_t classObjTag        = NULLTAG;
	tag_t participantType    = NULLTAG;
	tag_t userTag            = NULLTAG;
	tag_t participant        = NULLTAG;
	tag_t groupTag           = NULLTAG;
	
	tag_t *tAttachment       = NULLTAG;
	tag_t *secObjTags        = NULLTAG;
	tag_t *reflistTags       = NULLTAG;
	tag_t *roleTag           = NULLTAG;
	tag_t *membersTag        = NULLTAG;
	
	logical bypass =true;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	
	tc_strcpy(logPath,"");
	tc_strcat(logPath,"/tmp/TM_PROJECT_Access_REQ_");
	tc_strcat(logPath,timeStrUpdated);
	tc_strcat(logPath,"_");
	tc_strcat(logPath,"HandlerLog.txt");
	FILE *fptr    = fopen("PAR_Request.txt","w+");
	FILE *flogPtr = fopen(logPath,"w+");
	fprintf(flogPtr,"-Execution time = %s\n",timeStr);
	fprintf(flogPtr,"-logPath = %s\n",logPath);
	
	printf("\n ************************ Execution Started for tm_ProjectAccessHandler ************************ \n");
	printf("-Execution time = %s\n",timeStr);
	printf("-logPath = %s\n",logPath);
	
	EPM_ask_root_task(msg.task, &tRoot_task);
	EPM_ask_attachments(tRoot_task, EPM_target_attachment, &noOfAttachments, &tAttachment);
	fprintf(flogPtr,"-noOfAttachments = %d\n",noOfAttachments);
	
	for (i = 0; i < noOfAttachments; i++)
	{
		if (tAttachment[i]!= NULLTAG)
		{
			ITK_APICALL(AOM_ask_value_string(tAttachment[i],"item_id",&itemId),flogPtr);
			ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_Group",&t5GroupValue),flogPtr);
			ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_Role",&t5RoleValue),flogPtr);
			ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_TML_Manager",&t5TmlManagerValue),flogPtr);
			ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_ParRequestProjectCode",&t5ProjectCodeValue),flogPtr); 		
			
			fprintf(fptr,"-Item Id        = %s\n",itemId      );
			fprintf(fptr,"-Group          = %s\n",t5GroupValue      );
		    fprintf(fptr,"-ProjectCode's  = %s\n",t5ProjectCodeValue);
		    fprintf(fptr,"-Role's         = %s\n",t5RoleValue       );
		    fprintf(fptr,"-TmlManager     = %s\n",t5TmlManagerValue );
			
			fprintf(flogPtr,"-Item Id        = %s\n",itemId      );
			fprintf(flogPtr,"-Group          = %s\n",t5GroupValue      );
		    fprintf(flogPtr,"-ProjectCode's  = %s\n",t5ProjectCodeValue);
		    fprintf(flogPtr,"-Role's         = %s\n",t5RoleValue       );
		    fprintf(flogPtr,"-TmlManager     = %s\n",t5TmlManagerValue );
			
			fclose(fptr);
			
			itkApiCallStatus=ITK_APICALL(GRM_list_secondary_objects_only(tAttachment[i],NULLTAG,&secObjectCount, &secObjTags),flogPtr);
			
			fprintf(flogPtr,"*************** ITK status for GRM_list_secondary_objects_only = %d *************** \n",itkApiCallStatus);
			fprintf(flogPtr,"*************** secObjectCount = %d *************** \n",secObjectCount);
			
			if(secObjectCount==0)
			{
				fprintf(flogPtr,"If Condition Executed secObjectCount==0 \n");
				found=0;
			}
			else
			{
				for(itr=0;itr<secObjectCount;itr++)
				{
					itkApiCallStatus=ITK_APICALL(POM_class_of_instance(secObjTags[itr],&classObjTag),flogPtr);
					fprintf(flogPtr,"*************** ITK status for POM_class_of_instance = %d *************** \n",itkApiCallStatus);
							
					itkApiCallStatus=ITK_APICALL(POM_name_of_class(classObjTag,&secObjClass),flogPtr);
					fprintf(flogPtr,"*************** ITK status for POM_name_of_class = %d *************** \n",itkApiCallStatus);					
				    fprintf(flogPtr,"*************** Class name secObjClass = %s *************** \n",secObjClass);
								
					if(tc_strcmp(secObjClass,"Dataset")==0)
					{
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_tags(secObjTags[itr],"ref_list",&refListcount,&reflistTags),flogPtr);											
						fprintf(flogPtr,"*************** ITK status for AOM_ask_value_tags = %d *************** \n",itkApiCallStatus);
						fprintf(flogPtr,"*************** refListcount = %d *************** \n",refListcount);
						
						if(refListcount>0)
						{
							for(itr2=0;itr2<refListcount;itr2++)
							{
						       itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(reflistTags[itr2],"file_name",&fileName),flogPtr);
							   fprintf(flogPtr,"*************** ITK status for AOM_ask_value_string for file_name = %d *************** \n",itkApiCallStatus);
							   fprintf(flogPtr," -fileName = %s\n",fileName);
							   
							   itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(reflistTags[itr2],"original_file_name",&originalFileName),flogPtr);
							   fprintf(flogPtr,"*************** ITK status for AOM_ask_value_string for originalFileName = %d *************** \n",itkApiCallStatus);
							   fprintf(flogPtr," -originalFileName = %s\n",originalFileName);
							   
							   if(tc_strcmp(fileName,"PAR_Request.txt")==0||tc_strcmp(originalFileName,"PAR_Request.txt")==0)
							   {
								   found=1; 
								   fprintf(flogPtr,"*************** Found changed to 1 \n");
								   break;
							   }
							}
						}
					}
				}
			}
			if(found==0)
			{
				fprintf(flogPtr,"*************** Executing condition where found ==0 *************** \n");
				
				// Creation of dataset 

                itkApiCallStatus=ITK_APICALL(AE_find_datasettype2("Text",&datasetType),flogPtr);
				fprintf(flogPtr,"ITK status for AE_find_datasettype2 = %d\n",itkApiCallStatus);
                					
                itkApiCallStatus=ITK_APICALL(AE_create_dataset_with_id(datasetType,itemId,itemId,itemId,"A",&datasetTag),flogPtr);
				fprintf(flogPtr,"ITK status for AE_create_dataset_with_id = %d\n",itkApiCallStatus);
				                
                itkApiCallStatus=ITK_APICALL(AE_import_named_ref(datasetTag,"Text","PAR_Request.txt",itemId,SS_TEXT),flogPtr);
				fprintf(flogPtr,"ITK status for AE_import_named_ref = %d\n",itkApiCallStatus);
				   
                itkApiCallStatus=ITK_APICALL(AOM_refresh(datasetTag,TRUE),flogPtr);
				fprintf(flogPtr,"ITK status for AOM_refresh = %d\n",itkApiCallStatus);
				          
                itkApiCallStatus=ITK_APICALL(AOM_save_without_extensions(datasetTag),flogPtr);
				fprintf(flogPtr,"ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);
				     
                // Attachment of dataset
                
                itkApiCallStatus=ITK_APICALL(GRM_find_relation_type("IMAN_Rendering",&relationTypeTag),flogPtr);
				fprintf(flogPtr,"ITK status for GRM_find_relation_type = %d\n",itkApiCallStatus);
                
                itkApiCallStatus=ITK_APICALL(GRM_create_relation(tAttachment[i],datasetTag,relationTypeTag,NULLTAG,&relationTag),flogPtr);
				fprintf(flogPtr,"ITK status for GRM_create_relation = %d\n",itkApiCallStatus);
                
                itkApiCallStatus=ITK_APICALL(GRM_save_relation(relationTag),flogPtr);
				fprintf(flogPtr,"ITK status for GRM_save_relation = %d\n",itkApiCallStatus);        
				fprintf(flogPtr," *********** Dataset successfully created and attached. *********** \n");
				
			}
			
			// Process for participant Assignment started.
			
			fprintf(flogPtr," *********** Process for participant Assignment started *********** \n");
			
			itkApiCallStatus=ITK_APICALL(EPM_get_participanttype("T5_TML_Manager_Participant",&participantType),flogPtr);	
			fprintf(flogPtr,"*************** ITK status for EPM_get_participanttype = %d *************** \n",itkApiCallStatus);	
			
			if(participantType!=NULLTAG)
			{
				fprintf(flogPtr," *********** Participant type found  ***********\n");
				
				itkApiCallStatus=ITK_APICALL(SA_find_user2(t5TmlManagerValue,&userTag),flogPtr);
				fprintf(flogPtr," *************** ITK status for SA_find_user2 = %d *************** \n",itkApiCallStatus);
				
				if(userTag!=NULLTAG)
				{
				    fprintf(flogPtr," ***********User found from API SA_find_user2  ***********\n");
				    
				    itkApiCallStatus=ITK_APICALL(AOM_UIF_ask_value(userTag,"user_id",  &fuserId ),flogPtr);
				    fprintf(flogPtr," *************** ITK status for AOM_UIF_ask_value for userId = %d *************** \n",itkApiCallStatus);
				    
				    itkApiCallStatus=ITK_APICALL(AOM_UIF_ask_value(userTag,"user_name",&fuserName),flogPtr);
				    fprintf(flogPtr," *************** ITK status for AOM_UIF_ask_value for userName = %d ***************  \n",itkApiCallStatus);
				    
				    fprintf(flogPtr," - fuserId = %s  \n",fuserId);
				    fprintf(flogPtr," - fuserName = %s  \n",fuserId);
				   	   
				    itkApiCallStatus=ITK_APICALL(POM_ask_user_default_group(userTag,&groupTag),flogPtr);
			        fprintf(flogPtr," *************** ITK status for POM_ask_user_default_group = %d *************** \n",itkApiCallStatus);
				   
				    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(groupTag,"name",  &fGroupName ),flogPtr);
		            fprintf(flogPtr," *************** ITK status for AOM_UIF_ask_value for group = %d *************** \n",itkApiCallStatus);
		    		    
		            fprintf(flogPtr," - fGroupName group = %s  \n",fGroupName);
					
					//itkApiCallStatus=ITK_APICALL(SA_find_group("ERC_Designers_Group",&groupTag),flogPtr);
				    //fprintf(flogPtr,"ITK status for SA_find_group = %d\n",itkApiCallStatus);
				   			   
				    if(groupTag!=NULLTAG)
				    {
				        fprintf(flogPtr," ***********Group found from API SA_find_group  ***********\n");
						
						itkApiCallStatus=ITK_APICALL(SA_ask_roles_from_group(groupTag,&roleCount,&roleTag),flogPtr);  
				        fprintf(flogPtr,"ITK status for SA_ask_roles_from_group = %d\n",itkApiCallStatus);
						fprintf(flogPtr," - roleCount = %d \n", roleCount);
						if(roleCount>0)
						{
				            fprintf(flogPtr," ***********role found from API SA_ask_roles_from_group  ***********\n");
							
							for(itr=0;itr<roleCount;itr++)
							{
								itkApiCallStatus=ITK_APICALL(AOM_UIF_ask_value(roleTag[itr],"object_name",  &roleObjName ),flogPtr);
				                fprintf(flogPtr," *************** ITK status for AOM_UIF_ask_value for object_name of role = %d *************** \n",itkApiCallStatus);
								fprintf(flogPtr," ****************************** -itr = %d -roleObjName = %s ******************************\n",itr+1,roleObjName);
								
								itkApiCallStatus=ITK_APICALL(SA_find_groupmember_by_role(roleTag[itr],groupTag,&noOfUsers,&membersTag),flogPtr);
				                fprintf(flogPtr,"ITK status for SA_find_groupmember_by_role = %d\n",itkApiCallStatus);
								
								fprintf(flogPtr," - noOfUsers = %d \n", noOfUsers);
								if(noOfUsers>0)
								{
									for(itr2=0;itr2<noOfUsers;itr2++)
									{
										itkApiCallStatus=ITK_APICALL(AOM_UIF_ask_value(membersTag[itr2],"user_name",  &nUserName),flogPtr);
				                        fprintf(flogPtr," *************** ITK status for AOM_UIF_ask_value for user_name = %d *************** \n",itkApiCallStatus);
										
										fprintf(flogPtr," - nUserName         = %s length(nUserName) = %d \n", nUserName,tc_strlen(nUserName));
										fprintf(flogPtr," - t5TmlManagerValue = %s length(t5TmlManagerValue) = %d \n", t5TmlManagerValue,tc_strlen(t5TmlManagerValue));
										
										if(tc_strcmp(nUserName,t5TmlManagerValue)==0)
										{
											fprintf(flogPtr," Matching user found nUserName==t5TmlManagerValue \n");
											
											itkApiCallStatus=ITK_APICALL(EPM_create_participant(membersTag[itr2],participantType,&participant),flogPtr);
											fprintf(flogPtr,"ITK status for EPM_create_participant = %d\n",itkApiCallStatus);					
											
											itkApiCallStatus=ITK_APICALL(AOM_lock(tAttachment[i]),flogPtr);
											fprintf(flogPtr,"ITK status for AOM_lock = %d\n",itkApiCallStatus);				
																						
											itkApiCallStatus=ITK_APICALL(PARTICIPANT_add_participant(tAttachment[i],bypass,participant),flogPtr);
											fprintf(flogPtr,"ITK status for PARTICIPANT_add_participant = %d\n",itkApiCallStatus);				
											
											itkApiCallStatus=ITK_APICALL(AOM_save_without_extensions(tAttachment[i]),flogPtr);
											fprintf(flogPtr,"ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);				
											
											itkApiCallStatus=ITK_APICALL(AOM_unlock(tAttachment[i]),flogPtr);
											fprintf(flogPtr,"ITK status for AOM_unlock = %d\n",itkApiCallStatus);			
											
											itkApiCallStatus=ITK_APICALL(AOM_refresh(tAttachment[i],TRUE),flogPtr);
											fprintf(flogPtr,"ITK status for AOM_refresh = %d\n",itkApiCallStatus);		

                                            assignSuccess=1;
											fprintf(flogPtr,"\n ************** Participant Assignment successfull.Process successfully completed. ************** \n");
											break;
										}
									}
								}
								else
								{ 							     
									fprintf(flogPtr," *********** Users not found from API SA_find_groupmember_by_role for a particular role ***********\n");
								}
								if(assignSuccess==1)
								{
									fprintf(flogPtr,"\n ************** Final break Executed Process completed with so generated on 21_02_2025. ************** \n");
									break;
								}
							}
						}
						else
						{
				           fprintf(flogPtr," ***********role not found from API SA_ask_roles_from_group  ***********\n");
						}
				    }
				    else
				    {   
				        fprintf(flogPtr," ***********Group not found from API SA_find_group  ***********\n");
				    }
				}
				else
				{ 
				    fprintf(flogPtr," *********** User not found from API SA_find_user2  ***********\n");
				}
			}
			else
			{
				fprintf(flogPtr," *********** Participant type not found ***********\n");
			}
		}
	}
	printf("\n ************************ Execution Completed for tm_ProjectAccessHandler ************************ \n");
	fclose(flogPtr);
	return (ITK_ok);
}
extern int register_tmProjectAccessHandler(int *decision, va_list args)
{
    *decision = ALL_CUSTOMIZATIONS;
	int	ifail = ITK_ok;

   if( ITK_ok == EPM_register_action_handler("Project_Access_Handler", "Project Access Request Handler", TmProjectAccessHandler))
   {
		printf("\t\n Registered Action Handler Project_Access_Handler \n");fflush(stdout);
   }
   else
   {
		printf("\t FAILED to register Action Handler Project_Access_Handler\n");fflush(stdout);
   }

	return ifail;

}

extern DLLAPI int tm_ProjectAccessHandler_register_callbacks()
{
    CUSTOM_register_exit("tm_ProjectAccessHandler", "USER_init_module", (CUSTOM_EXIT_ftn_t)register_tmProjectAccessHandler);
	printf("\n *********** ***********  Login successfull to teamcenter with tm_ProjectAccessHandler *********** ***********   \n"); 
	return ( ITK_ok );
}

