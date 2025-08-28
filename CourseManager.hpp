/***************************************************************************************
 * CourseManager.hpp
 * Feature 2: Add/Remove courses on a Profile.
 *
 * STANDARD LIBRARIES USED:
 *  <string>     : for raw user input strings.
 *  <vector>     : tokenized course inputs.
 *  <algorithm>  : std::find, std::sort, std::unique for de-duplication.
 *  <iostream>   : user feedback.
 ****************************************************************************************/
#pragma once
#include <string>

namespace sb {

class Profile;

class CourseManager {
public:
    // Add courses from a comma-separated string (normalized to UPPER).
    // Skips duplicates. Prints a summary to std::cout.
    void addCourses(Profile& prof, const std::string& commaSeparated);

    // Remove courses by 1-based indices (comma-separated).
    // Ignores invalid indices; prints a summary to std::cout.
    void removeCourses(Profile& prof, const std::string& commaSeparated);

    // Utility used when creating/resetting a profile: normalize + de-dup input
    // (UPPERCASE, remove empty, preserve first occurrence order).
    static std::vector<std::string> normalizeDedup(const std::vector<std::string>& raw);
};

} // namespace sb