/*Date - 16-05-2025 
        */
/* Objective :- Perform signOff with Utility
 */
/*Required headers */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/grm.h>
#include <tccore/aom_prop.h>
#include <tccore/custom.h>
#include <tccore/tctype.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <res/res_itk.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <tccore/aom_prop.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <sa/user.h>
#include <tc/folder.h>
#include <ae/datasettype.h>
#include <ae/dataset.h>
#include <epm/signoff.h>


// Global Variables 

tag_t *qryObjTags = NULLTAG;	

void trimLeading(char* str)
{
    int index, i;
    index = 0;
    /* Find last index of whitespace character */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }
    if(index != 0)
    {
        /* Shit all trailing characters to its left */
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; // Make sure that string is NULL terminated
    }
}
void trimTrailing(char* str)
{
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }
    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
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

// Count Total Dash in string.
int dashCount(char *x)
{
    int i,count=0;
    for(i=0;x[i]!='\0';i++)
    {
        if(x[i]==(char)(39))
        {
            count++;   
        }
    }
    return(count);
}
int spaceCount(char *x)
{
    int i,count=0;
    for(i=0;x[i]!='\0';i++)
    {
        if(x[i]==(char)(32))
        {
            count++;   
        }
    }
    return(count);
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

/* Query One Function Execution */

int queryData(char *queryName,char *qEntry,char* qValues,int queryPrmCount ,FILE *flogPtr)
{
	
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	int itr  = 0;
	int itr2 = 0;
	int itj  = 0;
	int itk1 = 0;
	int itk2 = 0;
	
	
	char *queryRealName    = NULL;	
	
	char *tEntryName       = (char*) MEM_alloc(100*sizeof(char));
	char *tValueName       = (char*) MEM_alloc(100*sizeof(char));

	
	char *queryEntryInput [queryPrmCount];
	char *queryValuesInput[queryPrmCount];
		
	tag_t      qryTag = NULLTAG;
    

	
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


/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile	     =   NULL;
    char *sGroup             =   NULL;
	char *sOtherUser         =   NULL;
	char *staskId            =   NULL;
	char *timeStr            =   NULL;
	char *timeStrUpdated     =   NULL;
    char *timeStrUpdatedNew  =   NULL;
	char *loggedInUser       =   NULL;
	char *loggedInUserId     =   NULL;
	char *wrkListName        =   NULL;
	char *wrkListString      =   NULL;
	char *taskObjName        =   NULL; 
	char *taskObjStr         =   NULL;
	char *taskObjType        =   NULL;
	char *taskJobName        =   NULL;
	char *taskWorkFlowName   =   NULL;
	char *reviewerName       =   NULL;
	
	
    char *logPath            = (char*) MEM_alloc(200*sizeof(char));
	char *queryInput         = (char*) MEM_alloc(500*sizeof(char));

							 				
	int itkApiCallStatus     = 0;
	int qryOutput            = 0;
	int itr                  = 0;
	int itr1                 = 0;
	int itr2                 = 0;
	int perSignOffTaskCount  = 0;
	int secObjCount          = 0;
	int taskReviewersCount   = 0;
	int nSignOffs            = 0;
	int nSignOffCount        = 0;

									
	tag_t loggedInUserTag         = NULLTAG;
	tag_t taskRootTaskTag         = NULLTAG;
	tag_t *wrkListTaskToPerform   = NULLTAG;
	tag_t *wrkListSecObj          = NULLTAG;
	tag_t *taskReviewersTags      = NULLTAG;
	tag_t *nSignOffTags           = NULLTAG;
	tag_t *nSignOffTag1           = NULLTAG;
	
	EPM_signoff_decision_t 	decision = EPM_approve_decision;
	
		
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-p=");
    sGroup         = ITK_ask_cli_argument("-g=");
	sOtherUser     = ITK_ask_cli_argument("-user=");
	staskId        = ITK_ask_cli_argument("-taskId=");
	
	//tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"PerformSignOffLogFile_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
		
	FILE *flogPtr=fopen(logPath,"w+");	
					
	fprintf(flogPtr,"- Date and Time = %s",timeStr);
	fprintf(flogPtr,"\n- Date and Time = %s",timeStrUpdatedNew);	
	fprintf(flogPtr,"\n- sUserName     = %s",sUserName      );
	fprintf(flogPtr,"\n- sPasswordFile = %s",sPasswordFile  );
	fprintf(flogPtr,"\n- sGroup        = %s",sGroup         );	
	fprintf(flogPtr,"\n- sOtherUser    = %s",sOtherUser     );	
	fprintf(flogPtr,"\n- staskId       = %s",staskId        );	
	
		
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    //itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
	
	itkApiCallStatus=ITK_APICALL(ITK_init_module(sUserName,sPasswordFile,sGroup),flogPtr); 
	
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
		
	if(itkApiCallStatus==ITK_ok)
    {
		printf("\n \n ************************************************************************** \n Login successful to teamcenter \n");	
        fprintf(flogPtr,"\n Login Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"\n Name of the logged in user = %s",loggedInUser);	
			
			itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(loggedInUserTag,"object_string",&loggedInUserId),flogPtr);		
			fprintf(flogPtr,"\n - loggedInUserId = %s",loggedInUserId);	
			
			if(tc_strcmp(loggedInUser,"infodba")==0)
			{
				fprintf(flogPtr,"\n Login User found = Infodba ");
				tc_strcpy(queryInput,"My Worklist");
				tc_strcat(queryInput,",");				
				tc_strcat(queryInput,"Muktesh Girdhani (mukteshg.ttl)");	 
 
				//tc_strcat(queryInput,"infodba (infodba)"); 
				qryOutput = queryData("General...","Type,Owning User",queryInput,2,flogPtr);
				fprintf(flogPtr,"\n - qryOutput = %d",qryOutput);
				if(qryOutput > 0)
				{
					for(itr = 0;itr<qryOutput;itr++)
					{
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[itr],"object_name",  &wrkListName  ),flogPtr);
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[itr],"object_string",&wrkListString),flogPtr);
						
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_tags(qryObjTags[itr],"tasks_to_perform",&perSignOffTaskCount,&wrkListTaskToPerform),flogPtr);
						itkApiCallStatus=ITK_APICALL(GRM_list_secondary_objects_only(qryObjTags[itr],NULLTAG,&secObjCount,&wrkListSecObj),flogPtr);
						
						
						fprintf(flogPtr,"\n - wrkListName         = %s",wrkListName  );
						fprintf(flogPtr,"\n - wrkListString       = %s",wrkListString);
						fprintf(flogPtr,"\n - perSignOffTaskCount = %d",perSignOffTaskCount);
						fprintf(flogPtr,"\n - secObjCount         = %d",secObjCount);
						
						for(itr1 = 0;itr1 < perSignOffTaskCount;itr1++)
						{
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"object_name",  &taskObjName),flogPtr);
						    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"object_string",&taskObjStr),flogPtr);
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"object_type",&taskObjType),flogPtr);
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"job_name",&taskJobName),flogPtr);
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_tag(wrkListTaskToPerform[itr1],"fnd0RootTask",&taskRootTaskTag),flogPtr);
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_tags(wrkListTaskToPerform[itr1],"awp0Reviewers",&taskReviewersCount,&taskReviewersTags),flogPtr);
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(taskRootTaskTag,"object_name",&taskWorkFlowName),flogPtr);
							
							fprintf(flogPtr,"\n **************************** itr1 = %d ******************************************* ",itr1+1);
							
							fprintf(flogPtr,"\n - taskObjName        = %s",taskObjName);
						    fprintf(flogPtr,"\n - taskObjStr         = %s",taskObjStr);
						    fprintf(flogPtr,"\n - taskObjType        = %s",taskObjType);
						    fprintf(flogPtr,"\n - taskJobName        = %s",taskJobName);		
							fprintf(flogPtr,"\n - taskWorkFlowName   = %s",taskWorkFlowName);	
                            fprintf(flogPtr,"\n - taskReviewersCount = %d",taskReviewersCount);								
							
							if(tc_strcmp(taskObjType,"EPMPerformSignoffTask")==0)
							{
								fprintf(flogPtr,"\n Target taskObjType Found");
								if(tc_strcmp(taskWorkFlowName,"Project access extension request process workflow")==0)
							    {
							        fprintf(flogPtr,"\n Target taskWorkFlowName Found ");
							    	if(tc_strcmp(taskObjName,"TML Admin team") == 0 || tc_strcmp(taskObjName,"TML Spoc") == 0)
									{
										fprintf(flogPtr,"\n Target taskObjName Found "); 
									    for(itr2 = 0;itr2<taskReviewersCount;itr2++)	
										{
											itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(taskReviewersTags[itr2],"object_name",&reviewerName),flogPtr);
											fprintf(flogPtr,"\n - reviewerName        = %s",reviewerName);
											if(tc_strstr(reviewerName,"Muktesh Girdhani (mukteshg.ttl)")!=NULL)																							
											{
												fprintf(flogPtr,"\n Target User Found with SignOff Rights"); 
												
												itkApiCallStatus=ITK_APICALL(EPM_create_adhoc_signoff(wrkListTaskToPerform[itr1],loggedInUserTag,&nSignOffCount,&nSignOffTag1),flogPtr);
												
												fprintf(flogPtr,"\n ITK API status for EPM_create_adhoc_signoff = %d",itkApiCallStatus);
												
												fprintf(flogPtr,"\n - nSignOffCount = %d",nSignOffCount);
												
												itkApiCallStatus=ITK_APICALL(EPM_set_adhoc_signoff_selection_done(wrkListTaskToPerform[itr1], true),flogPtr);
												
												fprintf(flogPtr,"\n ITK API status for EPM_set_adhoc_signoff_selection_done = %d",itkApiCallStatus);
												
																																						    
                                                itkApiCallStatus=ITK_APICALL(EPM_get_user_all_signoffs(wrkListTaskToPerform[itr1],&nSignOffs,&nSignOffTags),flogPtr);												
											 
											    fprintf(flogPtr,"\n - nSignOffs = %d",nSignOffs);	

												if(nSignOffs > 0)
												{																										  
													itkApiCallStatus=ITK_APICALL(EPM_set_task_decision3(wrkListTaskToPerform[itr1],nSignOffTags[0],decision,"TargetDone_Per"),flogPtr); 																								    
												    fprintf(flogPtr,"\n API status for EPM_set_task_decision3 = %d ",itkApiCallStatus);
												}  
												
											}											
										}											   
									}
							    }
							}
						}				
					}
				}												
			}
			else
			{
				fprintf(flogPtr,"\n Login User found != Infodba ");				
				tc_strcpy(queryInput,"My Worklist");
				tc_strcat(queryInput,",");				
				tc_strcat(queryInput,loggedInUserId);				
				qryOutput = queryData("General...","Type,Owning User",queryInput,2,flogPtr);
				fprintf(flogPtr,"\n - qryOutput = %d",qryOutput);
				if(qryOutput > 0)
				{
					for(itr = 0;itr<qryOutput;itr++)
					{
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[itr],"object_name",  &wrkListName  ),flogPtr);
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[itr],"object_string",&wrkListString),flogPtr);
						
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_tags(qryObjTags[itr],"tasks_to_perform",&perSignOffTaskCount,&wrkListTaskToPerform),flogPtr);
						itkApiCallStatus=ITK_APICALL(GRM_list_secondary_objects_only(qryObjTags[itr],NULLTAG,&secObjCount,&wrkListSecObj),flogPtr);
						
						
						fprintf(flogPtr,"\n - wrkListName         = %s",wrkListName  );
						fprintf(flogPtr,"\n - wrkListString       = %s",wrkListString);
						fprintf(flogPtr,"\n - perSignOffTaskCount = %d",perSignOffTaskCount);
						fprintf(flogPtr,"\n - secObjCount         = %d",secObjCount);
						
						for(itr1 = 0;itr1 < perSignOffTaskCount;itr1++)
						{
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"object_name",  &taskObjName),flogPtr);
						    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"object_string",&taskObjStr),flogPtr);
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"object_type",&taskObjType),flogPtr);
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(wrkListTaskToPerform[itr1],"job_name",&taskJobName),flogPtr);
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_tag(wrkListTaskToPerform[itr1],"fnd0RootTask",&taskRootTaskTag),flogPtr);
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_tags(wrkListTaskToPerform[itr1],"awp0Reviewers",&taskReviewersCount,&taskReviewersTags),flogPtr);
							
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(taskRootTaskTag,"object_name",&taskWorkFlowName),flogPtr);
							
							fprintf(flogPtr,"\n **************************** itr1 = %d ******************************************* ",itr1+1);
							
							fprintf(flogPtr,"\n - taskObjName        = %s",taskObjName);
						    fprintf(flogPtr,"\n - taskObjStr         = %s",taskObjStr);
						    fprintf(flogPtr,"\n - taskObjType        = %s",taskObjType);
						    fprintf(flogPtr,"\n - taskJobName        = %s",taskJobName);		
							fprintf(flogPtr,"\n - taskWorkFlowName   = %s",taskWorkFlowName);	
                            fprintf(flogPtr,"\n - taskReviewersCount = %d",taskReviewersCount);								
							
							if(tc_strcmp(taskObjType,"EPMPerformSignoffTask")==0)
							{
								fprintf(flogPtr,"\n Target taskObjType Found");
								if(tc_strcmp(taskWorkFlowName,"Project access extension request process workflow")==0)
							    {
							        fprintf(flogPtr,"\n Target taskWorkFlowName Found ");
							    	if(tc_strcmp(taskObjName,"TML Admin team") == 0 || tc_strcmp(taskObjName,"TML Spoc") == 0)
									{
										fprintf(flogPtr,"\n Target taskObjName Found "); 
									    for(itr2 = 0;itr2<taskReviewersCount;itr2++)	
										{
											itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(taskReviewersTags[itr2],"object_name",&reviewerName),flogPtr);
											fprintf(flogPtr,"\n - reviewerName        = %s",reviewerName);
											if(tc_strstr(reviewerName,loggedInUserId)!=NULL)
											{
												fprintf(flogPtr,"\n Target User Found with SignOff Rights"); 
																																			    
                                                itkApiCallStatus=ITK_APICALL(EPM_get_user_all_signoffs(wrkListTaskToPerform[itr1],&nSignOffs,&nSignOffTags),flogPtr);												
											 
											    fprintf(flogPtr,"\n - nSignOffs = %d",nSignOffs);	

												if(nSignOffs > 0)
												{																																							  
													itkApiCallStatus=ITK_APICALL(EPM_set_task_decision3(wrkListTaskToPerform[itr1],nSignOffTags[0],decision,"TargetDone_123456789"),flogPtr); 
													fprintf(flogPtr,"\n API status for EPM_set_task_decision3 = %d ",itkApiCallStatus);						    
												}
											}											
										}											   
									}
							    }
							}
						}				
					}
				}				
			}
			                       
            fprintf(flogPtr,"\n ******************** Process Completed Successfully ******************** \n ");										  	
		    fclose(flogPtr);																							
		}
	    else
	    {
		   printf("\n Login failed in TC \n");	
		   fprintf(flogPtr,"\n Error : Login failed in TC");
	    }
	}
	return(ITK_ok);
}