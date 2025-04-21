/*Date - 07-04-2025 */
 /* 
    Objective :- PLM AU Sap
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

// Global Declaration

char *todayTime;
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
	char *sUserName	         =  NULL;
    char *sPasswordFile	     =  NULL;
    char *sGroup             =  NULL;
	char *timeStr            =  NULL;
	char *timeStrUpdated     =  NULL;
    char *timeStrUpdatedNew  =  NULL;
	char *loggedInUser       =  NULL;
	char *trsAfter           =  NULL;
	char *trsBefore          =  NULL;
	char *plant              =  NULL;
	char *trgObjPartNo       =  NULL; 
	char *trgObjRev          =  NULL;
	char *trgObjSeq          =  NULL;
    char *logPath            = (char*) MEM_alloc(200*sizeof(char));
	char *csvDataInfo        = (char*) MEM_alloc(200*sizeof(char));
							 							 
	char *queryTime1         = (char*) MEM_alloc(100*sizeof(char));
	char *queryTime2         = (char*) MEM_alloc(100*sizeof(char));	
    char *qInput             = (char*) MEM_alloc(500*sizeof(char));
	               todayTime = (char*) MEM_alloc(100*sizeof(char));
	
    char *chkPartNo[25000]; 
    char *chkRev   [25000];
    char *chkSeq   [25000];	
	
	char filedata[500];
	char timeArray[100];
		
	int itkApiCallStatus = 0;
	int lineCount = 1;
	int tItr      = 0;
	int tItr2     = 0;
	
	int objCount = 0;
	int mCount   = 0;
										
	tag_t loggedInUserTag       = NULLTAG;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	trsAfter       = ITK_ask_cli_argument("-trsAfter=");
	trsBefore      = ITK_ask_cli_argument("-trsBefore=");
	plant          = ITK_ask_cli_argument("-plant=");
	
	
	//tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"PlmAuLogFileLogFile_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
			
	//tc_strcpy(csvDataInfo,"/proj/dmusession/jtcodes/ReportData/");
	tc_strcat(csvDataInfo,"PlmAuLogReport__UAPROD");
	tc_strcat(csvDataInfo,".csv");
		
	FILE *flogPtr=fopen(logPath,"w+");	
					
	fprintf(flogPtr,"  - Date and Time = %s\n",timeStr);
	fprintf(flogPtr,"  - Date and Time = %s\n",timeStrUpdatedNew);	
	fprintf(flogPtr,"  - sUserName     = %s\n",sUserName      );
	fprintf(flogPtr,"  - sPasswordFile = %s\n",sPasswordFile  );
	fprintf(flogPtr,"  - sGroup        = %s\n",sGroup         );
    fprintf(flogPtr,"  - trsAfter      = %s\n",trsAfter       );
	fprintf(flogPtr,"  - trsBefore     = %s\n",trsBefore      );
	fprintf(flogPtr,"  - csvDataInfo   = %s\n",csvDataInfo    );
				
	FILE *fcsvDataInfo  = fopen(csvDataInfo,"r");
		
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
		
	if(itkApiCallStatus==ITK_ok)
    {
		printf("\n \n ************************************************************************** \n Login successful to teamcenter \n");	
        fprintf(flogPtr,"\n Login Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"\n Name of the logged in user = %s",loggedInUser);	
			if(fcsvDataInfo== NULL)
		    {
		    	fcsvDataInfo = fopen(csvDataInfo,"a+");
		    	fprintf(flogPtr,"\n -csvDataInfo file not found so new file created and opened.");	                                                                                                                                                                                                                                                                                                                                   
		        fprintf(fcsvDataInfo,"Sr.no,Part No,Revision,Sequesnce\n");
			}
		    else
		    {
		    	fprintf(flogPtr,"\n File found so only opened in append mode.");					
		    	fcsvDataInfo = fopen(csvDataInfo,"a+");
		    }
							   
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,trsAfter);
			tc_strcat(qInput,",");
			tc_strcat(qInput,trsBefore);	
			tc_strcat(qInput,",");
			tc_strcat(qInput,plant);		
			
			objCount = queryData("PLMSAP AU LOG","Transfer_After,Transfer_Before,Plant",qInput,3,flogPtr);
			
			fprintf(flogPtr," - objCount = %d ",objCount);
			
		    if(objCount<=25000)
			{
				for(tItr = 0;tItr < objCount;tItr++)
				{
					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[tItr],"t5_Part_number",&trgObjPartNo),flogPtr);
                    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[tItr],"t5_Revision"   ,&trgObjRev   ),flogPtr);
                    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[tItr],"t5_Sequence"   ,&trgObjSeq   ),flogPtr);
				
				    fprintf(flogPtr,"\n %d,%s,%s,%s\n",tItr+1,trgObjPartNo,trgObjRev,trgObjSeq);
					chkPartNo[tItr] =  (char*) MEM_alloc(75*sizeof(char));
					chkRev[tItr]    =  (char*) MEM_alloc(5*sizeof(char));
				    chkSeq[tItr]    =  (char*) MEM_alloc(5*sizeof(char));
					
					tc_strcpy(chkPartNo[tItr],trgObjPartNo);
					tc_strcpy(chkRev[tItr],trgObjRev);
					tc_strcpy(chkSeq[tItr],trgObjSeq);
					
				}	
                for(tItr = 0;tItr < objCount;tItr++)
				{
					for(tItr2 = 0;tItr2 < objCount;tItr2++)
					{
					   if(tItr!=tItr2 && tc_strcmp(chkPartNo[tItr],"No")!=0 && tc_strcmp(chkPartNo[tItr],chkPartNo[tItr2])==0 && tc_strcmp(chkRev[tItr],chkRev[tItr2]) ==0 && tc_strcmp(chkSeq[tItr],chkSeq[tItr2]) ==0)	
					   {
						   fprintf(flogPtr,"\n ********************** Match Found ");
						   tc_strcpy(chkPartNo[tItr2],"No");
					   }						   
					}
				}
				
				for(tItr = 0;tItr < objCount;tItr++)
				{
					if(tc_strcmp(chkPartNo[tItr],"No")!=0)
					{
						fprintf(fcsvDataInfo,"%d,%s,%s,%s\n",mCount+1,chkPartNo[tItr],chkRev[tItr],chkSeq[tItr]);
						mCount++;
					}						
				}
			}
			else
			{
				fprintf(flogPtr,"\n ****************** Memory Allocation failed **********************");
			}
			
		}
	}
	return(0);
}




