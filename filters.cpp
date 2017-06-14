#include "db/db.h"
#include "inputfile.h"
#include "grpinfo.h"
#include <stdint.h>
#include <iostream>
#include <map>
#include <queue>
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
	stringstream ss;
	string line;
	size_t numsamples;

	map<uint8_t, string> uberconmap;
	map<uint8_t, pair<uint8_t, uint8_t> > lanemidmap;
	typedef map<uint64_t, GrpInfo*> Grpmap; // 48 bytes each

	/*
	 * Store uber consensus of each sample
	 */

	q.getln(line);
	string id, ubr;
	if(db.retrieve(line, ss, UBERCON) != 0)
		cout << "db error" << endl;
	while(ss >> id && ss >> ubr) {
		uint8_t idnum = stoi(id);
		uberconmap.insert(make_pair(idnum, ubr));
	}
	ss.clear();
	numsamples = uberconmap.size();

	cout << numsamples << endl;
	cout << uberconmap.at(22) << endl;

	/*
	 * Identify each sample with lane/mid nums
	 */

	q.getln(line);
	if(db.retrieve(line, ss, LANEMID) != 0)
		cout << "db error" << endl;
	string lane, mid;
	while(ss >> id && ss >> lane && ss >> mid) {
		uint8_t idnum = stoi(id);
		uint8_t lanenum = stoi(lane);
		uint8_t midnum = stoi(mid);
		lanemidmap.insert(make_pair(idnum, make_pair(lanenum, midnum)));
	}
	ss.clear();
	assert(numsamples == lanemidmap.size());

	cout << (int)lanemidmap.at(22).first << ":" << (int)lanemidmap.at(22).second << endl;

	/*
	 * Create a Grpmap for each sampleID
	 */	

	Grpmap grpmaparr[numsamples]; // 48 * 96 = 4608 bytes!
	size_t cntsamples = 0;
	while(cntsamples < numsamples) {
		cntsamples++; // start from 1
		queue<pair<int, int> > pairsque;
		int fiveint[5]; // members, pos, type, subst, hetero

		while(mut.getln(line) && !starts_with(line, END_OF_SMPL)) {
			if(starts_with(line, "#")) {
				if(!pairsque.empty()) {
					stringstream ss(line);
					string skip;
					ss >> skip;
					for(int i = 0; i < 5; i++)
						ss >> fiveint[i];
					while(!pairsque.empty()) {
						int grpid = pairsque.front().first;
						int freq = pairsque.front().second;
						pairsque.pop();
						cout << "group" << grpid << ": " << freq << " " << fiveint[0] << " " << fiveint[1] << " " << fiveint[2] << " " << fiveint[3] << " " << fiveint[4]  << endl;
					}
					cout << "HERE" << endl;
				}
			} else {
				stringstream ss(line);
				int grpid, freq;
				ss >> grpid >> freq;
				pairsque.push(make_pair(grpid, freq));
			}
		}
		cout << "END_OF_SMPL " << cntsamples << endl;
	}
	cout << sizeof(grpmaparr) << endl;

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


