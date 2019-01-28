#pragma once
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using namespace boost::filesystem;

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
