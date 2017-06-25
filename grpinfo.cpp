#include "grpinfo.h"
#include <sstream>
#include <iostream>

GrpInfo::GrpInfo()
{
	GrpInfo(0, "none");
}

GrpInfo::GrpInfo(const uint32_t id, string str) throw(GrpError)
{
	grpid = id;
	num_members = 0;
	num_true_som_subst = 0;
	num_true_het_subst = 0;
	num_true_som_indel = 0;
	num_true_het_indel = 0;
	matchingsample = 0;
	howmany = 0;
	Mptr vm(new Mutvec);
	mptr = move(vm);
	consensus = str;
}

void GrpInfo::update(const int arr[6])
{
	// [0] freq, [1] members, [2] pos, [3] type, [4] sub, [5] het
	unsigned char c = (arr[5] << 4) | arr[4];
	switch(c) {
		case 0x01:
			num_true_som_subst++;
			break;
		case 0x11:
			num_true_het_subst++;
			break;
		case 0x00:
			num_true_som_indel++;
			break;
		case 0x10:
			num_true_het_indel++;
			break;
		default:
			break;
	}
	const Mutation m = {(uint16_t)arr[0], (uint16_t)arr[2], (uint8_t)arr[3], c};
	mptr->push_back(m);
	if(num_members == 0) 
		num_members = arr[1];
}

uint32_t *GrpInfo::getgrpid()
{
	return &grpid;
}

string *GrpInfo::getgrcon()
{
	return &consensus;
}

size_t GrpInfo::getmutnum()
{
	return mptr->size();
}

string GrpInfo::muttostring(const Mutation &m) const
{
	stringstream ss;
	ss << (int)m.freq;
	ss << "|" << (int)m.pos;
	ss << "|" << (int)m.type;
	ss << "|" << (int)(m.hetero_subst & 0xf); // lo sub_nib 
	ss << "|" << (int)((m.hetero_subst >> 4) & 0xf); // hi het_nib;
	return ss.str();
}

string GrpInfo::grptostring() const
{
	stringstream ss;
	ss << (int)grpid;
	ss << "\t" << (int)num_members;
	ss << "\t" << (int)num_true_som_subst;
	ss << "\t" << (int)num_true_het_subst;
//	ss << "\t" << (int)num_true_som_indel;
//	ss << "\t" << (int)num_true_het_indel;
//	ss << "\t" << (int)num_true_het_indel;
//	ss << "\t" << (int)matchingsample;
//	ss << "\t" << (int)howmany;
	return ss.str();
}

void GrpInfo::print() const
{
	cout << grptostring() << "\t" << (int)mptr->size() << "\t";
	for (auto i = mptr->begin(); i != mptr->end(); ++i)
		cout << muttostring(*i) << "\t";
	cout << endl;
}
