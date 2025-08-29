/***************************************************************************************
 * MatchSuggester.hpp
 * Feature: Suggest study partners using shared courses + overlapping availability.
 *
 * STANDARD LIBRARIES USED:
 *  <vector>    : collections of profiles and matches
 *  <string>    : course codes & names
 *  <algorithm> : set_intersection (manual), sort
 ****************************************************************************************/
#pragma once
#include "Profile.hpp"
#include <vector>
#include <string>

namespace sb {

struct Match {
    const Profile* person;                  // pointer to matched profile
    std::vector<std::string> sharedCourses; // uppercased
    int overlapMinutes;                     // total overlap across the week
};

class MatchSuggester {
public:
    // Suggest up to maxResults partners from 'all' (excluding 'self') who share at least
    // one course AND have at least minOverlapMinutes of overlapping availability (any days).
    // Results are sorted by overlapMinutes DESC, then by name ASC.
    std::vector<Match> suggest(const Profile& self,
                               const std::vector<Profile>& all,
                               int minOverlapMinutes = 30,
                               std::size_t maxResults = 5) const;

private:
    static bool sameUser(const Profile& a, const Profile& b);
    static std::vector<std::string> sharedCoursesUpper(const Profile& a, const Profile& b);
    static int totalOverlapMinutes(const Profile& a, const Profile& b);
};

} // namespace sb