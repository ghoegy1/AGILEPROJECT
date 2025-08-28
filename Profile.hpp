/***************************************************************************************
 * Profile.hpp
 * Feature 1: A student's profile containing name, Clemson email, courses, and availability.
 *
 * STANDARD LIBRARIES USED:
 *  <string>     : user name/email fields.
 *  <vector>     : course list and availability vector.
 *  <iostream>   : for printing helpers (declarations only; implemented in .cpp).
 ****************************************************************************************/
#pragma once
#include <string>
#include <vector>

namespace sb {

// Days of the week kept compact for availability.
enum class Day : int { Mon=0, Tue, Wed, Thu, Fri, Sat, Sun };
extern const char* kDayNames[7];

// Single availability window on a day: [start,end) minutes since midnight.
struct AvailabilitySlot {
    Day  day;
    int  start; // minutes since midnight
    int  end;   // minutes since midnight
};

// The student profile object (single user in this CLI prototype).
class Profile {
public:
    // Create/Reset fields (Feature 1). Returns true on success.
    void createOrReset(const std::string& name,
                       const std::string& email,
                       const std::vector<std::string>& coursesUpperDedup);

    // Accessors / modifiers
    const std::string& name()  const { return name_;  }
    const std::string& email() const { return email_; }
    const std::vector<std::string>& courses() const { return courses_; }
    std::vector<std::string>& coursesMutable() { return courses_; }

    const std::vector<AvailabilitySlot>& availability() const { return availability_; }
    std::vector<AvailabilitySlot>& availabilityMutable() { return availability_; }

    bool exists() const { return exists_; }
    void clearAvailability(); // utility used on reset

    // Pretty printers (to keep main lean)
    void printCourses() const;
    void printAvailability() const;
    void show() const;

private:
    std::string name_;
    std::string email_;
    std::vector<std::string> courses_;
    std::vector<AvailabilitySlot> availability_;
    bool exists_ = false;
};

} // namespace sb