#include "inputfile.h"
using namespace std;

InputFile::InputFile(const char *fname) throw(InputFileError)
{
	f.open(fname);
	if(!f.is_open())
		throw InputFileError("Can't open ifstream");
}

InputFile::~InputFile()
{
	f.close();
}

bool InputFile::getln(string &cmd) throw(InputFileError)
{
	if(!getline(f, cmd)) {
		throw InputFileError("No more line from ifstream");
		return false;
	}
	return true;
}

ifstream *InputFile::fp()
{
	return &f;
}