#include "Package.h"
#include "random.h"

#define DEBUG 0
#define DEBUG1 0
#define PACK_INIT 1

extern bool SPIN;
extern bool GRIT_COUNT;
extern bool MORE_INFO;
extern bool FILL_HOLES;
extern bool COLOUR_ID;



inline coordinate<int> Random_V_int(const coordinate<int> & n)
// generiert einen neuen Mittelpunkt fuer das Partikel in der Packung aus
{
	std::random_device rd;
	pcg rand(rd);
	std::uniform_int_distribution<> x(0, n.x - 1);
	std::uniform_int_distribution<> y(0, n.y - 1);
	std::uniform_int_distribution<> z(0, n.z - 1);

	return { x(rand),y(rand),z(rand) };
}

inline coordinate<double> Random_V_double(const coordinate<int> & n)
// generiert einen neuen Mittelpunkt fuer das Partikel in der Packung aus
{
	std::random_device rd;
	pcg rand(rd);
	std::uniform_real_distribution<> x(1 - n.x, n.x - 1);
	std::uniform_real_distribution<> y(1 - n.y, n.y - 1);
	std::uniform_real_distribution<> z(1 - n.z, n.z - 1);

	return { x(rand),y(rand),z(rand) };
}

inline int Random_No(const unsigned int &n)
// generates random Int
{
	std::random_device rd;
	pcg rand(rd);
	std::uniform_int_distribution<> p(0, n);
	return p(rand);
}

inline double Random_No(const double& n)
// generates random double
{
	std::random_device rd;
	pcg rand(rd);
	std::uniform_real_distribution<> p(0, n);
	return p(rand);
}



//Package::Package() {}
Package::Package(std::string const& d) :pack_path(d)
{
	run_t = steady_clock::now();
	std::fstream file(pack_path, std::ios_base::in);
	double scale = 0.0, solid_share = 0.0;
	uint32_t temp = 0;
	tried = 0;
	colour = 1;
	grit_choice = 0; 
	//here Parameters are read from Parameter file
	if (file.is_open())
	{
		file >> dim_pack.x >> dim_pack.y >> dim_pack.z >> max_solid >> scale_diversity;
		max_vol = dim_pack.x*dim_pack.y*dim_pack.z;
		while (file >> scale >> solid_share)
		{
			if (!GRIT_COUNT) temp = temp + static_cast<uint32_t>(((solid_share*max_solid) / 10000)*max_vol);
			if (GRIT_COUNT) temp = temp + static_cast<uint32_t>(solid_share);
			por_threshold.insert(std::pair<double, uint32_t>(scale, temp));
		}
	}
	file.close();
	it_now = por_threshold.rbegin();

	//here the 3D Array for the Package is created this variable will be saved in output file
	if (PACK_INIT)
	{
		double i = 0;
		int perc = 10;
		package.resize(dim_pack.x);
		for (auto& y : package)
		{
			y.resize(dim_pack.y);
			for (auto& z : y)
			{
				z.resize(dim_pack.z, 0);
			}
			if (perc < (i / dim_pack.x) * 100)
			{
				std::cout << "Initialise package: " << perc << "%\n";
				perc += 10;
			}

			i++;
		}
	}

	//here is a general summery of the stats printed in command line
	cout << "\n\n Package parameters:"
		<< "\n Package volume: " << max_vol
		<< "\n Solid share: " << max_solid << "%"
		<< "\t Scale diversity: " << scale_diversity << "%"
		;
	//here are the scaling factors with according solid limits printed 
	for (auto it = por_threshold.rbegin(); it != por_threshold.rend(); ++it)
	{
		cout << "\n Basic scale: " << it->first
			<< "\t=>\t Solid share border: " << static_cast<double>(it->second) / static_cast<double>(max_vol) * 100 << "%";
	}
	cout << "\n\n";

}

Package::~Package()
{
	CreateFile();
}

inline void Package::spin_and_scale(coordinate<int> &v, const Grit& p)
{
	coordinate<double> x = v * static_cast<double>(1);
	if (SPIN)
	{
		x = p.rot_v*(p.rot_v*x)*(1 - cos(p.rot_alpha))
			+ x * cos(p.rot_alpha)
			+ (p.rot_v.cross_product(x))*sin(p.rot_alpha);
	}
	x = x * real_scale;
	v = { static_cast<int>(x.x),static_cast<int>(x.y),static_cast<int>(x.z) };
}

inline void Package::fill_holes(coordinate<int>& n, const int &i)
{
	n = { n.x + dir_to_center(n).x * v_fill[i].x,
	n.y + dir_to_center(n).y * v_fill[i].y,
	n.z + dir_to_center(n).z * v_fill[i].z };
}

