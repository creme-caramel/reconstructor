#include "filters.h"
#include "db/db.h"
#include "io/inputfile.h"

#include <queue>
#include <boost/algorithm/string/predicate.hpp> // start_with
using namespace boost::algorithm;

#define QFILE "sql/queries"
#define END_OF_SMPL "EOS"
#define MI 6 // // mutinfo: freq, members, pos, type, subst, hetero

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
	I8strmap uberconmap;
	I8strstrmap lanemidmap;

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

	I32gimap grpmaps[numsamples];
	size_t cnt = 0; // count samples
	q.getln(sqlcmd);
	while(cnt < numsamples) { // for each sample [1, 96]
		cnt++; // start from 1
		queue<pair<int, int> > pairsque;
		int grpid;
		int mutinfo[MI]; 
		while(mut.getln(mutln) && !starts_with(mutln, END_OF_SMPL)) {
			if(starts_with(mutln, "#")) {
				if(!pairsque.empty()) {
					makemutinfo(mutinfo, mutln, (int)MI);
					while(!pairsque.empty()) {
						grpid = pairsque.front().first;
						mutinfo[0] = pairsque.front().second;
						pairsque.pop();
						 // Build Grpmap members
						if(grpmaps[cnt-1].count(grpid) > 0) {
							// already grpid exists.
							grpmaps[cnt-1].at(grpid)->update(mutinfo);
						} else {
							// new grp id detected.
							string cmd, consensus;
							cmd = sqlcmd;
							getselectcmd(cmd, lanemidmap, grpid, cnt); 
							if(db.retrieve(cmd, consensus) != 0)
								cout << "db err" << endl;
							Giptr gi(new GrpInfo(grpid, consensus));
							grpmaps[cnt-1].insert(make_pair(grpid, move(gi)));
							grpmaps[cnt-1].at(grpid)->update(mutinfo);
						}
					}
				}
			} else {
				pairsque.push(getidfr(mutln));
			}
		}
		// printmap(grpmaps[cnt-1], 2);
	}
	return EXIT_SUCCESS;
}