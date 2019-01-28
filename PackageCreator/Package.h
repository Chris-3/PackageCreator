#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <ctime>
#include <ratio>
#include <chrono>

#include "Grit.h"
using namespace std::chrono;
using std::cout;



class Package
{
public:
	//Constructor
	//Package();
	Package(std::string const&);

	//Destructor
	~Package();

	//Manipulatoren
	
	void FillPackage(std::vector<Grit>&);
	//this function fills the package with particles
	std::map<double, uint32_t>::const_reverse_iterator GetScale();
	//sucht die naechste Korngroese die einzufuegen ist heraus
	bool IsNotFull();
	//checkt ob der maximale feststoffanteil erreicht worden ist
	void CreateFile();
	//generiert Ausgabedatei im Verzeichnis der Partikelfiles
	void Status(Grit const&);
	//Prints the current status after adding particle/grit


private:

	std::vector<std::vector<std::vector<uint8_t>>> package;
	//This is the 
	coordinate<int16_t> dim_pack;
	//dimension der erstellten Packung
	std::string pack_path;
	//Pfad zum Arbeitsordner
	uint32_t max_vol;
	//gesamtes Volumen der Packung in Voxel
	uint32_t solid_vox;
	//aktueller Feststoffanteil in Voxel

	std::map<double, uint32_t> por_threshold;
	//all scaling factors and corresponding solid share 
	std::map<double, uint32_t>::const_reverse_iterator it_now; 
	//current scaling factor and corresponding solid share 
	double scale_diversity;
	//bereich in % indem dem scale schwankt

	uint32_t count;
	//zaehlt eingefuegte partikel
	double max_solid; 
	//maximaler Feststoffanteil in %
	uint32_t tried;
	//Anzahl der Versuche um ein Korn einzufuegen
	steady_clock::time_point run_t;
	//runtime of the programm
	const std::vector<coordinate<int16_t>> v_fill = { {0,0,0}, {1,1,1} ,{0,1,1},{0,0,1},{0,1,0},{1,0,0},{1,1,0},{1,0,1}  , { -1,-1,-1 }, { 0,-1,-1 }, { 0,0,-1 }, { 0,-1,0 }, { -1,0,0 }, { -1,-1,0 }, { -1,0,-1 }};
	//Array for checking neighbouring voxels
	int added_vox;
	//counts voxels added per particle
	double real_scale;
	//the calculated real scale including scale diversity
	std::vector<std::string> stat;
	//record of every particle added
	std::vector<std::string> stat_general;
	//general statistic for every scaling factor
	uint8_t colour;
	//coulor ID in final Package
	unsigned int grit_choice;
	unsigned int nr_of_grit;

	void CheckIfFree(Grit&, coordinate<int16_t> const&);
	//checkt ob die position in der Packung schon belegt ist
	void AddGrit(Grit&, const coordinate<int16_t> &);
	//fügt nach erfolgreicher überprüfung ein Partikel in das gesamtpacket hinzu
	inline void IsInFrame(coordinate<int16_t>&);
	// koorrigiert coordinaten die ueber die Grenzen hinausgehen
	inline void spin_and_scale(coordinate<int16_t>&, const Grit&);
	//spinnes the vector and scales it
	inline coordinate<int16_t> dir_to_center(coordinate<int16_t>);
	//creates a vektor pointing from point to center of gravity
	inline void fill_holes(coordinate<int16_t>&, const int &);
	//if coordinates overlap this function checks neighbouring voxels for free spots
	void get_stat(const std::ostringstream&, int&);
};






