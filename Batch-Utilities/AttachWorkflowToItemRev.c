/*Objective - Itk utility to attach a workflow process template to an Item Revision 
Input : 
-u=Username
-p=Password
-g=Group
-itemid=Item id of target item. 
-itemrevid=Item rev id of target revision. 
-workflowtemplate=Name of the target workflow process.
*/

/* In this case of auto login with out parameters . testcmi will be the logged in user.*/

/*Required headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/custom.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <res/res_itk.h>
#include <ctype.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <epm/epm.h>
#include <epm/epm_task_template_itk.h>

/*Declarations */

int itkApiCallStatus = -1;
tag_t *qryObjTags = NULLTAG;

/*Custom API error handling function */

int ITK_APICALL(int x,FILE *Flogfile){      
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
                    fprintf(Flogfile,"Error code - %d Error message - %s\n",ifails[index],texts[index]);					
                } 
           EMH_clear_errors();
        }
        return status;
    }
/*Main function implementation */ 


int queryData(char *queryName,char *qEntry,char* qValues,int queryPrmCount ,FILE *flogPtr)
{
	date_t rlsDate;
	
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	int itr  = 0;
	int itr2 = 0;
	int itj  = 0;
	int itk1 = 0;
	int itk2 = 0;
	int rlsStatusCount   = 0;
	int fRlsCount        = 0;
	
	char *queryRealName    = NULL;
	char *tarObjId         = NULL;
	char *tarObjName       = NULL;
	char *rlsStatusName    = NULL;
	char *rlsStatusName1   = NULL;
	char *rlsStatusObjStr  = NULL;
	char *dataRlsStr       = NULL;	
	
	char *tEntryName       = (char*) MEM_alloc(100*sizeof(char));
	char *tValueName       = (char*) MEM_alloc(100*sizeof(char));
	char *todaysDateStr    = (char*) MEM_alloc(100*sizeof(char));
	char *tokrlsDateStr    = (char*) MEM_alloc(100*sizeof(char));
	
	char *queryEntryInput [queryPrmCount];
	char *queryValuesInput[queryPrmCount];
		
	tag_t      qryTag = NULLTAG;
    //tag_t *qryObjTags = NULLTAG;	
	tag_t *rlsStatusList = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of queryData function started *********** \n");
	
	fprintf(flogPtr,"\n - queryName        = %s",queryName);
	fprintf(flogPtr,"\n - qEntry           = %s",qEntry);
	fprintf(flogPtr,"\n - qValues          = %s",qValues);
	fprintf(flogPtr,"\n - queryPrmCount    = %d",queryPrmCount);
	
	for(itr = 0;qEntry[itr]!='\0';itr++)
	{
		if(qEntry[itr] == ',')
		{
			tEntryName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tEntryName = %s",tEntryName);
			queryEntryInput[itk1] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryEntryInput[itk1],tEntryName);
			itk1++;
			
		}
		else if(itr+1 == tc_strlen(qEntry))
		{
			tEntryName[itj] = qEntry[itr];
			itj++;
			tEntryName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tEntryName = %s",tEntryName);
			queryEntryInput[itk1] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryEntryInput[itk1],tEntryName);				
		}
		else
		{
			tEntryName[itj] = qEntry[itr];
			itj++;
		}
	}
	for(itr = 0;qValues[itr]!='\0';itr++)
	{
		if(qValues[itr] == ',')
		{
			tValueName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tValueName = %s",tValueName);
			queryValuesInput[itk2] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryValuesInput[itk2],tValueName);
			itk2++;
		}
		else if(itr+1 == tc_strlen(qValues))
		{
			tValueName[itj] = qValues[itr];
			itj++;
			tValueName[itj]='\0';
			itj = 0;
			fprintf(flogPtr,"\n tValueName = %s",tValueName);
			queryValuesInput[itk2] = (char*) MEM_alloc(100*sizeof(char));
			tc_strcpy(queryValuesInput[itk2],tValueName);
		}
		else
		{
			tValueName[itj] = qValues[itr];
			itj++;
		}
	}
	for(itr = 0;itr < queryPrmCount;itr++)
	{
		fprintf(flogPtr,"\n -queryEntryInput[%d] = %s - queryValuesInput[%d] = %s",itr,queryEntryInput[itr],itr,queryValuesInput[itr]);
	}
	itkApiCallStatus=ITK_APICALL(QRY_find2(queryName,&qryTag),flogPtr);
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n Query found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryRealName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,queryPrmCount,queryEntryInput,queryValuesInput,&qryObjCount,&qryObjTags),flogPtr);
		fprintf(flogPtr,"\n -queryRealName   = %s",queryRealName);
		fprintf(flogPtr,"\n -qryObjCount     = %d",qryObjCount);
		
        
		if(qryObjCount>0)
		{
			fprintf(flogPtr,"\n *************** Execting condition where qryObjCount > 0 ************** ");
	        
		}
		else
		{
			fprintf(flogPtr,"\n Execting condition where qryObjCount <= 0 ");
		}		
	}
	else
	{
		fprintf(flogPtr,"\n Error : Query not found .... ");
	}
	fprintf(flogPtr," \n *********** Execution of queryData function Ended *********** \n");
	return(qryObjCount);
}
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile	     =   NULL;
    char *sGroup             =   NULL;
	char *loggedInUser       =   NULL; 
	char *itemid             =   NULL;
	char *itemrevid          =   NULL;
	char *workflowtemplate   =   "Change Request Life Cycle";
	char *revObjName         =   NULL;
    char *revObjStr          =   NULL;
    char *revt5Reason        =   NULL;
    char *revt5DisReason     =   NULL;
	char *processName        =   (char*)malloc(1000*sizeof(char));
	
	int attachTypes = 1;
	int queryObjCount  = 0;
	
	
	tag_t loggedInUserTag    =  NULLTAG;
	tag_t itemTag            =  NULLTAG;
	tag_t itemRevTag         =  NULLTAG;
	tag_t templateTag        =  NULLTAG;
	tag_t newProcessTag      =  NULLTAG;
	
	tag_t *itemRevListTag;
	
	sUserName        = ITK_ask_cli_argument("-u=");
    sPasswordFile    = ITK_ask_cli_argument("-p=");
    sGroup           = ITK_ask_cli_argument("-g=");
	itemid           = ITK_ask_cli_argument("-itemid=");
	itemrevid        = ITK_ask_cli_argument("-itemrevid=");
	//workflowtemplate = ITK_ask_cli_argument("-workflowtemplate=");
	
	FILE *flogPtr=fopen("AttachWorkLog.txt","w+");
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	
	fprintf(flogPtr,"\n-u = %s ",sUserName);
	fprintf(flogPtr,"\n-p = %s ",sPasswordFile);
	fprintf(flogPtr,"\n-g = %s ",sGroup);
	fprintf(flogPtr,"\n-itemid = %s ",itemid);
	fprintf(flogPtr,"\n-itemrevid = %s ",itemrevid);
	fprintf(flogPtr,"\n-workflowtemplate = %s ",workflowtemplate);
	
	
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
	ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	itkApiCallStatus=ITK_APICALL(ITK_init_module(sUserName,sPasswordFile,sGroup),flogPtr); 
	fprintf(flogPtr,"Login itkApiCallStatus = %d ",itkApiCallStatus);
	
	if(itkApiCallStatus==ITK_ok)
    {
		printf("**************************************************************************Login successful to teamcenter \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC...");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
	    {
		    fprintf(flogPtr,"\nName of the logged in user = %s \n",loggedInUser);	
		    itkApiCallStatus=ITK_APICALL(ITEM_find_item(itemid,&itemTag),flogPtr);
		    if(itkApiCallStatus==0)
		    {
		    	fprintf(flogPtr,"\nItem with '%s' itemid found ",itemid);
		    	ITK_APICALL(ITEM_find_revision(itemTag,itemrevid,&itemRevTag),flogPtr);
		    	if(itemRevTag!=NULLTAG)
		    	{
		    		fprintf(flogPtr,"\nItemrev with '%s' itemrevid found ",itemrevid);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"object_name",&revObjName),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"object_string",&revObjStr),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_reason",&revt5Reason),flogPtr);
					ITK_APICALL(AOM_ask_value_string(itemRevTag,"t5_DmlReasonDisplay",&revt5DisReason),flogPtr);
					
					fprintf(flogPtr,"\n - revObjName     = %s",revObjName);
					fprintf(flogPtr,"\n - revObjStr      = %s",revObjStr);
					fprintf(flogPtr,"\n - revt5Reason    = %s",revt5Reason);
					fprintf(flogPtr,"\n - revt5DisReason = %s",revt5DisReason);
					
					ITK_APICALL(EPM_find_process_template(workflowtemplate,&templateTag),flogPtr);
					
					if(templateTag != NULLTAG)
					{
						fprintf(flogPtr,"\n Template Found "); 
						
						tc_strcpy(processName,"");						
						tc_strcat(processName,"Change Request Life Cycle : ");										
						tc_strcat(processName,revObjName);
						tc_strcat(processName," ");
						tc_strcat(processName,revt5DisReason);
						
						fprintf(flogPtr,"\n - processName = %s ",processName);
						
						//itemRevListTag = &itemRevTag;
						
						
						//queryObjCount = queryData("Item Revision...","Item ID,Revision","24CU010020,NR",2,flogPtr);
						fprintf(flogPtr," - queryObjCount = %d ",queryObjCount);
						
						// Working    
						//itkApiCallStatus = ITK_APICALL(EPM_create_process(processName,"New Initiation",templateTag,1,qryObjTags,&attachTypes,&newProcessTag),flogPtr);
					    
						itkApiCallStatus = ITK_APICALL(EPM_create_process(processName,"New Initiation",templateTag,1,&itemRevTag,&attachTypes,&newProcessTag),flogPtr);
						
						fprintf(flogPtr,"\n -final ITK status = %d",itkApiCallStatus);
					} 
					
					
		    	}
		    	else{
		    		fprintf(flogPtr,"\nItemrev with '%s' itemrevid not found  ",itemrevid);
		    	}
		    }
		    else
		    {
		    	fprintf(flogPtr,"\nItem with '%s' itemid not found ",itemid);
		    }
		}	
	}
	else
	{
		printf("\n****************************nLogin failed in TC \n");	
		fprintf(flogPtr,"\n*******************nLogin failed in TC \n");
	}
	return(ITK_ok);
}
