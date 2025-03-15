/*Date - 14-03-2025 */
 /* 
    Objective :- UpdateProjectAccess Client code to extract info of Non Erc Objects
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
#include <tccore/project.h>
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

char  *todayTime;
int   queryMasCount    = 0; 

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

tag_t* queryData(char *queryName,char *qEntry,char* qValues,int queryPrmCount ,FILE *flogPtr)
{
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
    tag_t *qryObjTags = NULLTAG;

	
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
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,queryPrmCount,queryEntryInput,queryValuesInput,&queryMasCount,&qryObjTags),flogPtr);
		fprintf(flogPtr,"\n -queryRealName   = %s",queryRealName);
		fprintf(flogPtr,"\n -queryMasCount   = %d",queryMasCount);
  
		if(queryMasCount>0)
		{
			fprintf(flogPtr,"\n *************** Execting condition where queryMasCount > 0 ************** ");
		}
		else
		{
			fprintf(flogPtr,"\n Execting condition where queryMasCount <= 0 ");
		}		
	}
	else
	{
		fprintf(flogPtr,"\n Error : Query not found .... ");
	}
	fprintf(flogPtr," \n *********** Execution of queryData function Ended *********** \n");
	return(qryObjTags);
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
				   
    char *projMasterId       = (char*) MEM_alloc(5000*sizeof(char));
	
    char *usrObjName         = NULL; 
	char *usrObjStr          = NULL; 
	char *usrObjId           = NULL; 
	char *projId             = NULL;
	
				   
	
	char filedata[500];
	char timeArray[100];
		
	int itkApiCallStatus    = 0;
	int lineCount           = 1;
	int tItr                = 0;
	int userQueryCount      = 0;
	int projectAccessCount  = 0;
	int itr                 = 0;
	int itr2                = 0;
	int proAccessCount      = 0;
   
	
	tag_t loggedInUserTag       = NULLTAG;
	tag_t *projAccessTags       = NULLTAG;
	
		
		
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	
	tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"UserProjectAccessRecordLog_UAPROD_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
			
	tc_strcpy(csvDataInfo,"/user/uaprodtrl/muktesh/User_Project_Access/");
	tc_strcat(csvDataInfo,"UserProjectAccessRecord_UAPROD_");
	tc_strcat(csvDataInfo,timeStrUpdatedNew);
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
		    	fprintf(fcsvDataInfo,"SrNo,User Id,Total Access count,Project Ids\n");	                                                                                                                                                                                                                                                                                                                                                               
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
						
			tag_t *userTagsList  = queryData("UserCheck","Id,Status","*,0",2,flogPtr);
			userQueryCount  = queryMasCount;
			fprintf(flogPtr,"\n - userQueryCount = %d",userQueryCount); 
			fprintf(flogPtr,"\n - queryMasCount  = %d",queryMasCount);
			for(itr = 0;itr < queryMasCount ; itr++)
			{
				itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(userTagsList[itr],"object_string",   &usrObjStr    ),flogPtr); 
				itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(userTagsList[itr],"user_id",         &usrObjId     ),flogPtr); 
			    fprintf(flogPtr,"\n %d) - usrObjStr = %s , - usrObjId = %s ",itr+1,usrObjStr,usrObjId);
			    
				itkApiCallStatus=ITK_APICALL(PROJ_find_user_project(userTagsList[itr],true,&proAccessCount,&projAccessTags),flogPtr); 
			    fprintf(flogPtr,"\n - proAccessCount = %d",proAccessCount);
				
				tc_strcpy(projMasterId,"");
				//tc_strcat(projMasterId,"\"");
				for(itr2 = 0;itr2 < proAccessCount;itr2++)
				{
				    itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(projAccessTags[itr2],"project_id",   &projId    ),flogPtr); 
				    fprintf(flogPtr," \n -projId = %s",projId); 
				    if(itr2 == 0)
				    {
					    tc_strcat(projMasterId,projId);
				    }
				    else
				    {
					    tc_strcat(projMasterId,",");
					    tc_strcat(projMasterId,projId);
				    }
				}
				//tc_strcat(projMasterId,"\"");
				fprintf(flogPtr,"\n - projMasterId = %s ",projMasterId);
				fprintf(fcsvDataInfo,"%d,%s,%d,%s  \n",itr+1,usrObjId,proAccessCount,projMasterId);	 
			}
			
			
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
