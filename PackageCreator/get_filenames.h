#pragma once
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using namespace boost::filesystem;
double default_perc[] = { 16.0, 23.0, 35.0, 4.7, 0.2, 0.1 };

void get_filenames(path p, std::vector<std::string> &fnames)
//this function searches targed directory and saves all filenames in fnames 
{
	//std::vector<path> v;
	try
	{
		if (p == "-")
		{
			p = current_path();
		}
		if (exists(p))
		{
			if (is_regular_file(p))
				cout << p << " size is " << file_size(p) << '\n';

			else if (is_directory(p))
			{
				//cout << p << " is a directory containing:\n";

				for (auto&& x : directory_iterator(p))
				{
					std::string n = x.path().generic_string();
					fnames.push_back(n);
					//v.push_back(x.path());
				}
			}
			else
				cout << p << " exists, but is not a regular file or directory\n press any key to continue\n";
			std::cin.get();
		}
		else
			cout << p << " does not exist\n press any key to continue\n";
		std::cin.get();
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << "\n press any key to continue\n";
		std::cin.get();
	}
}
