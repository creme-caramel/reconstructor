#include "db/db.h"
#include "grpinfo.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <map>
using namespace std;

int main(int argc, char **argv) {
	if(argc != 3) {
		cout << "Usage: filters [unfiltered].db mutation.list" << endl;
		return EXIT_FAILURE;
	}

	Db db(argv[1]);
	stringstream ss;
	uint8_t numsamples;

	map<uint8_t, uint8_t[2]> lanemidlist;
	map<uint8_t, string> uberconlist;
	
	char *sql = (char *)"select id, uber_consensus from uber_table";
	string id, ubr;
	if(db.retrieve(sql, ss) != 0)
		cout << "db error" << endl;
	while(ss >> id && ss >> ubr) {
		uint8_t idnum = stoi(id);
		uberconlist.insert(make_pair(idnum, ubr));
	}
	numsamples = uberconlist.size();
cout << (int)numsamples << endl;
cout << uberconlist.at(22) << endl;
	ss.clear();

	sql = (char *)"select id, lane, mid from uber_table";

/*
	map<uint64_t, GrpData*> grplist;
	if(grplist(234).count() > 0) {
		// already there. just add on
	}
	else {
		GrpData whatever;
		grplist.insert(make_pair(234, &whatever));
	}
*/

	return EXIT_SUCCESS;
}


