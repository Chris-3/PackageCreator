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
#include <signal.h>

#include "Grit.h"


using namespace std::chrono;
using std::cout;
extern sig_atomic_t stopFlag;


/*****************************************************************************
the following class initialises the package, 
scales and spinnes the particles, adds them to the package 
and documents the data
*****************************************************************************/


class Package 
{
public:
	//Constructor
	Package(const std::string &, const int16_t &, const std::string &);
	//Destructor
	~Package();

	//this function fills the package with particles
	void fill_package(std::vector<Grit>&);
	//returns current basic scale and solid share border
	std::map<double, uint32_t>::const_reverse_iterator get_scale();
	//generates output file from calculated package
	void create_file();
	//prints the current status after adding particle/grit
	void status(Grit const&);

	
private:
	//options
	const int16_t options;
	//the data of this member will be saved in output file
	std::vector<std::vector<std::vector<uint8_t>>> package;
	//dimension of the created package
	coordinate<int> dim_pack;
	//path to source directory
	std::string pack_path;
	//total volume of package
	uint32_t max_vol;
	//number of solid voxels in package
	uint32_t solid_vox;
	//all scaling factors and corresponding solid share 
	std::map<double, uint32_t> por_threshold;
	//current scaling factor and corresponding solid share 
	std::map<double, uint32_t>::const_reverse_iterator it_now;
	//range for scale diversity in %
	double scale_diversity;

	//counter for added particles
	uint32_t count;
	//maximum solid share %
	double max_solid;
	//counter for number of tries for each particle
	uint32_t tried;
	//runtime of the programm
	steady_clock::time_point run_t;
	//Array for checking neighbouring voxels
	const std::vector<coordinate<int>> v_fill = { {0,0,0}, {1,1,1} ,{0,1,1},{0,0,1},{0,1,0},{1,0,0},{1,1,0},{1,0,1}  , { -1,-1,-1 }, { 0,-1,-1 }, { 0,0,-1 }, { 0,-1,0 }, { -1,0,0 }, { -1,-1,0 }, { -1,0,-1 }};
	//counts voxels added per particle
	int added_vox;
	//the calculated real scale including scale diversity
	double real_scale;
	//record of every particle added
	std::vector<std::string> stat;
	//general statistic for every scaling factor
	std::vector<std::string> stat_general;
	//colour ID in final Package
	uint8_t colour;
	//number of different particlemodels
	unsigned int nr_of_grit;
	//current particlemodel
	unsigned int grit_choice;
	

	//checks for existing particles were new particle should be inserted
	void check_if_free(Grit&, coordinate<int> const&);
	//if all checks are valid this function inserts particle at given position and orientation
	void add_grit(Grit&, const coordinate<int> &);
	//checks vector for periodicity
	inline void is_in_frame(coordinate<int>&);
	//spins the vector and scales it
	inline void spin_and_scale(coordinate<int>&, const Grit&);
	//creates a vektor pointing from point to center of gravity
	inline coordinate<int> dir_to_center(coordinate<int>);
	//if coordinates overlap this function checks neighbouring voxels for free spots
	inline void fill_holes(coordinate<int>&, const int &);
	//creates line for statistic for each particle added to package
	void get_stat(const std::ostringstream&, int&);
	//initialises empty package
	void init_empty_pack();	
	//loads eisting package from file
	void load_pack(const std::string &);

	
};






