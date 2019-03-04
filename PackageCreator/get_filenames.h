#pragma once
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using namespace boost::filesystem;




//this function searches targed directory and saves all filenames in fnames 
bool get_filenames(std::vector<std::string> &fnames, int16_t & options)
{
	path p;
	try
	{
		if (options&CURRENT_DIR)
		{
			p = current_path();
		}
		if (!(options&CURRENT_DIR))
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
void get_default_settings( std::vector<std::string>& fnames, int16_t &options)
{
	//OPT.COLOUR_ID = false;
	char choice = ' ';
	std::string dir;
	for (;;)
	{

		cout << "\n 0 Start programm "
			<< "\n 1 Spinning  of particles: " << ((options&SPIN) ? "on" : "off")
			<< "\n 2 Count particles: " << ((options&GRIT_COUNT) ? "on" : "off")
			<< "\n 3 Get more info in statistic file: " << ((options&MORE_INFO) ? "on" : "off")
			<< "\n 4 Assign every basic scale a diffent ID: " << ((options&COLOUR_ID) ? "on" : "off")
			<< "\n 5 Load existing Package: " << ((options&LOAD_PACKAGE) ? "on" : "off")
			<< "\n 6 Change working directory: " << ((options&CURRENT_DIR) ? "off" : "on")
			//<< "\n 7 Fill holes when overlapping voxels occur: " << (FILL_HOLES ? "on" : "off")
			<< "\n\n To alter default settings press corresponding number, enter 0 to start program: "
			;

		std::cin >> choice;

		switch (choice)
		{
		case '1':
			options ^= SPIN;
			continue;
		case '2':
			options ^= GRIT_COUNT;
			continue;
		case '3':
			options ^= MORE_INFO;
			continue;
		case '4':
			options ^= COLOUR_ID;
			continue;
		case '5':
			options ^= LOAD_PACKAGE;
			cout << "\n sorry doesn't work jet \n";
			continue;
		case '6':
			options ^= CURRENT_DIR;
			continue;
			/*case '7':
				FILL_HOLES = !FILL_HOLES;
				continue;*/
		default:
			while (!get_filenames(fnames,options));

			return;
		}
	}
}
