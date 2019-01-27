#include <iostream>
#include <vector>
#include <map>
#include <algorithm>


#define DEBUG 0
#define DEBUG1 0

#include "get_filenames.h"
#include "Grit.h"
#include "Package.h"

bool SPIN = true;
bool GRIT_COUNT = false;
bool MORE_INFO = false;
bool FILL_HOLES = false;
bool COLOUR_ID = false;
bool LOAD_PACKAGE = false;



bool getGritsofScale(std::vector<Grit> &, const std::vector<std::string> &, const double&);
//sucht im Zielverzeichnis nach allen Dateien (->Partikeln) eines bestimmten Skalierungsfaktors und speichert sie ein Vektor array mit Var. Namen x
void get_default_settings(int , char** , std::vector<std::string>&);
//this function gets the new default settings if more options are entered


int main(int argc, char* argv[])
{
	std::vector<std::string> fnames;
	//in dieser Liste (fnames) wird der komplette Pfad aller Dateien des Zielverzeichnisses gespeichert
	//if (argc < 2) getFileNames("D:/Programmieren/Masterarbeit_andrea/Partikel", fnames);
	if (argc < 2) getFileNames("-", fnames);//Default Modus
	else get_default_settings(argc, argv, fnames);//Called if programm gets parameters
	

	for (auto const & param : fnames)
	{
		if (param.find("Parameter") != std::string::npos)
		{
			Package pack(param);//class Package is initialised with parameter file

			std::vector<Grit> x;//this will be the list of the original particles

			if (!getGritsofScale(x, fnames, pack.GetScale()->first))
			{
				std::cout << "No file with name \"Particle_\" found\n";
				break;
			}

			std::cout << "\n Start adding particles: \n";
			pack.FillPackage(x);
			return 0;
		}
	}
	std::cout << "No file with name \"Parameter\" found\n";

	return 0;
}


bool getGritsofScale(std::vector<Grit> &x, std::vector<std::string> const& fnames, const double& sc)
//this function fills list with particle data
//returns false when no  particle files
{
	bool no_file = false;
	//if (DEBUG)std::cout << sc << std::endl;
	for (unsigned int i = 0; i < fnames.size(); i++)
	{
		//if (fnames[i].find(std::to_string(sc).erase(3, std::string::npos)) != std::string::npos)
		if (fnames[i].find("Particle_") != std::string::npos)
		{
			//if (DEBUG1)cout << fnames[i];
			Grit n(fnames[i]);
			x.push_back(n);
			no_file = true;
		}

	}
	return no_file;
}

void get_default_settings(int argc, char ** argv, std::vector<std::string>& fnames)
{
	for (int i = 1; i < argc; i++)
	{
		if (argv[i] == "-s")
		{
			SPIN = !SPIN;
			continue;
		}
		if (argv[i] == "-n")
		{
			GRIT_COUNT = !GRIT_COUNT;
			continue;
		}
		if (argv[i] == "-i")
		{
			MORE_INFO = !MORE_INFO;
			continue;
		}
		if (argv[i] == "-f")
		{
			FILL_HOLES = !FILL_HOLES;
			continue;
		}
		if (argv[i] == "-c")
		{
			COLOUR_ID = !COLOUR_ID;
			continue;
		}
		/*if (argv[i] == "-l")
		{
			LOAD_PACKAGE = !LOAD_PACKAGE;
			i++;
			fnames.push_back(argv[i]);
			continue;
		}*/
		getFileNames(argv[i], fnames);
	}
}
