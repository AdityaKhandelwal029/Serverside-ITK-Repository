/*Objective - Read users homes folder and get the list of all the components present in it. Add a file log.*/

/*Required headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tccore/item.h>
#include <tcinit/tcinit.h>
#include <tccore/aom.h>
#include <tccore/custom.h>
#include <tc/tc_startup.h>
#include <tc/emh.h>
#include <res/res_itk.h>
#include <ctype.h>
#include <pom/pom/pom.h>
#include <tccore/uom.h>
#include <user_exits/user_exits.h>
#include <rdv/arch.h>
#include <textsrv/textserver.h>
#include <time.h>
#include <tc/folder.h>
#include <sa/user.h>

/*Declarations */

int itkApiCallStatus = -1;

/*Custom API error handling function */

int ITK_APICALL(int x,FILE *Flogfile){      
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
                    fprintf(Flogfile,"Error code - %d Error message - %s\n",ifails[index],texts[index]);					
                } 
           EMH_clear_errors();
        }
        return status;
    }
/*Main function implementation */ 
    
int ITK_user_main (int argc, char ** argv)
{
	int i;
    int  homeReferencersCount;	
	char *sUserName	              = NULL;
    char *sPasswordFile	          = NULL;
    char *sGroup                  = NULL;
	char *loggedInUser            = NULL; 
	char *objectNamePropertyValue = NULL;
	char *objectTypePropertyValue = NULL;
	
	
	tag_t loggedInUserTag    =  NULLTAG;
	tag_t newUserTag         =  NULLTAG;
	tag_t userHomeFolderTag  =  NULLTAG;
	tag_t *homeReferencers   =  NULLTAG;
	
	FILE *flogPtr=fopen("/user/testcmi/Dev/devgroups/Aditya/testfolder/Userhome-info.txt","w+");
	fprintf(flogPtr,__DATE__);
	fprintf(flogPtr,"\n");
	fprintf(flogPtr,__TIME__);
	ITK_APICALL(ITK_initialize_text_services( ITK_BATCH_TEXT_MODE ),flogPtr);
    ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	sUserName     = ITK_ask_cli_argument("-u=");
    sPasswordFile = ITK_ask_cli_argument("-pf=");
    sPasswordFile = ITK_ask_cli_argument("-g=");
	
	itkApiCallStatus=ITK_APICALL(ITK_auto_login(),flogPtr); 
	ITK_APICALL(ITK_set_journalling(TRUE),flogPtr);
	
	if(itkApiCallStatus==ITK_ok)
        {
		printf("**************************************************************************Login successful to teamcenter \n");	
        fprintf(flogPtr,"\nLogin Successfull to TC \n");	
        itkApiCallStatus=ITK_APICALL(POM_get_user(&loggedInUser,&loggedInUserTag),flogPtr);
	    if(itkApiCallStatus==ITK_ok)
			{
				fprintf(flogPtr,"\nName of the logged in user = %s \n",loggedInUser);	
			}
		//scanf("\nPlease enter user name - %s ",findUser);
		//fprintf(flogPtr,"\n User to be searched = %s ",findUser);
		itkApiCallStatus=ITK_APICALL(SA_find_user2("Aditya",&newUserTag),flogPtr);
		if(itkApiCallStatus==0)
		{
			fprintf(flogPtr,"\nUser with given id found successfully");
			itkApiCallStatus=ITK_APICALL(SA_ask_user_home_folder(newUserTag,&userHomeFolderTag),flogPtr);
			if(itkApiCallStatus==0)
			{
				fprintf(flogPtr,"\nUser's Home folder found successfully");
				itkApiCallStatus=ITK_APICALL(FL_ask_references(userHomeFolderTag,FL_fsc_as_ordered ,&homeReferencersCount,&homeReferencers),flogPtr);
				fprintf(flogPtr,"\nTotal count of referecers of folder = %d",homeReferencersCount);
				for(i=0;i<homeReferencersCount;i++)
				{
					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(homeReferencers[i],"object_name",&objectNamePropertyValue),flogPtr);
					itkApiCallStatus=ITK_APICALL(AOM_ask_value_string(homeReferencers[i],"object_type",&objectTypePropertyValue),flogPtr);
					fprintf(flogPtr,"\n The object at position '%d' is with '%s' name and '%s' type. ",i,objectNamePropertyValue,objectTypePropertyValue);
				}
			}
			else{
				fprintf(flogPtr,"\n Users home folder not found \n");
			}
		}
		else{
			fprintf(flogPtr,"\nUser with given id not found successfully \n");
		}
	}
	 else
	   {
		  printf("\n************************************************************************************nLogin failed in TC \n");	
		  fprintf(flogPtr,"\n************************************************************************************nLogin failed in TC \n");
	   }
	return(ITK_ok);
}

