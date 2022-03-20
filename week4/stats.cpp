#include "stats.h"

void Stats::AddMethod(string_view method)
{
    for (const auto &valid_method : valid_methods_)
    {
        if (valid_method == method)
        {
            methods_qty[method]++;
            return;
        }
    }

    // not valid method
    methods_qty[not_valid_method_]++;
}

void Stats::AddUri(string_view uri)
{
    for (const auto &valid_uri : valid_uries_)
    {
        if (valid_uri == uri)
        {
            uri_qty[uri]++;
            return;
        }
    }

    // not valid uri
    uri_qty[not_valid_uri_]++;
}

const map<string_view, int>& Stats::GetMethodStats() const
{
    return methods_qty;
}

const map<string_view, int>& Stats::GetUriStats() const
{
    return uri_qty;
}

HttpRequest ParseRequest(string_view line)
{
    HttpRequest req;
    line.remove_prefix(min(line.find_first_not_of(" "), line.size()));

    auto end_pos_method = line.find(' ');
    req.method = line.substr(0,end_pos_method);

    auto end_pos_uri = line.find(' ', end_pos_method + 1);
    req.uri = line.substr(end_pos_method + 1,end_pos_uri - end_pos_method - 1);

    req.protocol = line.substr(end_pos_uri + 1);

    return req;
}