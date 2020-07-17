// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef utils_h
#define utils_h

#include <string>
#include <vector>
#include <set>

namespace Sky {
    std::string escape(const std::string& str);
    std::string unescape(const std::string& str);

    template<typename T> T unique(const T& values) {
        std::set<typename T::value_type> set;
        set.insert(values.begin(), values.end());
        T result;
        result.assign(set.begin(), set.end());
        return result;
    }
}

#endif