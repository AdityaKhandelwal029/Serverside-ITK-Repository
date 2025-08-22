/*
   Date - 17-07-2025 
*/
/* Objective :- User Project Access List.
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

// function to check for space in String 

char *checkstring(char *str,FILE *Flogfile )
{
	int itr = 0;
	int spcFound = 0;
	
	char *newStr = (char*) MEM_alloc(500*sizeof(char));
	
	fprintf(Flogfile,"\n********** Started Executing stringConvertor str = %s ********** \n",str);
	
	for(itr = 0;str[itr]!='\0';itr++)
	{
		if(str[itr]==' ')
		{
			spcFound = 1;
		}
	}
	
	fprintf(Flogfile,"\n - spcFound = %d",spcFound);
	
	if(spcFound == 0)
	{
		fprintf(Flogfile,"\n********** Ended Executing stringConvertor str = %s and new str = %s ********** \n",str,"NULL");
		return(str);
	}
	else
	{
		tc_strcpy(newStr,"'");
		tc_strcat(newStr,str);
		tc_strcat(newStr,"'");	
		fprintf(Flogfile,"\n********** Ended Executing stringConvertor str = %s and new str = %s ********** \n",str,newStr);
		return(newStr);
	}
	
}

// Convert String to int

char* intToString(int value) {
    // Allocate memory (enough for int, including minus sign and null terminator)
    char* str = (char*)malloc(12 * sizeof(char)); // Enough for 32-bit int
    if (str == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Convert int to string
    sprintf(str, "%d", value);

    return str;
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
	char *prdItemId          =   NULL;  
	char *modelfmly          =   NULL;
	char *prdmodel           =   NULL;
	char *family             =   NULL;
	char *feature            =   NULL;	
    char *configObjName      =   NULL;
	char *configObjString    =   NULL;
	char *configObjdesc      =   NULL; 
	char *cfgIdValue         =   NULL;
	
    char *logPath            = (char*) MEM_alloc(200*sizeof(char));
	char *queryInput         = (char*) MEM_alloc(500*sizeof(char));
	char *updmodelfmly       = (char*) MEM_alloc(500*sizeof(char));
	char *updprdmodel        = (char*) MEM_alloc(500*sizeof(char));
	char *updfamily          = (char*) MEM_alloc(500*sizeof(char));
	char *updfeature         = (char*) MEM_alloc(500*sizeof(char));	
	char *subCondition       = (char*) MEM_alloc(500*sizeof(char));
	char *avlCondition       = (char*) MEM_alloc(500*sizeof(char));
	
	char *finalId            = (char*) MEM_alloc(50*sizeof(char));
			
    char filedata[500];
		
	int itkApiCallStatus     = 0;
	int qryOutput            = 0;
	int itr                  = 0;
	int itr1                 = 0;
	int itr2                 = 0;
	int count                = 0;
	int modelCount           = 0;
	int ruleCount            = 0;
	int optValueCount        = 0;
	
	int idGen                = 0;
	int temp                 = 0;
	int maxidGen             = 0;
									
	tag_t loggedInUserTag             = NULLTAG;
	tag_t itemTag                     = NULLTAG;
	tag_t configPerspective           = NULLTAG;
	tag_t type                        = NULLTAG;
	tag_t createInputTag              = NULLTAG;
	tag_t ruleTag                     = NULLTAG;
	
	tag_t *cfg0ModelsTags             =  NULLTAG;
	tag_t *cfg0AvailabilityRulesTags  =  NULLTAG;
	tag_t *cfg0OptionValuesTags       =  NULLTAG;
	
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
	tc_strcat(logPath,"LoadAVlRule_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
		
	FILE *flogPtr=fopen(logPath,"w+");	
	FILE *fIntCsv=fopen(sFile,"r");
					
	fprintf(flogPtr,"- Date and Time = %s",timeStr);
	fprintf(flogPtr,"\n- Date and Time = %s",timeStrUpdatedNew);	
	fprintf(flogPtr,"\n- sUserName     = %s",sUserName      );
	fprintf(flogPtr,"\n- sPasswordFile = %s",sPasswordFile  );
	fprintf(flogPtr,"\n- sGroup        = %s",sGroup         );	
	fprintf(flogPtr,"\n- sFile         = %s",sFile         );	
				
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
		    	fprintf(flogPtr,"\nFile found and successfully opened ... \n");
		    	
		    	while(fgets(filedata, 500, fIntCsv) != NULL) 
                {
		    		fprintf(flogPtr,"\n Line = %d , Data Found =  %s",count,filedata);
                    if(count>0)
		    		{
		    			prdItemId  = strtok(filedata,",");
		    			modelfmly  = strtok(NULL,",");
		    	        prdmodel   = strtok(NULL,",");
		    	        family     = strtok(NULL,",");
						feature    = strtok(NULL,",");
						
						fprintf(flogPtr,"\n prdItemId = %s",prdItemId );
	                    fprintf(flogPtr,"\n modelfmly = %s",modelfmly );
	                    fprintf(flogPtr,"\n prdmodel  = %s",prdmodel  );
	                    fprintf(flogPtr,"\n family    = %s",family    );
	                    fprintf(flogPtr,"\n feature   = %s",feature   );
																		 
						updmodelfmly = checkstring(modelfmly,flogPtr);
	                    updprdmodel  = checkstring(prdmodel ,flogPtr);
						updfamily    = checkstring(family   ,flogPtr);
						updfeature   = checkstring(feature  ,flogPtr);
						
						fprintf(flogPtr,"\n updmodelfmly = %s",updmodelfmly );
	                    fprintf(flogPtr,"\n updprdmodel  = %s",updprdmodel  );
	                    fprintf(flogPtr,"\n updfamily    = %s",updfamily    );
	                    fprintf(flogPtr,"\n updfeature   = %s",updfeature   );
															
						tc_strcpy(subCondition,"[Teamcenter]");
						tc_strcat(subCondition,updfamily);												
						tc_strcat(subCondition," = ");
						tc_strcat(subCondition,updfeature);
																		
						tc_strcpy(avlCondition,"[");
						tc_strcat(avlCondition,prdItemId);
						tc_strcat(avlCondition,"]");										
						tc_strcat(avlCondition,updmodelfmly);
						tc_strcat(avlCondition," = ");
						tc_strcat(avlCondition,updprdmodel);
						
						fprintf(flogPtr,"\n subCondition = %s",subCondition );
	                    fprintf(flogPtr,"\n avlCondition = %s",avlCondition );
						
						itkApiCallStatus=ITK_APICALL(ITEM_find_item(prdItemId,&itemTag),flogPtr);
			            fprintf(flogPtr,"API Status for ITEM_find_item = %d \n",itkApiCallStatus);
			            if(itkApiCallStatus==0&&itemTag!=NULLTAG)
			            {
			            	fprintf(flogPtr,"\n Executing If Condition C1 where item_tag found. ");
			            	fprintf(flogPtr,"Item with %s itemId found  .... \n",prdItemId);
			            	ITK_APICALL(AOM_ask_value_string(itemTag,"object_name"           ,&configObjName   ),flogPtr);
			            	ITK_APICALL(AOM_ask_value_string(itemTag,"object_string"         ,&configObjString ),flogPtr);
			            	ITK_APICALL(AOM_ask_value_string(itemTag,"object_desc"           ,&configObjdesc   ),flogPtr);
			            	   ITK_APICALL(AOM_ask_value_tag(itemTag,"cfg0ConfigPerspective" ,&configPerspective),flogPtr);
			            	   
			                fprintf(flogPtr,"- configObjName   = %s\n",configObjName  );
			                fprintf(flogPtr,"- configObjString = %s\n",configObjString);
			                fprintf(flogPtr,"- configObjdesc   = %s\n",configObjdesc  );
			            	
			            				    
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
			             		
			             		for(itr=0;itr<ruleCount;itr++)
								{
									ITK_APICALL(AOM_ask_value_string(cfg0AvailabilityRulesTags[itr],"cfg0ObjectId",&cfgIdValue),flogPtr);
									idGen = 0;
									
									for(itr1 = 0; cfgIdValue[itr1]!='\0';itr1++)
									{
										temp  = (int)cfgIdValue[itr1]-48;
										idGen = (idGen*10)+temp;
									}
									
									fprintf(flogPtr,"\n - cfgIdValue   = %s\n",cfgIdValue);
									fprintf(flogPtr,"\n - idGen        = %d\n",idGen);
									
									if(idGen>maxidGen)
									{
										maxidGen = idGen;
									}
								}								
								fprintf(flogPtr,"\n - maxidGen  = %d\n",maxidGen);
								maxidGen++;
								fprintf(flogPtr,"\n - maxidGen[1]  = %d\n",maxidGen);
								
								finalId = intToString(maxidGen);								
								fprintf(flogPtr,"\n - finalId  = %s \n",finalId);
								
								// Rule Creation 
								
								itkApiCallStatus=ITK_APICALL(TCTYPE_ask_type("Cfg0AvailabilityRule",&type),flogPtr);
                                fprintf(flogPtr," API STATUS FOR TCTYPE_ask_type = %d ",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(TCTYPE_construct_create_input(type,&createInputTag),flogPtr);
	                            fprintf(flogPtr," API STATUS FOR TCTYPE_construct_create_input = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_set_value_string(createInputTag,"cfg0ObjectId",finalId),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_string[1] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_set_value_string(createInputTag,"object_name",finalId),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_string[2] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_set_value_string(createInputTag,"cfg0Message","Enter Message Here"),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_string[3] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_set_value_string(createInputTag,"cfg0Severity","Error"),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_string[4] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_set_value_tags(createInputTag,"cfg0ProductItems",1,&itemTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_tags = %d \n",itkApiCallStatus);
					            
	                            itkApiCallStatus=ITK_APICALL(TCTYPE_create_object(createInputTag,&ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR TCTYPE_create_object = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_save_with_extensions(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_save_with_extensions[1] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_refresh(ruleTag,0),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_refresh = %d \n",itkApiCallStatus);
					            
					            fprintf(flogPtr,"\n *********************************************************** 111111 \n");
								
								// Now set the include rule expression, applicability and subject
					
					            itkApiCallStatus=ITK_APICALL(AOM_lock(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_lock = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_set_value_string(ruleTag,"cfg0SubjectCondition",subCondition),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_string[5] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_save_with_extensions(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_save_with_extensions[2] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_unlock(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_unlock[1] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_lock(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_lock = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_refresh(ruleTag,0),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_refresh[1] = %d \n",itkApiCallStatus);
					            
					            fprintf(flogPtr,"\n *********************************************************** 222222222 \n");
					            
				                // Adding condition
					            
				                itkApiCallStatus=ITK_APICALL(AOM_set_value_string(ruleTag,"cfg0ApplicabilityCondition",avlCondition),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_set_value_string[5] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_save_with_extensions(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_save_with_extensions[3] = %d \n",itkApiCallStatus);
					            
					            itkApiCallStatus=ITK_APICALL(AOM_unlock(ruleTag),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_unlock[2] = %d \n",itkApiCallStatus);
					            
				                itkApiCallStatus=ITK_APICALL(AOM_refresh(ruleTag,0),flogPtr);
					            fprintf(flogPtr," API STATUS FOR AOM_refresh[2] = %d \n",itkApiCallStatus);
								
				                fprintf(flogPtr,"\n *********************************************************** 333333 \n");
												
			            	}
                            else
			                {
			 	                fprintf(flogPtr,"\n Error : Config Perspective Tag NOT found."); 
			                }							
						}
						else
			            {
			            	fprintf(flogPtr,"\n Error : Item with %s itemId not found in Teamcenter.",prdItemId);
			            }			            						            
		    		}
					count++;
		    	}
		    }
			else
		    {
                fprintf(flogPtr,"File not found...\n");			
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