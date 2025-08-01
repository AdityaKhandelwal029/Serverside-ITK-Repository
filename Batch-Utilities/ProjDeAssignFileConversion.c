/*
   Date - 17-07-2025 
*/
/* Objective :- Exe Application to create a Input file to run ProjDeAssign.exe
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


struct queryReturnObj
{
	tag_t* qryStructTags; 
	int qryObjCount; 
};

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

struct queryReturnObj queryData(char *queryName,char *qEntry,char* qValues,int queryPrmCount ,FILE *flogPtr)
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
		
	tag_t qryTag = NULLTAG;
	tag_t *qryObjTags = NULLTAG;
	
	struct queryReturnObj structObj;
	structObj.qryStructTags = NULLTAG;
    structObj.qryObjCount   = 0;
	
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
		itkApiCallStatus=ITK_APICALL(QRY_execute(qryTag,queryPrmCount,queryEntryInput,queryValuesInput,&structObj.qryObjCount,&structObj.qryStructTags),flogPtr);
		fprintf(flogPtr,"\n -queryRealName   = %s",queryRealName);
		fprintf(flogPtr,"\n -qryObjCount     = %d",structObj.qryObjCount);
        
		if(structObj.qryObjCount>0)
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
	return(structObj);
}



/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile	     =   NULL;
    char *sGroup             =   NULL;
    char *sFile              =   NULL;
	
	char *timeStr            =   NULL;
	char *timeStrUpdated     =   NULL;
    char *timeStrUpdatedNew  =   NULL;
	char *loggedInUser       =   NULL;
	char *loggedInUserId     =   NULL;
	char *projectId          =   NULL;
	char *accessObjStr       =   NULL;
	
	char *fileUserId         =   NULL;
	char *fileProjectIds     =   NULL;
	char *groupName          =   NULL;
     char *roleName          =   NULL;
	
	char *userIdInput        =  (char*) MEM_alloc(50*sizeof(char));		
    char *logPath            =  (char*) MEM_alloc(200*sizeof(char));
	char *filePath           =  (char*) MEM_alloc(200*sizeof(char));
	char *projId             =  (char*) MEM_alloc(200*sizeof(char));
	char *qryInputValue      =  (char*) MEM_alloc(200*sizeof(char));
	char *projectIds         =  (char*) MEM_alloc(5000*sizeof(char));
	
	char *fileInputValues    =  (char*) MEM_alloc(200*sizeof(char));
	
	
	
	char filedata[5000];	
							 				
	int itkApiCallStatus     = 0;
	int qryOutput            = 0;
	int itr                  = 0;
	int itr1                 = 0;
	int itr2                 = 0;
	int userCount            = 1;
										
	tag_t loggedInUserTag    = NULLTAG;
	tag_t groupTag           = NULLTAG;
    tag_t roleTag            = NULLTAG;
	
	
	struct queryReturnObj qryObj1,qryObj2,qryObj3;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
    sFile          = ITK_ask_cli_argument("-file=");
	
	//tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"ProjDeAssignFileConversionLog_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
	//tc_strcpy(csvPath,"/user/cvprod/Program_Shells/DML_Support/Multi_Part_PO/11/");
	tc_strcat(filePath,"ProjDeAssign_input_");
	tc_strcat(filePath,timeStrUpdated);
	tc_strcat(filePath,".txt");
	
		
	FILE *flogPtr     = fopen(logPath,"w+");	
	FILE *filePathPtr = fopen(filePath,"w+");
	FILE *fIntCsv     = fopen(sFile,"r");
	
						
	fprintf(flogPtr,"- Date and Time   = %s",timeStr);
	fprintf(flogPtr,"\n- Date and Time = %s",timeStrUpdatedNew);	
	fprintf(flogPtr,"\n- sUserName     = %s",sUserName       );
	fprintf(flogPtr,"\n- sPasswordFile = %s",sPasswordFile   );
	fprintf(flogPtr,"\n- sGroup        = %s",sGroup          );	
	fprintf(flogPtr,"\n- sFile         = %s",sFile           );	
	fprintf(flogPtr,"\n- logPath       = %s",logPath         );	
	fprintf(flogPtr,"\n- filePath      = %s",filePath         );	
	
				
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
			
			if(fIntCsv!=NULL)
		    {
				fprintf(flogPtr,"\n Input CSV file found and opened successfully");	
								
				while(fgets(filedata, 5000, fIntCsv) != NULL) 
                {
				    fprintf(flogPtr,"\n %d.) Line = %d ,User Id(File) = %s",userCount,userCount,filedata);
					printf("\n %d.) Line = %d ,User Id(File) = %s",userCount,userCount,filedata);
					if(userCount > 1)
					{
						fileUserId       = strtok(filedata,",");
					    fileProjectIds   = strtok(NULL,",");
						
						fprintf(flogPtr," - fileUserId     = %s",fileUserId);
						fprintf(flogPtr," - fileProjectIds = %s",fileProjectIds);
						
						if(tc_strlen(fileUserId) > 0 && tc_strlen(fileProjectIds) > 0)
						{
							fprintf(flogPtr,"\n Executing Condition where valid -fileUserId and -fileProjectIds found");
							itr1 = 0;
							tc_strcpy(projId,"");
							for(itr = 0;itr<tc_strlen(fileProjectIds);itr++)
							{
								if(fileProjectIds[itr] == ' ')
								{
									projId[itr1] = '\0';
									
									// Query Projects Starts 
																		
									tc_strcpy(qryInputValue,projId);
									tc_strcat(qryInputValue,",");
									tc_strcat(qryInputValue,fileUserId);
									
									fprintf(flogPtr,"\n - projId = %s , - qryInputValue = %s",projId,qryInputValue);
																		
									qryObj1 = queryData("ProjectUserAccessDet","Project ID,Id",qryInputValue,2,flogPtr);							
			                        fprintf(flogPtr,"\n ***** QUERY ****  found = %d",qryObj1.qryObjCount);
									
									for(itr2 = 0;itr2 < qryObj1.qryObjCount;itr2++)
									{
										ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[itr2],"object_string",&accessObjStr),flogPtr);
										ITK_APICALL(AOM_ask_value_tag(qryObj1.qryStructTags[itr2],"group",&groupTag),flogPtr);										
										ITK_APICALL(AOM_ask_value_tag(qryObj1.qryStructTags[itr2],"role",&roleTag),flogPtr);
										
										ITK_APICALL(AOM_ask_value_string(groupTag,"name",&groupName),flogPtr);
										ITK_APICALL(AOM_ask_value_string(roleTag,"object_name",&roleName),flogPtr);
																																						
										tc_strcpy(fileInputValues,fileUserId);
										tc_strcat(fileInputValues,"~");
										tc_strcat(fileInputValues,groupName);
										tc_strcat(fileInputValues,"~");
										tc_strcat(fileInputValues,projId);
										tc_strcat(fileInputValues,"~");
										tc_strcat(fileInputValues,roleName);
										tc_strcat(fileInputValues,"~");
										
										fprintf(flogPtr,"\n - accessObjStr     = %s",accessObjStr);
										fprintf(flogPtr,"\n - groupName        = %s",groupName);
										fprintf(flogPtr,"\n - roleName         = %s",roleName);										
										fprintf(flogPtr,"\n - fileInputValues  = %s",fileInputValues);
										
										fprintf(filePathPtr,"%s\n",fileInputValues);
																				
									}										
																		
									itr1 = 0;
							        tc_strcpy(projId,"");
								}
								else if(itr+1 == tc_strlen(fileProjectIds))
								{
									//projId[itr1] = fileProjectIds[itr];
									//itr1++;
									
									projId[itr1] = '\0';
									
									// Query Projects Starts 
																		
									tc_strcpy(qryInputValue,projId);
									tc_strcat(qryInputValue,",");
									tc_strcat(qryInputValue,fileUserId);
									
									fprintf(flogPtr,"\n - projId = %s , - qryInputValue = %s",projId,qryInputValue);
																		
									qryObj1 = queryData("ProjectUserAccessDet","Project ID,Id",qryInputValue,2,flogPtr);							
			                        fprintf(flogPtr,"\n ***** QUERY ****  found = %d",qryObj1.qryObjCount);
									
									for(itr2 = 0;itr2 < qryObj1.qryObjCount;itr2++)
									{
										ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[itr2],"object_string",&accessObjStr),flogPtr);
										ITK_APICALL(AOM_ask_value_tag(qryObj1.qryStructTags[itr2],"group",&groupTag),flogPtr);										
										ITK_APICALL(AOM_ask_value_tag(qryObj1.qryStructTags[itr2],"role",&roleTag),flogPtr);
										
										ITK_APICALL(AOM_ask_value_string(groupTag,"name",&groupName),flogPtr);
										ITK_APICALL(AOM_ask_value_string(roleTag,"object_name",&roleName),flogPtr);
																																						
										tc_strcpy(fileInputValues,fileUserId);
										tc_strcat(fileInputValues,"~");
										tc_strcat(fileInputValues,groupName);
										tc_strcat(fileInputValues,"~");
										tc_strcat(fileInputValues,projId);
										tc_strcat(fileInputValues,"~");
										tc_strcat(fileInputValues,roleName);
										tc_strcat(fileInputValues,"~");
										
										fprintf(flogPtr,"\n - accessObjStr     = %s",accessObjStr);
										fprintf(flogPtr,"\n - groupName        = %s",groupName);
										fprintf(flogPtr,"\n - roleName         = %s",roleName);										
										fprintf(flogPtr,"\n - fileInputValues  = %s",fileInputValues);
										
										fprintf(filePathPtr,"%s\n",fileInputValues);
																				
									}										
																		
									itr1 = 0;
							        tc_strcpy(projId,"");									
								}
								else
								{
									projId[itr1] = fileProjectIds[itr];
									itr1++;
								}
							}
							
						}
                        else
						{
							fprintf(flogPtr,"\n Execting condition were valid entry not found ");
						}							
					}					
				    userCount++;
				}
		    }
			else
			{
			    fprintf(flogPtr,"\n Input CSV file not found execution failed ");	
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