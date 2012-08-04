#include <soci.h>
#include <sqlite3/soci-sqlite3.h>

#include "orm.hpp"

#include <iostream>
#include <string>
#include <exception>

using namespace soci;
using namespace std;

class phonebook
{
public:
	template<typename Action>
	void persist(Action& a)
	{
		orm::field(a, "name", name_);
		orm::field(a, "phone", phone_);		
	}

private:
	string name_;
	string phone_;
};

bool getName(string &name)
{
    cout << "Enter name: ";
    return cin >> name;
}

int main()
{
    try
    {
        session sql(sqlite3, "service=mydb user=john password=secret");
		sql.once << "drop table if exists phonebook";
		sql.once << "create table phonebook(id integer primary key, name varchar(20))";
    }
    catch (exception const &e)
    {
        cerr << "Error: " << e.what() << '\n';
    }
}
