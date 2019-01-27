#include "Grit.h"
#define DEBUG 0
#define DEBUG1 0
#define DEBUG2 0

using std::cout;
Grit::Grit() {}


Grit::Grit(std::string const& d) :grit_path(d)
{

	std::fstream file(grit_path, std::ios_base::in);
	uint32_t count = 0;
	bool voxl = 0;
	bool hasbeenGrit = false;
//	num_of_id = 0;
	std::cout << "\n Initialise Particle: " << grit_path << "\n";

	if (file.is_open())
	{
		file >> dim_part.x >> dim_part.y >> dim_part.z;
		while (file >> voxl)
		{
			if (voxl)
			{
				coordinate<int> x = { static_cast<int>(count / (dim_part.z*dim_part.y))
					, static_cast<int>((count % (dim_part.z*dim_part.y)) / dim_part.z)
					, static_cast<int>(count%dim_part.z) };//create vektor from file
				//if(DEBUG)x.print();
				p_img.push_front(x);
				volume++;
				hasbeenGrit = true;
			}
			if (hasbeenGrit && (((count / (dim_part.z*dim_part.y)) - p_img.front().x) > 5))
			{
				break;
			}
			count++;
		}
	}
	file.close();

	CenterOfGravity();
	GetId(grit_path);//extracts model number
	frame_points.resize(6, center_p);
	frame_points_new.resize(6, center_p);
	if (DEBUG)
	{
		std::cout << "\n volume" << volume << "\n centerpoint:";
		std::cout << "\n frame:";
	}

	ConvertCoordinates();//converts source point to center of gravity for every vector
	std::cout << " Partcle ID: " << id << "\t Volume: " << volume<<"\n";

}

Grit::~Grit() {}


inline int Grit::GetVolume() const
{
	return volume;
}

void Grit::IsFrame(const coordinate<int> & i)
{
	if (frame_points[0].x < i.x)frame_points[0] = i;
	if (frame_points[1].x > i.x)frame_points[1] = i;
	if (frame_points[2].y < i.y)frame_points[2] = i;
	if (frame_points[3].y > i.y)frame_points[3] = i;
	if (frame_points[4].z < i.z)frame_points[4] = i;
	if (frame_points[5].z > i.z)frame_points[5] = i;
}


void Grit::CenterOfGravity()
{
	center_p = { 0,0,0 };
	for (auto const& n : p_img)
	{
		center_p = center_p + n;
	}
	center_p.x = center_p.x / volume;
	center_p.y = center_p.y / volume;
	center_p.z = center_p.z / volume;
}

void Grit::ConvertCoordinates()
{
	for (auto& n : p_img)
	{
		IsFrame(n);
		n = n - center_p;
	}
	for (auto& n : frame_points)
	{

		n = n - center_p;
	}
}

void Grit::GetId(std::string const & d)
{
	for (auto i = (d.find("Particle_") + 9); d[i] != '_'; i++)
	{
		id.push_back(d[i]);
	}
	/*for (auto i = (d.find("Scaled_") + 7); d[i] != '.'; i++)
	{
		scale.push_back(d[i]);
	}*/
}




void Grit::get_rot_param(const coordinate<double>& _rot_v, const double & _rot_alpha)
{
	rot_v = _rot_v;
	rot_alpha = _rot_alpha;
}
