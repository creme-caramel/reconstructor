#ifndef GRPINFO_H
#define GRPINFO_H

#include <string>
#include <vector>
#include <stdexcept>
#include <stdint.h>
#include <memory>
using namespace std;

class GrpInfo {
	typedef struct {
		uint16_t freq;
		uint16_t pos; 
		uint8_t type;
		unsigned char hetero_subst;
	} Mutation;

	typedef vector<Mutation> Mutvec;

	struct MutvecDeleter {
		void operator()(Mutvec *mv) {
			delete(mv); 
		}
	};

	typedef unique_ptr<Mutvec, MutvecDeleter> Mptr;

	uint32_t grpid;
	uint16_t num_members;
	uint16_t num_true_som_subst;
	uint16_t num_true_het_subst;
	uint16_t num_true_som_indel;
	uint16_t num_true_het_indel;
	uint8_t matchingsample;
	uint8_t howmany;
	Mptr mptr;
	string consensus;

public:
	struct GrpError : public logic_error {
		GrpError(const string &msg = "") : logic_error(msg) {}
	};

	GrpInfo();
	GrpInfo(const uint32_t, string) throw(GrpError);
	void update(const int[]);
	uint32_t *getgrpid();
	string *getgrcon();
	size_t getmutnum();
	string muttostring(const Mutation &) const;
	string grptostring() const;
	void print() const;
};

struct GrpInfoDeleter {
	void operator()(GrpInfo *gi) {
		delete(gi);
	}
};

#endif /* GRPINFO_H */
