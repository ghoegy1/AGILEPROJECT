/***************************************************************************************
 * AvailabilityBrowser.hpp
 * Feature: Browse classmates’ availability (filter by course, optional day).
 *
 * STANDARD LIBRARIES USED:
 *  <vector>    : to return lists of classmates and their slots
 *  <string>    : course code & names
 *  <utility>   : std::pair for (name, slots)
 *  <algorithm> : find, transform
 ****************************************************************************************/
#pragma once
#include "Profile.hpp"
#include <vector>
#include <string>
#include <utility>

namespace sb {

class AvailabilityBrowser {
public:
    // Return (name, slots) for all classmates (excluding self) who are enrolled in courseCode.
    // courseCode matching is case-insensitive; internally normalized to UPPER.
    static std::vector<std::pair<std::string, std::vector<AvailabilitySlot>>>
    browseByCourse(const std::vector<Profile>& all, const Profile& self, const std::string& courseCode);

    // Same as above, but filter slots to a particular Day.
    static std::vector<std::pair<std::string, std::vector<AvailabilitySlot>>>
    browseByCourseAndDay(const std::vector<Profile>& all, const Profile& self,
                         const std::string& courseCode, Day day);

private:
    static bool sameUser(const Profile& a, const Profile& b);
    static bool hasCourse(const Profile& p, const std::string& normalizedCourse);
};

} // namespace sb