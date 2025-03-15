/****************************************************************************************************
*  File            :   t5ListEmptyVisPdfDesRev.c
*  Created By      :   Anu Nair
*  Created On      :   01 Mar 2020
*  Purpose         :   BUG in JT/PDF Translation, Dataset DirectModel and PDF have no named references. CC List the DesignRevision
*
*  Usage		   :./Test4dObj -u=ercpsup -p=XYT1ESA -g="" -formObjName="Test_211021"
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
#include <epm/epm.h>
#include <ae/dataset_msg.h>
#include <tccore/iman_msg.h>
#include <ps/ps.h>
#include <ps/ps_errors.h>
#include <time.h>
#include <ai/sample_err.h>
#include <tc/tc.h>
#include <tccore/grm_msg.h>
#include <tccore/workspaceobject.h>
#include <bom/bom.h>
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
#include <epm/releasestatus.h>
#include <tcinit/tcinit.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <res/reservation.h>
#include <tccore/aom.h>
#include <tccore/custom.h>
#include <tc/emh.h>
#include <ict/ict_userservice.h>
#include <tc/iman.h>
#include <tccore/imantype.h>
#include <sa/imanfile.h>
#include <lov/lov.h>
#include <lov/lov_msg.h>
#include <itk/mem.h>
#include <ss/ss_errors.h>
#include <sa/user.h>
#include <sa/tcfile.h>
#include <sa/tcfile_cache.h>
#include <tccore/grm.h>
#include <tc/folder.h>//new
#include <string.h>
#include <epm/cr_effectivity.h>
#include <user_exits/epm_toolkit_utils.h>
#include <pie/pie.h>
#include <time.h>
#include <Fnd0Participant/participant.h>
#define ITK_err 919002
#define ITK_errStore1 (EMH_USER_error_base + 5)
#define ITK_Prjerr (EMH_USER_error_base + 8)
#define PLM_error6 (EMH_USER_error_base+27)
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
int TmProjectAssignHandler(EPM_action_message_t msg)
{
	int noOfAttachments=0,i=0,itkApiCallStatus=0,itr1=0,found=0,itr2=0;
	int t1=0;
    int t2=0;
    int t3=0; 
	
	char *timeStr             =   NULL;
	char *timeStrUpdated      =   NULL;
	char *prefValue           =   NULL;
	char *itemId              =   NULL;
	char *t5GroupValue        =   NULL;
    char *t5ProjectCodeValue  =   NULL;
    char *t5RoleValue         =   NULL;
    char *t5TmlManagerValue   =   NULL;
	char *documentAuthorValue =   NULL;
	char *ownUserId           =   NULL;
	char *ownUserName         =   NULL;
	
	char *logPath             = (char*)malloc(200*sizeof(char));
	char *proCode             = (char*)malloc(100*sizeof(char)); 
    char *roleCode            = (char*)malloc(100*sizeof(char)); 	
	char *fileInput           = (char*)malloc(400*sizeof(char)); 
	char *shellInputPath      = (char*)malloc(200*sizeof(char));    
	char *shellExecute        = (char*)malloc(200*sizeof(char));      
	
	tag_t tRoot_task          = NULLTAG;
	tag_t owningUserTag       = NULLTAG;
	tag_t *tAttachment        = NULLTAG;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	
	printf("\n ************************ Execution Started for tm_ProjectAssignHandler ************************ \n");
	
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	PREF_ask_char_value("T5_PAR_Request_Path",0,&prefValue);
	
	tc_strcpy(logPath,"");
	tc_strcat(logPath,"/tmp/TM_PROJECT_Assign_REQ_");
	tc_strcat(logPath,timeStrUpdated);
	tc_strcat(logPath,"_");
	tc_strcat(logPath,"HandlerLog.txt");
	
	tc_strcpy(shellInputPath,"");
	tc_strcat(shellInputPath,prefValue);
	tc_strcat(shellInputPath,"ProjectAccessRequest/ProjectAssignShellInput_");
	tc_strcat(shellInputPath,timeStrUpdated);
	tc_strcat(shellInputPath,"_File.txt");
	
	FILE *fPtr = fopen(shellInputPath,"w+");
	FILE *flogPtr = fopen(logPath,"w+");
	
	fprintf(flogPtr,"-Execution time = %s\n",timeStr);
	fprintf(flogPtr,"-logPath        = %s\n",logPath);
	fprintf(flogPtr,"-shellInputPath = %s\n",shellInputPath);
	
	printf("-Execution time PROJECT_Assign_REQ = %s\n",timeStr);
	printf("-logPath        PROJECT_Assign_REQ = %s\n",logPath);
	printf("-shellInputPath PROJECT_Assign_REQ = %s\n",shellInputPath);
	
	
	if(fPtr!=NULL)
	{
	    EPM_ask_root_task(msg.task, &tRoot_task);
	    EPM_ask_attachments(tRoot_task, EPM_target_attachment, &noOfAttachments, &tAttachment);
	    fprintf(flogPtr,"-noOfAttachments = %d\n",noOfAttachments);
	    
	    for (i = 0; i < noOfAttachments; i++)
	    {
	    	if (tAttachment[i]!= NULLTAG)
	    	{
	    		fprintf(flogPtr," *************** *************** Working with attachment = %d  *************** *************** \n",i+1);	
	    		
	    		ITK_APICALL(AOM_ask_value_string(tAttachment[i],"item_id",&itemId),flogPtr);
	    		ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_Group",&t5GroupValue),flogPtr);
	    		ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_Role",&t5RoleValue),flogPtr);
	    		ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_TML_Manager",&t5TmlManagerValue),flogPtr);
	    		ITK_APICALL(AOM_ask_value_string(tAttachment[i],"t5_ParRequestProjectCode",&t5ProjectCodeValue),flogPtr); 
	    		ITK_APICALL(AOM_ask_value_string(tAttachment[i],"DocumentAuthor",&documentAuthorValue),flogPtr); 
	    		ITK_APICALL(AOM_ask_value_tag(tAttachment[i],"owning_user",&owningUserTag),flogPtr); 
	    		ITK_APICALL(AOM_ask_value_string(owningUserTag,"user_id",  &ownUserId  ),flogPtr);
	    		ITK_APICALL(AOM_ask_value_string(owningUserTag,"user_name",&ownUserName),flogPtr);
	    		
	    		fprintf(flogPtr,"-Item Id        = %s\n",itemId      );
	    		fprintf(flogPtr,"-Group          = %s\n",t5GroupValue      );
	    	    fprintf(flogPtr,"-ProjectCode's  = %s\n",t5ProjectCodeValue);
	    	    fprintf(flogPtr,"-Role's         = %s\n",t5RoleValue       );
	    	    fprintf(flogPtr,"-TmlManager     = %s\n",t5TmlManagerValue );
	    		fprintf(flogPtr,"-DocumentAuthor = %s\n",documentAuthorValue );
	    		fprintf(flogPtr,"-OwnUserId      = %s\n",ownUserId );
	    		fprintf(flogPtr,"-OwnUserName    = %s\n",ownUserName );
	    		
	    		if(tc_strcmp(ownUserId,documentAuthorValue)==0)
	    		{
	    			fprintf(flogPtr,"\n ***************** If condition executed where ownUserId==documentAuthorValue \n");
	    			for(itr1=0;t5ProjectCodeValue[itr1]!='\0';itr1++)
	    			{
	    			    if(t5ProjectCodeValue[itr1]==',')
	    			    {
	    			       proCode[t1]='\0';
	    			       t1=0;
	    			       fprintf(flogPtr,"\n ************* value for project = %s \n",proCode);
	    				   
	    			       for(itr2=0;t5RoleValue[itr2]!='\0';itr2++)
	    			       {
	    			           if(t5RoleValue[itr2]==',')
	    			           {
	    			               roleCode[t2]='\0';
	    			               t2=0;
	    			               fprintf(flogPtr," value for role = %s \n",roleCode);							   							   
	    						   tc_strcpy(fileInput,""); 
	    						   tc_strcpy(fileInput,ownUserId);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   tc_strcat(fileInput,t5GroupValue);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   tc_strcat(fileInput,proCode);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   tc_strcat(fileInput,roleCode);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   fprintf(flogPtr," -fileInput = %s \n",fileInput);
	    						   fprintf(fPtr,"%s\n",fileInput);
	    						   							   
	    			           }
	    			           else if(t5RoleValue[itr2+1]=='\0')
	    			           {
	    			               roleCode[t2]=t5RoleValue[itr2];
	    			               t2++;
	    			               roleCode[t2]='\0';
	    			               fprintf(flogPtr," value for role = %s \n",roleCode);
	    			               t2=0;
	    						   tc_strcpy(fileInput,""); 
	    						   tc_strcpy(fileInput,ownUserId);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   tc_strcat(fileInput,t5GroupValue);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   tc_strcat(fileInput,proCode);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   tc_strcat(fileInput,roleCode);
	    						   tc_strcat(fileInput,"~");
	    						   
	    						   fprintf(flogPtr," -fileInput = %s \n",fileInput);
	    						   fprintf(fPtr,"%s\n",fileInput);
	    						   							   
	    			           }
	    			           else
	    			           {
	    			               roleCode[t2]=t5RoleValue[itr2];
	    			               t2++;
	    			           }
	    			       }
	    			    }
	    			    else if(t5ProjectCodeValue[itr1+1]=='\0')
	    			    {
	    			        proCode[t1]=t5ProjectCodeValue[itr1];
	    			        t1++;
	    			        proCode[t1]='\0';
	    			        fprintf(flogPtr,"\n ************* value for project = %s \n",proCode);
	    			        for(itr2=0;t5RoleValue[itr2]!='\0';itr2++)
	    			        {
	    			            if(t5RoleValue[itr2]==',')
	    			            {
	    			                roleCode[t2]='\0';
	    			                t2=0;
	    			                fprintf(flogPtr," value for role = %s \n",roleCode);								
	    							
	    			                tc_strcpy(fileInput,"");  
	    						    tc_strcpy(fileInput,ownUserId);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    tc_strcat(fileInput,t5GroupValue);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    tc_strcat(fileInput,proCode);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    tc_strcat(fileInput,roleCode);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    fprintf(flogPtr," -fileInput = %s \n",fileInput);
	    						    fprintf(fPtr,"%s\n",fileInput);
	    																
	    			            }
	    			            else if(t5RoleValue[itr2+1]=='\0')
	    			            {
	    			                roleCode[t2]=t5RoleValue[itr2];
	    			                t2++;
	    			                roleCode[t2]='\0';
	    			                fprintf(flogPtr," value for role = %s \n",roleCode);
	    			                t2=0;
	    							
	    						    tc_strcpy(fileInput,""); 
	    						    tc_strcpy(fileInput,ownUserId);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    tc_strcat(fileInput,t5GroupValue);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    tc_strcat(fileInput,proCode);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    tc_strcat(fileInput,roleCode);
	    						    tc_strcat(fileInput,"~");
	    						    
	    						    fprintf(flogPtr," -fileInput = %s \n",fileInput);
	    						    fprintf(fPtr,"%s\n",fileInput);
	    																														
	    			            }
	    			            else
	    			            {
	    			                roleCode[t2]=t5RoleValue[itr2];
	    			                t2++;
	    			            }
	    			        }
	    			        t1=0;
	    			    }
	    			    else
	    			    {
	    			        proCode[t1]=t5ProjectCodeValue[itr1];
	    			        t1++;
	    			    }		    
	    			}	
	    			fclose(fPtr);
                    tc_strcpy(shellExecute,"");
	    			tc_strcat(shellExecute,prefValue);
	    			tc_strcat(shellExecute,"ProjectAccessRequest/ProjectAssignShell.sh");
	    	        tc_strcat(shellExecute," ");
	    	        tc_strcat(shellExecute,itemId);
					tc_strcat(shellExecute," ");
	    	        tc_strcat(shellExecute,shellInputPath);
					
	    	  
	    	        fprintf(flogPtr,"\n -shellExecute command = %s \n",shellExecute);
	    			printf("\n -shellExecute command = %s \n",shellExecute);
	    			
	    			printf("\n ########################################## Shell Execution for Project Assignment started in tm_ProjectAssignHandler.so  ############################################### \n");
	    	        system(shellExecute);
	    	        printf("\n ########################################## Shell Execution for Project Assignment Completed in tm_ProjectAssignHandler.so   ################################################ \n");	
	    			fprintf(flogPtr," \n **************** Shell Execution successfull from (so) **************** \n",shellExecute);	
	    			fclose(flogPtr);
	    		}
	    		else
	    		{
	    			fprintf(flogPtr,"\n ***************** Else condition executed where ownUserId != documentAuthorValue \n");
	    		}
	    	}
	    }		
	}
	else
	{
		fprintf(flogPtr,"\n ***************** Failed to open or write in ProjectAssignShellInput.txt please check logs and try again. ***************** \n");
		printf("\n ***************** Failed to open or write in ProjectAssignShellInput.txt please check logs and try again. ***************** \n");
	}
	printf("\n ************************ Execution Completed for tm_ProjectAssignHandler ************************ \n");
	return (ITK_ok);
}
extern int register_tmProjectAssignHandler(int *decision, va_list args)
{
    *decision = ALL_CUSTOMIZATIONS;
	int	ifail = ITK_ok;
   if( ITK_ok == EPM_register_action_handler("Project_Assign_Handler", "Project Access Request Handler", TmProjectAssignHandler))
   {
		printf("\t\n Registered Action Handler Project_Access_Handler \n");fflush(stdout);
   }
   else
   {
		printf("\t FAILED to register Action Handler Project_Access_Handler\n");fflush(stdout);
   }
	return ifail;
}

extern DLLAPI int tm_ProjectAssignHandler_register_callbacks()
{
    CUSTOM_register_exit("tm_ProjectAssignHandler", "USER_init_module", (CUSTOM_EXIT_ftn_t)register_tmProjectAssignHandler);
	printf("\n *********** ***********  Login successfull to teamcenter with tm_ProjectAssignHandler *********** *********** \n"); 
	return ( ITK_ok );
}


