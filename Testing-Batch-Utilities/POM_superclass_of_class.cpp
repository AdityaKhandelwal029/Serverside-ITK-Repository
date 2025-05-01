#include"Header.h"
int ITK_user_main(int argc, char* argv[])
{
	int x = login();
	if (x == 1)
	{
		int count = 0;
		tag_t class_t;
		tag_t *classlist_t;
		POM_class_id_of_class("POM_object", &class_t);
		POM_superclasses_of_class(class_t, &count, &classlist_t);
		cout << " Count = " << count << endl;
		for (int i = 0; i < count; i++)
		{
			char *class_name;
			POM_name_of_class(classlist_t[i], &class_name);
			cout << " i = " << i + 1 << " Name = " << class_name << endl;
		}
	}
	return(0);
}