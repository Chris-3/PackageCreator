#pragma once
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using namespace boost::filesystem;


extern bool SPIN;
extern bool GRIT_COUNT;
extern bool MORE_INFO;
extern bool FILL_HOLES;
extern bool COLOUR_ID;
extern bool LOAD_PACKAGE;
extern bool CURRENT_DIR;

//this function searches targed directory and saves all filenames in fnames 
bool get_filenames(std::vector<std::string> &fnames)
{
	path p;
	try
	{
		if (CURRENT_DIR)
		{
			p = current_path();
		}
		if (!CURRENT_DIR)
		{
			cout << "\n Enter Working directory: \n";
			std::cin >> p;
		}
		if (exists(p))
		{
			if (is_regular_file(p))
				cout << p << " size is " << file_size(p) << '\n';

			else if (is_directory(p))
			{
				for (auto&& x : directory_iterator(p))
				{
					std::string n = x.path().generic_string();
					fnames.push_back(n);
				}
				return true;
			}
			else
				cout << p << " exists, but is not a regular file or directory\n";
			return false;
		}
		else
			cout << p << " does not exist\n";
		return false;
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << "\n";
		return false;
	}
}

//this function prints a menu to get the new default settings 
void get_default_settings(int argc, char ** argv, std::vector<std::string>& fnames)
{
	char choice = ' ';
	std::string dir;
	for (;;)
	{
		cout << "\n 0 Start programm "
			<< "\n 1 Spinning  of particles: " << (SPIN ? "on" : "off")
			<< "\n 2 Count particles: " << (GRIT_COUNT ? "on" : "off")
			<< "\n 3 Get more info in statistic file: " << (MORE_INFO ? "on" : "off")
			<< "\n 4 Assign every basic scale a diffent ID: " << (COLOUR_ID ? "on" : "off")
			<< "\n 5 Load existing Package: " << (LOAD_PACKAGE ? "on" : "off")
			<< "\n 6 Change working directory: " << (CURRENT_DIR ? "off" : "on")
			//<< "\n 7 Fill holes when overlapping voxels occur: " << (FILL_HOLES ? "on" : "off")
			<< "\n\n To alter default settings press corresponing number, enter 0 to start programm: "
			;

		std::cin >> choice;

		switch (choice)
		{
		case '1':
			SPIN = !SPIN;
			continue;
		case '2':
			GRIT_COUNT = !GRIT_COUNT;
			continue;
		case '3':
			MORE_INFO = !MORE_INFO;
			continue;
		case '4':
			COLOUR_ID = !COLOUR_ID;
			continue;
		case '5':
			LOAD_PACKAGE = !LOAD_PACKAGE;
			cout << "\n sorry doesn't work jet \n";
			continue;
		case '6':
			CURRENT_DIR = !CURRENT_DIR;
			continue;
		case '7':
			FILL_HOLES = !FILL_HOLES;
			continue;
		default:
			while (!get_filenames(fnames));

			return;
		}
	}
}
