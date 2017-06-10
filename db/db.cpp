// db.cpp
#include "db.h"
#include <cstdio>
#include <cstring>
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
	return EXIT_SUCCESS; // 0
}

void Db::make_uberconlist(stringstream &ss)
{
	int bytes;
	const unsigned char * text;
	bytes = sqlite3_column_int(stmt, 0);
	text  = sqlite3_column_text(stmt, 1);
	ss << bytes << " " << text << "\n";
}

/*
void make_list(void (*p)(stringstream &), stringstream &ss)
{
	(*p)(ss);
}
*/

int Db::retrieve(const char *sql, stringstream &ss)
{
	Fptr ptr = &Db::make_uberconlist;
	sqlite3_prepare(db, sql, strlen(sql)+1, &stmt, NULL);
	int row = 0;
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