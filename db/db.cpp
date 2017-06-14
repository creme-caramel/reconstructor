// db.cpp
#include "db.h"
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;
using Fptr = void (Db::*)(stringstream &);

int callback(void *data, int argc, char **argv, char **azcolname)
{
	int i;
	for(i = 0; i < argc; i++) {
		printf("%s:%s ", azcolname[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return SQLITE_OK;
}

Db::Db() {}

Db::Db(const char *str) throw(Db::DbError)
{
	if(!str)
		throw DbError("null dbname");
	init(str);
}

Db::~Db()
{
	sqlite3_close(db);
}

int Db::init(const char *str)
{
	errmsg = 0;
	if(sqlite3_open(str, &db) != SQLITE_OK) {
		fprintf(stderr, "sqlite err: %s\n", sqlite3_errmsg(db));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void Db::make_uberconlist(stringstream &ss)
{
	int i0;
	const unsigned char * t1;
	i0 = sqlite3_column_int(stmt, 0);
	t1  = sqlite3_column_text(stmt, 1);
	ss << i0 << " " << t1 << "\n";
}

void Db::make_lanemidlist(stringstream &ss)
{
	int i0, i1, i2;
	i0 = sqlite3_column_int(stmt, 0);
	i1 = sqlite3_column_int(stmt, 1);
	i2 = sqlite3_column_int(stmt, 2);
	ss << i0 << " " << i1 << " " << i2 << "\n";
}

int Db::retrieve(const string &sql, stringstream &ss, int whichlist)
{
	Fptr ptr;
	switch(whichlist) {
	case 0:
		ptr = &Db::make_uberconlist;	
		break;
	case 1:
		ptr = &Db::make_lanemidlist;	
		break;
	default:
		break;
	}

	const char *cmd = sql.c_str();
	sqlite3_prepare(db, cmd, strlen(cmd)+1, &stmt, NULL);
	int row = 0; // for testing
	while(1) {
		int s;
		s = sqlite3_step(stmt);
		if(s == SQLITE_ROW) {
			row++;					
			(this->*ptr)(ss);		
		} else if(s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "failed\n");
			return EXIT_FAILURE;
		}
	}
	sqlite3_finalize(stmt);
	return EXIT_SUCCESS;
}

int Db::retrieve(const string &sql, string &str)
{
	const char *cmd = sql.c_str();
	sqlite3_prepare(db, cmd, strlen(cmd)+1, &stmt, NULL);
	int row = 0; // for testing
	while(1) {
		int s;
		s = sqlite3_step(stmt);
		if(s == SQLITE_ROW) {
			row++;					
			const unsigned char * t;
			t  = sqlite3_column_text(stmt, 0);
			string temp(reinterpret_cast<const char*>(t));
			str = temp;
		} else if(s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "failed\n");
			return EXIT_FAILURE;
		}
	}
	sqlite3_finalize(stmt);
	return EXIT_SUCCESS;
}


int Db::exec_old(char *sql, void *msg)
{
	int rc;
	rc = sqlite3_exec(db, sql, callback, msg, &errmsg);
	if(rc) {
		fprintf(stderr, "sqlite err: %s\n", errmsg);
		sqlite3_free(errmsg);
	}
	return rc;
}