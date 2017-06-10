#include "db/db.h"
#include "grpinfo.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <map>
#include <cassert>
using namespace std;

int main(int argc, char **argv) {
	if(argc != 3) {
		cout << "Usage: filters [unfiltered].db mutation.list" << endl;
		return EXIT_FAILURE;
	}

	enum { UBERCON, LANEMID };
	Db db(argv[1]);
	stringstream ss;
	char *sql;
	uint8_t numsamples; // for testing

	map<uint8_t, string> uberconlist;
	map<uint8_t, pair<uint8_t, uint8_t> > lanemidlist;

	/*
	 * Store uber consensus of each sample
	 */

	sql = (char *)"select id, uber_consensus from uber_table";
	string id, ubr;
	if(db.retrieve(sql, ss, UBERCON) != 0)
		cout << "db error" << endl;
	while(ss >> id && ss >> ubr) {
		uint8_t idnum = stoi(id);
		uberconlist.insert(make_pair(idnum, ubr));
	}
	ss.clear();

	numsamples = uberconlist.size();
	cout << (int)numsamples << endl;
	cout << uberconlist.at(22) << endl;

	/*
	 * Identify each sample with lane/mid nums
	 */

	sql = (char *)"select id, lane, mid from uber_table";
	if(db.retrieve(sql, ss, LANEMID) != 0)
		cout << "db error" << endl;
	string lane, mid;
	while(ss >> id && ss >> lane && ss >> mid) {
		uint8_t idnum = stoi(id);
		uint8_t lanenum = stoi(lane);
		uint8_t midnum = stoi(mid);
		lanemidlist.insert(make_pair(idnum, make_pair(lanenum, midnum)));
	}
	ss.clear();

	assert((int)numsamples == lanemidlist.size());
	cout << (int)lanemidlist.at(22).first << ":" << (int)lanemidlist.at(22).second << endl;

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


