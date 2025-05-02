#include"Header.h" 
int c = 0;
extern A1 int dll_register_callbacks();
extern A1 int login_execute_callbacks1(int *decision, va_list argv);
extern A1 int logout_execute_callbacks2(int *decision, va_list argv);

extern A1 int dll_register_callbacks()
{
	c = CUSTOM_register_exit("dll", "USER_init_module", (CUSTOM_EXIT_ftn_t)login_execute_callbacks1);
	c = CUSTOM_register_exit("dll", "USER_exit_module", (CUSTOM_EXIT_ftn_t)logout_execute_callbacks2);
	return(c);
}
extern A1 int login_execute_callbacks1(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf(" Login successfull to Dll \n"); 
	printf( " Welcome to DLL project \n"); 
	return(c);
}
extern A1 int logout_execute_callbacks2(int *decision, va_list argv)
{
	*decision = ALL_CUSTOMIZATIONS;
	printf(" Logout successfull  \n");
	printf(" Done with the process \n");
	return(c);
}

