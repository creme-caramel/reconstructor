#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <string>
#include <fstream>
#include <stdexcept>

class InputFile {
	std::ifstream f;
public:
	struct InputFileError : std::runtime_error {
		InputFileError(const char *msg) : std::runtime_error(msg) {}
	};

	InputFile(const char *) throw(InputFileError);
	~InputFile();
	bool getln(std::string &) throw(InputFileError);
	std::ifstream *fp();
};

#endif /* INPUTFILE_H */