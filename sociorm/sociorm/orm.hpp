#pragma once

#include <soci.h>

#include <sociorm/ptr.hpp>
#include <sociorm/query.hpp>

#include <string>
#include <memory>

namespace soci { namespace orm {

class class_info 
{
};

class orm
{
public:
    orm(soci::session& s);
    orm(backend_factory const & factory, std::string const & connect_string);
    orm(std::string const & backendName, std::string const & connect_string);
    explicit orm(std::string const & connectString);
    explicit orm(connection_pool & pool);

    /// \brief Return underlying soci session.
    session& session();

    /// \brief Setup mapping between class and database table
    template<typename Class>
    void map_class(const char* table_name);

    /// \brief Create or update database schema
    template<typename Class>
    void update_schema();

    /// \brief Insert new object to database.
    template<typename Class>
    void add(Class&& obj);

    /// \brief Insert new object to database and return reference to created object.
    template<typename Class>
    ptr<Class> add_getptr(Class&& obj);

    /// \brief Find new object in database.
    ///
    /// Return proxy object that can be implicitly converted to ptr<Class> or collection<ptr<Class>>. 
    /// Implicit conversion make proxy to invoke sql query.
    ///
    /// \code
    ///
    /// // Query first object that satisfies conditions
    /// ptr<User> joe = orm.query<User>().where("name = ?").bind("Joe");
    ///
    /// // The same as above
    /// ptr<User> joe = orm.query<ptr<User>>().where("name = ?").bind("Joe");
    /// 
    /// // Query all objects satisfying conditions, iterate over them
    /// typedef collection<ptr<User>> Users;
    /// 
    /// Users users = session.query<User>();
    /// 
    /// std::cerr << "We have " << users.size() << " users:" << std::endl;
    /// 
    /// for(ptr<User> user : users) // c++11 range based for
    /// {
    ///     std::cerr << " user " << user->name
    ///	              << " with karma of " << user->karma << std::endl;
    /// }
    /// 
    /// // Make non object query
    /// int count = orm.query<int>("select count(1) from user").where("name = ?").bind("Joe");
    ///
    /// \endcode
    template<typename Class>
    query<Class> query();

private:
    struct smart_deleter 
    {
        smart_deleter(bool del) : del_(del) {}

        void operator()(soci::session* s)
        {
            if (del_) 
                delete s;
        }

    private:
        bool del_;
    };

    std::unique_ptr<soci::session, smart_deleter> session_;
    std::map<std::type_info, class_info> class_map_;
};

inline orm::orm(soci::session& s)
    : session_(&s, smart_deleter(false))
{    
}

inline orm::orm(backend_factory const & factory, std::string const & connect_string)
    : session_(new soci::session(factory, connect_string), smart_deleter(true))
{
}

inline orm::orm(std::string const & backend_name, std::string const & connect_string)
    : session_(new soci::session(backend_name, connect_string), smart_deleter(true))
{
}

inline orm::orm(std::string const & connect_string)
    : session_(new soci::session(connect_string), smart_deleter(true))
{
}

inline orm::orm(connection_pool & pool)
    : session_(new soci::session(pool), smart_deleter(true))
{
}

inline session& orm::session()
{
    return *session_;
}

template<typename Class>
void orm::map_class(const char* table_name)
{

    class_map_[typeid(Class)] = 
}

/// \brief Override this to make type persistable non-intrusively.
template<typename Action, typename Type>
void persist(Action& a, Type& obj)
{
	obj.persist(a);
}

}}

