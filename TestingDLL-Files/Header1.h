//#include<stdio.h>
//#include<conio.h>
//#include<tc\tc_startup.h>
//#include<tccore\method.h>
//#include<tccore\custom.h>
//#include<tccore\aom_prop.h>
//#include<epm\epm.h>
//#include<pom\pom\pom.h>
//#include<tccore\releasestatus.h>
//#include<tccore/item.h>
//#include<iostream>
//#include <string> 
//#include<string.h>
//#include <vector>
//#include<sstream>
//#include <algorithm>
//using namespace std;
//
//
//#define DLLAPI _declspec(dllexport)
//
//int Status = ITK_ok;
//
//extern DLLAPI int dll_register_callbacks();
//extern DLLAPI int Flender_execute_callbacks1(int *decision, va_list argc);
//extern DLLAPI int Flender_execute_callbacks2(int *decision, va_list argc);
//extern DLLAPI int Flender_RelPrevRev_ActionH(EPM_action_message_t msg);
//
//
//#define Error_check(x)                                     \
//	{                                                      \
//        Status=x;                                          \
//       printf("\nstatus: %d",Status);                      \
//	 if( Status != ITK_ok)                                 \
//		{                                                  \
//		EMH_ask_error_text(Status, &Error_msg);            \
//		printf("\nError: %s", Error_msg);                  \
//		MEM_free(Error_msg);                               \
//        return Status;                                     \
//        }                                                  \
//    }                                                      \
