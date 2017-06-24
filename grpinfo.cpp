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

uint32_t *GrpInfo::getgrpid()
{
	return &grpid;
}

string *GrpInfo::getgrcon()
{
	return &consensus;
}

int GrpInfo::getmutnum()
{
	return mptr->size();
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

string GrpInfo::muttostring(const Mutation &m) const
{
	stringstream ss;
	int sub_nib = m.hetero_subst & 0xf; // lo
	int het_nib = (m.hetero_subst >> 4) & 0xf; // hi
	ss << sub_nib << het_nib;
	return ss.str();
}

string GrpInfo::grptostring() const
{
	stringstream ss;
	ss << num_members << " ";
	ss << num_true_som_subst << " ";
	ss << num_true_het_subst << " ";
	ss << num_true_som_indel << " ";
	ss << num_true_het_indel << " ";
	ss << num_true_het_indel << " ";
	ss << matchingsample << " ";
	ss << howmany << " ";
	return ss.str();
}

void GrpInfo::print() const
{
	cout << grptostring() << endl;
	for (auto i = mptr->begin(); i != mptr->end(); ++i)
		cout << muttostring(*i) << " ";
	cout << endl;
}
