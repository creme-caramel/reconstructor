#include "db/db.h"
#include "inputfile.h"
#include "grpinfo.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <cassert>
#include <boost/algorithm/string/predicate.hpp>
using namespace std;
using namespace boost::algorithm;

#define QFILE "sql/queries"
#define END_OF_SMPL "EOS"

int main(int argc, char **argv) {
	if(argc != 3) {
		cout << "Usage: filters [unfiltered].db mutation.list" << endl;
		return EXIT_FAILURE;
	}

	enum { UBERCON, LANEMID };
	Db db(argv[1]);
	InputFile q(QFILE);
	InputFile mut(argv[2]);
	string line;
	size_t numsamples;

	/*
	 * Store uber consensus of each sample
	 */

	q.getln(line);
	string id, ubr;
	if(db.retrieve(line, readdb, UBERCON) != 0)
		cout << "db error" << endl;
	while(readdb >> id && readdb >> ubr) {
		uint8_t idnum = stoi(id);
	}
	readdb.clear();

	/*
	 * Identify each sample with lane/mid nums
	 */

	q.getln(line);
	if(db.retrieve(line, readdb, LANEMID) != 0)
		cout << "db error" << endl;
	string lane, mid;
	while(readdb >> id && readdb >> lane && readdb >> mid) {
		uint8_t idnum = stoi(id);
		uint8_t lanenum = stoi(lane);
		uint8_t midnum = stoi(mid);
	}
	readdb.clear();

	/*
	 * Create a Grpmap for each sampleID
	 */	

	return EXIT_SUCCESS;
}


