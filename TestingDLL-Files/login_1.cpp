#include"Header.h" 
int c = 0;
extern "C" A1 int dll_register_callbacks();
extern A1 int dll_execute_callbacks1(int *decision, va_list argc);
extern A1 int dll_execute_callbacks2(int *decision, va_list argc); 

extern A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)dll_execute_callbacks2);
	return(c);
}
extern A1 int dll_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " ***********************************************************************" << endl;
	cout << " Welcome to DLL project " << endl;
	cout << " Logging in process successfull " << endl;
	cout << " ***********************************************************************" << endl;
	return(c);
}
extern A1 int dll_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	cout << " Logout successfull " << endl;
	cout << " Done with the process " << endl; 
	cout << " ***********************************************************************" << endl;
	cout << " ***********************************************************************" << endl;
	Sleep(4);
	return(c); 
}
