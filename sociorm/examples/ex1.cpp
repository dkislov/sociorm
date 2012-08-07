#include <soci.h>
#include <sqlite3/soci-sqlite3.h>

#include <sociorm/orm.hpp>

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
        soci::orm::orm orm(sqlite3, "service=mydb user=john password=secret");
        orm.map_class<phonebook>("phonebook");

    }
    catch (exception const &e)
    {
        cerr << "Error: " << e.what() << '\n';
    }
}
