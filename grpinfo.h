#ifndef GRPINFO_H
#define GRPINFO_H

#include <string>
#include <vector>
#include <stdexcept>
#include <stdint.h>
using namespace std;

class GrpInfo {
	typedef struct { // 6 bytes
		/*
		 * pos: mitomap position of the mutation
		 * type: type of mutation // aka is it a G to A, A to C, C to :
		 * freq: number of sequences within it's group that display the same mutation
		 * totalmember: total number of sequences with the group
		 * classified(hi): weather it is a substitution mutation or an insertion/deletion mutation
		 * classified(lo): weather the mutation is considered to be heteroplasmic or somatic
		 */

		//uint16_t totalmember; // already included as class field (num_members)
		uint16_t freq;
		uint16_t pos; 
		uint8_t type;
		unsigned char hetero_subst;
	} Mutation;

	/*
	 * num_members: number of sequences that make up the group
	 * num_true_som_subst: number of "true" somatic substition mutations in the group
	 * num_true_het_subst: number of "true" heteroplasmic substition mutations in the group
	 * num_true_som_indel: number of "true" somatic insertion/deletion mutations in the group
	 * num_true_het_indel: number of "true" heteroplasmic insertion/deletion mutations in the group
	 * matchingsample: if it is a contamination, the MID and Lane of the sample/uber consensus sequence it matches
	 * howmany: how many mutations a group needs to be considered a contamination
	 * consensus: this group's consensus sequence
	 * mutationlist: list of mutations in the group
	 */

	uint32_t grpid;
	uint16_t num_members;
	uint16_t num_true_som_subst;
	uint16_t num_true_het_subst;
	uint16_t num_true_som_indel;
	uint16_t num_true_het_indel;
	uint8_t matchingsample;
	uint8_t howmany; // ?
	const string *consensus;
	vector<Mutation> mutationlist;

public:
	struct GrpError : public logic_error {
		GrpError(const string &msg = "") : logic_error(msg) {}
	};

	GrpInfo();
	GrpInfo(const uint32_t) throw(GrpError);
	uint32_t *getgrpid();
	void update(const int[]);
	string muttostring(const Mutation &) const;
	string grptostring() const;
	void print() const;
};

#endif /* GRPINFO_H */
