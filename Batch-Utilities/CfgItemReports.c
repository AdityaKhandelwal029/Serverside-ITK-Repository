/*Date - 03-01-2025 */

/* Objective :- Export Availabily rule view of configurator context */

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

int stringConvertor(char *ruleConStr,char* ruleSubStr,FILE *Flogfile )
{
	char *fTempStr = (char*) MEM_alloc(200*sizeof(char));
	char *fXStr = (char*) MEM_alloc(500*sizeof(char));
	
	int i        = 0,eqlFound = 0,j=0,braket1=0,braket2=0;
	int fitr     = 0,itemp = 0;
	int dshCount = 0;
	int spcCount = 0;
	int dBefore  = 0,dAfter = 0,dTemp = 0,dFound = 0;
	
	
	fVariantClassId =   (char*) MEM_alloc(200*sizeof(char));
	fVariantObjId   =   (char*) MEM_alloc(200*sizeof(char));
	fFeatureObjId   =   (char*) MEM_alloc(200*sizeof(char));
	fFamilyObjId    =   (char*) MEM_alloc(200*sizeof(char));
	
	fprintf(Flogfile,"\n********** Started Executing stringConvertor -ruleConStr = %s,-ruleSubStr = %s ********** \n",ruleConStr,ruleSubStr);
	for(i=1;i<=2;i++)
	{
		if(i==1)
		{
			tc_strcpy(fTempStr,ruleConStr);
		}
		else
		{
			tc_strcpy(fTempStr,ruleSubStr);
		}
		
		fprintf(Flogfile,"\n\n -fTempStr[Input] = %s ",fTempStr);
		dshCount=dashCount(fTempStr);
		spcCount=spaceCount(fTempStr);
		
		fprintf(Flogfile,"\n Total dash[c] -fTempStr = %d   Total space[c] -fTempStr = %d",dshCount,spcCount);
	    if(dshCount==0)
	    {
			fprintf(Flogfile,"\n Executing if for -fTempStr where dshCount == 0.");
			
			eqlFound=0;	
			for(j=0;fTempStr[j]!='\0';j++)
			{
				if(fTempStr[j]==']')
			    {
			    	braket1=1;
			    }
				else if(fTempStr[j]=='=')
				{
					braket1=0;
					eqlFound=1;
					
					fXStr[fitr]='\0';
					fitr=0;
					
					if(i==1)
					{
						tc_strcpy(fVariantClassId,fXStr);
						fprintf(Flogfile,"\n -fVariantClassId = %s",fVariantClassId);
					}
					if(i==2)
					{
						tc_strcpy(fFamilyObjId,fXStr);
						fprintf(Flogfile,"\n -fFamilyObjId = %s",fFamilyObjId);
					}	
				}
				else if(fTempStr[j]==')')
				{
					eqlFound=0;
					braket2=1;
					
					fXStr[fitr]='\0';
					fitr=0;
					
					if(i==1)
					{
						tc_strcpy(fVariantObjId,fXStr);
						fprintf(Flogfile,"\n -fVariantObjId = %s",fVariantObjId);
					}
					if(i==2)
					{
						tc_strcpy(fFeatureObjId,fXStr);
						fprintf(Flogfile,"\n -fFeatureObjId = %s",fFeatureObjId);
					}	
				}
				else
				{
					if(braket1==1)
					{
						if(fTempStr[j]!=' ')
					    {
					    	fXStr[fitr]=fTempStr[j];
					        fitr++;
					    }
					}
					if(eqlFound==1)
					{
						if(fTempStr[j]!=' ')
					    {
					    	fXStr[fitr]=fTempStr[j];
					        fitr++;
					    }
					}
				}
			}
			fprintf(Flogfile,"\n After if condition where dshCount == 0.");	
		}
		else
		{
			fprintf(Flogfile,"\n Executing else for -fTempStr where dshCount > 0.");
			dBefore  = 0;
		    dAfter   = 0;
		    dTemp    = 0;
		    dFound   = 0;
		    eqlFound = 0;
			fitr     = 0;
			braket1  = 0;
			braket2  = 0;
			
			if(dshCount==2)
		    {
				fprintf(Flogfile,"\n Executing if for -fTempStr where dshCount == 2.");
				for(j=0;fTempStr[j]!='\0';j++)
				{
					if(fTempStr[j]==(char)(39))
					{
						dTemp++;
					}
					if(fTempStr[j]=='=')
					{
						dBefore = dTemp;
					    dTemp = 0; 
					}
				}
				dAfter = dTemp;
				fprintf(Flogfile,"\n -dBefore = %d",dBefore);
				fprintf(Flogfile,"\n -dAfter  = %d",dAfter);
				
				if(dBefore == 2 && dAfter == 0)
				{
					fprintf(Flogfile,"\n Executing if for -fTempStr where dBefore == 2 && dAfter == 0");
				    for(j=0;fTempStr[j]!='\0';j++)
					{
						if(fTempStr[j]==(char)(39))
						{
							if(dFound==0)
							{
								dFound=1;
							}
							else
							{
								if(dFound==1)
								{
									dFound=2;
									fXStr[fitr]='\0';
					                fitr=0;
								    if(i==1)
								    {
								    	tc_strcpy(fVariantClassId,fXStr);
						                fprintf(Flogfile,"\n -fVariantClassId = %s",fVariantClassId);
								    }
								    if(i==2)
								    {
								    	tc_strcpy(fFamilyObjId,fXStr);
						                fprintf(Flogfile,"\n -fFamilyObjId = %s",fFamilyObjId);
								    }									
								}								
							}
						}
						else if(fTempStr[j]=='=')
						{
							eqlFound = 1;
							dFound   = 0;
						}
						else if(fTempStr[j]==')')
						{
							fXStr[fitr]='\0';
					        fitr=0;
							if(i==1)
					        {
					        	tc_strcpy(fVariantObjId,fXStr);
					        	fprintf(Flogfile,"\n -fVariantObjId = %s",fVariantObjId);
					        }
					        if(i==2)
					        {
					        	tc_strcpy(fFeatureObjId,fXStr);
					        	fprintf(Flogfile,"\n -fFeatureObjId = %s",fFeatureObjId);
					        }	
						}
						else
						{
							if(dFound==1)
							{
								fXStr[fitr]=fTempStr[j];
					            fitr++;
							}
							if(eqlFound==1)
							{
								if(fTempStr[j]!=' ')
					            {
					            	fXStr[fitr]=fTempStr[j];
					                fitr++;
					            }
							}							
						}
					}
					fprintf(Flogfile,"\n After if condition where -fTempStr where dBefore == 2 && dAfter == 0.");
				}
				
				else
				{
					if(dBefore == 0 && dAfter == 2)
				    {
				    	fprintf(Flogfile,"\n Executing if for -fTempStr where dBefore == 0 && dAfter == 2");
				        for(j=0;fTempStr[j]!='\0';j++)
						{
							if(fTempStr[j]==']')
			                {
			                	braket1=1;
			                }
							else if(fTempStr[j]=='=')
							{
								braket1=0;
					            eqlFound=1;
								
								fXStr[fitr]='\0';
					            fitr=0;
								
								if(i==1)
					            {
					            	tc_strcpy(fVariantClassId,fXStr);
					            	fprintf(Flogfile,"\n -fVariantClassId = %s",fVariantClassId);
					            }
					            if(i==2)
					            {
					            	tc_strcpy(fFamilyObjId,fXStr);
					            	fprintf(Flogfile,"\n -fFamilyObjId = %s",fFamilyObjId);
					            }	
							}
							else if(fTempStr[j]==(char)(39))
							{
								if(dFound==0)
							    {
							    	dFound=1;
							    }
								else
								{
									if(dFound==1)
									{
										dFound=2;
	                                    fXStr[fitr]='\0';
	                                    fitr=0;
		                                
	                                    if(i==1)
		                                {
		                                	tc_strcpy(fVariantObjId,fXStr);
		                                	fprintf(Flogfile,"\n -fVariantObjId = %s",fVariantObjId);
		                                }
		                                if(i==2)
		                                {
		                                	tc_strcpy(fFeatureObjId,fXStr);
		                                	fprintf(Flogfile,"\n -fFeatureObjId = %s",fFeatureObjId);
		                                }		
									}
								}
							}
							else
							{
								if(braket1==1)
					            {
					            	if(fTempStr[j]!=' ')
					                {
					                	fXStr[fitr]=fTempStr[j];
					                    fitr++;
					                }
					            }
								if(dFound==1)
								{
									fXStr[fitr]=fTempStr[j];
					                fitr++;
								}
							}
						}
					    fprintf(Flogfile,"\n After if condition where -fTempStr where dBefore == 0 && dAfter == 2");
					
					}
				}				
			}
			else if(dshCount==4)
			{
				fprintf(Flogfile,"\n Executing elseif for -fTempStr where dshCount == 4.");
				for(j=0;fTempStr[j]!='\0';j++)
				{
					if(fTempStr[j]==(char)(39))
					{
						dTemp++;
					}
					if(fTempStr[j]=='=')
					{
						dBefore = dTemp;
					    dTemp = 0; 
					}
				}
				dAfter = dTemp;
				fprintf(Flogfile,"\n -dBefore = %d",dBefore);
				fprintf(Flogfile,"\n -dAfter  = %d",dAfter);
				if(dBefore == 2 && dAfter == 2)
			    {
					for(j=0;fTempStr[j]!='\0';j++)
					{
						if(fTempStr[j]==(char)(39))
						{
							if(dFound==0)
							{
								dFound=1;
							}
							else if(dFound==1)
							{
								dFound=2;
								fXStr[fitr]='\0';
	                            fitr=0;
								
								if(i==1)
					            {
					            	tc_strcpy(fVariantClassId,fXStr);
					            	fprintf(Flogfile,"\n -fVariantClassId = %s",fVariantClassId);
					            }
					            if(i==2)
					            {
					            	tc_strcpy(fFamilyObjId,fXStr);
					            	fprintf(Flogfile,"\n -fFamilyObjId = %s",fFamilyObjId);
					            }	
							}
							else if(dFound==2)
							{
								dFound = 3;
							}
							else
							{								
								if(dFound==3)
								{
									fXStr[fitr]='\0';
	                                fitr=0;									
									if(i==1)
		                            {
		                            	tc_strcpy(fVariantObjId,fXStr);
		                            	fprintf(Flogfile,"\n -fVariantObjId = %s",fVariantObjId);
		                            }
		                            if(i==2)
		                            {
		                            	tc_strcpy(fFeatureObjId,fXStr);
		                            	fprintf(Flogfile,"\n -fFeatureObjId = %s",fFeatureObjId);
		                            }		
								}							
							}
						}
						else if(fTempStr[j]=='=')
						{
							eqlFound=1;
						}
						else
						{
							if(dFound==1 || dFound==3)
							{
								fXStr[fitr]=fTempStr[j];
					            fitr++;
							}
						}
					}
				}
				else
				{
					fprintf(Flogfile,"\n Executing else for -fTempStr where Data Not Proper.");
				}
			}
			else
		    {
		    	fprintf(Flogfile,"\n Executing else for -fTempStr where dshCount > 4 ");
		    }	
			
		}
	}
    fprintf(Flogfile,"\n ********** Ended Execution of stringConvertor with Argument \n -fVariantClassId = %s -fVariantObjId = %s -fFamilyObjId = %s -fFeatureObjId = %s\n",fVariantClassId,fVariantObjId,fFamilyObjId,fFeatureObjId);	
    return(1);	
}

