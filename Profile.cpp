/***************************************************************************************
 * Profile.cpp — implementation
 ****************************************************************************************/
#include "Profile.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <iostream>

namespace sb {

const char* kDayNames[7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

void Profile::createOrReset(const std::string& name,
                            const std::string& email,
                            const std::vector<std::string>& coursesUpperDedup) {
    // Set simple fields
    name_ = name;
    email_ = email;
    // Replace courses with normalized & de-duplicated list provided by caller.
    courses_ = coursesUpperDedup;
    // Reset availability to avoid stale windows from a previous profile.
    availability_.clear();
    exists_ = true;
}

void Profile::clearAvailability() {
    availability_.clear();
}

void Profile::printCourses() const {
    if (courses_.empty()) {
        std::cout << "  (no courses yet)\n";
        return;
    }
    for (size_t i = 0; i < courses_.size(); ++i) {
        std::cout << "  [" << (i+1) << "] " << courses_[i] << "\n";
    }
}

void Profile::printAvailability() const {
    if (availability_.empty()) {
        std::cout << "  (no availability yet)\n";
        return;
    }
    Day current = static_cast<Day>(-1);
    for (const auto& s : availability_) {
        if (current != s.day) {
            current = s.day;
            std::cout << "  " << kDayNames[static_cast<int>(s.day)] << ": ";
        } else {
            std::cout << "                   ";
        }
        std::cout << formatHHMM(s.start) << " - " << formatHHMM(s.end) << "\n";
    }
}

void Profile::show() const {
    if (!exists_) {
        std::cout << "No profile exists yet. Choose '1' to create one.\n";
        return;
    }
    std::cout << "\n==== Your Profile ====\n";
    std::cout << "Name   : " << (name_.empty() ? "(not set)" : name_) << "\n";
    std::cout << "Email  : " << (email_.empty() ? "(not set)" : email_) << "\n";
    std::cout << "Courses:\n";
    printCourses();
    std::cout << "Availability:\n";
    printAvailability();
    std::cout << "======================\n\n";
}

} // namespace sb