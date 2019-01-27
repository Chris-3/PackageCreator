#pragma once
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using namespace boost::filesystem;
double default_perc[] = { 16.0, 23.0, 35.0, 4.7, 0.2, 0.1 };

//funktion wird voraussichtlich nicht mehr gebraucht könnte sich aber noch als nützlich erweisen
//template<typename T>
//bool contains(std::list<T> & listOfElements, const T & element)
//{
//	// Find the iterator if element in list
//	auto it = std::find(listOfElements.begin(), listOfElements.end(), element);
//	//return if iterator points to end or not. It points to end then it means element
//	// does not exists in list
//	return it != listOfElements.end();
//}

void getFileNames(path p, std::vector<std::string> &fnames)
//generiert eine List von Path Objekten in denen die Informationen der Files (Name,Pfad,etc) zu den Partikeln gespeichert ist
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
				cout << p << " exists, but is not a regular file or directory\n";
		}
		else
			cout << p << " does not exist\n";
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << '\n';
	}
}

//
//std::map<string, double> getScalingFactor(const std::vector<string> & fnames, double *percent =default_perc)
//{
//	std::map<string, double> my_map;
//
//	int j = 0;
//	for (unsigned int i = fnames.size()-1; i > 0 ; i--)
//	{
//		if (!my_map.count(fnames[i].substr(fnames[i].size() - 7, 3)))
//		{
//			my_map.insert(std::pair<string, double>(fnames[i].substr(fnames[i].size() - 7, 3), percent[j++]));
//		}
//	}
//	return my_map;
//}
