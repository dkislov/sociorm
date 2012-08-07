#pragma once

#include <soci.h>

#include <vector>
#include <string>

namespace soci { namespace orm { namespace detail {

struct field_info
{
    int type_;
    const char* column_name_;
    bool is_pk_;
    bool is_fk_;
};

struct class_info 
{
    std::string table_;
    std::vector<field_info> fields_;

    /// \name Precompiled statements
    statement st_insert_;   //!< Insert single record
    statement st_update_;   //!< Update single record
    statement st_remove_;   //!< Remove single record
    statement st_select_;   //!< Read single record
};

}}}
