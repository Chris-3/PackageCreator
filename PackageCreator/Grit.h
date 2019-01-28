#pragma once
#include <iostream>
#include <vector>
#include <forward_list>
#include <fstream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <iomanip>      // std::setprecision
#include <math.h>
#include <typeinfo>
#include <unordered_set>

# define M_PI           3.14159265358979323846  /* pi */
/*****************************************************************************
the following struct is the implementaion of a mathemathical 3D vector class
*****************************************************************************/

template <typename T>
struct coordinate {

	T x;
	T y;
	T z;

	coordinate<T> normalized() const
	{
		T n = 0;
		coordinate<T> temp = *this;
		n = sqrt(x*x + y * y + z * z);
		return temp / n;
	}

	coordinate<T> cross_product(coordinate<T> const& v) const
	{
		return { y*v.z - z * v.y,
			z*v.x - x * v.z,
			x*v.y - y * v.x };
	}


};
template <typename T1, typename T2>
inline coordinate<T2> operator + (coordinate<T1> const & a, coordinate<T2> const & b)
{
	return { a.x + b.x ,a.y + b.y ,a.z + b.z };
}
template <typename T1, typename T2>
inline coordinate<T2> operator - (coordinate<T1> const & a, coordinate<T2> const & b)
{
	return { a.x - b.x ,a.y - b.y ,a.z - b.z };
}
template <typename T1, typename T2>
inline coordinate<T2> operator * (coordinate<T1> const & a, T2 const & b)
{
	return { a.x*b, a.y*b, a.z*b };
}
template <typename T1, typename T2>
inline coordinate<T2> operator / (coordinate<T1> const & a, T2 const & b)
{
	return { a.x / b, a.y / b, a.z / b };
}
template <typename T1, typename T2>
inline T1 operator * (coordinate<T1> const & a, coordinate<T2> const & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
template <typename T>
inline bool operator == (coordinate<T> const & a, coordinate<T> const & b)
{
	if(a.x != b.x)return false;
	if(a.y != b.y)return false;
	if(a.z != b.z)return false;
	return true;
}
template<class T>
std::ostream& operator<<(std::ostream &os, const coordinate<T>&v) {
	os << "{ x" << v.x << " y" << v.y << " z" << v.z << "}";
	return os;
}
template <typename T1, typename T2>
inline bool operator < (coordinate<T1> const & a, coordinate<T2> const & b)
{
	if(a.x < b.x)return true;
	if (a.y < b.y)return true;
	if (a.z < b.z)return true;
	return false;
}
template <typename T1, typename T2>
inline bool operator > (coordinate<T1> const & a, coordinate<T2> const & b)
{
	if (a.x > b.x)return true;
	if (a.y > b.y)return true;
	if (a.z > b.z)return true;
	return false;
}
template <typename T>
inline coordinate<T> abs(const coordinate<T> & a)
{
	return{ abs(a.x),abs(a.y),abs(a.z) };
}

/*****************************************************************************
the following class manages the initialisation of Particles and 
documents the parameters of scaled and spinned particles
*****************************************************************************/


class Grit
{
public:
	Grit();
	Grit(std::string const&);
	~Grit();
	
	
	void is_frame(const coordinate<int16_t> &);
	//checks if current particle is one of the outer most
	void center_of_gravity();
	//calculates center of gravity
	void convert_coordinates();
	//converts vectors to new starting point from center of gravity
	void get_rot_param(const coordinate<double>&,const double&);
	//this function saves the rotation parameters for every added particle

private:
	
	std::forward_list<coordinate<int16_t>> p_img;
	//saves a vector to every solid voxel found in the original file
	coordinate<int16_t> center_p;
	//vector to calculated center of gravity from absolut starting point
	coordinate<int16_t> dim_part;
	//dimension of the file the particle is loaded from
	std::vector<coordinate<int16_t>> frame_points;
	//outer limits of the particle (original file)
	std::vector<coordinate<int16_t>> frame_points_new;
	//outer limits of the particle (spinned and scaled)
	uint32_t volume_or;
	//original Volume of particle
	uint32_t volume;
	//volume after spinning and scaling
	std::string id;
	//modelnumber of particle
	std::string grit_path;
	//filepath of source file
	coordinate<double> rot_v;
	//parameter for rotation matrix (unit vector)
	double rot_alpha;
	//parameter for rotation matrix (alpha)
	coordinate<int16_t> v_trans;
	//new center of gravity in package

	void get_id(std::string const&);
	//extracts Modelnumber from filename

	friend class Package;
};

