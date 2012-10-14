#include <soci.h>
#include <sqlite3/soci-sqlite3.h>

#include <sociorm/orm.hpp>

#include <iostream>
#include <string>
#include <exception>

using namespace soci;
using namespace std;

class phonebook : public orm::base
{
public:
    phonebook() {}
    phonebook(const char* name, const char* phone) : name_(name), phone_(phone) {}

    static const char* table_name() 
    {
        return "phonebook"; 
    }

	template<typename Action>
	void persist(Action& a)
	{
		a.field("name", name_);
		a.field("phone", phone_);		
	}

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
        orm::orm s(sqlite3, "mydb.sqlite3");

        phonebook book[] = { phonebook("taras", "3123"), phonebook("julia", "32133") };

        s.session().once << "create temp table phonebook (name varchar(20) primary key, phone varchar(20) )";

        s.save(book[0]);
        s.save(book[1]);

        rowset<phonebook> rs = (s.session().prepare << "select * from phonebook");

        vector<phonebook> v(rs.begin(), rs.end());
        return 0;
    }
    catch (exception const &e)
    {
        cerr << "Error: " << e.what() << '\n';
    }
}
