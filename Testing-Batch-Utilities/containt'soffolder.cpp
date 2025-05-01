#include"Header.h" 
ofstream data1("D:/Dataoffolder.txt");  
string temp = "Folder",temp2="Item";
void Ask_child(int gcount,tag_t *list_t)
{
	for (int j = 0; j < gcount; j++)
	{
		char *obj_name,*classname;
		tag_t classid;
		AOM_UIF_ask_value(list_t[j], "object_name", &obj_name);
		POM_class_of_instance(list_t[j], &classid);
		POM_name_of_class(classid, &classname); 
		data1 << " Object name = " << obj_name<<endl;
		data1 << " Class of the object = " << classname<<endl; 
		if (classname == temp)
		{

		}
		if (classname == temp2)
		{

		}
	}
}
int ITK_user_main(int argc, char* argv[])
{ 
	int x = login();
	if (x == 1)
	{
		data1 << "************************* Login successfull **************** "<<endl;
		int itemcount,fr_count;
		char *classname;
		tag_t classid;
		tag_t *itemcount_t,*fr_t; 
		WSOM_find2("content-folder", &itemcount, &itemcount_t); 
		data1 << " Folder name searched = " << "content-folder" << endl; 
		data1 << " Total folders found with the same name = " << itemcount<<endl;
		for (int i = 0; i < itemcount; i++)
		{
			POM_class_of_instance(itemcount_t[i], &classid);
			POM_name_of_class(classid, &classname);
			if (classname == temp)
			{
				FL_ask_references(itemcount_t[i], FL_fsc_as_ordered, &fr_count, &fr_t); 
				data1 << " Total component inside main folder = " << fr_count<<endl;
				if (fr_count > 0)
				{
					Ask_child(fr_count, fr_t);
				}
			}
		}
	}
	return(0);
}