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

struct queryReturnObj
{
	tag_t* qryStructTags; 
	int qryObjCount; 
};

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
	char *taskId             =   NULL;
	char *timeStr            =   NULL;
	char *timeStrUpdated     =   NULL;
    char *timeStrUpdatedNew  =   NULL;
	char *loggedInUser       =   NULL;
	
	char *userName           =   NULL;
	char *userObjStr         =   NULL;
	char *userId             =   NULL;	
	char *userdflGrp         =   NULL;
	char *userOsName         =   NULL;	
	char *objName            =   NULL;
	char *objStr             =   NULL;
	char *objType            =   NULL;
	char *grpName            =   NULL;
	
	char *newuserName        =   NULL;
	char *designGrp          =   NULL;
	char *projectCode        =   NULL;
	
	char *newObjName         =   NULL;
	char *newObjStr          =   NULL;
	char *costCenterName     =   NULL;
	char *costCenterNumber   =   NULL;
				
    char *logPath            = (char*) MEM_alloc(200*sizeof(char));
	char *queryInput         = (char*) MEM_alloc(200*sizeof(char));
	char *csvPath            = (char*) MEM_alloc(200*sizeof(char));
							 				
	int itkApiCallStatus = 0;
	int itr1             = 0;
	int itr2             = 0;
	int userdflGrpCount  = 0;
	int cnt              = 0;
    
	struct queryReturnObj qryObj1,qryObj2,qryObj3;
									
	tag_t loggedInUserTag       = NULLTAG;
	tag_t userdflTags           = NULLTAG;
	tag_t userProfile           = NULLTAG;
		
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	
	//tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"UserCreatedData_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
		
	FILE *flogPtr=fopen(logPath,"w+");	
					
	fprintf(flogPtr,"- Date and Time = %s",timeStr);
	fprintf(flogPtr,"\n- Date and Time = %s",timeStrUpdatedNew);	
	fprintf(flogPtr,"\n- sUserName     = %s",sUserName      );
	fprintf(flogPtr,"\n- sPasswordFile = %s",sPasswordFile  );
	fprintf(flogPtr,"\n- sGroup        = %s",sGroup         );	
	
		
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
			
			qryObj1 = queryData("UserFndACtive","License Level,Status,Id","0,0,*",3,flogPtr);							
			fprintf(flogPtr,"\n ***** QUERY ****  found = %d",qryObj1.qryObjCount);
			
			printf("\n ***** QUERY ****  found = %d",qryObj1.qryObjCount);
			
			//for(itr1=0;itr1<5;itr1++)			
            for(itr1=0;itr1<qryObj1.qryObjCount;itr1++)				
			{								
				ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[itr1],"user_name"    ,&userName  ),flogPtr);
                ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[itr1],"object_string",&userObjStr),flogPtr);
				ITK_APICALL(AOM_ask_value_string(qryObj1.qryStructTags[itr1],"user_id",&userId),flogPtr);
				
				ITK_APICALL(AOM_ask_value_tag(qryObj1.qryStructTags[itr1],"fnd0custom_user_profile",&userProfile),flogPtr);
				ITK_APICALL(AOM_ask_value_string(userProfile,"t5_costcentername",&costCenterName),flogPtr);
				ITK_APICALL(AOM_ask_value_string(userProfile,"t5_costcenter1",&costCenterNumber),flogPtr);
				
				
				//ITK_APICALL(AOM_ask_value_tags(qryObj1.qryStructTags[itr1],"default_group",&userdflGrpCount,&userdflTags),flogPtr);
				//ITK_APICALL(AOM_ask_value_string(userdflTags[0],"object_name",&grpName),flogPtr);
				
				ITK_APICALL(POM_ask_user_default_group(qryObj1.qryStructTags[itr1],&userdflTags),flogPtr);
				ITK_APICALL(AOM_ask_value_string(userdflTags,"name",&grpName),flogPtr);
				
               
			    fprintf(flogPtr,"\n ********************* itr1 = %d",itr1);
				
				printf("\n ********************* itr1 = %d",itr1);
				
				fprintf(flogPtr,"\n -userName          = %s",userName);
				fprintf(flogPtr,"\n -userObjStr        = %s",userObjStr);				
				fprintf(flogPtr,"\n -userId            = %s",userId);				
				fprintf(flogPtr,"\n -userdflGrp        = %s",userdflGrp);
				fprintf(flogPtr,"\n -userdflGrpCount   = %d",userdflGrpCount);
				fprintf(flogPtr,"\n -grpName           = %s",grpName);     
                fprintf(flogPtr,"\n -costCenterName    = %s",costCenterName);    	
                fprintf(flogPtr,"\n -costCenterNumber  = %s",costCenterNumber);    					
											
				tc_strcpy(queryInput,userObjStr);
				tc_strcat(queryInput,",");
				tc_strcat(queryInput,"01-Apr-2025 00:00");
				tc_strcat(queryInput,",");
				tc_strcat(queryInput,"30-Jun-2025 23:59");  
								
				if(tc_strcmp(grpName,"PLM_Support_Group")!=0 && tc_strcmp(grpName,"dba")!=0 && tc_strcmp(grpName,"APL")!=0)
				{
					qryObj2 = queryData("General...","Owning User,Created After,Created Before",queryInput,3,flogPtr);
				    fprintf(flogPtr,"\n ********************* qryObj2.qryObjCount = %d",qryObj2.qryObjCount);
				
				    if(qryObj2.qryObjCount > 0)
				    {
						STRNG_replace_str(userName," ","_",&newuserName);
						tc_strcpy(csvPath,newuserName);
						tc_strcat(csvPath,"_");
						tc_strcat(csvPath,timeStrUpdatedNew);
	                    tc_strcat(csvPath,".csv");
						
						FILE *fcsvPtr=fopen(csvPath,"w+");
						
						fprintf(fcsvPtr,"User Name ,%s,User Id ,%s,Cost Center Name,%s,Cost Center Number,%s",userName,userId,costCenterName,costCenterNumber);
						fprintf(fcsvPtr,"\n Sr.no,Object Name,Object String,Object Type,Design Group,Project Code");
						
						cnt = 1;
						
				    	for(itr2=0;itr2<qryObj2.qryObjCount;itr2++)					
			            {
				        	ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[itr2],"object_name"    ,&objName),flogPtr);
                            ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[itr2],"object_string"  ,&objStr ),flogPtr);					
				        	ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[itr2],"object_type"    ,&objType),flogPtr);
							
							ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[itr2],"t5_DesignGrp"    ,&designGrp  ),flogPtr);
							ITK_APICALL(AOM_ask_value_string(qryObj2.qryStructTags[itr2],"t5_ProjectCode"  ,&projectCode),flogPtr);
																		        	
                            if(tc_strstr(objType,"EPM")==NULL)
				    		{				    	
				        	    fprintf(flogPtr,"\n ********************* itr2 = %d",itr2);
				        	    fprintf(flogPtr,"\n -objName    = %s",objName);
				                fprintf(flogPtr,"\n -objStr     = %s",objStr );
				        	    fprintf(flogPtr,"\n -objType    = %s",objType);
								fprintf(flogPtr,"\n -objType    = %s",designGrp  );
								fprintf(flogPtr,"\n -objType    = %s",projectCode);
								
								STRNG_replace_str(objName,","," ",&newObjName);
								STRNG_replace_str(objStr,","," " ,&newObjStr );
																
								fprintf(fcsvPtr,"\n%d,%s,%s,%s,%s,%s",cnt,newObjName,newObjStr,objType,designGrp,projectCode);
								cnt++;
				    		}						    	
                        }
						
						fclose(fcsvPtr);
				    }
				}
                else
				{
					fprintf(flogPtr,"Condition Skipped for %s userName ",userObjStr);
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