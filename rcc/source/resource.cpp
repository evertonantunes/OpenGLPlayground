#include "resource.hpp"

#include <algorithm>
#include <vector>

namespace
{
    struct Entry
    {
        std::string_view path;
        const unsigned char *data;
        std::size_t size;
    };

    static std::vector<Entry> s_entry;
}

extern bool add_resource( const std::string_view path, const unsigned char *data, const std::size_t size )
{
    s_entry.push_back(Entry{path, data, size});
    return true;
}

std::string_view application::resource::get(const std::string_view &path)
{
    const auto it = std::find_if(begin(s_entry), end(s_entry), [&path]( const auto &item )
    {
        return item.path == path;
    });

    if (it != end(s_entry))
    {
        return {reinterpret_cast<const char*>(it->data), it->size};
    }

    return {};
}
