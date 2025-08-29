/***************************************************************************************
 * test_availability_manager.cpp
 * Unit tests for Feature 3: sb::AvailabilityManager (add/merge/remove availability)
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>  : assertions for test expectations
 *  <iostream> : summary output
 *  <vector>   : slot comparisons if needed
 *  <string>   : (not strictly needed here but common)
 ****************************************************************************************/

 #include <cassert>
 #include <iostream>
 #include <vector>
 #include <string>
 
 #include "Profile.hpp"
 #include "AvailabilityManager.hpp"
 #include "CourseManager.hpp"
 #include "Utils.hpp"
 
 using namespace sb;
 
 int main() {
     {
         // Test 1: overlapping same-day slots merge into one
         Profile p;
         AvailabilityManager am;
 
         // Profile must exist
         auto norm = CourseManager::normalizeDedup({"CPSC 2150"});
         p.createOrReset("El", "el@clemson.edu", norm);
 
         // Add 10:00-12:00 and 11:00-13:00 on Monday → expect 10:00-13:00
         am.addAvailability(p, Day::Mon, 10*60, 12*60);
         am.addAvailability(p, Day::Mon, 11*60, 13*60);
 
         const auto& v = p.availability();
         assert(v.size() == 1);
         assert(v[0].day == Day::Mon);
         assert(v[0].start == 10*60);
         assert(v[0].end   == 13*60);
     }
 
     {
         // Test 2: invalid slot (end <= start) is rejected (no change)
         Profile p;
         AvailabilityManager am;
 
         auto norm = CourseManager::normalizeDedup({"MATH 1080"});
         p.createOrReset("El", "el@clemson.edu", norm);
 
         // Start with an existing valid slot
         am.addAvailability(p, Day::Tue, 8*60, 9*60);
         size_t before = p.availability().size();
 
         // Try to add invalid
         am.addAvailability(p, Day::Tue, 9*60, 9*60); // end == start
         am.addAvailability(p, Day::Tue, 10*60, 9*60); // end < start
 
         assert(p.availability().size() == before); // unchanged
     }
 
     {
         // Test 3: removeAvailability removes by 1-based index
         Profile p;
         AvailabilityManager am;
 
         auto norm = CourseManager::normalizeDedup({"ENGL 1030"});
         p.createOrReset("El", "el@clemson.edu", norm);
 
         // Add two non-overlapping Monday slots
         am.addAvailability(p, Day::Mon, 9*60, 10*60);   // [1]
         am.addAvailability(p, Day::Mon, 14*60, 15*60);  // [2]
 
         // Remove the second
         am.removeAvailability(p, 2);
 
         const auto& v = p.availability();
         assert(v.size() == 1);
         assert(v[0].day == Day::Mon);
         assert(v[0].start == 9*60);
         assert(v[0].end   == 10*60);
     }
 
     std::cout << "[test_availability_manager] All tests passed.\n";
     return 0;
 }