/*Date - 06-01-2025 */

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
	
    char *logPath            =   (char*) MEM_alloc(200*sizeof(char));
	char *csvLog             =   (char*) MEM_alloc(200*sizeof(char));
	
	int itkApiCallStatus = 0;
	
	tag_t loggedInUserTag             = NULLTAG;

	time_t mytime = time(NULL);
	timeStr = ctime(&mytime);
	timeStr[strlen(timeStr)-1] = '\0';
	STRNG_replace_str(timeStr," ","_",&timeStrUpdated);
	STRNG_replace_str(timeStrUpdated,":","_",&timeStrUpdatedNew);
    	
	sUserName      = ITK_ask_cli_argument("-u=");
    sPasswordFile  = ITK_ask_cli_argument("-pf=");
    sGroup         = ITK_ask_cli_argument("-g=");
	
	//tc_strcpy(logPath,"/tmp/");  // /tmp/ - to be added if required
	tc_strcat(logPath,"DataCollectionLog_");
	tc_strcat(logPath,timeStrUpdatedNew);
	tc_strcat(logPath,".txt");
	
	//tc_strcpy(csvLog,"/tmp/");  //  /tmp/ - to be added if required
	tc_strcat(csvLog,"UserLoginInfo_");
	tc_strcat(csvLog,".csv");

	
	FILE *flogPtr=fopen(logPath,"w+");
	FILE *fcsvPtr=fopen(csvLog,"r");
	
	fprintf(flogPtr,"Date and Time - %s\n",timeStr);
	fprintf(flogPtr,"Date and Time - %s\n",timeStrUpdatedNew);
	
	
	fprintf(flogPtr,"- sUserName         = %s\n",sUserName      );
	fprintf(flogPtr,"- sPasswordFile     = %s\n",sPasswordFile  );
	fprintf(flogPtr,"- sGroup            = %s\n",sGroup         );
		
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
			if(fcsvPtr==NULL)
			{
				fprintf(flogPtr,"UserLoginInfo_ File not found \n");
			}
			else
			{
				
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