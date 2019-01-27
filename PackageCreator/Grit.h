#pragma once
#include <iostream>
#include <vector>
#include <forward_list>
#include <fstream>
#include <string>
//#include <time.h> 
#include <stdlib.h>     /* srand, rand */
#include <iomanip>      // std::setprecision
#include <math.h>
#include <typeinfo>
#include <unordered_set>

# define M_PI           3.14159265358979323846  /* pi */

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
		return temp / n;//{ this->x / n,this->y / n, this->z / n };
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

class Grit
{
public:
	Grit();
	Grit(std::string const&);
	~Grit();

	//Zugriffsfunktionen
	inline int GetVolume()const;//gibt Volumen des Aktuellen Partikels aus
	//angle GetParameterRotMatrix()const;//gibt die Winkel der Rotationsmatrix aus
	//void GetFramePoints()const;//gibt die ‰uﬂeren Punkte des Partikels aus
	
	//Manipulatoren
	//coordinate<int> Spin(coordinate<double>, double const &, double const &);
	//dreht das Partikel um einen zufaelligen Winkel
	void IsFrame(const coordinate<int> &);
	//checkt ob das aktuelle voxel das aeusserste ist
	void CenterOfGravity();
	//berechnet den schwerpunkt
	void ConvertCoordinates();
	//rechnet die coordinaten auf vektoren vom Schwerpunkt aus um
	void GetId(std::string const&);
	//coordinate<int> dir_to_center(coordinate<int>);
	//creates a vektor pointing from point to center of gravity
	//void GetNeighbours(std::vector<coordinate<int>>&, coordinate<int>);
	void get_rot_param(const coordinate<double>&,const double&);


private:
	

	//std::forward_list<coordinate<int>> p_img;
	//saves a vector to every solid voxel found in the original file
	std::forward_list<coordinate<int>> p_img;
	// Liste von Vektoren gredreht und skaliert
	coordinate<int> center_p;
	//Vektor auf die neue Position eines Kornes in der gesamt Packung
	coordinate<int> dim_part;
	//Dimension der Ausgangsdatei
	std::vector<coordinate<int>> frame_points;
	//aeusserten punkte eines Kornes
	std::vector<coordinate<int>> frame_points_new;

	uint32_t volume_or;
	//Anzahl der Voxel eines Partikels ->>original file
	uint32_t volume;
	//Anzahl der Voxel eines Partikels
	std::string id;
	//ID des Partikels
	//int num_of_id;
	//anzahl der Partikel dieser ID in der gesamtpackung
	std::string grit_path;
	//Speicherpfad der Ursprungsdatei
	coordinate<double> rot_v;
	double rot_alpha;
	coordinate<int> v_trans;

	friend class Package;
};

