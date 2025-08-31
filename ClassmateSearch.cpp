/***************************************************************************************
 * ClassmateSearch.cpp — implementation
 ****************************************************************************************/
#include "ClassmateSearch.hpp"
#include "Utils.hpp"
#include <algorithm>

namespace sb {

bool ClassmateSearch::isSelf(const Profile& a, const Profile& b) {
    if (!a.email().empty() && !b.email().empty()) return a.email() == b.email();
    if (!a.name().empty() && !b.name().empty()) return a.name() == b.name();
    return &a == &b;
}

bool ClassmateSearch::hasCourse(const Profile& p, const std::string& normalized) {
    for (const auto& c : p.courses()) if (c == normalized) return true;
    return false;
}

bool ClassmateSearch::icontains(const std::string& hay, const std::string& needle) {
    if (needle.empty()) return true;
    std::string H = hay, N = needle;
    H = upperCopy(H);
    N = upperCopy(N);
    return std::search(H.begin(), H.end(), N.begin(), N.end()) != H.end();
}

std::vector<const Profile*> ClassmateSearch::byCourse(const std::vector<Profile>& all,
                                                      const Profile& self,
                                                      const std::string& courseCode) {
    std::vector<const Profile*> out;
    std::string normalized = upperCopy(trim(courseCode));
    for (const auto& p : all) {
        if (isSelf(p, self)) continue;
        if (hasCourse(p, normalized)) out.push_back(&p);
    }
    return out;
}

std::vector<const Profile*> ClassmateSearch::byName(const std::vector<Profile>& all,
                                                    const Profile& self,
                                                    const std::string& nameSubstr) {
    std::vector<const Profile*> out;
    for (const auto& p : all) {
        if (isSelf(p, self)) continue;
        std::string key = p.name().empty() ? p.email() : p.name();
        if (icontains(key, nameSubstr)) out.push_back(&p);
    }
    return out;
}

} // namespace sb