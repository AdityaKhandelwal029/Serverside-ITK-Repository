/*Date - 06-01-2025 
       - 23-01-2025 */
/* Objective :- DataCollectionExe Client code to extract info of 
   -> Total Logged in user.
   -> Total DML released 
   -> Total DMl with ERC Released Status.
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
char *fVariantClassId = NULL;
char *fVariantObjId   = NULL;
char *fFeatureObjId   = NULL;
char *fFamilyObjId    = NULL;

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

int queryCreationData(char *itemType,char *createdFrom ,FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	
	char *queryName    = NULL;
	char *qryEntry[2]  = {"Type","Created After"};
	char *qryValues[2] = {itemType,createdFrom};
	
	tag_t      qryTag = NULLTAG;
    tag_t *qryObjTags = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of queryCreationData function started *********** \n");
	fprintf(flogPtr,"\n -qryValues[0] Type          = %s",qryValues[0]);
	fprintf(flogPtr,"\n -qryValues[1] Created After = %s",qryValues[1]);
	
	
	itkApiCallStatus=ITK_APICALL(QRY_find2("General...",&qryTag),flogPtr);
	
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n General Query found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,2,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);
	    fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
		fprintf(flogPtr,"\n -queryName   = %s",queryName);
	}
	else
	{
		fprintf(flogPtr,"Error : General Query not found .... ");
	}
	
	fprintf(flogPtr," \n *********** Execution of queryCreationData function Ended *********** \n");
	
	return(qryObjCount);
}
int queryReleaseData(char *itemType,char *rlsFrom ,char *rlsStatus, FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	
	char *queryName    = NULL;
	char *qryEntry[3]  = {"Type","Released After","Release Status"};
	char *qryValues[3] = {itemType,rlsFrom,rlsStatus};
	
	tag_t      qryTag = NULLTAG;
    tag_t *qryObjTags = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of queryReleaseData function started *********** \n");
	fprintf(flogPtr,"\n -qryValues[0] Type           = %s",qryValues[0]);
	fprintf(flogPtr,"\n -qryValues[1] Released After = %s",qryValues[1]);
	fprintf(flogPtr,"\n -qryValues[2] Release Status = %s",qryValues[2]);
	
	itkApiCallStatus=ITK_APICALL(QRY_find2("General...",&qryTag),flogPtr);
	
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n General Query found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,3,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);
	    fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
		fprintf(flogPtr,"\n -queryName   = %s",queryName);
	}
	else
	{
		fprintf(flogPtr,"Error : General Query not found .... ");
	}
	
	fprintf(flogPtr," \n *********** Execution of queryReleaseData function Ended *********** \n");
	
	return(qryObjCount);
}

int designCreationData(char *itemType,char *createdFrom ,FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	
	char *queryName    = NULL;
	char *qryEntry[2]  = {"Part Type","Created After"};
	char *qryValues[2] = {itemType,createdFrom};
	
	tag_t      qryTag = NULLTAG;
    tag_t *qryObjTags = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of [designCreationData] function started *********** \n");
	fprintf(flogPtr,"\n -qryValues[0] Part Type     = %s",qryValues[0]);
	fprintf(flogPtr,"\n -qryValues[1] Created After = %s",qryValues[1]);
	
	
	itkApiCallStatus=ITK_APICALL(QRY_find2("Design Revision",&qryTag),flogPtr);
	
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n Design Revision... found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,2,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);
	    fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
		fprintf(flogPtr,"\n -queryName   = %s",queryName);
	}
	else
	{
		fprintf(flogPtr,"Error : Design Revision query not found ");
	}
	
	fprintf(flogPtr," \n *********** Execution of [designCreationData] function Ended *********** \n");
	
	return(qryObjCount);
}
int designReleaseData(char *itemType,char *rlsFrom ,char *rlsStatus, FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	
	int i                = 0;
	int itr              = 0;
	int itr2             = 0;
	int rlsStatusCount   = 0;
	int fRlsCount        = 0;
	
	date_t rlsDate;
	
	char *tarObjId      = NULL;
	char *tarObjName    = NULL;
	
	char *rlsStatusName = NULL;
	char *dataRlsStr    = NULL;
	char *queryName     = NULL;
	
	char *todaysDateStr    = (char*) MEM_alloc(100*sizeof(char));
	char *tokrlsDateStr    = (char*) MEM_alloc(100*sizeof(char));
		
	char  *qryEntry[3] = {"Part Type","Released After","Release Status"};
	char *qryValues[3] = {itemType,rlsFrom,rlsStatus};
	
	tag_t      qryTag    = NULLTAG;
    tag_t *qryObjTags    = NULLTAG;
	
	tag_t *rlsStatusList = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of [designReleaseData] function started *********** \n");
	fprintf(flogPtr,"\n -qryValues[0] Part Type      = %s",qryValues[0]);
	fprintf(flogPtr,"\n -qryValues[1] Released After = %s",qryValues[1]);
	fprintf(flogPtr,"\n -qryValues[2] Release Status = %s",qryValues[2]);
	
	itkApiCallStatus=ITK_APICALL(QRY_find2("Design Revision",&qryTag),flogPtr);
	
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n Design Revision found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,3,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);
	    fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
		fprintf(flogPtr,"\n -queryName   = %s",queryName);
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
						fprintf(flogPtr,"\n -rlsStatusName = %s",rlsStatusName);
						if(tc_strcmp(rlsStatusName,rlsStatus)==0)
						{
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_date(rlsStatusList[itr2],"date_released",&rlsDate),flogPtr);									
						    itkApiCallStatus=ITK_APICALL(ITK_date_to_string(rlsDate,&dataRlsStr),flogPtr);							
							fprintf(flogPtr,"\n -dataRlsStr = %s",dataRlsStr);	
						
                            todaysDateStr = subString(rlsFrom,0,11);	
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
		fprintf(flogPtr,"Error : Design Revision query not found ");
	}
	
	fprintf(flogPtr," \n \n - qryObjCount = %d , -fRlsCount = %d",qryObjCount,fRlsCount);
	fprintf(flogPtr," \n *********** Execution of [designReleaseData] function Ended *********** \n");
	
	return(fRlsCount);
}
////////////////////////////

int dml16RErcReleaseData(char *rlsStatus,char *rlsFrom ,FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	
	int i                = 0;
	int itr              = 0;
	int itr2             = 0;
	int rlsStatusCount   = 0;
	int fRlsCount        = 0;
	
	date_t rlsDate;
	
	char *queryName     = NULL;
	char *tarObjId      = NULL;
	char *tarObjName    = NULL;
	
	char *rlsStatusName = NULL;
	char *dataRlsStr    = NULL;
	
	char *todaysDateStr    = (char*) MEM_alloc(100*sizeof(char));
	char *tokrlsDateStr    = (char*) MEM_alloc(100*sizeof(char));
	
	char  *qryEntry[4] = {"Name","date_released_after","Release Type","Design Group"};
	char *qryValues[4] = {rlsStatus,rlsFrom,"ECU Parts Release","16"};
	
	tag_t      qryTag    = NULLTAG;
    tag_t *qryObjTags    = NULLTAG;
	tag_t *rlsStatusList = NULLTAG; 
	
	fprintf(flogPtr,"\n ********************* Execution of [dml16RErcReleaseData] function started ********************* \n");
	fprintf(flogPtr,"\n -qryValues[0] Name                = %s",qryValues[0]);
	fprintf(flogPtr,"\n -qryValues[1] date_released_after = %s",qryValues[1]);
	fprintf(flogPtr,"\n -qryValues[2] Release Type        = %s",qryValues[2]);
	fprintf(flogPtr,"\n -qryValues[3] Design Group        = %s",qryValues[3]);
	
	itkApiCallStatus=ITK_APICALL(QRY_find2("ERCDMLReleased_Type",&qryTag),flogPtr);	
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n ERCDML Released_type found .... ");
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,4,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);    		
		fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
		fprintf(flogPtr,"\n -queryName   = %s",queryName);
		
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
						fprintf(flogPtr,"\n -rlsStatusName = %s",rlsStatusName);
						if(tc_strcmp(rlsStatusName,rlsStatus)==0)
						{
							itkApiCallStatus=ITK_APICALL(AOM_ask_value_date(rlsStatusList[itr2],"date_released",&rlsDate),flogPtr);									
						    itkApiCallStatus=ITK_APICALL(ITK_date_to_string(rlsDate,&dataRlsStr),flogPtr);							
							fprintf(flogPtr,"\n -dataRlsStr = %s",dataRlsStr);	
						
                            todaysDateStr = subString(rlsFrom,0,11);	
                            tokrlsDateStr = subString(dataRlsStr,0,11);										
							
							fprintf(flogPtr,"\n -todaysDateStr = %s",todaysDateStr);
							fprintf(flogPtr,"\n -tokrlsDateStr = %s",tokrlsDateStr);
																												
							if(tc_strcmp(todaysDateStr,tokrlsDateStr)==0)
							{
								fprintf(flogPtr," \n Executing Condition where -todaysDateStr == tokrlsDateStr");
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
		fprintf(flogPtr,"Error : ERCDML Released_type not found ");
	}	
	fprintf(flogPtr," \n \n - qryObjCount = %d , -fRlsCount = %d",qryObjCount,fRlsCount);
	fprintf(flogPtr," \n ********************* Execution of [dml16RErcReleaseData] function Ended ********************* \n");	
	return(fRlsCount);
}

int queryDataSetCreationData(char *itemType,char *createdFrom ,char *pattern,FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
		
	char  *queryName     = NULL;
		
	char  *qryEntry[3]   = {"Type","Created After","Name1"};
	char *qryValues[3]   = {itemType,createdFrom,pattern};
			
	tag_t      qryTag   = NULLTAG;
    tag_t *qryObjTags   = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of queryDataSetCreationData function started *********** \n");	
		
	fprintf(flogPtr,"\n -qryValues[0] Dataset Type  = %s",qryValues[0]);
	fprintf(flogPtr,"\n -qryValues[1] Created After = %s",qryValues[1]);
	fprintf(flogPtr,"\n -qryValues[2] Name1         = %s",qryValues[2]);
				
	itkApiCallStatus=ITK_APICALL(QRY_find2("DirectModel",&qryTag),flogPtr);
	
	if(qryTag!=NULLTAG)
    {
	    fprintf(flogPtr,"\n DirectModel Query Found ");
		
		itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,3,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);
	    fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
		fprintf(flogPtr,"\n -queryName   = %s",queryName);				 		
	}
	else
	{
		fprintf(flogPtr,"Error : DirectModel Query not found .... ");
	}
	fprintf(flogPtr," \n *********** Execution of queryDataSetCreationData function Ended *********** \n");	
	return(qryObjCount);
}

int queryAutoSorData(char *dateFrom ,char *pattern,int qryPrm, FILE *flogPtr)
{
	int      qryObjCount = 0;
	int itkApiCallStatus = 0;
	
	char *queryName     = NULL;
	
	char *qryEntry  [1] = {"Created After"};
	char *qryValues [1] = {dateFrom};
	
	char *qryEntry1  [2] = {"SOR Remark","Date Modified After"};
	char *qryValues1 [2] = {pattern,dateFrom};
		
	tag_t      qryTag = NULLTAG;
    tag_t *qryObjTags = NULLTAG;
	
	fprintf(flogPtr,"\n *********** Execution of queryAutoSorData function started *********** \n");
	fprintf(flogPtr,"\n - qryPrm = %d ",qryPrm);
	if(qryPrm == 1)
	{
	    fprintf(flogPtr,"\n -qryValues[0] Created After = %s",qryValues[0]);
		
	    itkApiCallStatus=ITK_APICALL(QRY_find2("AutoSorQuery",&qryTag),flogPtr);
	
	    if(qryTag!=NULLTAG)
        {
	        fprintf(flogPtr,"\n AutoSorQuery Query found .... ");
	    	itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
	    	itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,1,qryEntry,qryValues,&qryObjCount,&qryObjTags),flogPtr);
	        fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
	    	fprintf(flogPtr,"\n -queryName   = %s",queryName);
	    }
		else
	    {
	    	fprintf(flogPtr,"Error : AutoSorQuery Query not found .... ");
	    }
	}
	else
	{
	    fprintf(flogPtr,"\n -qryValues1[0] SOR Remark          = %s",qryValues1[0]);
		fprintf(flogPtr,"\n -qryValues1[1] Date Modified After = %s",qryValues1[1]);
				
	    itkApiCallStatus=ITK_APICALL(QRY_find2("AutoSorQuery",&qryTag),flogPtr);
	
	    if(qryTag!=NULLTAG)
        {
	        fprintf(flogPtr,"\n AutoSorQuery Query found .... ");
	    	itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(qryTag,"object_string",&queryName),flogPtr);
	    	itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,2,qryEntry1,qryValues1,&qryObjCount,&qryObjTags),flogPtr);
	        fprintf(flogPtr,"\n -qryObjCount = %d",qryObjCount);
	    	fprintf(flogPtr,"\n -queryName   = %s",queryName);
	    }
		else
	    {
	    	fprintf(flogPtr,"Error : AutoSorQuery Query not found .... ");
	    }
	}
	
	fprintf(flogPtr," \n *********** Execution of queryAutoSorData function Ended *********** \n");
	
	return(qryObjCount);
}


/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile	     =   NULL;
    char *sGroup             =   NULL;
	char *timeStr            =   NULL;
	char *timeStrUpdated     =   NULL;
    char *timeStrUpdatedNew  =   NULL;
	char *loggedInUser       =   NULL;
	char *NtmpTime           = (char*) MEM_alloc(10*sizeof(char));
	char *day                = (char*) MEM_alloc(10*sizeof(char));
	char *month              = (char*) MEM_alloc(10*sizeof(char));
	char *date               = (char*) MEM_alloc(10*sizeof(char));
	char *tmpTime            = (char*) MEM_alloc(10*sizeof(char));
	char *year               = (char*) MEM_alloc(10*sizeof(char));
	char *tmp                = (char*) MEM_alloc(10*sizeof(char));
    char *logPath            = (char*) MEM_alloc(200*sizeof(char));
	char *userInfo           = (char*) MEM_alloc(200*sizeof(char));
	char *csvDataInfo        = (char*) MEM_alloc(200*sizeof(char));
							 							 
	char *user2c9            = (char*) MEM_alloc(10*sizeof(char));
    char *user1c6            = (char*) MEM_alloc(10*sizeof(char));
	char *queryInput1        = (char*) MEM_alloc(100*sizeof(char));
	char *queryInput2        = (char*) MEM_alloc(100*sizeof(char));
			
	char filedata[500];
	char timeArray[100];
		
	int itkApiCallStatus = 0;
	int lineCount = 1;
	int tItr      = 0;
	
	int crtDML    = 0;
	int rlsDMl    = 0;
			      
	int crtaSor   = 0;
	int rlsaSor   = 0;
	
	int crtproIndent = 0;
	int rlsProIndent = 0;
	
	int crtTdm = 0;
	int rlsTdm = 0;
			   
	int crtCtr = 0;
	int rlsCtr = 0;
			   
	int crtGr  = 0;
	int rlsGr  = 0;
			   
	int crtFir = 0;
	int rlsFir = 0; 
	
	int crtTestPlan = 0;
	int rlsTestPlan = 0;
	
	int crtVc       = 0;
	int rlsVc       = 0;
				    
	int crtModule   = 0;
	int rlsModule   = 0;
	
	int crtTcPartAssembly       = 0;
	int rlsTCPartAssembly       = 0;
							    
	int crtTcPartComponent      = 0;
	int rlsTcPartComponent      = 0;
							    
	int crtFtcPart              = 0;
	int rlsFtcPart              = 0;
	
	int crt16REcuRelease        = 0;
	int rls16REcuRelease        = 0;
	
	int tcPartCheckInAssembly   = 0;
	int tcPartCheckInComponent  = 0;
	int tcPartCheckInModule     = 0;
	int tcPartsCheckIn          = 0;
		
	int tcPartCheckOutAssembly  = 0;
	int tcPartCheckOutComponent = 0;
	int tcPartCheckOutModule    = 0;
	int tcPartsCheckOut         = 0;
	
	int jtCreated               = 0;
									
	tag_t loggedInUserTag       = NULLTAG;
		
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	
	tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"DataCollectionLogFile_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
	tc_strcpy(userInfo,"/user/cvprod/muktesh/Cron/user.txt");			
	tc_strcpy(csvDataInfo,"/proj/dmusession/jtcodes/ReportData/");
	tc_strcat(csvDataInfo,"DataCollectionExeInfo");
	tc_strcat(csvDataInfo,".csv");
		
	FILE *flogPtr=fopen(logPath,"w+");	
					
	fprintf(flogPtr,"\n- Date and Time = %s",timeStr);
	fprintf(flogPtr,"\n- Date and Time = %s",timeStrUpdatedNew);	
	fprintf(flogPtr,"\n- sUserName     = %s",sUserName      );
	fprintf(flogPtr,"\n- sPasswordFile = %s",sPasswordFile  );
	fprintf(flogPtr,"\n- sGroup        = %s",sGroup         );	
	fprintf(flogPtr,"\n- userInfo      = %s",userInfo       );
	fprintf(flogPtr,"\n- csvDataInfo   = %s",csvDataInfo    );
				
	FILE *fuserInfo     = fopen(userInfo,"r");
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
			
			if(fuserInfo==NULL)
			{
				fprintf(flogPtr,"\n Error: userInfo File not found So Process Stopped [Point 1] \n");
			}
			else
			{
				if(fcsvDataInfo== NULL)
				{
					fcsvDataInfo = fopen(csvDataInfo,"a+");
					fprintf(flogPtr,"\n -csvDataInfo file not found so new file created and opened.");	
					fprintf(fcsvDataInfo,"Data and Time,PoolManager(2c9),PoolManager(1c6),DML Created,DML Released,Auto SOR Created,Auto SOR Released,Proto Indent Created,Proto Indent Released,Test Request Ext Created,Test Request Ext Released,Test Request Int Created,Test Request Int Released,GR Created,GR Released,FIR Created,FIR Released,Test Plan Created,Test Plan Released,VC Created,VC Released,Module Created,Module Released,TC Part Created,TC Part Released,DML16R Released,TC Parts Checked In,TC Parts Checked Out,JT Created");	                                                                                                                                                                                                                                                                                                                                                               
				}
				else
				{
					fprintf(flogPtr,"\n File found so only opened in append mode.");					
					fcsvDataInfo = fopen(csvDataInfo,"a+");
				}
				while(fgets(filedata, 500, fuserInfo) != NULL)
				{
					fprintf(flogPtr,"\n ********* Line count = %d Data found = %s ",lineCount,filedata);
					if(lineCount==1)
					{
						tc_strcpy(user2c9,(char*)filedata);						
						trimLeading(user2c9);
						trimTrailing(user2c9);						
						fprintf(flogPtr,"\n -user2c9 = %s",user2c9);						
					}
					if(lineCount == 2)
					{
						tc_strcpy(user1c6,(char*)filedata);						
						trimLeading(user1c6);
						trimTrailing(user1c6);						
						fprintf(flogPtr,"\n -user1c6 = %s",user1c6);	
					}
					lineCount++;					
				}											
				lineCount=1;
				
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
				
				tc_strncpy(NtmpTime,tmpTime,5);	            
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
	            	            	            
	            tc_strcpy(queryInput1,"");
	            tc_strcat(queryInput1,date);
	            tc_strcat(queryInput1,"-");
	            tc_strcat(queryInput1,month);
	            tc_strcat(queryInput1,"-");
	            tc_strcat(queryInput1,year);
	            tc_strcat(queryInput1," ");
	            tc_strcat(queryInput1,"06:00");
	            
	            fprintf(flogPtr,"\n -queryInput1 = %s",queryInput1);
	            
	            tc_strcpy(queryInput2,"");
	            tc_strcat(queryInput2,date);
	            tc_strcat(queryInput2,"-");
	            tc_strcat(queryInput2,month);
	            tc_strcat(queryInput2,"-");
	            tc_strcat(queryInput2,year);
	            tc_strcat(queryInput2," ");
	            tc_strcat(queryInput2,NtmpTime);
	            
	            fprintf(flogPtr,"\n -queryInput2 = %s",queryInput2);
												
				// 1.) For type - > DML (ChangeRequest,ChangeRequestRevision)					
				crtDML = queryCreationData("ChangeRequest",queryInput1,flogPtr);
				rlsDMl = queryReleaseData("ChangeRequestRevision",queryInput1,"T5_LcsErcRlzd",flogPtr);
				fprintf(flogPtr,"\n ******************** - crtDML = %d ",crtDML);
				fprintf(flogPtr,"\n ******************** - rlsDMl = %d ",rlsDMl);
												
				// 2.) For type - > AutoSoa (T5_AutoSor,T5_AutoSorRevision)						
				crtaSor = queryAutoSorData(queryInput1,"",1,flogPtr);
				rlsaSor = queryAutoSorData(queryInput1,"Shopping cart successfully created*",2,flogPtr);			
				fprintf(flogPtr,"\n ******************** -crtaSor = %d ",crtaSor);
				fprintf(flogPtr,"\n ******************** -rlsaSor = %d ",rlsaSor);
				
				// 3.) For type - > Proto Indent (T5_ERCInd,T5_ERCIndRevision)						
				crtproIndent = queryCreationData("T5_ERCInd",queryInput1,flogPtr);
				rlsProIndent = queryReleaseData("T5_ERCIndRevision",queryInput1,"T5_LcsErcRlzd",flogPtr);				
				fprintf(flogPtr,"\n ******************** - crtproIndent = %d ",crtproIndent);
				fprintf(flogPtr,"\n ******************** - rlsProIndent = %d ",rlsProIndent);
				
				// 4.) For type - > TDM (T5_TestRq,T5_TestRqRevision)						
				crtTdm = queryCreationData("T5_TestRq",queryInput1,flogPtr);
				rlsTdm = queryReleaseData("T5_TestRqRevision",queryInput1,"T5_LcsErcRlzd",flogPtr);				
				fprintf(flogPtr," ******************** - crtTdm = %d ",crtTdm);
				fprintf(flogPtr," ******************** - rlsTdm = %d ",rlsTdm);
				
				// 5.) For type - > CTR (T5_CTR_TESTDATA,T5_CTR_TESTDATARevision)						
				crtCtr = queryCreationData("T5_CTR_TESTDATA",queryInput1,flogPtr);
				rlsCtr = queryReleaseData("T5_CTR_TESTDATARevision",queryInput1,"T5_LcsErcRlzd",flogPtr);				
				fprintf(flogPtr,"\n ******************** -crtCtr = %d ",crtCtr);
				fprintf(flogPtr,"\n ******************** -rlsCtr = %d ",rlsCtr);
				
				// 6.) For type - > GR (T5_GrnRep,T5_GrnRepRevision)						
				crtGr = queryCreationData("T5_GrnRep",queryInput1,flogPtr);
				rlsGr = queryReleaseData("T5_GrnRepRevision",queryInput1,"T5_LcsErcRlzd",flogPtr);				
				fprintf(flogPtr,"\n ******************** -crtGr = %d ",crtGr);
				fprintf(flogPtr,"\n ******************** -rlsGr = %d ",rlsGr);
				
				// 7.) For type - > FIR (T5_Fir,T5_FirRevision)						
				crtFir = queryCreationData("T5_Fir",queryInput1,flogPtr);
				rlsFir = queryReleaseData("T5_FirRevision",queryInput1,"T5_LcsErcRlzd",flogPtr);				
				fprintf(flogPtr,"\n ******************** -crtFir = %d ",crtFir);
				fprintf(flogPtr,"\n ******************** -rlsFir = %d ",rlsFir);
				
				// 8.) For type - > TestPlan (T5_TestPlan,T5_TestPlanRevision)						
				crtTestPlan = queryCreationData("T5_TestPlan",queryInput1,flogPtr);
				rlsTestPlan = queryReleaseData("T5_TestPlanRevision",queryInput1,"T5_LcsErcRlzd",flogPtr);				
				fprintf(flogPtr,"\n ******************** -crtTestPlan = %d ",crtTestPlan);
				fprintf(flogPtr,"\n ******************** -rlsTestPlan = %d ",rlsTestPlan);
				
				// 9.) For type - > VC (Vehicle)	               
				crtVc = designCreationData("Vehicle",queryInput1,flogPtr);
                rlsVc = designReleaseData("Vehicle",queryInput1,"T5_LcsErcRlzd",flogPtr);
				fprintf(flogPtr,"\n ******************** -crtVc = %d ",crtVc);
				fprintf(flogPtr,"\n ******************** -rlsVc = %d ",rlsVc);
								
                // 10.) For type - > Module (Module)				
				crtModule = designCreationData("Module",queryInput1,flogPtr);
                rlsModule = designReleaseData("Module",queryInput1,"T5_LcsErcRlzd",flogPtr);
				fprintf(flogPtr,"\n ******************** -crtModule = %d ",crtModule);
				fprintf(flogPtr,"\n ******************** -rlsModule = %d ",rlsModule);

                // 11.) For type - > TCPART Assembly (TCPART Assembly)				
				crtTcPartAssembly = designCreationData("Assembly",queryInput1,flogPtr);
                rlsTCPartAssembly = designReleaseData("Assembly",queryInput1,"T5_LcsErcRlzd",flogPtr);
				fprintf(flogPtr,"\n ******************** -crtTcPartAssembly = %d ",crtTcPartAssembly);
				fprintf(flogPtr,"\n ******************** -rlsTCPartAssembly = %d ",rlsTCPartAssembly);
								
                // 12.) For type - > TCPART COMPONENT (TCPART COMPONENT)
                crtTcPartComponent = designCreationData("Component",queryInput1,flogPtr);
                rlsTcPartComponent = designReleaseData("Component",queryInput1,"T5_LcsErcRlzd",flogPtr);
				fprintf(flogPtr,"\n ******************** -crtTcPartComponent = %d ",crtTcPartComponent);
				fprintf(flogPtr,"\n ******************** -rlsTcPartComponent = %d ",rlsTcPartComponent);				

                // 13.) For type - > ERCDML ECUPARTRelease 																
                rls16REcuRelease  = dml16RErcReleaseData("T5_LcsErcRlzd",queryInput1,flogPtr);				
				fprintf(flogPtr,"\n ******************** -rls16REcuRelease = %d ",rls16REcuRelease);	
				
                crtFtcPart	= crtTcPartAssembly+crtTcPartComponent; 			
				rlsFtcPart  = rlsTCPartAssembly+rlsTcPartComponent;
				
				// 14.) TC Parts Checked In (Assembly + Component + Module) With ERC_Review status				
				tcPartCheckInAssembly   = designReleaseData("Assembly" ,queryInput1 ,"T5_LcsReview" ,flogPtr);
				tcPartCheckInComponent  = designReleaseData("Component",queryInput1 ,"T5_LcsReview" ,flogPtr);
				tcPartCheckInModule     = designReleaseData("Module"   ,queryInput1 ,"T5_LcsReview" ,flogPtr);
				
				fprintf(flogPtr,"\n ******************** -tcPartCheckInAssembly  = %d ",tcPartCheckInAssembly );
				fprintf(flogPtr,"\n ******************** -tcPartCheckInComponent = %d ",tcPartCheckInComponent);
				fprintf(flogPtr,"\n ******************** -tcPartCheckInModule    = %d ",tcPartCheckInModule   );
				
				tcPartsCheckIn = tcPartCheckInAssembly + tcPartCheckInComponent + tcPartCheckInModule;
				
				fprintf(flogPtr,"\n ******************** -tcPartsCheckIn    = %d ",tcPartsCheckIn);
				
								
				// 15.) TC Parts Checked Out (Assembly + Component + Module) With ERC_Working status				
				tcPartCheckOutAssembly  = designReleaseData("Assembly" ,queryInput1 ,"T5_LcsWorking" ,flogPtr);
				tcPartCheckOutComponent = designReleaseData("Component",queryInput1 ,"T5_LcsWorking" ,flogPtr);
				tcPartCheckOutModule    = designReleaseData("Module"   ,queryInput1 ,"T5_LcsWorking" ,flogPtr);
				
				fprintf(flogPtr,"\n ******************** -tcPartCheckOutAssembly  = %d ",tcPartCheckOutAssembly );
				fprintf(flogPtr,"\n ******************** -tcPartCheckOutComponent = %d ",tcPartCheckOutComponent);
				fprintf(flogPtr,"\n ******************** -tcPartCheckOutModule    = %d ",tcPartCheckOutModule   );
				
				tcPartsCheckOut = tcPartCheckOutAssembly + tcPartCheckOutComponent + tcPartCheckOutModule;
				
				fprintf(flogPtr,"\n ******************** -tcPartsCheckOut    = %d ",tcPartsCheckOut);
															
				// 16.) JT Dataset Created 				
				jtCreated = queryDataSetCreationData("Direct Model",queryInput1,"*ALF*",flogPtr);				
				fprintf(flogPtr,"\n ******************** -jtCreated    = %d ",jtCreated);
												
				fprintf(fcsvDataInfo,"\n %s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d ",timeStr,user2c9,user1c6,crtDML,rlsDMl,crtaSor,rlsaSor,
					crtproIndent,rlsProIndent,crtTdm,rlsTdm,crtCtr,rlsCtr,crtGr,rlsGr,crtFir,rlsFir,crtTestPlan,rlsTestPlan,crtVc,rlsVc,crtModule,rlsModule,crtFtcPart,rlsFtcPart,rls16REcuRelease,tcPartsCheckIn,tcPartsCheckOut,jtCreated);	
                   
                fprintf(flogPtr,"\n ******************** Process Completed Successfully ******************** \n ");								
				fclose(fcsvDataInfo);	
				fclose(flogPtr);								
			}												
		}
	    else
	    {
		   printf("\n Login failed in TC \n");	
		   fprintf(flogPtr,"\n Error : Login failed in TC");
	    }
	}
	return(ITK_ok);
}