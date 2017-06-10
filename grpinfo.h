#ifndef GRPINFO_H
#define GRPINFO_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iosfwd>
#include <stdint.h>
using namespace std;

class GrpInfo {
/*
	num_seq: number of sequences that make up the group
	num_true_som_subst: number of "true" somatic substition mutations in the group
	num_true_het_subst: number of "true" heteroplasmic substition mutations in the group
	num_true_som_indel: number of "true" somatic insertion/deletion mutations in the group
	num_true_het_indel: number of "true" heteroplasmic insertion/deletion mutations in the group
	matchingsample: if it is a contamination, the MID and Lane of the sample/uber consensus sequence it matches
	howmany: how many mutations a group needs to be considered a contamination
	consensus: this group's consensus sequence
	mutationlist: list of mutations in the group
*/
	uint64_t num_seq;
	uint64_t num_true_som_subst;
	uint64_t num_true_het_subst;
	uint64_t num_true_som_indel;
	uint64_t num_true_het_indel;
	uint8_t matchingsample;
	uint8_t howmany; // ?
	string *consensus;
	vector<struct Mutation> mutationlist;

public:
	struct Mutation {
/*
		pos: mitomap position of the mutation
		type: type of mutation // aka is it a G to A, A to C, C to :
		freq: number of sequences within it's group that display the same mutation
		totalmember: total number of sequences with the group
		classified(hi): weather it is a substitution mutation or an insertion/deletion mutation
		classified(lo): weather the mutation is considered to be heteroplasmic or somatic
*/
		uint16_t pos; 
		uint8_t type;
		uint16_t freq;
		uint16_t totalmember;
		char classified;
	};

	struct GrpError : public logic_error {
		GrpError(const string &msg = "") : logic_error(msg) {}
	};

	GrpInfo();
	GrpInfo(const string &) throw(GrpError);
	string tostring() const;

};

#endif /* GRPINFO_H */
