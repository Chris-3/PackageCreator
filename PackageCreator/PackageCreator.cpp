#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

enum opt_flags
{
	SPIN = 1 << 0,
	GRIT_COUNT = 1 << 1,
	MORE_INFO = 1 << 2,
	FILL_HOLES = 1 << 3,
	COLOUR_ID = 1 << 4,
	LOAD_PACKAGE = 1 << 5,
	CURRENT_DIR = 1 << 6
};


#include "get_filenames.h"
#include "Grit.h"
#include "Package.h"



// this function searches for every file named "Particle_" and creates a Grit object for each
bool getGritsofScale(std::vector<Grit> &, const std::vector<std::string> &, const double&);

int main(int argc, char* argv[])
{
	int16_t options = SPIN + COLOUR_ID + CURRENT_DIR;

	cout << "\n*********PackageCreator*********\n";
	std::vector<std::string> fnames;	//here are all filenames in target directory saved

	get_default_settings(fnames, options);

	for (auto const & param : fnames)
	{
		if (param.find("Parameter") != std::string::npos)
		{
			Package pack(param,options);//class Package is initialised with parameter file

			std::vector<Grit> x;//this will be the list of the original particles

			if (!getGritsofScale(x, fnames, pack.get_scale()->first))
			{
				std::cout << " No file with name \"Particle_\" found\n press any key to continue\n";
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

