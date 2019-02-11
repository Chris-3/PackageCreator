#ifndef OPTION_H
#define OPTION_H
class options
{
public:
	options();
	~options();

	bool SPIN;
	bool GRIT_COUNT;
	bool MORE_INFO;
	bool FILL_HOLES;
	bool COLOUR_ID;
	bool LOAD_PACKAGE;
	bool CURRENT_DIR;
private:

};

options::options()
{
	SPIN = true;
	GRIT_COUNT = false;
	MORE_INFO = false;
	FILL_HOLES = false;
	COLOUR_ID = true;
	LOAD_PACKAGE = false;
	CURRENT_DIR = true;
}

options::~options()
{
}

#endif