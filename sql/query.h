#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <fstream>
#include <stdexcept>

class Query {
	std::ifstream f;
public:
	struct QueryError : std::runtime_error {
		QueryError(const char *msg) : std::runtime_error(msg) {}
	};

	Query(const char *);
	~Query();
	void getquery(std::string &);
	std::ifstream *fp();
};

#endif /* QUERY_H */