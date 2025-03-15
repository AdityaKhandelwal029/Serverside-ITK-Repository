/*Date - 24-02-2025 */
 /* 
    Objective :- DataCollectionExe Client code to extract info of Non Erc Objects
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

int queryData(char *queryName,char *qEntry,char* qValues,char *rlsStatusToCheck,int queryPrmCount ,FILE *flogPtr)
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
    tag_t *qryObjTags = NULLTAG;	
	tag_t *rlsStatusList = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of queryData function started *********** \n");
	
	fprintf(flogPtr,"\n - queryName        = %s",queryName);
	fprintf(flogPtr,"\n - qEntry           = %s",qEntry);
	fprintf(flogPtr,"\n - qValues          = %s",qValues);
	fprintf(flogPtr,"\n - rlsStatusToCheck = %s",rlsStatusToCheck);
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
			for(itr=0;itr<qryObjCount;itr++)
			{
				itkApiCallStatus=ITK_APICALL(AOM_ask_value_tags(qryObjTags[itr],"release_status_list",&rlsStatusCount,&rlsStatusList),flogPtr);
			    fprintf(flogPtr,"\n -rlsStatusCount = %d",rlsStatusCount);				
				if(rlsStatusCount>0)
				{
				    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[itr],"item_id",     &tarObjId   ),flogPtr); 
					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryObjTags[itr],"object_name", &tarObjName ),flogPtr);
			    		
				    for(itr2=0;itr2<rlsStatusCount;itr2++)
					{
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(rlsStatusList[itr2],"object_name",&rlsStatusName),flogPtr);
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(rlsStatusList[itr2],"object_string",&rlsStatusObjStr),flogPtr);
						itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(rlsStatusList[itr2],"name",&rlsStatusName1),flogPtr);
												
						fprintf(flogPtr,"\n -rlsStatusName   = %s",rlsStatusName);
						fprintf(flogPtr,"\n -rlsStatusName1  = %s",rlsStatusName1);
						fprintf(flogPtr,"\n -rlsStatusObjStr = %s",rlsStatusObjStr);
						
						if(tc_strcmp(rlsStatusObjStr,rlsStatusToCheck)==0)
						{
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_date(rlsStatusList[itr2],"date_released",&rlsDate),flogPtr);									
						    itkApiCallStatus=ITK_APICALL(ITK_date_to_string(rlsDate,&dataRlsStr),flogPtr);							
							fprintf(flogPtr,"\n -dataRlsStr = %s",dataRlsStr);	
						
                            todaysDateStr = subString(todayTime,0,11);	
                            tokrlsDateStr = subString(dataRlsStr,0,11);										
							
							fprintf(flogPtr,"\n -todaysDateStr = %s",todaysDateStr);
							fprintf(flogPtr,"\n -tokrlsDateStr = %s",tokrlsDateStr);
																												
							if(tc_strcmp(todaysDateStr,tokrlsDateStr)==0)
							{
								fprintf(flogPtr,"\n Executing Condition where -todaysDateStr == tokrlsDateStr");
								fprintf(flogPtr,"\n ******************************************************************************************************************************");
					            fprintf(flogPtr,"\n -tarObjId   = %s",tarObjId  );
					            fprintf(flogPtr,"\n -tarObjName = %s",tarObjName);
								fprintf(flogPtr,"\n ******************************************************************************************************************************");						
								fRlsCount++;								
							}							
						}					    					
					}
				}
			}		
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
	return(fRlsCount);
}


/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =  NULL;
    char *sPasswordFile	     =  NULL;
    char *sGroup             =  NULL;
	char *timeStr            =  NULL;
	char *timeStrUpdated     =  NULL;
    char *timeStrUpdatedNew  =  NULL;
	char *loggedInUser       =  NULL;
	char *NtmpTime           = (char*) MEM_alloc(10*sizeof(char));
	char *day                = (char*) MEM_alloc(10*sizeof(char));
	char *month              = (char*) MEM_alloc(10*sizeof(char));
	char *date               = (char*) MEM_alloc(10*sizeof(char));
	char *tmpTime            = (char*) MEM_alloc(10*sizeof(char));
	char *year               = (char*) MEM_alloc(10*sizeof(char));
	char *tmp                = (char*) MEM_alloc(10*sizeof(char));
    char *logPath            = (char*) MEM_alloc(200*sizeof(char));
	char *csvDataInfo        = (char*) MEM_alloc(200*sizeof(char));
							 							 
	char *queryTime1         = (char*) MEM_alloc(100*sizeof(char));
	char *queryTime2         = (char*) MEM_alloc(100*sizeof(char));	
    char *qInput             = (char*) MEM_alloc(500*sizeof(char));
	               todayTime = (char*) MEM_alloc(100*sizeof(char));
	
	char filedata[500];
	char timeArray[100];
		
	int itkApiCallStatus = 0;
	int lineCount = 1;
	int tItr      = 0;
										
	tag_t loggedInUserTag       = NULLTAG;
	
	// 1.1.) Module
	
	int mfgModuleMbomRls = 0;
	int mfgModuleApljRls = 0;
	int mfgModuleApldRls = 0;
	int mfgModuleApllRls = 0;
	int mfgModuleApluRls = 0;
	int mfgModuleAplpRls = 0;
	int mfgModuleSsijRls = 0;
    int mfgModuleSsidRls = 0;
    int mfgModuleSsilRls = 0;
    int mfgModuleSsiuRls = 0;
    int mfgModuleSsipRls = 0;
	
	// 1.2.) Stage Assembly
	
	int sAMbomRlz = 0;
	int sAapljRlz = 0;
	int sAapldRlz = 0;
	int sAapllRlz = 0;
	int sAapluRlz = 0;
	int sAaplpRlz = 0;
	int sAssijRlz = 0;
	int sAssidRlz = 0;
	int sAssilRlz = 0;
	int sAssiuRlz = 0;
	int sAssipRlz = 0;
	
	// 2.) Raw Material -> T5_RawPartRevision
	
	int rawPrtAplcRlz = 0;
	int rawPrtAplaRlz = 0;
	int rawPrtAplsRlz = 0;
	int rawPrtApljRlz = 0;
	int rawPrtApldRlz = 0;
	int rawPrtApllRlz = 0;
	int rawPrtApluRlz = 0;
	int rawPrtAplpRlz = 0;
	int rawPrtSsicRlz = 0;
	int rawPrtSsisRlz = 0;
	int rawPrtSsiaRlz = 0;
	int rawPrtSsijRlz = 0;
	int rawPrtSsidRlz = 0;
	int rawPrtSsilRlz = 0;
	int rawPrtSsiuRlz = 0;
	int rawPrtSsipRlz = 0;
	
	// 3.1) MBOM DML -> T5_MBOMDMLRevision
	
	int mbomDmlRevMbomRls = 0;
    int mbomDmlRevApljRls = 0;
    int mbomDmlRevApldRls = 0;
    int mbomDmlRevApllRls = 0;
    int mbomDmlRevApluRls = 0;
    int mbomDmlRevAplpRls = 0;
    int mbomDmlRevSsijRls = 0;
    int mbomDmlRevSsidRls = 0;
    int mbomDmlRevSsilRls = 0;
    int mbomDmlRevSsiuRls = 0;
    int mbomDmlRevSsipRls = 0;
	 
	// 3.2) MBOM TASK -> T5_MBOMTASKRevision
	
	int mbomTaskRevMbomRls = 0;
    int mbomTaskRevApljRls = 0;
    int mbomTaskRevApldRls = 0;
    int mbomTaskRevApllRls = 0;
    int mbomTaskRevApluRls = 0;
    int mbomTaskRevAplpRls = 0;
    int mbomTaskRevSsijRls = 0;
    int mbomTaskRevSsidRls = 0;
    int mbomTaskRevSsilRls = 0;
    int mbomTaskRevSsiuRls = 0;
    int mbomTaskRevSsipRls = 0;
	
	// 4.1) APL DML -> T5_APLDMLRevision
	
	int aplDmlAplcRlz = 0;
	int aplDmlAplaRlz = 0;
	int aplDmlAplsRlz = 0;
	int aplDmlApljRlz = 0;
	int aplDmlApldRlz = 0;
	int aplDmlApllRlz = 0;
	int aplDmlApluRlz = 0;
	int aplDmlAplpRlz = 0;
	int aplDmlSsicRlz = 0;
	int aplDmlSsisRlz = 0;
	int aplDmlSsiaRlz = 0;
	int aplDmlSsijRlz = 0;
	int aplDmlSsidRlz = 0;
	int aplDmlSsilRlz = 0;
	int aplDmlSsiuRlz = 0;
	int aplDmlSsipRlz = 0;
	
	
	// 4.2) APL TASK -> T5_APLTaskRevision
	
	int aplTaskAplcRlz = 0;
	int aplTaskAplaRlz = 0;
	int aplTaskAplsRlz = 0;
	int aplTaskApljRlz = 0;
	int aplTaskApldRlz = 0;
	int aplTaskApllRlz = 0;
	int aplTaskApluRlz = 0;
	int aplTaskAplpRlz = 0;
	int aplTaskSsicRlz = 0;
	int aplTaskSsisRlz = 0;
	int aplTaskSsiaRlz = 0;
	int aplTaskSsijRlz = 0;
	int aplTaskSsidRlz = 0;
	int aplTaskSsilRlz = 0;
	int aplTaskSsiuRlz = 0;
	int aplTaskSsipRlz = 0;
	
	// 5.) Estimate Sheet -> T5_EstimateSheetRevision
	
	int estSheetRevAplFinalRls = 0; 
    int estSheetRevPsdFinalRls = 0;
	
	// 6.) EPA -> T5_EPARevision 
	
	//int epaSsicRls = 0; // Not for CV
    //int epaSsisRls = 0; // Not for CV
    //int epaSsiaRls = 0; // Not for CV
    int epaSsijRls = 0;
    int epaSsidRls = 0;
    int epaSsilRls = 0;
    int epaSsiuRls = 0;
    int epaSsipRls = 0;
	
	// 7.) MBPA -> T5_MBPAMRevision
	
	int mbpamSsicRls = 0; // Not for CV
    int mbpamSsisRls = 0; // Not for CV
    int mbpamSsiaRls = 0; // Not for CV
    int mbpamSsijRls = 0;
    int mbpamSsidRls = 0;
    int mbpamSsilRls = 0;
    int mbpamSsiuRls = 0;
    int mbpamSsipRls = 0;
	
	
		
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	
	tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"DataCollectionNonErcLogFile_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
			
	tc_strcpy(csvDataInfo,"/proj/dmusession/jtcodes/ReportData/");
	tc_strcat(csvDataInfo,"DataCollectionExeInfoNonErc_CVPROD");
	tc_strcat(csvDataInfo,".csv");
		
	FILE *flogPtr=fopen(logPath,"w+");	
					
	fprintf(flogPtr,"  - Date and Time = %s\n",timeStr);
	fprintf(flogPtr,"  - Date and Time = %s\n",timeStrUpdatedNew);	
	fprintf(flogPtr,"  - sUserName     = %s\n",sUserName      );
	fprintf(flogPtr,"  - sPasswordFile = %s\n",sPasswordFile  );
	fprintf(flogPtr,"  - sGroup        = %s\n",sGroup         );	
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
		    	fprintf(fcsvDataInfo,"Data and Time,Module MBOM Released,Module APLJ Released,Module APLD Released,Module APLL Released,Module APLU Released,Module APLP Released,Module STDSIJ Released,Module STDSID Released,Module STDSIL Released,Module STDSIU Released,Module STDSIP Released,Stage Assembly MBOM Released,Stage Assembly APLJ Released,Stage Assembly APLD Released,Stage Assembly APLL Released,Stage Assembly APLU Released,Stage Assembly APLP Released,Stage Assembly STDSIJ Released,Stage Assembly STDSID Released,Stage Assembly STDSIL Released,Stage Assembly STDSIU Released,Stage Assembly STDSIP Released,Raw Material APLC Released,Raw Material APLA Released,Raw Material APLS Released,Raw Material APLJ Released,Raw Material APLD Released,Raw Material APLL Released,Raw Material APLU Released,Raw Material APLP Released,Raw Material STDSIC Released,Raw Material STDSIS Released,Raw Material STDSIA Released,Raw Material STDSIJ Released,Raw Material STDSID Released,Raw Material STDSIL Released,Raw Material STDSIU Released,Raw Material STDSIP Released,MBOM DML MBOM Released,MBOM DML APLJ Released,MBOM DML APLD Released,MBOM DML APLL Released,MBOM DML APLU Released,MBOM DML APLP Released,MBOM DML STDSIJ Released,MBOM DML STDSID Released,MBOM DML STDSIL Released,MBOM DML STDSIU Released,MBOM DML STDSIP Released,MBOM Task MBOM Released,MBOM Task APLJ Released,MBOM Task APLD Released,MBOM Task APLL Released,MBOM Task APLU Released,MBOM Task APLP Released,MBOM Task STDSIJ Released,MBOM Task STDSID Released,MBOM Task STDSIL Released,MBOM Task STDSIU Released,MBOM Task STDSIP Released,APL DML APLC Released,APL DML APLA Released,APL DML APLS Released,APL DML APLJ Released,APL DML APLD Released,APL DML APLL Released,APL DML APLU Released,APL DML APLP Released,APL DML STDSIC Released,APL DML STDSIS Released,APL DML STDSIA Released,APL DML STDSIJ Released,APL DML STDSID Released,APL DML STDSIL Released,APL DML STDSIU Released,APL DML STDSIP Released,APL Task APLC Released,APL Task APLA Released,APL Task APLS Released,APL Task APLJ Released,APL Task APLD Released,APL Task APLL Released,APL Task APLU Released,APL Task APLP Released,APL Task STDSIC Released,APL Task STDSIS Released,APL Task STDSIA Released,APL Task STDSIJ Released,APL Task STDSID Released,APL Task STDSIL Released,APL Task STDSIU Released,APL Task STDSIP Released,Estimate Sheet APL Final Released,Estimate Sheet PSD Final Released,EPA STDSIJ Released,EPA STDSID Released,EPA STDSIL Released,EPA STDSIU Released,EPA STDSIP Released,MBPA STDSIJ Released,MBPA STDSID Released,MBPA STDSIL Released,MBPA STDSIU Released,MBPA STDSIP Released");	                                                                                                                                                                                                                                                                                                                                                               
		    }
		    else
		    {
		    	fprintf(flogPtr,"\n File found so only opened in append mode.");					
		    	fcsvDataInfo = fopen(csvDataInfo,"a+");
		    }
			for(tItr=0;timeStr[tItr]!='\0';tItr++)
            {
                timeArray[tItr]=timeStr[tItr];
            }
			
	        timeArray[tItr]='\0';
	        day     = tc_strtok(timeArray," ");
	        month   = tc_strtok(NULL," "); 	
	        date    = tc_strtok(NULL," ");
	        if(tc_strlen(date)==0&&tc_strcmp(date,"")==0)
	        {
	           fprintf(flogPtr,"\n -date    = %d\n",tc_strlen(date));
	           date = tc_strtok(NULL," ");
	           fprintf(flogPtr," ***** Double toking used for date command ***** \n");
	        }
	        tmpTime = tc_strtok(NULL," ");				
	        year    = tc_strtok(NULL," ");
			
			strncpy(NtmpTime,tmpTime,5);	            
	        fprintf(flogPtr,"\n- day      = %s",day);
	        fprintf(flogPtr,"\n- month    = %s",month); 	
	        fprintf(flogPtr,"\n- date     = %s",date); 	
	        fprintf(flogPtr,"\n- tmpTime  = %s",tmpTime); 
			fprintf(flogPtr,"\n- NtmpTime = %s",NtmpTime); 
	        fprintf(flogPtr,"\n- year     = %s",year); 	
	        
	        if(tc_strlen(date)==1)
	        {
	        	tc_strcpy(tmp,"");
	        	tc_strcpy(tmp,"0");
	        	tc_strcat(tmp,date);
	        	
	        	tc_strcpy(date,tmp);
	        	fprintf(flogPtr," -newDate = %s\n",date);
	        }
	        	            	            
	        tc_strcpy(queryTime1,"");
	        tc_strcat(queryTime1,date);
	        tc_strcat(queryTime1,"-");
	        tc_strcat(queryTime1,month);
	        tc_strcat(queryTime1,"-");
	        tc_strcat(queryTime1,year);
	        tc_strcat(queryTime1," ");
	        tc_strcat(queryTime1,"06:00");
	        
	        fprintf(flogPtr,"\n -queryTime1 = %s",queryTime1);
			
			tc_strcpy(todayTime,queryTime1);
			
			fprintf(flogPtr,"\n -todayTime = %s",todayTime);
			
	        tc_strcpy(queryTime2,"");
	        tc_strcat(queryTime2,date);
	        tc_strcat(queryTime2,"-");
	        tc_strcat(queryTime2,month);
	        tc_strcat(queryTime2,"-");
	        tc_strcat(queryTime2,year);
	        tc_strcat(queryTime2," ");
	        tc_strcat(queryTime2,NtmpTime);
	        
	        fprintf(flogPtr,"\n -queryTime2 = %s",queryTime2);
			
			// 1.1) For type - > Mfg module		
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MBOM Released");
            tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");		
			
			mfgModuleMbomRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"MBOM Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
            tc_strcat(qInput,",");
 			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
			
			mfgModuleApljRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLJ Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
            tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
									
			mfgModuleApldRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLD Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
            tc_strcat(qInput,","); 
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
						
			mfgModuleApllRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLL Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
						
			mfgModuleApluRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLU Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLP Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");	
			
			mfgModuleAplpRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLP Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");	
			
			mfgModuleSsijRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIJ Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
            tc_strcat(qInput,",");			
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
			
			mfgModuleSsidRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSID Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");			
			mfgModuleSsilRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIL Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
			mfgModuleSsiuRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIU Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Module");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
									
			mfgModuleSsipRls = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIP Released",4,flogPtr);
			
									
			fprintf(flogPtr," - mfgModuleMbomRls = %d\n",mfgModuleMbomRls);
			fprintf(flogPtr," - mfgModuleApljRls = %d\n",mfgModuleApljRls);
			fprintf(flogPtr," - mfgModuleApldRls = %d\n",mfgModuleApldRls);
			fprintf(flogPtr," - mfgModuleApllRls = %d\n",mfgModuleApllRls);
			fprintf(flogPtr," - mfgModuleAplpRls = %d\n",mfgModuleAplpRls);
			fprintf(flogPtr," - mfgModuleAplpRls = %d\n",mfgModuleAplpRls);
			fprintf(flogPtr," - mfgModuleSsijRls = %d\n",mfgModuleSsijRls);
			fprintf(flogPtr," - mfgModuleSsidRls = %d\n",mfgModuleSsidRls);
			fprintf(flogPtr," - mfgModuleSsilRls = %d\n",mfgModuleSsilRls);
			fprintf(flogPtr," - mfgModuleSsiuRls = %d\n",mfgModuleSsiuRls);
			fprintf(flogPtr," - mfgModuleSsipRls = %d\n",mfgModuleSsipRls);
								                    
            // 1.2) For type - > Stage Assembly 
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MBOM Released");
            tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");		
			
			sAMbomRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"MBOM Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
            tc_strcat(qInput,",");
 			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
			
			sAapljRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLJ Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
            tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
									
			sAapldRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLD Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
            tc_strcat(qInput,","); 
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
						
			sAapllRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLL Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
						
			sAapluRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLU Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLP Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");	
			
			sAaplpRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"APLP Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");	
			
			sAssijRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIJ Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
            tc_strcat(qInput,",");			
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
			
			sAssidRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSID Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");			
			sAssilRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIL Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");	
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
			sAssiuRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIU Released",4,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Stage Assembly");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MD9Y01:MA9Z01:MH9Z02:MD9Y02:MF9Y01:MH9W01:MH9W02:MD9X01:MD9X02:MD9W01:MD9Z01");
									
			sAssipRlz = queryData("Design Revision","Part Type,Released After,Release Status,Module Address",qInput,"STDSIP Released",4,flogPtr);
											
			fprintf(flogPtr," - sAMbomRlz = %d\n",sAMbomRlz);
			fprintf(flogPtr," - sAapljRlz = %d\n",sAapljRlz);
			fprintf(flogPtr," - sAapldRlz = %d\n",sAapldRlz);
			fprintf(flogPtr," - sAapllRlz = %d\n",sAapllRlz);
			fprintf(flogPtr," - sAapluRlz = %d\n",sAapluRlz);
			fprintf(flogPtr," - sAaplpRlz = %d\n",sAaplpRlz);
			fprintf(flogPtr," - sAssijRlz = %d\n",sAssijRlz);
			fprintf(flogPtr," - sAssidRlz = %d\n",sAssidRlz);
			fprintf(flogPtr," - sAssilRlz = %d\n",sAssilRlz);
			fprintf(flogPtr," - sAssiuRlz = %d\n",sAssiuRlz);
			fprintf(flogPtr," - sAssipRlz = %d\n",sAssipRlz);
			
			// 2.) Raw Material 
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLC Released");
			
			rawPrtAplcRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLC Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLA Released");
			
			rawPrtAplaRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLA Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLS Released");
			
			rawPrtAplsRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLS Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
			
			
			rawPrtApljRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
			
			rawPrtApldRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLD Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
			
			rawPrtApllRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			
			rawPrtApluRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLP Released");
			
			rawPrtAplpRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLP Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIC Released");
			
			rawPrtSsicRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIC Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIS Released");
			
			rawPrtSsisRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIS Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIA Released");
			
			rawPrtSsiaRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIA Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");
			
			rawPrtSsijRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
			
			rawPrtSsidRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");
			
			rawPrtSsilRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");
			
			rawPrtSsiuRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Raw Part Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			
			rawPrtSsipRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);
			
			fprintf(flogPtr," - rawPrtAplcRlz = %d\n",rawPrtAplcRlz);
			fprintf(flogPtr," - rawPrtAplaRlz = %d\n",rawPrtAplaRlz);
			fprintf(flogPtr," - rawPrtAplsRlz = %d\n",rawPrtAplsRlz);
			fprintf(flogPtr," - rawPrtApljRlz = %d\n",rawPrtApljRlz);
			fprintf(flogPtr," - rawPrtApldRlz = %d\n",rawPrtApldRlz);
			fprintf(flogPtr," - rawPrtApllRlz = %d\n",rawPrtApllRlz);
			fprintf(flogPtr," - rawPrtApluRlz = %d\n",rawPrtApluRlz);
			fprintf(flogPtr," - rawPrtAplpRlz = %d\n",rawPrtAplpRlz);
			fprintf(flogPtr," - rawPrtSsicRlz = %d\n",rawPrtSsicRlz);
			fprintf(flogPtr," - rawPrtSsisRlz = %d\n",rawPrtSsisRlz);
			fprintf(flogPtr," - rawPrtSsiaRlz = %d\n",rawPrtSsiaRlz);
			fprintf(flogPtr," - rawPrtSsijRlz = %d\n",rawPrtSsijRlz);
			fprintf(flogPtr," - rawPrtSsidRlz = %d\n",rawPrtSsidRlz);
			fprintf(flogPtr," - rawPrtSsilRlz = %d\n",rawPrtSsilRlz);
			fprintf(flogPtr," - rawPrtSsiuRlz = %d\n",rawPrtSsiuRlz);
			fprintf(flogPtr," - rawPrtSsipRlz = %d\n",rawPrtSsipRlz);
			
			// 3.1) MBOM DML REV 
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"MBOM Released");
			
			mbomDmlRevMbomRls = queryData("General...","Type,Released After,Release Status",qInput,"MBOM Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
			
			
            mbomDmlRevApljRls = queryData("General...","Type,Released After,Release Status",qInput,"APLJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
			
			
            mbomDmlRevApldRls = queryData("General...","Type,Released After,Release Status",qInput,"APLD Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
			
            mbomDmlRevApllRls = queryData("General...","Type,Released After,Release Status",qInput,"APLL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			
			
            mbomDmlRevApluRls = queryData("General...","Type,Released After,Release Status",qInput,"APLU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
		    tc_strcat(qInput,"APLP Released");
			
            mbomDmlRevAplpRls = queryData("General...","Type,Released After,Release Status",qInput,"APLP Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");
			
            mbomDmlRevSsijRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
			
            mbomDmlRevSsidRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");
			
            mbomDmlRevSsilRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");
			
            mbomDmlRevSsiuRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"MBOMDML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			
            mbomDmlRevSsipRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);
			
			fprintf(flogPtr," - mbomDmlRevMbomRls = %d\n",mbomDmlRevMbomRls);
            fprintf(flogPtr," - mbomDmlRevApljRls = %d\n",mbomDmlRevApljRls);
            fprintf(flogPtr," - mbomDmlRevApldRls = %d\n",mbomDmlRevApldRls);
            fprintf(flogPtr," - mbomDmlRevApllRls = %d\n",mbomDmlRevApllRls);
            fprintf(flogPtr," - mbomDmlRevApluRls = %d\n",mbomDmlRevApluRls);
            fprintf(flogPtr," - mbomDmlRevAplpRls = %d\n",mbomDmlRevAplpRls);
            fprintf(flogPtr," - mbomDmlRevSsijRls = %d\n",mbomDmlRevSsijRls);
            fprintf(flogPtr," - mbomDmlRevSsidRls = %d\n",mbomDmlRevSsidRls);
            fprintf(flogPtr," - mbomDmlRevSsilRls = %d\n",mbomDmlRevSsilRls);
            fprintf(flogPtr," - mbomDmlRevSsiuRls = %d\n",mbomDmlRevSsiuRls);
            fprintf(flogPtr," - mbomDmlRevSsipRls = %d\n",mbomDmlRevSsipRls);
			
			// 3.2) MBOM TASK REV 
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"MBOM Released");
			
			mbomTaskRevMbomRls = queryData("General...","Type,Released After,Release Status",qInput,"MBOM Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
			
            mbomTaskRevApljRls = queryData("General...","Type,Released After,Release Status",qInput,"APLJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
			
            mbomTaskRevApldRls = queryData("General...","Type,Released After,Release Status",qInput,"APLD Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
			
            mbomTaskRevApllRls = queryData("General...","Type,Released After,Release Status",qInput,"APLL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			
            mbomTaskRevApluRls = queryData("General...","Type,Released After,Release Status",qInput,"APLU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"APLP Released");
			
            mbomTaskRevAplpRls = queryData("General...","Type,Released After,Release Status",qInput,"APLP Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");
			
			
            mbomTaskRevSsijRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
			
            mbomTaskRevSsidRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");
			
            mbomTaskRevSsilRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");
			
            mbomTaskRevSsiuRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBOMTASK Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			
            mbomTaskRevSsipRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);
			
			fprintf(flogPtr," - mbomTaskRevMbomRls = %d\n",mbomTaskRevMbomRls);
			fprintf(flogPtr," - mbomTaskRevApljRls = %d\n",mbomTaskRevApljRls);
			fprintf(flogPtr," - mbomTaskRevApldRls = %d\n",mbomTaskRevApldRls);
			fprintf(flogPtr," - mbomTaskRevApllRls = %d\n",mbomTaskRevApllRls);
			fprintf(flogPtr," - mbomTaskRevApluRls = %d\n",mbomTaskRevApluRls);
			fprintf(flogPtr," - mbomTaskRevAplpRls = %d\n",mbomTaskRevAplpRls);
			fprintf(flogPtr," - mbomTaskRevSsijRls = %d\n",mbomTaskRevSsijRls);
			fprintf(flogPtr," - mbomTaskRevSsidRls = %d\n",mbomTaskRevSsidRls);
			fprintf(flogPtr," - mbomTaskRevSsilRls = %d\n",mbomTaskRevSsilRls);
			fprintf(flogPtr," - mbomTaskRevSsiuRls = %d\n",mbomTaskRevSsiuRls);
			fprintf(flogPtr," - mbomTaskRevSsipRls = %d\n",mbomTaskRevSsipRls);
			
			// 4.1) T5_APLDMLRevision 
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLC Released");
			
			aplDmlAplcRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLC Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLA Released");
			
			aplDmlAplaRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLA Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLS Released");
			
			aplDmlAplsRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLS Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
			
			
			aplDmlApljRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
			
			aplDmlApldRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLD Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
			
			aplDmlApllRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			
			aplDmlApluRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLP Released");
			
			aplDmlAplpRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLP Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIC Released");
			
			aplDmlSsicRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIC Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIS Released");
			
			aplDmlSsisRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIS Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIA Released");
			
			aplDmlSsiaRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIA Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");
			
			aplDmlSsijRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
			
			aplDmlSsidRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");
			
			aplDmlSsilRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");
			
			aplDmlSsiuRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL DML Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			
			aplDmlSsipRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);
			
			fprintf(flogPtr," - aplDmlAplcRlz = %d\n",aplDmlAplcRlz);
            fprintf(flogPtr," - aplDmlAplaRlz = %d\n",aplDmlAplaRlz);
            fprintf(flogPtr," - aplDmlAplsRlz = %d\n",aplDmlAplsRlz);
            fprintf(flogPtr," - aplDmlApljRlz = %d\n",aplDmlApljRlz);
            fprintf(flogPtr," - aplDmlApldRlz = %d\n",aplDmlApldRlz);
            fprintf(flogPtr," - aplDmlApllRlz = %d\n",aplDmlApllRlz);
            fprintf(flogPtr," - aplDmlApluRlz = %d\n",aplDmlApluRlz);
            fprintf(flogPtr," - aplDmlAplpRlz = %d\n",aplDmlAplpRlz);
            fprintf(flogPtr," - aplDmlSsicRlz = %d\n",aplDmlSsicRlz);
            fprintf(flogPtr," - aplDmlSsisRlz = %d\n",aplDmlSsisRlz);
            fprintf(flogPtr," - aplDmlSsiaRlz = %d\n",aplDmlSsiaRlz);
            fprintf(flogPtr," - aplDmlSsijRlz = %d\n",aplDmlSsijRlz);
            fprintf(flogPtr," - aplDmlSsidRlz = %d\n",aplDmlSsidRlz);
            fprintf(flogPtr," - aplDmlSsilRlz = %d\n",aplDmlSsilRlz);
            fprintf(flogPtr," - aplDmlSsiuRlz = %d\n",aplDmlSsiuRlz);
            fprintf(flogPtr," - aplDmlSsipRlz = %d\n",aplDmlSsipRlz);
						
			// 4.2) T5_APLTASKRevision
						
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLC Released");
			
			aplTaskAplcRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLC Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLA Released");
			
			aplTaskAplaRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLA Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLS Released");
			
			aplTaskAplsRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLS Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLJ Released");
			
			
			aplTaskApljRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLD Released");
			
			aplTaskApldRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLD Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLL Released");
			
			aplTaskApllRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLU Released");
			
			aplTaskApluRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APLP Released");
			
			aplTaskAplpRlz = queryData("General...","Type,Released After,Release Status",qInput,"APLP Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIC Released");
			
			aplTaskSsicRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIC Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIS Released");
			
			aplTaskSsisRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIS Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIA Released");
			
			aplTaskSsiaRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIA Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIJ Released");
			
			aplTaskSsijRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSID Released");
			
			aplTaskSsidRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIL Released");
			
			aplTaskSsilRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIU Released");
			
			aplTaskSsiuRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);
			
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"APL Task Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"STDSIP Released");
			
			aplTaskSsipRlz = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);
			
			fprintf(flogPtr," - aplTaskAplcRlz = %d\n",aplTaskAplcRlz);
			fprintf(flogPtr," - aplTaskAplaRlz = %d\n",aplTaskAplaRlz);
			fprintf(flogPtr," - aplTaskAplsRlz = %d\n",aplTaskAplsRlz);
			fprintf(flogPtr," - aplTaskApljRlz = %d\n",aplTaskApljRlz);
			fprintf(flogPtr," - aplTaskApldRlz = %d\n",aplTaskApldRlz);
			fprintf(flogPtr," - aplTaskApllRlz = %d\n",aplTaskApllRlz);
			fprintf(flogPtr," - aplTaskApluRlz = %d\n",aplTaskApluRlz);
			fprintf(flogPtr," - aplTaskAplpRlz = %d\n",aplTaskAplpRlz);
			fprintf(flogPtr," - aplTaskSsicRlz = %d\n",aplTaskSsicRlz);
			fprintf(flogPtr," - aplTaskSsisRlz = %d\n",aplTaskSsisRlz);
			fprintf(flogPtr," - aplTaskSsiaRlz = %d\n",aplTaskSsiaRlz);
			fprintf(flogPtr," - aplTaskSsijRlz = %d\n",aplTaskSsijRlz);
			fprintf(flogPtr," - aplTaskSsidRlz = %d\n",aplTaskSsidRlz);
			fprintf(flogPtr," - aplTaskSsilRlz = %d\n",aplTaskSsilRlz);
			fprintf(flogPtr," - aplTaskSsiuRlz = %d\n",aplTaskSsiuRlz);
			fprintf(flogPtr," - aplTaskSsipRlz = %d\n",aplTaskSsipRlz);
													
													
			// 5.) T5_EstimateSheetRevision		
 
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Estimate Sheet Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"APL Final Released");
			
			estSheetRevAplFinalRls = queryData("General...","Type,Released After,Release Status",qInput,"APL Final Released",3,flogPtr);
													
			tc_strcpy(qInput,"");
			tc_strcpy(qInput,"Estimate Sheet Revision");
			tc_strcat(qInput,",");
			tc_strcat(qInput,queryTime1);
			tc_strcat(qInput,",");
			tc_strcat(qInput,"PSD Final Released");
			
			estSheetRevPsdFinalRls = queryData("General...","Type,Released After,Release Status",qInput,"PSD Final Released",3,flogPtr);										
													
			fprintf(flogPtr," - estSheetRevAplFinalRls = %d\n",estSheetRevAplFinalRls);
			fprintf(flogPtr," - estSheetRevPsdFinalRls = %d\n",estSheetRevPsdFinalRls);
			
			
			
			// 6.) EPA 
						
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"EPA Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIJ Released");	
			
			epaSsijRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"EPA Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSID Released");
			
			epaSsidRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);	
				
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"EPA Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIL Released");	
			
			epaSsilRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"EPA Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIU Released");
			
			epaSsiuRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"EPA Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIP Released");		
			epaSsipRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);	
			
            fprintf(flogPtr," - epaSsijRls = %d\n",epaSsijRls);
            fprintf(flogPtr," - epaSsidRls = %d\n",epaSsidRls);
            fprintf(flogPtr," - epaSsilRls = %d\n",epaSsilRls);
            fprintf(flogPtr," - epaSsiuRls = %d\n",epaSsiuRls);
            fprintf(flogPtr," - epaSsipRls = %d\n",epaSsipRls);
															
			// 7.) MBPA		
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBPA (NON-EPA) Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIJ Released");
			
            mbpamSsijRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIJ Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBPA (NON-EPA) Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSID Released");
			
            mbpamSsidRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSID Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBPA (NON-EPA) Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIL Released");
			
            mbpamSsilRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIL Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBPA (NON-EPA) Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIU Released");
			
            mbpamSsiuRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIU Released",3,flogPtr);	
			
			tc_strcpy(qInput,"");
            tc_strcpy(qInput,"MBPA (NON-EPA) Revision");
            tc_strcat(qInput,",");
            tc_strcat(qInput,queryTime1);
            tc_strcat(qInput,",");
            tc_strcat(qInput,"STDSIP Released");
			
            mbpamSsipRls = queryData("General...","Type,Released After,Release Status",qInput,"STDSIP Released",3,flogPtr);	
			
            fprintf(flogPtr," - mbpamSsijRls = %d\n",mbpamSsijRls);
            fprintf(flogPtr," - mbpamSsidRls = %d\n",mbpamSsidRls);
            fprintf(flogPtr," - mbpamSsilRls = %d\n",mbpamSsilRls);
            fprintf(flogPtr," - mbpamSsiuRls = %d\n",mbpamSsiuRls);
            fprintf(flogPtr," - mbpamSsipRls = %d\n",mbpamSsipRls);


			// Printing Final report in CSV file.
			
			fprintf(fcsvDataInfo,"\n %s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",timeStr,
			mfgModuleMbomRls,mfgModuleApljRls,mfgModuleApldRls,mfgModuleApllRls,mfgModuleApluRls,mfgModuleAplpRls,mfgModuleSsijRls,mfgModuleSsidRls,mfgModuleSsilRls,mfgModuleSsiuRls,mfgModuleSsipRls,
			sAMbomRlz,sAapljRlz,sAapldRlz,sAapllRlz,sAapluRlz,sAaplpRlz,sAssijRlz,sAssidRlz,sAssilRlz,sAssiuRlz,sAssipRlz,
			rawPrtAplcRlz,rawPrtAplaRlz,rawPrtAplsRlz,rawPrtApljRlz,rawPrtApldRlz,rawPrtApllRlz,rawPrtApluRlz,rawPrtAplpRlz,rawPrtSsicRlz,rawPrtSsisRlz,rawPrtSsiaRlz,rawPrtSsijRlz,rawPrtSsidRlz,rawPrtSsilRlz,rawPrtSsiuRlz,rawPrtSsipRlz,
			mbomDmlRevMbomRls,mbomDmlRevApljRls,mbomDmlRevApldRls,mbomDmlRevApllRls,mbomDmlRevApluRls,mbomDmlRevAplpRls,mbomDmlRevSsijRls,mbomDmlRevSsidRls,mbomDmlRevSsilRls,mbomDmlRevSsiuRls,mbomDmlRevSsipRls,
			mbomTaskRevMbomRls,mbomTaskRevApljRls,mbomTaskRevApldRls,mbomTaskRevApllRls,mbomTaskRevApluRls,mbomTaskRevAplpRls,mbomTaskRevSsijRls,mbomTaskRevSsidRls,mbomTaskRevSsilRls,mbomTaskRevSsiuRls,mbomTaskRevSsipRls,
			aplDmlAplcRlz,aplDmlAplaRlz,aplDmlAplsRlz,aplDmlApljRlz,aplDmlApldRlz,aplDmlApllRlz,aplDmlApluRlz,aplDmlAplpRlz,aplDmlSsicRlz,aplDmlSsisRlz,aplDmlSsiaRlz,aplDmlSsijRlz,aplDmlSsidRlz,aplDmlSsilRlz,aplDmlSsiuRlz,aplDmlSsipRlz,
			aplTaskAplcRlz,aplTaskAplaRlz,aplTaskAplsRlz,aplTaskApljRlz,aplTaskApldRlz,aplTaskApllRlz,aplTaskApluRlz,aplTaskAplpRlz,aplTaskSsicRlz,aplTaskSsisRlz,aplTaskSsiaRlz,aplTaskSsijRlz,aplTaskSsidRlz,aplTaskSsilRlz,aplTaskSsiuRlz,aplTaskSsipRlz,
			estSheetRevAplFinalRls,estSheetRevPsdFinalRls,
			epaSsijRls,epaSsidRls,epaSsilRls,epaSsiuRls,epaSsipRls,
			mbpamSsijRls,mbpamSsidRls,mbpamSsilRls,mbpamSsiuRls,mbpamSsipRls	
			);
	
			fprintf(flogPtr,"\n ******************** Process Completed Successfully ******************** \n ");								
			fclose(fcsvDataInfo);	
			fclose(flogPtr);									    
		}	
        else
		{
			printf("\n Login user not found in TC \n");	
	        fprintf(flogPtr,"\n Error : Login user not found in TC");
		}			 
	}
	else
	{
	    printf("\n Login failed in TC \n");	
	    fprintf(flogPtr,"\n Error : Login failed in TC");
	}	
	return(ITK_ok);
}