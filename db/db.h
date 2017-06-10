#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <sstream>
#include <stdexcept>

class Db {
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *errmsg;

	int init(const char *);
	void make_uberconlist(std::stringstream &);
	//void make_list(void (*ptr)(std::stringstream &), std::stringstream &);
public:
	struct DbError : public std::logic_error {
		DbError(const std::string &msg = "") : std::logic_error(msg) {}
	};

	Db();
	Db(const char *) throw(DbError);
	~Db();

	int retrieve(const char *, std::stringstream &ss);
	int exec_old(char *, void *);
	friend int callback(void *, int, char **, char **);
};

#endif /* DB_H */