void Package::AddGrit(Grit& p_to_add, const coordinate<int> &v)
{
	added_vox = 0;
	if (DEBUG) std::cout << "\n Patikel einfuegen\n";
	for (auto n : p_to_add.p_img)
	{
		spin_and_scale(n, p_to_add);
		n = n + v;
		IsInFrame(n);
		unsigned int fill;
		if (FILL_HOLES)fill = v_fill.size();
		if (!FILL_HOLES)fill = 1;
		for (unsigned int i = 0; i < fill; i++)
		{
			fill_holes(n, i);
			if (package[n.x][n.y][n.z])continue;
			IsInFrame(n);
			package[n.x][n.y][n.z] = colour; //static_cast<uint8_t>(it_now->first);
			solid_vox++;
			added_vox++;
			break;
		}
	}
	count++;
	Status(p_to_add);
	tried = 0;
	grit_choice = Random_No(nr_of_grit);
}


void Package::CheckIfFree(Grit& p, const coordinate<int> & v)
{
	//if (package[p_to_check.center_p.x][p_to_check.center_p.y][p_to_check.center_p.z]) return false;
	//if (DEBUG1) std::cout << " check Eckpunkte\n";
	p.get_rot_param(Random_V_double(dim_pack).normalized(), Random_No(M_PI * 2));
	/*static int j = 0;
	p.get_rot_param(v_fill[j]* static_cast<double>(1), 0);
	j++;
	if (j >= v_fill.size()-1)j = 0;*/
	real_scale = (it_now->first / por_threshold.rbegin()->first) + ((Random_No(scale_diversity * 2) / 100.0) - (scale_diversity / 100.0))* (it_now->first / por_threshold.rbegin()->first);
	p.v_trans = v;
	int n_new = 0;

	for (auto n : p.frame_points)
	{
		spin_and_scale(n, p);
		p.frame_points_new[n_new++] = n;
		if (DEBUG)cout << " n2:" << n;
		n = n + v;
		IsInFrame(n);
		if (DEBUG)cout << " n3:" << n;
		if (package[n.x][n.y][n.z])return;
	}
	n_new = 0;
	if (DEBUG)cout << "\n\n";
	for (auto n : p.p_img)
	{
		spin_and_scale(n, p);
		n = n + v;
		IsInFrame(n);
		if (package[n.x][n.y][n.z])return;
	}
	AddGrit(p, v);
	return;
}


void Package::FillPackage(std::vector<Grit>& particles)
//this function fills the package with particles
{
	coordinate<int> v;
	nr_of_grit = particles.size() - 1;
	
	while (true)
	{
		if (it_now->second <= solid_vox && !GRIT_COUNT) ++it_now;
		if (it_now->second <= count && GRIT_COUNT) ++it_now;
		if (it_now == por_threshold.rend())return;
		tried++;
		v = Random_V_int(dim_pack);
		if (package[v.x][v.y][v.z])continue;
		
		CheckIfFree(particles[grit_choice], v);
	}
}

std::map<double, uint32_t>::const_reverse_iterator Package::GetScale()
{
	return it_now;
}

bool Package::IsNotFull()
{
	if ((max_solid / 100)*max_vol > solid_vox)return true;
	return false;
}

void Package::CreateFile()
{
	double i = 0;
	int perc = 5;
	pack_path.replace(pack_path.begin() + pack_path.find_last_of('/') + 1, pack_path.end(), "new_package.leS");

	std::cout << "\n write file in: " << pack_path << "\n";

	std::ofstream fileo;
	//std::ostringstream oss;
	fileo.open(pack_path);
	fileo << dim_pack.x << ' ' << dim_pack.y << ' ' << dim_pack.z;
	bool k = true;
	for (auto const& x : package)
	{
		for (auto const& y : x)
		{
			fileo << '\n';
			for (unsigned int z = 0; z < ((y.size() * 2) - 1); z++)
			{
				if (!(z % 2))fileo << std::to_string(y[z / 2]);
				if ((z % 2))fileo << ' ';
			}

		}
		if (perc < (i / dim_pack.x) * 100)
		{
			std::cout << "|";
			perc += 5;
		}
		i++;
	}
	fileo.close();
	std::cout << "\n";

	steady_clock::time_point run_t_now = steady_clock::now();
	duration<double> runtime = duration_cast<duration<double>>(run_t_now - run_t);
	int t = static_cast<int>(runtime.count());
	std::ostringstream time;
	time.flush();
	time.fill('0');
	time << std::setw(2) << t / 3600 << ":" << std::setw(2) << (t % 3600) / 60 << ":" << std::setw(2) << t % 60;

	pack_path.replace(pack_path.begin() + pack_path.find_last_of('/') + 1, pack_path.end(), "statistic.txt");
	std::cout << "\n write statistics in: " << pack_path << "\n";

	fileo.open(pack_path);
	fileo << " Dimension: \t" << dim_pack
		<< "\n Porosity: \t" << 100 - static_cast<double>(solid_vox) / static_cast<double>(max_vol) * 100 << "%"
		<< "\n Scale Diversity: \t" << scale_diversity << "%"
		<< "\n Solid Voxel: \t" << solid_vox
		<< "\n Number of particles: \t" << count
		<< "\n Time to create: \t" << time.str()
		<< "\n\n";

	fileo << "Runtime" << "\tBasic_scale" << "\tNumber_of_Particles" << "\tSolid_share" << "\tSolid_voxel" << "\n";

	for (auto const& line : stat_general)
	{
		fileo << line;
	}

	fileo << "\n\nRuntime" << "\tBasic_scale" << "\tReal_scale" << "\tModelnumber" << "\tVolume" << "\n";

	for (auto const& line : stat)
	{
		fileo << line;
	}
	fileo.close();

	std::cout << "\n file schreiben fertig\n";
	std::cin.get();
}

