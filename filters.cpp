#include "db/db.h"
#include "inputfile.h"
#include "grpinfo.h"
#include <stdint.h>
#include <iostream>
#include <map>
#include <queue>
#include <cassert>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
using namespace std;
using namespace boost::algorithm;
using namespace boost::adaptors;

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
	string sqlcmd, mutln;
	size_t numsamples;

	map<uint8_t, string> uberconmap;
	map<uint8_t, pair<string, string> > lanemidmap;

	/*
	 * Store uber consensus of each sample
	 */

	q.getln(sqlcmd);
	string id, ubr;
	if(db.retrieve(sqlcmd, ss, UBERCON) != 0)
		cout << "db error" << endl;

	while(ss >> id && ss >> ubr) {
		uint8_t idnum = stoi(id);
		uberconmap.insert(make_pair(idnum, ubr));
	}
	ss.clear();
	numsamples = uberconmap.size();

	//cout << numsamples << endl;
	//cout << uberconmap.at(22) << endl;

	// TEST4
/*
	for(int i=1; i <= numsamples; i++) {
		cout << i << endl;
		cout << uberconmap.at(i) << endl;
	}
*/
	/*
	 * Identify each sample with lane/mid nums
	 */

	q.getln(sqlcmd);
	if(db.retrieve(sqlcmd, ss, LANEMID) != 0)
		cout << "db error" << endl;
	string lane, mid;

	while(ss >> id && ss >> lane && ss >> mid) {
		uint8_t idnum = stoi(id);
		lanemidmap.insert(make_pair(idnum, make_pair(lane, mid)));
	}
	ss.clear();
	assert(numsamples == lanemidmap.size());

	/*
	 * Create a Grpmap for each sampleID
	 */

	typedef map<uint64_t, GrpInfo*> Grpmap; // 48 bytes each
	Grpmap grpmaps[numsamples]; // 48 * 96 = 4608 bytes!
	size_t cnt = 0; // count samples
	q.getln(sqlcmd);

	while(cnt < numsamples) {
		cnt++; // start from 1
		queue<pair<int, int> > pairsque;
		int mutinfo[6]; // freq, members, pos, type, subst, hetero

		while(mut.getln(mutln) && !starts_with(mutln, END_OF_SMPL)) {
			if(starts_with(mutln, "#")) {
				if(!pairsque.empty()) {
					stringstream ss(mutln);
					string skip;
					ss >> skip;
					for(int i = 1; i < 6; i++)
						ss >> mutinfo[i];
					while(!pairsque.empty()) {
						int grpid = pairsque.front().first;
						mutinfo[0] = pairsque.front().second;
						pairsque.pop();
						// TEST1
						//cout << "group" << grpid << ": " << mutinfo[0] << " " << mutinfo[1] << " " << mutinfo[2] << " " << mutinfo[3] << " " << mutinfo[4] << " " << mutinfo[5]  << endl;

						/*
						 * Build Grpmap members
						 */

						if(grpmaps[cnt-1].count(grpid) > 0) {
							// already id exists.
							//grpmaps[cnt-1].at(grpid)->update(mutinfo);
						} else {
							string lane, mid;
							string cmd = sqlcmd;
							stringstream grpss;

							lane = lanemidmap.at(cnt).first;
							mid = lanemidmap.at(cnt).second;
							grpss << grpid;
							
							cmd.replace(53, 1, lane);
							cmd.replace(cmd.end()-18, cmd.end()-17, mid);
							cmd.replace(cmd.end()-2, cmd.end()-1, grpss.str());
							// TEST3
							//cout << cmd << endl;

							string consensus;
							if(db.retrieve(cmd, consensus) != 0)
								cout << "db error" << endl;
							// TEST3
							//cout << consensus << endl;

							GrpInfo gi(consensus);
							grpmaps[cnt-1].insert(make_pair(grpid, &gi));
							grpmaps[cnt-1].at(grpid)->update(mutinfo);
						}
					}
					// TEST1
					//cout << "LINE_BREAK_IN_OUTPUT_TXT" << endl;
				}
			} else {
				stringstream ss(mutln);
				int grpid, freq;
				ss >> grpid >> freq;
				pairsque.push(make_pair(grpid, freq));
			}
		}
		// TEST1
		//cout << "END_OF_SMPL " << cnt << endl;
/*
		// TEST2 (very slow boost)
		vector<uint16_t> keys;
		boost::copy(grpmaps[cnt-1] | map_keys, back_inserter(keys));
		for (auto i: keys)
			std::cout << (int)i << ' ';
		cout << endl;
*/
	}
	return EXIT_SUCCESS;
}
