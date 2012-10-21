#include <soci.h>
#include <sqlite3/soci-sqlite3.h>

#include <sociorm/orm.hpp>
#include <sociorm/serial.hpp>

#include <iostream>
#include <string>
#include <exception>

using namespace soci;
using namespace std;

struct notebook : public orm::base<orm::serial>
{
    template<typename Action>
    void persist(Action& a)
    {
        a.primary_key("id");
        a.field("first_name", first_name_);
        a.field("last_name", last_name_);
    }

    string first_name_;
    string last_name_;
};

struct phonebook : public notebook
{
    phonebook() {}

    phonebook(const char* first_name, const char* last_name, const char* phone)
        : phone_(phone)
    {
        first_name_ = first_name;
        last_name_ = last_name;
    }

	template<typename Action>
	void persist(Action& a)
	{
        a.derived_from<notebook>("id");
		a.field("phone", phone_);		
	}

	string phone_;
};

bool getName(string &name)
{
    cout << "Enter name: ";
    return (cin >> name) == nullptr;
}

int main()
{
    try
    {   
        orm::manager s(sqlite3, "mydb.sqlite3");

        phonebook book[] = { phonebook("taras", "kozlov", "3123"), phonebook("julia", "dudnikova", "32133") };

        s.session().once << 
            "create temp table notebook ( "
                "id integer primary key autoincrement, "
                "first_name varchar(20), "
                "last_name varchar(20) )";

        s.session().once << 
            "create temp table phonebook ( "
            "id integer primary key, "
            "phone varchar(20), "
            "foreign key(id) references notebook(id) on delete cascade)";

        s.map_class<notebook>("notebook");
        s.map_class<phonebook>("phonebook");

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
