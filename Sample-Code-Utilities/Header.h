#include<iostream>
#include<string>
using namespace std;


#include<stdio.h> //Header files
#include<stdlib.h> //Header files
#include<string.h>

#include<conio.h>
#include<tc\tc_startup.h> // Header files for main function
#include<tcinit/tcinit.h>  // Header files for login and logout API's
#include<tccore\item.h> // Header files for item related API's
#include<tc\emh.h> // Header files for error handling API's
#include<tccore\aom.h> //Header file for property related API's
#include<tccore\aom_prop.h> //Header file for property related API's
#include<sa\user.h> //Header file for user related API's
#include<qry\qry.h> //Header file for query related API's
#include<epm\epm.h> //Header files to access workflow related APIS;
#include<tccore\grm.h> //Header file for relation related API's
#include<tc\folder.h> //Header file for folder related API's
#include<ae\dataset.h> //Header file for dataset related API's
#include<ae\datasettype.h> //Header file for dataset related API's
#include<tccore\tctype.h> //Header file for TCTYPR related API's
#include<bom\bom.h>//Header files for BOM related API's
#include<tccore\project.h>//Header file for TC Project related API
#include<tccore\custom.h>
#include <vector>





//Macros for practice
#define Error_check(x)                                     \
	{                                                      \
        Status=x;                                          \
      char* Error_msg = NULL;                              \
       printf("\nstatus: %d",Status);                      \
	 if( Status != ITK_ok)                                 \
		{                                                  \
		EMH_ask_error_text(Status, &Error_msg);            \
		printf("\nError: %s", Error_msg);                  \
		MEM_free(Error_msg);                               \
        return Status;                                     \
        }                                                  \
    }                                                      \
                                       

void Display_usage()
{
	printf("\n********** ITK Batch Utility Help **********");
	    printf("\n1) -u= Enter user id");
		    printf("\n2)-p= Enter Password");
			    printf("\n3)-g= Enter group");
				    printf("\n4)-h  For displaying help file");
				     	printf("\n5)-help For displaying help file");

}
