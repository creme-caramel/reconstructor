#include "query.h"
using namespace std;

Query::Query(const char *fname)
{
	f.open(fname);
	if(!f.is_open())
		throw QueryError("err opening ifstream");
}

Query::~Query()
{
	f.close();
}

void Query::getquery(string &cmd)
{
	if(!getline(f, cmd))
		throw QueryError("err getting line from ifstream");
}

ifstream *Query::fp()
{
	return &f;
}