/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	char *sUserName	         =   NULL;
    char *sPasswordFile	     =   NULL;
    char *sGroup             =   NULL;
	char *sPartNo            =   NULL;
	char *sSysUser           =   NULL;
	char *sHelp              =   NULL;
	char *loggedInUser       =   NULL;
	char *timeStr            =   NULL;
	char *timeStrUpdated     =   NULL;
    char *timeStrUpdatedNew  =   NULL;
	char *configObjName      =   NULL;
	char *configObjString    =   NULL;
	char *configObjdesc      =   NULL; 
	char *variObjName        =   NULL;   
	char *variObjString      =   NULL;
	char *variObjdesc        =   NULL;
	char *variObjId          =   NULL;
	char *variObjFamilyId    =   NULL;
	char *optValueitemId     =   NULL;
	char *optValueObjName    =   NULL;
	char *optValueObjString  =   NULL;
	char *optValueObjdesc    =   NULL;
	char *optValueFamilyId   =   NULL;
	char *ruleObjName        =   NULL;
	char *ruleObjString      =   NULL;
	char *ruleObjdesc        =   NULL;
	char *ruleCondition      =   NULL;
	char *ruleSubject        =   NULL;
	char *tProModId          =   NULL;
	char *tFeature           =   NULL;
	char *fldObjName         =   NULL;
	char *fldObjType         =   NULL;
	char *sysUserId          =   (char*) MEM_alloc(200*sizeof(char));
	char *datasetIdValue     =   (char*) MEM_alloc(200*sizeof(char));
	
	char **masterArry[2000][200];	
	 
    char *logPath            =   (char*) MEM_alloc(200*sizeof(char));
	char *csvLog             =   (char*) MEM_alloc(200*sizeof(char));
	
	int itkApiCallStatus = 0;
	int modelCount       = 0;
	int ruleCount        = 0;
	int optValueCount    = 0;
	int itr              = 0;
	int configPerspectiveCount = 0;
	int it=0,jt=0,tPro=0,tFea=0;
	int mAit     = 0;
	int mAjt     = 0;
	int exeYes   = 0;
	int fldFound = 0;
	int noOfReferences = 0;
	
	tag_t loggedInUserTag             = NULLTAG;
	tag_t itemTag                     = NULLTAG;
	tag_t configPerspective           = NULLTAG;
	tag_t sysUserTag                  = NULLTAG;
    tag_t sysHomeTag                  = NULLTAG;
	tag_t newFolderTag                = NULLTAG;
	tag_t datasetType                 = NULLTAG; 
	tag_t datasetTag                  = NULLTAG;
	tag_t rdatasetTag                 = NULLTAG;
	
	
	tag_t *cfg0ModelsTags             =  NULLTAG;
	tag_t *cfg0AvailabilityRulesTags  =  NULLTAG;
	tag_t *cfg0OptionValuesTags       =  NULLTAG;
	tag_t *folderReference            = NULLTAG;
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	sPartNo        = ITK_ask_cli_argument("-i=");
	sSysUser       = ITK_ask_cli_argument("-j=");
	
	tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"CfgItemReportLog_");
	tc_strcat(logPath,sPartNo);
	tc_strcat(logPath,"_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
	tc_strcpy(csvLog,"/tmp/");  //  /tmp/ - to be added if required
	tc_strcat(csvLog,"CfgItemReportExcel_");
	tc_strcat(csvLog,sPartNo);
	tc_strcat(csvLog,"_");
	tc_strcat(csvLog,timeStrUpdatedNew);
	tc_strcat(csvLog,".csv");
	
	tc_strcpy(datasetIdValue,sPartNo);
	tc_strcat(datasetIdValue,"_Applicability_Matrix_");
	tc_strcat(datasetIdValue,timeStrUpdatedNew);

	
	FILE *flogPtr=fopen(logPath,"w+");
	FILE *fcsvPtr=fopen(csvLog,"w+");
	
	fprintf(flogPtr,"Date and Time - %s\n",timeStr);
	
	fprintf(flogPtr,"- sUserName         = %s\n",sUserName      );
	fprintf(flogPtr,"- sPasswordFile     = %s\n",sPasswordFile  );
	fprintf(flogPtr,"- sGroup            = %s\n",sGroup         );
	fprintf(flogPtr,"- sPartNo           = %s\n",sPartNo        );
	fprintf(flogPtr,"- sSysUser          = %s\n",sSysUser       );
	fprintf(flogPtr,"- logPath           = %s\n",logPath        );
	fprintf(flogPtr,"- csvLog            = %s\n",csvLog         );
	fprintf(flogPtr,"- datasetIdValue    = %s\n",datasetIdValue );
	fprintf(flogPtr,"- timeStrUpdated    = %s\n",timeStrUpdated );
	fprintf(flogPtr,"- timeStrUpdatedNew = %s\n",timeStrUpdatedNew );
		
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
    {
		printf("************************************************************************** \n Login successful to teamcenter \n");	
        fprintf(flogPtr,"Login Successfull to TC\n");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"Name of the logged in user = %s\n",loggedInUser);	
			itkApiCallStatus=ITK_APICALL(ITEM_find_item(sPartNo,&itemTag),flogPtr);
			fprintf(flogPtr,"API Status for ITEM_find_item = %d \n",itkApiCallStatus);
			if(itkApiCallStatus==0&&itemTag!=NULLTAG)
			{
				fprintf(flogPtr,"\n Executing If Condition C1 where item_tag found. ");
				fprintf(flogPtr,"Item with %s itemId found  .... \n",sPartNo);
				ITK_APICALL(AOM_ask_value_string(itemTag,"object_name"           ,&configObjName   ),flogPtr);
				ITK_APICALL(AOM_ask_value_string(itemTag,"object_string"         ,&configObjString ),flogPtr);
				ITK_APICALL(AOM_ask_value_string(itemTag,"object_desc"           ,&configObjdesc   ),flogPtr);
				   ITK_APICALL(AOM_ask_value_tag(itemTag,"cfg0ConfigPerspective" ,&configPerspective),flogPtr);
				   
			    fprintf(flogPtr,"- configObjName   = %s\n",configObjName  );
			    fprintf(flogPtr,"- configObjString = %s\n",configObjString);
			    fprintf(flogPtr,"- configObjdesc   = %s\n",configObjdesc  );
				
				fprintf(fcsvPtr,"\n\n");
				fprintf(fcsvPtr,",,Item Id,Object Name,Object String,Object Description \n");
				fprintf(fcsvPtr,",,%s,%s,%s,%s\n",sPartNo,configObjName,configObjString,configObjdesc);				 
				fprintf(fcsvPtr,"\n\n");
			    
			    if(configPerspective!=NULLTAG)
			    {
					fprintf(flogPtr,"\n Executing If Condition C2 where configPerspective found. ");
			 	    fprintf(flogPtr," Config Perspective Tag found..... \n"); 
			 		ITK_APICALL(AOM_ask_value_tags(configPerspective,"cfg0Models",&modelCount,&cfg0ModelsTags),flogPtr);
			 		ITK_APICALL(AOM_ask_value_tags(configPerspective,"cfg0AvailabilityRules",&ruleCount,&cfg0AvailabilityRulesTags),flogPtr);
			 		ITK_APICALL(AOM_ask_value_tags(configPerspective,"cfg0OptionValues",&optValueCount,&cfg0OptionValuesTags),flogPtr);
			 		
			 		fprintf(flogPtr,"- modelCount      = %d\n",modelCount   );
			 		fprintf(flogPtr,"- ruleCount       = %d\n",ruleCount    );
			 		fprintf(flogPtr,"- optValueCount   = %d\n",optValueCount);
			 		
			 		fprintf(flogPtr,"\n *************************** Product Model *********************************** \n");
			 		
					it=0;
					jt=0;
					
					masterArry[it][jt] = (char **)malloc(200*sizeof(char));														
				    tc_strcpy((char *)masterArry[it][jt],"Family");
					jt++;
					
					masterArry[it][jt] = (char **)malloc(200*sizeof(char));							
					tc_strcpy((char *)masterArry[it][jt],"Features");
														
			 		for(itr=0;itr<modelCount;itr++)
			 		{
						it=0;
					    jt=2+itr;
						
			 			ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags[itr],"object_name"  ,&variObjName   ),flogPtr);
			            ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags[itr],"object_string",&variObjString ),flogPtr);
			            ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags[itr],"object_desc"  ,&variObjdesc   ),flogPtr);	
                        ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags[itr],"cfg0ObjectId" ,&variObjId   ),flogPtr);	
                        ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags[itr],"cfg0FamilyObjectId" ,&variObjFamilyId   ),flogPtr);						
                        
						fprintf(flogPtr,"\n %d.) -variObjName = %s,   -variObjString = %s,   -variObjdesc = %s,    -variObjId = %s,  -variObjFamilyId = %s  ,-masterArry[it][jt] = %s",
						itr+1,variObjName,variObjString,variObjdesc,variObjId,variObjFamilyId,masterArry[it][jt]);
						
						masterArry[it][jt]= (char **)malloc(200*sizeof(char));
						tc_strcpy((char *)masterArry[it][jt],variObjFamilyId);
                        it++;
						
						masterArry[it][jt]= (char **)malloc(200*sizeof(char));
						tc_strcpy((char *)masterArry[it][jt],variObjId);	
					}
										
					it=2;
					jt=0;
			 		fprintf(flogPtr,"\n\n *************************** Features [Option values] ***********************************\n\n");
			 		
			 		for(itr=0;itr<optValueCount;itr++)
			 		{
			 			ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags[itr],"object_name"        ,&optValueObjName   ),flogPtr);
			            ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags[itr],"object_string"      ,&optValueObjString ),flogPtr);
			            ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags[itr],"object_desc"        ,&optValueObjdesc   ),flogPtr);
						ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags[itr],"cfg0ObjectId"       ,&optValueitemId   ),flogPtr);						
						ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags[itr],"cfg0FamilyObjectId" ,&optValueFamilyId   ),flogPtr);
						                       
						masterArry[it][jt]= (char **)malloc(200*sizeof(char));
						tc_strcpy((char *)masterArry[it][jt],optValueFamilyId);
                        jt++;	
                        
						masterArry[it][jt]= (char **)malloc(200*sizeof(char));
						tc_strcpy((char *)masterArry[it][jt],optValueitemId);
												
                        it++;
						jt=0;
						
			 			fprintf(flogPtr,"\n %d.) -optValueObjName = %s,-optValueObjString = %s,-optValueObjdesc = %s,-optValueitemId = %s -optValueFamilyId = %s",itr+1,optValueObjName,optValueObjString,optValueObjdesc,optValueitemId,optValueFamilyId);
			 		}
			 		
			 		fprintf(flogPtr,"\n\n *************************** Rule Availabily *********************************** \n\n");
			 		
			 		for(itr=0;itr<ruleCount;itr++)
			 		{
			 			ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags[itr],"object_name"                ,&ruleObjName   ),flogPtr);
			 			ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags[itr],"object_string"              ,&ruleObjString ),flogPtr);
			 			ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags[itr],"object_desc"                ,&ruleObjdesc   ),flogPtr);	
			 			ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags[itr],"cfg0ApplicabilityCondition" ,&ruleCondition ),flogPtr);	
			 			ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags[itr],"cfg0SubjectCondition"       ,&ruleSubject   ),flogPtr);	
			 			
			 			fprintf(flogPtr,"\n %d.) -ruleObjName = %s,-ruleObjString = %s,-ruleObjdesc = %s,-ruleCondition = %s,-ruleSubject = %s",
			 			itr+1,ruleObjName,ruleObjString,ruleObjdesc,ruleCondition,ruleSubject);
						
						fprintf(flogPtr,"\n -ruleCondition(L) = %d -ruleSubject(L) = %d",tc_strlen(ruleCondition),tc_strlen(ruleSubject));
						
						if(tc_strlen(ruleCondition)>0 && tc_strlen(ruleSubject)>0)
						{
							// Condition where Not equal also found.	
							
							if(tc_strstr(ruleCondition,"!=")!=NULL)
							{
								fprintf(flogPtr,"\n Error : Found [!=] in -ruleCondition execution stopped. ");
							}
                             
							// Condition where only equal to is only found.
                            else
							{
								stringConvertor(ruleCondition,ruleSubject,flogPtr);
								fprintf(flogPtr,"\n ***************************************************** \n");
								fprintf(flogPtr," -fVariantClassId = %s ,length = %d",fVariantClassId,tc_strlen(fVariantClassId));
								fprintf(flogPtr," -fVariantObjId   = %s ,length = %d",fVariantObjId  ,tc_strlen(fVariantObjId  ));
								fprintf(flogPtr," -fFeatureObjId   = %s ,length = %d",fFeatureObjId  ,tc_strlen(fFeatureObjId  ));
								fprintf(flogPtr," -fFamilyObjId    = %s ,length = %d",fFamilyObjId   ,tc_strlen(fFamilyObjId   ));
							    if(tc_strlen(fVariantClassId)>0 && tc_strlen(fVariantObjId) >0 && tc_strlen(fFeatureObjId) && tc_strlen(fFamilyObjId))
								{
									fprintf(flogPtr,"\n Executing If Condition C3 where Valid Length found. ");
									exeYes=0;
									for(mAit=2;mAit<=optValueCount+1;mAit++)
									{
										if(tc_strlen((char *)masterArry[mAit][0])>0 && tc_strlen((char *)masterArry[mAit][1]) && tc_strcmp(fFamilyObjId,(char *)masterArry[mAit][0])==0 && tc_strcmp(fFeatureObjId,(char *)masterArry[mAit][1])==0)
										{
											fprintf(flogPtr,"\n Executing If Condition C4 where Valid target fFamilyObjId and fFeatureObjId found. ");
											for(mAjt=0;mAjt<=modelCount+1;mAjt++)
											{
												if(tc_strlen((char *)masterArry[1][mAjt])>0 && tc_strcmp(fVariantObjId,(char *)masterArry[1][mAjt])==0 && tc_strcmp(fVariantClassId,(char *)masterArry[0][mAjt])==0)
												{
													fprintf(flogPtr,"\n Executing Final If Condition C5 where Valid target fVariantObjId and fVariantClassId found. ");
												    fprintf(flogPtr,"\n -mAit = %d",mAit);
													fprintf(flogPtr,"\n -mAjt = %d",mAjt);
													
													masterArry[mAit][mAjt]= (char **)malloc(10*sizeof(char));
													tc_strcpy((char *)masterArry[mAit][mAjt],"Yes");
													exeYes=1;
												}
												if(exeYes==1)
												{
													break;
												}
											}
										}
										if(exeYes==1)
										{
											break;
										}
									}
								}
								else
								{
									fprintf(flogPtr,"\n Error : Invalid Length found. ");
								}
							}								
						}
					}
									
					fprintf(flogPtr,"\n");
					//fprintf(fcsvPtr,"\n\n");
					for(mAit=0;mAit<=optValueCount+1;mAit++)
					{
						for(mAjt=0;mAjt<=modelCount+1;mAjt++)
						{
							if(tc_strlen((char *)masterArry[mAit][mAjt])==0)
							{
								masterArry[mAit][mAjt]=(char **)malloc(5*sizeof(char));
								tc_strcpy((char *)masterArry[mAit][mAjt],"");
							}												
							fprintf(flogPtr,"%s,",masterArry[mAit][mAjt]);
							fprintf(fcsvPtr,",%s",masterArry[mAit][mAjt]);
						}
						fprintf(flogPtr,"\n");
						fprintf(fcsvPtr,"\n");
					}
                    fprintf(flogPtr,"\n \n Execution completed logs and csv created ."); 
                    printf("\n Execution completed logs and csv created ."); 
                    fclose(fcsvPtr);				
					fprintf(flogPtr," \n ************** Execution  started for Folder creation and dataset Attachment **************** ");
					
					sysUserId=subString(sSysUser,4,tc_strlen(sSysUser));
					fprintf(flogPtr,"\n -sysUserId = %s ",sysUserId);
					
					ITK_APICALL(SA_find_user2(sysUserId,&sysUserTag),flogPtr);
					if(sysUserTag!=NULLTAG)
					{
						ITK_APICALL(SA_ask_user_home_folder(sysUserTag,&sysHomeTag),flogPtr);
						if(sysHomeTag!=NULLTAG)
						{
							ITK_APICALL(FL_ask_references(sysHomeTag,FL_fsc_as_ordered,&noOfReferences,&folderReference),flogPtr);
							fprintf(flogPtr,"\n -noOfReferences = %d",noOfReferences);
							for(itr=0;itr<noOfReferences;itr++)
							{
								ITK_APICALL(AOM_ask_value_string(folderReference[itr],"object_name",&fldObjName   ),flogPtr);
								ITK_APICALL(AOM_ask_value_string(folderReference[itr],"object_type",&fldObjType   ),flogPtr);
								
								fprintf(flogPtr,"\n -fldObjName = %s",fldObjName);
								fprintf(flogPtr,"\n -fldObjType = %s",fldObjType);
								
								
								if(tc_strcmp(fldObjType,"Folder")==0 && tc_strcmp(fldObjName,"Product Configurator Reports") == 0)
								{
									fldFound=1;
									newFolderTag=folderReference[itr];
									break;
								}
				
							}
							fprintf(flogPtr,"\n -fldFound = %d",fldFound);
							fprintf(flogPtr,"\n- datasetIdValue     = %s\n",datasetIdValue );
							if(fldFound==1)
							{
								fprintf(flogPtr,"\n ************** Dataset Attachment started Folder available **************** ");

								itkApiCallStatus=ITK_APICALL(AE_find_datasettype2("MSExcel",&datasetType),flogPtr);
				                fprintf(flogPtr,"ITK status for AE_find_datasettype2 = %d\n",itkApiCallStatus);

								if(datasetType!=NULLTAG)
								{
                                    itkApiCallStatus=ITK_APICALL(AE_create_dataset_with_id(datasetType,datasetIdValue,datasetIdValue,datasetIdValue,"A",&datasetTag),flogPtr);
				                    fprintf(flogPtr,"ITK status for AE_create_dataset_with_id = %d\n",itkApiCallStatus);
									if(datasetTag!=NULLTAG)
									{
                                        itkApiCallStatus=ITK_APICALL(AE_import_named_ref(datasetTag,"excel",csvLog,datasetIdValue,SS_TEXT),flogPtr);
				                        fprintf(flogPtr,"ITK status for AE_import_named_ref = %d\n",itkApiCallStatus); 

									    itkApiCallStatus=ITK_APICALL(AOM_refresh(datasetTag,TRUE),flogPtr);
				                        fprintf(flogPtr,"ITK status for AOM_refresh = %d\n",itkApiCallStatus);
				                                  
                                        itkApiCallStatus=ITK_APICALL(AOM_save_without_extensions(datasetTag),flogPtr);
				                        fprintf(flogPtr,"ITK status for AOM_save = %d\n",itkApiCallStatus);	
										
										// Attachment of dataset started with folder.
										
										 fprintf(flogPtr,"\n ************** Dataset Attachment started with folder **************** ");

								         ITK_APICALL(FL_insert(newFolderTag,datasetTag,999),flogPtr);
								         fprintf(flogPtr,"\n ITK status for FL_insert = %d\n",itkApiCallStatus);
								         
								         ITK_APICALL(AOM_save_without_extensions(newFolderTag),flogPtr);
								         fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);

										 fprintf(flogPtr,"\n \n *********************** Process completed successfully. ********************************** ");
									}
									else
									{
										fprintf(flogPtr,"\n Error : datasetTag == NULLTAG. Dataset not created.");
									}
								}
								else
								{
									fprintf(flogPtr,"\n Error : datasetType not found.");
								}
								
							}
							else
							{
								fprintf(flogPtr,"\n ************** Folder creation  started **************** ");
								
								ITK_APICALL(FL_create2("Product Configurator Reports","Product Configurator Reports",&newFolderTag),flogPtr);
                                fprintf(flogPtr,"\n ITK status for FL_create2 = %d\n",itkApiCallStatus);
                                if(newFolderTag!=NULLTAG)
								{
                                    ITK_APICALL(AOM_save_without_extensions(newFolderTag),flogPtr);
                                    fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);

								    ITK_APICALL(FL_insert(sysHomeTag,newFolderTag,000),flogPtr);
									fprintf(flogPtr,"\n ITK status for FL_insert = %d\n",itkApiCallStatus);

								    ITK_APICALL(AOM_save_without_extensions(sysHomeTag),flogPtr);
									fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);
                                    
									fprintf(flogPtr,"\n ************** Dataset Attachment started After folder Creation. **************** ");
                                    itkApiCallStatus=ITK_APICALL(AE_find_datasettype2("MSExcel",&datasetType),flogPtr);
				                    fprintf(flogPtr,"ITK status for AE_find_datasettype2 = %d\n",itkApiCallStatus);

								    if(datasetType!=NULLTAG)
								    {
                                        itkApiCallStatus=ITK_APICALL(AE_create_dataset_with_id(datasetType,datasetIdValue,datasetIdValue,datasetIdValue,"A",&datasetTag),flogPtr);
				                        fprintf(flogPtr,"ITK status for AE_create_dataset_with_id = %d\n",itkApiCallStatus);
								    	if(datasetTag!=NULLTAG)
								    	{
                                            itkApiCallStatus=ITK_APICALL(AE_import_named_ref(datasetTag,"excel",csvLog,datasetIdValue,SS_TEXT),flogPtr);
				                            fprintf(flogPtr,"ITK status for AE_import_named_ref = %d\n",itkApiCallStatus); 
								    
								    	    itkApiCallStatus=ITK_APICALL(AOM_refresh(datasetTag,TRUE),flogPtr);
				                            fprintf(flogPtr,"ITK status for AOM_refresh = %d\n",itkApiCallStatus);
				                                      
                                            itkApiCallStatus=ITK_APICALL(AOM_save_without_extensions(datasetTag),flogPtr);
				                            fprintf(flogPtr,"ITK status for AOM_save = %d\n",itkApiCallStatus);	
								    		
								    		// Attachment of dataset started with folder.
								    		
								    		 fprintf(flogPtr,"\n ************** Dataset Attachment started with folder **************** ");
								    
								             ITK_APICALL(FL_insert(newFolderTag,datasetTag,999),flogPtr);
								             fprintf(flogPtr,"\n ITK status for FL_insert = %d\n",itkApiCallStatus);
								             
								             ITK_APICALL(AOM_save_without_extensions(newFolderTag),flogPtr);
								             fprintf(flogPtr,"\n ITK status for AOM_save_without_extensions = %d\n",itkApiCallStatus);

											 fprintf(flogPtr,"\n \n *********************** Process completed successfully. ********************************** ");
								    	}
								    	else
								    	{
								    		fprintf(flogPtr,"\n Error : datasetTag == NULLTAG. Dataset not created.");
								    	}
								    }
								    else
								    {
								    	fprintf(flogPtr,"\n Error : datasetType not found.");
								    }                                                                      
								}
								else
								{
                                    fprintf(flogPtr,"\n Error : New Creation failed.");
								}																								 
							}
						}
						else
						{
							fprintf(flogPtr,"Error : Home folder Tag not found.");
						}
					}						
					else
					{
						fprintf(flogPtr,"Error : System user Tag not found.");
					}
			    }
			    else
			    {
			 	   fprintf(flogPtr,"\n Error : Config Perspective Tag NOT found."); 
			    }
			}
			else
			{
				fprintf(flogPtr,"\n Error : Item with %s itemId not found in Teamcenter.",sPartNo);
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