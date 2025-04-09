/*Date - 03-01-2025 */

/*Objective :- CfgCmpVariantReport utility will compare two variants and will 
               attach report in target folder attached in user's home folder. */

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
	char *sUserName	          =   NULL;
    char *sPasswordFile	      =   NULL;
    char *sGroup              =   NULL;
	char *sPartNo             =   NULL;
	char *sHelp               =   NULL;
	char *loggedInUser        =   NULL;
	char *timeStr             =   NULL;
	char *timeStrUpdated      =   NULL;
	char *timeStrUpdatedNew   =   NULL;
	char *itemCmpData         =   NULL;
	char *sSysUser            =   NULL;
	char *cfgRuleItemName     =   NULL;
	char *sPartNo1            =   (char*) MEM_alloc(200*sizeof(char));
	char *sPartNo2            =   (char*) MEM_alloc(200*sizeof(char));
	char *var1                =   (char*) MEM_alloc(200*sizeof(char));
	char *var2                =   (char*) MEM_alloc(200*sizeof(char));
	char *configObjName1      =   NULL;
	char *configObjString1    =   NULL;
	char *configObjdesc1      =   NULL; 
	char *variObjName1        =   NULL;   
	char *variObjString1      =   NULL;
	char *variObjdesc1        =   NULL;
	char *variObjId1          =   NULL;
	char *variObjFamilyId1    =   NULL;
	char *optValueitemId1     =   NULL;
	char *optValueFamilyId1   =   NULL;
	
	char *optValueObjName1    =   NULL;
	char *optValueObjString1  =   NULL;
	char *optValueObjdesc1    =   NULL;
	char *ruleObjName1        =   NULL;
	char *ruleObjString1      =   NULL;
	char *ruleObjdesc1        =   NULL;
	char *ruleCondition1      =   NULL;
	char *ruleSubject1        =   NULL;
	char *tProModId1          =   NULL;
	char *tFeature1           =   NULL;
	
	char *configObjName2      =   NULL;
	char *configObjString2    =   NULL;
	char *configObjdesc2      =   NULL; 
	char *variObjName2        =   NULL;   
	char *variObjString2      =   NULL;
	char *variObjdesc2        =   NULL;
	char *variObjId2          =   NULL;
	char *variObjFamilyId2    =   NULL;
	char *optValueitemId2     =   NULL;
	char *optValueFamilyId2   =   NULL;
	char *optValueObjName2    =   NULL;
	char *optValueObjString2  =   NULL;
	char *optValueObjdesc2    =   NULL;
	char *ruleObjName2        =   NULL;
	char *ruleObjString2      =   NULL;
	char *ruleObjdesc2        =   NULL;
	char *ruleCondition2      =   NULL;
	char *ruleSubject2        =   NULL;
	char *tProModId2          =   NULL;
	char *tFeature2           =   NULL;
	
	char *tProModId           =   NULL;
	char *tFeature            =   NULL;
	char *fldObjName          =   NULL;
	char *fldObjType          =   NULL;
		
	char **masterArry[2000][200];	
	 
    char *logPath            =   (char*) MEM_alloc(200*sizeof(char));
	char *csvLog             =   (char*) MEM_alloc(200*sizeof(char));
	char *datasetIdValue     =   (char*) MEM_alloc(200*sizeof(char));
	char *tokerArray         =   (char*) MEM_alloc(200*sizeof(char));
	
		
	char *temp1              =   (char*) MEM_alloc(200*sizeof(char));
	char *fTVariantClassId   =   (char*) MEM_alloc(200*sizeof(char));
	char *sysUserId          =   (char*) MEM_alloc(200*sizeof(char));
	
	
	int itkApiCallStatus,itr,jtr,toker=1,iToker = 0;
	
	int modelCount1             = 0;
	int ruleCount1              = 0;
	int optValueCount1          = 0;
	int configPerspectiveCount1 = 0;
	
	int modelCount2             = 0;
	int ruleCount2              = 0;
	int optValueCount2          = 0;
	int configPerspectiveCount2 = 0;
	
	int it=0,jt=0,tPro=0,tFea=0,i=0;
	int mAit   = 0;
	int mAjt   = 0;
	int exeYes = 0;
	int found1 = 0;
	int found2 = 0;
	int cfgTagCount  = 0;
	int fldFound = 0;
	int noOfReferences = 0;
	
	tag_t loggedInUserTag             =  NULLTAG;
	tag_t configPerspective1          =  NULLTAG;
	tag_t configPerspective2          =  NULLTAG;
	tag_t itemTag1                    =  NULLTAG;
	tag_t itemTag2                    =  NULLTAG;
	tag_t sysUserTag                  = NULLTAG;
    tag_t sysHomeTag                  = NULLTAG;
	tag_t newFolderTag                = NULLTAG;
	tag_t datasetType                 = NULLTAG; 
	tag_t datasetTag                  = NULLTAG;
	tag_t rdatasetTag                 = NULLTAG;
	
	tag_t *cfgRuleItemTag                =  NULLTAG;		
	tag_t *cfg0ModelsTags1               =  NULLTAG;
	tag_t *cfg0AvailabilityRulesTags1    =  NULLTAG;
	tag_t *cfg0OptionValuesTags1         =  NULLTAG;
	tag_t *cfg0ModelsTags2               =  NULLTAG;
	tag_t *cfg0AvailabilityRulesTags2    =  NULLTAG;
	tag_t *cfg0OptionValuesTags2         =  NULLTAG;
	tag_t *folderReference               = NULLTAG;
	
	
	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
	
    tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"CfgCmpVariReportLog_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
	tc_strcpy(csvLog,"/tmp/");  //  /tmp/ - to be added if required
	tc_strcat(csvLog,"CfgCmpVariReportExcel_");
	tc_strcat(csvLog,timeStrUpdatedNew);
	tc_strcat(csvLog,".csv");
    
	tc_strcat(datasetIdValue,"CompareVariant_Report_");
	tc_strcat(datasetIdValue,timeStrUpdatedNew);

    FILE *flogPtr = fopen(logPath,"w+");
	FILE *fcsvPtr = fopen(csvLog,"w+");
	
	fprintf(flogPtr,"Date and Time - %s\n",timeStr);
	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	itemCmpData    = ITK_ask_cli_argument("-i=");
    sSysUser       = ITK_ask_cli_argument("-j=");
	
	fprintf(flogPtr,"\n  -sUserName          = %s\n",sUserName      );
	fprintf(flogPtr,"    -sPasswordFile      = %s\n",sPasswordFile  );
	fprintf(flogPtr,"    -sGroup             = %s\n",sGroup         );
	fprintf(flogPtr,"    -i                  = %s\n",itemCmpData    );
	fprintf(flogPtr,"    -j                  = %s\n",sSysUser       );
	fprintf(flogPtr,"    - logPath           = %s\n",logPath        );
	fprintf(flogPtr,"    - csvLog            = %s\n",csvLog         );
	fprintf(flogPtr,"    - datasetIdValue    = %s\n",datasetIdValue );
	fprintf(flogPtr,"    - timeStrUpdatedNew = %s\n",timeStrUpdatedNew );
		
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr);
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
    {
		printf(" \n ******************* Login successful to teamcenter ******************* \n");	
        fprintf(flogPtr,"\n Login Successfull to TC");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
		{
		    fprintf(flogPtr,"\n Name of the logged in user = %s\n",loggedInUser);	
			for(i=0;itemCmpData[i]!='\0';i++)
			{
				if(itemCmpData[i]=='|')
				{
					tokerArray[iToker]='\0';
					iToker=0;
					if(toker==1)
					{
						tc_strcpy(sPartNo1,tokerArray);
					}
					else if(toker==2)
					{
						tc_strcpy(var1,tokerArray);
					}
					else
					{
						tc_strcpy(sPartNo2,tokerArray);
					}
					toker++;
				}
				else if(i+1==tc_strlen(itemCmpData)||itemCmpData[i+1]=='\0')
				{
					tokerArray[iToker]=itemCmpData[i];
					iToker++;
					tokerArray[iToker]='\0';
					if(toker==4)
					{
						tc_strcpy(var2,tokerArray);
					}
				}
				else
				{
					tokerArray[iToker]=itemCmpData[i];
					iToker++;
				}
			}			
			fprintf(flogPtr," -sPartNo1  = %s\n",sPartNo1);
			fprintf(flogPtr," -var1      = %s\n",var1);
			fprintf(flogPtr," -sPartNo2  = %s\n",sPartNo2);
			fprintf(flogPtr," -var2      = %s\n",var2);
																			
			if(tc_strlen(sPartNo1)>0 && tc_strlen(var1)>0 && tc_strlen(sPartNo2)>0 && tc_strlen(var2)>0)
			{
				fprintf(flogPtr,"\n Executing If Condition C1 where data entered is valid. ");
				itkApiCallStatus=ITK_APICALL(ITEM_find_item(sPartNo1,&itemTag1),flogPtr);
				itkApiCallStatus=ITK_APICALL(ITEM_find_item(sPartNo2,&itemTag2),flogPtr);
				if(itemTag1!=NULLTAG && itemTag2!=NULLTAG)
				{
					fprintf(flogPtr,"\n Executing If Condition C2 where both configContext found. ");
					ITK_APICALL(AOM_ask_value_string(itemTag1,"object_name"           ,&configObjName1   ),flogPtr);
				    ITK_APICALL(AOM_ask_value_string(itemTag1,"object_string"         ,&configObjString1 ),flogPtr);
				    ITK_APICALL(AOM_ask_value_string(itemTag1,"object_desc"           ,&configObjdesc1   ),flogPtr);
				    ITK_APICALL(AOM_ask_value_tag(itemTag1,"cfg0ConfigPerspective" ,&configPerspective1),flogPtr);
				
				    ITK_APICALL(AOM_ask_value_string(itemTag2,"object_name"           ,&configObjName2   ),flogPtr);
				    ITK_APICALL(AOM_ask_value_string(itemTag2,"object_string"         ,&configObjString2 ),flogPtr);
				    ITK_APICALL(AOM_ask_value_string(itemTag2,"object_desc"           ,&configObjdesc2   ),flogPtr);
				    ITK_APICALL(AOM_ask_value_tag(itemTag2,"cfg0ConfigPerspective" ,&configPerspective2),flogPtr);
				
				    fprintf(flogPtr,"- configObjName1   = %s\n",configObjName1  );
			        fprintf(flogPtr,"- configObjString1 = %s\n",configObjString1);
			        fprintf(flogPtr,"- configObjdesc1   = %s\n",configObjdesc1  );
				
				    fprintf(flogPtr,"- configObjName2    = %s\n",configObjName2  );
			        fprintf(flogPtr,"- configObjString2  = %s\n",configObjString2);
			        fprintf(flogPtr,"- configObjdesc2    = %s\n",configObjdesc2  );
				   
				   
				    if(configPerspective1!=NULLTAG && configPerspective2!=NULLTAG)
				    {
					    fprintf(flogPtr,"\n Executing If Condition C3 where both configPerspective found."); 
						fprintf(fcsvPtr,"\n");
				        fprintf(fcsvPtr,",,Variant compare Report ");
				        fprintf(fcsvPtr,"\n,,Id,Name,Object,Description,Variant ID");
				        fprintf(fcsvPtr,"\n,,%s,%s,%s,%s,%s",sPartNo1,configObjName1,configObjString1,configObjdesc1,var1);
				        fprintf(fcsvPtr,"\n,,%s,%s,%s,%s,%s",sPartNo2,configObjName2,configObjString2,configObjdesc2,var2);
                        
                        ITK_APICALL(AOM_ask_value_tags(configPerspective1,"cfg0Models",&modelCount1,&cfg0ModelsTags1),flogPtr);
			 		    ITK_APICALL(AOM_ask_value_tags(configPerspective1,"cfg0AvailabilityRules",&ruleCount1,&cfg0AvailabilityRulesTags1),flogPtr);
			 		    ITK_APICALL(AOM_ask_value_tags(configPerspective1,"cfg0OptionValues",&optValueCount1,&cfg0OptionValuesTags1),flogPtr);
										
					    ITK_APICALL(AOM_ask_value_tags(configPerspective2,"cfg0Models",&modelCount2,&cfg0ModelsTags2),flogPtr);
			 		    ITK_APICALL(AOM_ask_value_tags(configPerspective2,"cfg0AvailabilityRules",&ruleCount2,&cfg0AvailabilityRulesTags2),flogPtr);
			 		    ITK_APICALL(AOM_ask_value_tags(configPerspective2,"cfg0OptionValues",&optValueCount2,&cfg0OptionValuesTags2),flogPtr);
					
			 		
			 		    fprintf(flogPtr,"\n- modelCount1      = %d",modelCount1   );
			 		    fprintf(flogPtr,"\n- ruleCount1       = %d",ruleCount1    );
			 		    fprintf(flogPtr,"\n- optValueCount1   = %d",optValueCount1);
                                         
					    fprintf(flogPtr,"\n- modelCount2      = %d",modelCount2   );
			 		    fprintf(flogPtr,"\n- ruleCount2       = %d",ruleCount2    );
			 		    fprintf(flogPtr,"\n- optValueCount2   = %d",optValueCount2);

						fprintf(flogPtr,"\n *************************** Product Model *********************************** \n");
						for(itr=0;itr<modelCount1;itr++)
			 		    {
			 		    	ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags1[itr],"object_name"  ,&variObjName1   ),flogPtr);
			                ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags1[itr],"object_string",&variObjString1 ),flogPtr);
			                ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags1[itr],"object_desc"  ,&variObjdesc1   ),flogPtr);	
                            ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags1[itr],"cfg0ObjectId" ,&variObjId1     ),flogPtr);
                            ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags1[itr],"cfg0FamilyObjectId",&variObjFamilyId1     ),flogPtr);								
                            
					    	if(tc_strcmp(variObjId1,var1)==0)
							{
								found1=1;
								break;
							}
					    }
						for(itr=0;itr<modelCount2;itr++)
			 		    {
			 		    	ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags2[itr],"object_name"  ,&variObjName2   ),flogPtr);
			                ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags2[itr],"object_string",&variObjString2 ),flogPtr);
			                ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags2[itr],"object_desc"  ,&variObjdesc2   ),flogPtr);	
                            ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags2[itr],"cfg0ObjectId" ,&variObjId2     ),flogPtr);	
                            ITK_APICALL(AOM_ask_value_string(cfg0ModelsTags2[itr],"cfg0FamilyObjectId" ,&variObjFamilyId2     ),flogPtr);								
                            
					    	if(tc_strcmp(variObjId2,var2)==0)
							{
								found2=1;					
								break;
							}
					    }
						if(found1 == 1 && found2 ==1 )
						{
							fprintf(flogPtr,"\n Executing If Condition C4 where both variant IDs are valid. \n");							
							
							fprintf(fcsvPtr,"\n\n");
							masterArry[it][jt] = (char **)malloc(200*sizeof(char));														
						    tc_strcpy((char *)masterArry[it][jt],"Family");
						    jt++;
							
							masterArry[it][jt] = (char **)malloc(200*sizeof(char));							
						    tc_strcpy((char *)masterArry[it][jt],"Feature");
						    jt++;
							
							tc_strcpy(temp1,"");
							tc_strcpy(temp1,sPartNo1);
							tc_strcat(temp1,"/");
							tc_strcat(temp1,variObjFamilyId1);
							fprintf(flogPtr," -temp1[1] = %s",temp1);
																																														
							masterArry[it][jt] = (char **)malloc(200*sizeof(char));														
							tc_strcpy((char *)masterArry[it][jt],temp1);							
						    jt++;
							
							tc_strcpy(temp1,"");
							tc_strcpy(temp1,sPartNo2);
							tc_strcat(temp1,"/");
							tc_strcat(temp1,variObjFamilyId2);
							fprintf(flogPtr," -temp1[2] = %s",temp1);
																												
							masterArry[it][jt] = (char **)malloc(200*sizeof(char));							
						    tc_strcpy((char *)masterArry[it][jt],temp1);						    
							
							it++;
							jt=2;
							masterArry[it][jt] = (char **)malloc(200*sizeof(char));							
						    tc_strcpy((char *)masterArry[it][jt],variObjId1);
						    jt++;
							
							masterArry[it][jt] = (char **)malloc(200*sizeof(char));							
						    tc_strcpy((char *)masterArry[it][jt],variObjId2);
						    														
							it=2;
					        jt=0;
			 		        fprintf(flogPtr,"\n\n *************************** Features [Option values] ***********************************\n\n");
					        for(itr=0;itr<optValueCount1;itr++)
			 		        {
								found1=0;
			 		        	ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags1[itr],"object_name"  ,&optValueObjName1   ),flogPtr);
			                    ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags1[itr],"object_string",&optValueObjString1 ),flogPtr);
			                    ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags1[itr],"object_desc"  ,&optValueObjdesc1   ),flogPtr);
					        	ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags1[itr],"cfg0ObjectId" ,&optValueitemId1    ),flogPtr);
								ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags1[itr],"cfg0FamilyObjectId" ,&optValueFamilyId1  ),flogPtr);
					        	     	
			 		        	fprintf(flogPtr,"\n %d.) -optValueObjName1 = %s,-optValueObjString1 = %s,-optValueObjdesc1 = %s,-optValueitemId1 = %s ,-optValueFamilyId1 = %s",itr+1,optValueObjName1,optValueObjString1,optValueObjdesc1,optValueitemId1,optValueFamilyId1);
			 		            for(jtr=1;jtr<it;jtr++)
								{
									fprintf(flogPtr,"\n -masterArry ID[][] = %s,-masterArry Name[][] = %s",masterArry[jtr][0],masterArry[jtr][1]);
									if(tc_strcmp((char*)masterArry[jtr][0],optValueFamilyId1)==0 && tc_strcmp((char*)masterArry[jtr][1],optValueitemId1)==0)
									{
										found1=1;
										fprintf(flogPtr,"\n *****Duplicate Matching Feature with same ID Found in Variant 1************ \n");
										break;
									}
								}
								if(found1==0)
								{
									masterArry[it][jt]= (char **)malloc(200*sizeof(char));
					        	    tc_strcpy((char *)masterArry[it][jt],optValueFamilyId1);
                                    jt++;	
                                
					        	    masterArry[it][jt]= (char **)malloc(200*sizeof(char));
					        	    tc_strcpy((char *)masterArry[it][jt],optValueitemId1);
					        							
                                    it++;
					        	    jt=0;
								}										
							}
							fprintf(flogPtr,"\n -it[p1] = %d",it);
							fprintf(flogPtr,"\n -jt[p1] = %d",jt);
							
					        for(itr=0;itr<optValueCount2;itr++)
							{
								found1=0;
								ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags2[itr],"object_name"  ,&optValueObjName2   ),flogPtr);
			                    ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags2[itr],"object_string",&optValueObjString2 ),flogPtr);
			                    ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags2[itr],"object_desc"  ,&optValueObjdesc2   ),flogPtr);
					        	ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags2[itr],"cfg0ObjectId" ,&optValueitemId2   ),flogPtr);
								ITK_APICALL(AOM_ask_value_string(cfg0OptionValuesTags2[itr],"cfg0FamilyObjectId" ,&optValueFamilyId2   ),flogPtr);
							 
							    fprintf(flogPtr,"\n %d.) -optValueObjName2 = %s,-optValueObjString2 = %s,-optValueObjdesc2 = %s,-optValueitemId2 = %s,-optValueFamilyId2 = %s",itr+1,optValueObjName2,optValueObjString2,optValueObjdesc2,optValueitemId2,optValueFamilyId2);
							    
								for(jtr=1;jtr<it;jtr++)
								{
									fprintf(flogPtr,"\n -masterArry ID[][] = %s,-masterArry Name[][] = %s",masterArry[jtr][0],masterArry[jtr][1]);
									if(tc_strcmp((char*)masterArry[jtr][0],optValueFamilyId2)==0 && tc_strcmp((char*)masterArry[jtr][1],optValueitemId2)==0)
									{
										found1=1;
										fprintf(flogPtr,"\n ***** Matching Feature Found ************ \n");
										break;
									}
								}
								if(found1==0)
								{
									masterArry[it][jt]= (char **)malloc(200*sizeof(char));
					        	    tc_strcpy((char *)masterArry[it][jt],optValueFamilyId2);
                                    jt++;	
                                
					        	    masterArry[it][jt]= (char **)malloc(200*sizeof(char));
					        	    tc_strcpy((char *)masterArry[it][jt],optValueitemId2);
					        							
                                    it++;
					        	    jt=0; 
								}							
							}
							fprintf(flogPtr,"\n -it[p2] = %d",it);
							fprintf(flogPtr,"\n -jt[p2] = %d",jt);
							
							fprintf(flogPtr,"\n ******************************************** Executing RuleCount1 ************************************** \n");
							
							for(itr=0;itr<ruleCount1;itr++)
							{
								ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags1[itr],"object_name"                ,&ruleObjName1   ),flogPtr);
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags1[itr],"object_string"              ,&ruleObjString1 ),flogPtr);
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags1[itr],"object_desc"                ,&ruleObjdesc1   ),flogPtr);	
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags1[itr],"cfg0ApplicabilityCondition" ,&ruleCondition1 ),flogPtr);	
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags1[itr],"cfg0SubjectCondition"       ,&ruleSubject1   ),flogPtr);	
								   ITK_APICALL(AOM_ask_value_tags(cfg0AvailabilityRulesTags1[itr],"cfg0ProductItems",&cfgTagCount,&cfgRuleItemTag ),flogPtr);
								   ITK_APICALL(AOM_ask_value_string(cfgRuleItemTag[0],"item_id",&cfgRuleItemName ),flogPtr);
								   
									        
								fprintf(flogPtr,"\n \n %d.) -ruleObjName1 = %s,-ruleObjString1 = %s,-ruleObjdesc1 = %s,-ruleCondition1 = %s,-ruleSubject1 = %s -cfgRuleItemName[1] = %s ,-sPartNo1 = %s",
			 			                    itr+1,ruleObjName1,ruleObjString1,ruleObjdesc1,ruleCondition1,ruleSubject1,cfgRuleItemName,sPartNo1);
									
                                fprintf(flogPtr,"\n -ruleCondition1(L) = %d -ruleSubject1(L) = %d",tc_strlen(ruleCondition1),tc_strlen(ruleSubject1));								
								
								if(tc_strlen(ruleCondition1)>0 && tc_strlen(ruleSubject1)>0)
						        {							       							    								    								
								    // Condition where Not equal also found.										
							        if(tc_strstr(ruleCondition1,"!=")!=NULL)
							        {
							        	fprintf(flogPtr,"\n Error : Found [!=] in -ruleCondition1. ");
							        }	
									
								    // Condition where only equal to is only found.
                                    stringConvertor(ruleCondition1,ruleSubject1,flogPtr);
								    fprintf(flogPtr,"\n ***************************************************** \n");
								    fprintf(flogPtr," -fVariantClassId = %s ,length = %d\n",fVariantClassId,tc_strlen(fVariantClassId));
								    fprintf(flogPtr," -fVariantObjId   = %s ,length = %d\n",fVariantObjId  ,tc_strlen(fVariantObjId  ));
								    fprintf(flogPtr," -fFeatureObjId   = %s ,length = %d\n",fFeatureObjId  ,tc_strlen(fFeatureObjId  ));
								    fprintf(flogPtr," -fFamilyObjId    = %s ,length = %d\n",fFamilyObjId   ,tc_strlen(fFamilyObjId   ));	

                                    tc_strcpy(fTVariantClassId,"");
									tc_strcpy(fTVariantClassId,sPartNo1);
									tc_strcat(fTVariantClassId,"/");
									tc_strcat(fTVariantClassId,fVariantClassId);															
									fprintf(flogPtr," -fTVariantClassId = %s ,length = %d",fTVariantClassId,tc_strlen(fTVariantClassId));
								    
									if(tc_strlen(fVariantClassId)>0 && tc_strlen(fVariantObjId) >0 && tc_strlen(fFeatureObjId) && tc_strlen(fFamilyObjId) && tc_strcmp(sPartNo1,cfgRuleItemName)==0)
								    {
								    	fprintf(flogPtr,"\n ###### Executing If Condition C5 where Valid length found After StringConvertor function. ######");
								    	exeYes=0;
								    	for(mAit=2;mAit<it;mAit++)
								    	{
								    		if(tc_strlen((char *)masterArry[mAit][0])>0 && tc_strlen((char *)masterArry[mAit][1])>0 && tc_strcmp(fFamilyObjId,(char *)masterArry[mAit][0])==0 && tc_strcmp(fFeatureObjId,(char *)masterArry[mAit][1])==0)
								    		{
								    			fprintf(flogPtr,"\n ###### Executing if Condition C6 where -fFamilyObjId == masterArry[mAit][0] && -fFeatureObjId == masterArry[mAit][1] ######");
								    			for(mAjt=0;mAjt<4;mAjt++)
								    			{
								    				if(tc_strlen((char *)masterArry[0][mAjt])>0&& tc_strlen((char *)masterArry[1][mAjt])>0 && tc_strcmp(fTVariantClassId,(char *)masterArry[0][mAjt])==0 && tc_strcmp(fVariantObjId,(char *)masterArry[1][mAjt])==0)
								    				{
								    					fprintf(flogPtr,"\n ###### Executing final if Condition C7 where -fTVariantClassId == masterArry[0][mAjt] && -fVariantObjId == masterArry[1][mAjt] ######");
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
								}																										
							}
							
							fprintf(flogPtr,"\n ******************************************** Executing RuleCount2 ************************************** \n");
							
							for(itr=0;itr<ruleCount2;itr++)
							{
								ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags2[itr],"object_name"                ,&ruleObjName2   ),flogPtr);
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags2[itr],"object_string"              ,&ruleObjString2 ),flogPtr);
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags2[itr],"object_desc"                ,&ruleObjdesc2   ),flogPtr);	
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags2[itr],"cfg0ApplicabilityCondition" ,&ruleCondition2 ),flogPtr);	
			 			        ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags2[itr],"cfg0SubjectCondition"       ,&ruleSubject2   ),flogPtr);
                                ITK_APICALL(AOM_ask_value_tags(cfg0AvailabilityRulesTags2[itr],"cfg0ProductItems",&cfgTagCount,&cfgRuleItemTag ),flogPtr);
								   ITK_APICALL(AOM_ask_value_string(cfgRuleItemTag[0],"item_id",&cfgRuleItemName ),flogPtr);								
			 			        
								fprintf(flogPtr,"\n \n %d.) -ruleObjName2 = %s,-ruleObjString2 = %s,-ruleObjdesc2 = %s,-ruleCondition2 = %s,-ruleSubject2 = %s,-cfgRuleItemName[2] = %s ,-sPartNo2 = %s",
			 			                    itr+1,ruleObjName2,ruleObjString2,ruleObjdesc2,ruleCondition2,ruleSubject2,cfgRuleItemName,sPartNo2);
								
								fprintf(flogPtr,"\n -ruleCondition2(L) = %d -ruleSubject2(L) = %d",tc_strlen(ruleCondition2),tc_strlen(ruleSubject2));				
								
								if(tc_strlen(ruleCondition2)>0 && tc_strlen(ruleSubject2)>0)
						        {								
								    // Condition where Not equal also found.										
							        if(tc_strstr(ruleCondition2,"!=")!=NULL)
							        {
							        	fprintf(flogPtr,"\n Error : Found [!=] in -ruleCondition2. ");
							        }	
									
								    // Condition where only equal to is only found.
                                    stringConvertor(ruleCondition2,ruleSubject2,flogPtr);
	                                fprintf(flogPtr,"\n ***************************************************** \n");
	                                fprintf(flogPtr," -fVariantClassId = %s ,length = %d",fVariantClassId,tc_strlen(fVariantClassId));
	                                fprintf(flogPtr," -fVariantObjId   = %s ,length = %d",fVariantObjId  ,tc_strlen(fVariantObjId  ));
	                                fprintf(flogPtr," -fFeatureObjId   = %s ,length = %d",fFeatureObjId  ,tc_strlen(fFeatureObjId  ));
	                                fprintf(flogPtr," -fFamilyObjId    = %s ,length = %d",fFamilyObjId   ,tc_strlen(fFamilyObjId   ));	
									
                                    tc_strcpy(fTVariantClassId,"");
									tc_strcpy(fTVariantClassId,sPartNo2);
									tc_strcat(fTVariantClassId,"/");
									tc_strcat(fTVariantClassId,fVariantClassId);															
									fprintf(flogPtr," -fTVariantClassId = %s ,length = %d",fTVariantClassId,tc_strlen(fTVariantClassId));
									
								    
									if(tc_strlen(fVariantClassId)>0 && tc_strlen(fVariantObjId) >0 && tc_strlen(fFeatureObjId) && tc_strlen(fFamilyObjId)&& tc_strcmp(sPartNo2,cfgRuleItemName)==0)
								    {
								    	fprintf(flogPtr,"\n ###### Executing If Condition C5 where Valid length found After StringConvertor function. ######");
								    	exeYes=0;
								    	for(mAit=2;mAit<it;mAit++)
								    	{
								    		if(tc_strlen((char *)masterArry[mAit][0])>0 && tc_strlen((char *)masterArry[mAit][1])>0 && tc_strcmp(fFamilyObjId,(char *)masterArry[mAit][0])==0 && tc_strcmp(fFeatureObjId,(char *)masterArry[mAit][1])==0)
								    		{
								    			fprintf(flogPtr,"\n ###### Executing if Condition C6 where -fFamilyObjId == masterArry[mAit][0] && -fFeatureObjId == masterArry[mAit][1] ######");
								    			for(mAjt=0;mAjt<4;mAjt++)
								    			{
								    				if(tc_strlen((char *)masterArry[0][mAjt])>0&& tc_strlen((char *)masterArry[1][mAjt])>0 && tc_strcmp(fTVariantClassId,(char *)masterArry[0][mAjt])==0 && tc_strcmp(fVariantObjId,(char *)masterArry[1][mAjt])==0)
								    				{
								    					fprintf(flogPtr,"\n ###### Executing final if Condition C7 where -fTVariantClassId == masterArry[0][mAjt] && -fVariantObjId == masterArry[1][mAjt] ######");
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
								}
							}																																
							// Final Printing in csv File.
							
							for(mAit=0;mAit<it;mAit++)
					        {
					        	for(mAjt=0;mAjt<4;mAjt++)
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
						// Condition where Variant ID not found
						else
						{
						    if(found1 == 0)
							{
								fprintf(flogPtr,"\n Error : Variant with -var1 not found !");
							}
							if(found2 == 0)
							{
								fprintf(flogPtr,"\n Error : Variant with -var2 not found !");
							}
						}
				    }
				    else
				    {
					    if(configPerspective1 == NULLTAG)
						{
							fprintf(flogPtr,"\n Error : Item with -configPerspective1 not found !");
						}
						if(configPerspective2 == NULLTAG)
						{
							fprintf(flogPtr,"\n Error : Item with -configPerspective2 not found !");
						}
				    }
				}
				else
				{
					if(itemTag1==NULLTAG)
					{
						fprintf(flogPtr,"\n Error : Item with -itemTag1 not found !");
					}
					if(itemTag2 == NULLTAG)
					{
						fprintf(flogPtr,"\n Error : Item with -itemTag2 not found !");
					}
				}
			}
			else
			{
				fprintf(flogPtr,"\n Error : - Required variable found with length 0.");
			}   
	    }
	    else
	    {
		     printf("\n Error : Login failed in TC");	
		    fprintf(flogPtr,"\n Error : Login failed in TC");
	    }
	}
	return(ITK_ok);
}

