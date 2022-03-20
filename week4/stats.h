#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <array>
using namespace std;

class Stats
{
public:
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;

private:
    map<string_view, int> methods_qty = {{"DELETE", 0},
                                         {"GET", 0},
                                         {"PUT", 0},
                                         {"POST", 0},
                                         {"UNKNOWN", 0}};

    map<string_view, int> uri_qty = {{"/", 0},
                                     {"/order", 0},
                                     {"/product", 0},
                                     {"/basket", 0},
                                     {"/help", 0},
                                     {"unknown", 0}};

    const array<string_view, 4> valid_methods_ =
            {"GET", "POST", "PUT", "DELETE"};

    const array<string_view, 5> valid_uries_ =
            {"/", "/order", "/product", "/basket", "/help"};

    const string_view not_valid_method_ = "UNKNOWN";
    const string_view not_valid_uri_ = "unknown";
};

HttpRequest ParseRequest(string_view line);