void Package::get_stat(const std::ostringstream& time, int& count_p_scale)
{
	std::ostringstream oss2;
	oss2.flush();
	oss2 << time.str()
		<< "\t" << it_now->first
		<< "\t" << count_p_scale
		<< "\t" << static_cast<double>(solid_vox) / static_cast<double>(max_vol) * 100 << "%"
		<< "\t" << solid_vox << "\n";
	stat_general.push_back(oss2.str());
	count_p_scale = 0;
	if (COLOUR_ID)colour++;
}

void Package::Status(Grit const& p)
{
	steady_clock::time_point run_t_now = steady_clock::now();
	duration<double> runtime = duration_cast<duration<double>>(run_t_now - run_t);
	double t = runtime.count();
	static double t_add = 0;
	static int count_p_scale = 0;
	std::ostringstream oss;
	std::ostringstream time;
	time.flush();
	time.fill('0');
	time << std::setw(2) << static_cast<int>(t) / 3600 << ":"
		<< std::setw(2) << (static_cast<int>(t) % 3600) / 60 << ":"
		<< std::setw(2) << static_cast<int>(t) % 60;
	std::cout
		<< " Total solid share: " << static_cast<double>(solid_vox) / static_cast<double>(max_vol) * 100 << "%"
		<< "\n Particle count: " << count
		<< "\n Solid voxel: " << solid_vox
		<< "\n Empty voxel: " << max_vol - solid_vox
		<< "\n Paticle ID: " << p.id << "\t Real scale: " << real_scale * 100 << "% = " << por_threshold.rbegin()->first*real_scale << "mm \t Volume: " << added_vox
		<< "\n Nr. of tries: " << tried
		<< "\n Basic scale (" << it_now->first << ") up to " << static_cast<double>(it_now->second) / static_cast<double>(max_vol) * 100 << "%  => " << it_now->second << " Voxel"
		<< "\n Current runtime: " << time.str()
		<< "\n time to add: " << t - t_add << "sec";
	count_p_scale++;

	if (!GRIT_COUNT && solid_vox >= it_now->second)get_stat(time, count_p_scale);
	if (GRIT_COUNT && count >= it_now->second)get_stat(time, count_p_scale);

	oss.flush();
	oss << time.str()
		<< "\t" << it_now->first
		<< "\t" << real_scale
		<< "\t" << p.id
		<< "\t" << added_vox
		;
	if (MORE_INFO)
	{
		oss << "\t" << p.v_trans
			<< "\t" << p.rot_v
			<< "\t" << p.rot_alpha
			;
		for (auto i : p.frame_points_new) oss << i;
	}


	oss << "\n";
	stat.push_back(oss.str());
	cout << "\n\n";
	t_add = runtime.count();

}

inline void Package::IsInFrame(coordinate<int> & i)
//checks vector for periodicity
{
	if (i.x < 0)i.x = dim_pack.x + i.x;
	if (i.x >= dim_pack.x)i.x = i.x - dim_pack.x;

	if (i.y < 0)i.y = dim_pack.y + i.y;
	if (i.y >= dim_pack.y)i.y = i.y - dim_pack.y;

	if (i.z < 0)i.z = dim_pack.z + i.z;
	if (i.z >= dim_pack.z)i.z = i.z - dim_pack.z;
}

inline coordinate<int> Package::dir_to_center(coordinate<int> v_temp)
{
	if (abs(v_temp.x)) v_temp.x = v_temp.x / abs(v_temp.x);
	if (abs(v_temp.y)) v_temp.y = v_temp.y / abs(v_temp.y);
	if (abs(v_temp.z)) v_temp.z = v_temp.z / abs(v_temp.z);
	return v_temp * -1;
}
