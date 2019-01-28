#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


#define DEBUG 0
#define DEBUG1 0
#define VISUAL_STUD 0

bool SPIN = true;
bool GRIT_COUNT = false;
bool MORE_INFO = false;
bool FILL_HOLES = false;
bool COLOUR_ID = true;
bool LOAD_PACKAGE = false;
bool CURRENT_DIR = true;

#include "get_filenames.h"
#include "Grit.h"
#include "Package.h"

bool getGritsofScale(std::vector<Grit> &, const std::vector<std::string> &, const double&);
// this function searches for every file named "Particle_" and creates a Grit object for each
void get_default_settings(int, char**, std::vector<std::string>&);
//this function prints a menu to get the new default settings 


int main(int argc, char* argv[])
{
	cout << "\n*********PackageCreator*********\n";
	std::vector<std::string> fnames;
	//here are all filenames in target directory saved
	
	get_default_settings(argc, argv, fnames);

	for (auto const & param : fnames)
	{
		if (param.find("Parameter") != std::string::npos)
		{
			Package pack(param);//class Package is initialised with parameter file

			std::vector<Grit> x;//this will be the list of the original particles

			if (!getGritsofScale(x, fnames, pack.get_scale()->first))
			{
				std::cout << "No file with name \"Particle_\" found\n press any key to continue\n";
				std::cin.get();
				return 0;
			}

			std::cout << "\n Start adding particles: \n";
			pack.fill_package(x);
			return 0;
		}
	}
	std::cout << "No file with name \"Parameter\" found\n press any key to continue\n";
	std::cin.get();
	return 0;
}


//this function fills list with particle data (returns false when no particle files)
bool getGritsofScale(std::vector<Grit> &x, std::vector<std::string> const& fnames, const double& sc)
{
	bool no_file = false;
	for (unsigned int i = 0; i < fnames.size(); i++)
	{
		if (fnames[i].rfind("Particle_") != std::string::npos)
		{
			Grit n(fnames[i]);
			x.push_back(n);
			no_file = true;
		}

	}
	return no_file;
}

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
