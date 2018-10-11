#include "mime_types.h"
#include <algorithm>
#include <array>

namespace http {
namespace server {
namespace mime_types {

struct mapping
{
    const char* extension;
    const char* mime_type;
};

std::array<mapping, 5> mappings{mapping{ "gif", "image/gif" },
                                mapping{ "htm", "text/html" },
                                mapping{ "html", "text/html" },
                                mapping{ "jpg", "image/jpeg" },
                                mapping{ "png", "image/png" }};

std::string extension_to_type(const std::string& extension)
{
    auto iter = std::find_if(mappings.begin(), mappings.end(), [&extension](const mapping& m)
    {
        return m.extension == extension;
    });

    if(iter != mappings.end())
        return iter->mime_type;

    return "text/plain";
}

} // namespace mime_types
} // namespace server
} // namespace http
