/***************************************************************************************
 * ClassmateSearch.hpp
 * Feature: Search classmates by course OR by name (substring, case-insensitive).
 *
 * STANDARD LIBRARIES USED:
 *  <vector>    : results
 *  <string>    : query input
 *  <algorithm> : std::find_if, std::search
 ****************************************************************************************/
#pragma once
#include "Profile.hpp"
#include <vector>
#include <string>

namespace sb {

class ClassmateSearch {
public:
    // Returns classmates (excluding self) enrolled in 'courseCode' (case-insensitive).
    static std::vector<const Profile*> byCourse(const std::vector<Profile>& all,
                                                const Profile& self,
                                                const std::string& courseCode);

    // Returns classmates (excluding self) whose name CONTAINS 'nameSubstr' (case-insensitive).
    // If a classmate doesn't have a name, email is used for matching instead.
    static std::vector<const Profile*> byName(const std::vector<Profile>& all,
                                              const Profile& self,
                                              const std::string& nameSubstr);

private:
    static bool isSelf(const Profile& a, const Profile& b);
    static bool hasCourse(const Profile& p, const std::string& normalized);
    static bool icontains(const std::string& hay, const std::string& needle);
};

} // namespace sb