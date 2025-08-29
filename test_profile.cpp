/***************************************************************************************
 * test_profile.cpp
 * Unit tests for Feature 1: sb::Profile (create/reset profile & availability clear)
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>  : assertions for test expectations
 *  <iostream> : simple "all tests passed" output
 *  <vector>   : building course lists
 *  <string>   : passing name/email strings
 ****************************************************************************************/

 #include <cassert>
 #include <iostream>
 #include <vector>
 #include <string>
 
 #include "Profile.hpp"
 #include "CourseManager.hpp" // for normalizeDedup used during creation
 #include "Utils.hpp"
 
 using namespace sb;
 
 int main() {
     {
         // Test 1: createOrReset sets fields, exists() becomes true, availability cleared
         Profile p;
         std::vector<std::string> raw = {"cpsc 2150", "MATH 1080", "cpsc 2150"}; // dup on purpose
         auto norm = CourseManager::normalizeDedup(raw);
 
         p.createOrReset("El", "el@clemson.edu", norm);
 
         assert(p.exists() == true);
         assert(p.name() == "El");
         assert(p.email() == "el@clemson.edu");
         // normalized & deduped: {"CPSC 2150", "MATH 1080"}
         const auto& courses = p.courses();
         assert(courses.size() == 2);
         assert(courses[0] == "CPSC 2150");
         assert(courses[1] == "MATH 1080");
         assert(p.availability().empty());
     }
 
     {
         // Test 2: resetting profile replaces courses and clears previous availability
         Profile p;
         // First create:
         auto norm1 = CourseManager::normalizeDedup({"CPSC 2150"});
         p.createOrReset("A", "a@clemson.edu", norm1);
         // Add some availability directly:
         p.availabilityMutable().push_back({Day::Mon, 9*60, 10*60});
 
         // Reset with different courses:
         auto norm2 = CourseManager::normalizeDedup({"MATH 1080", "ENGL 1030"});
         p.createOrReset("A", "a@clemson.edu", norm2);
 
         assert(p.exists());
         assert(p.courses().size() == 2);
         assert(p.courses()[0] == "MATH 1080");
         assert(p.courses()[1] == "ENGL 1030");
         // Availability should be cleared on reset
         assert(p.availability().empty());
     }
 
     {
         // Test 3: clearAvailability wipes availability but keeps profile & courses
         Profile p;
         auto norm = CourseManager::normalizeDedup({"CPSC 2310"});
         p.createOrReset("B", "b@clemson.edu", norm);
         p.availabilityMutable().push_back({Day::Tue, 600, 660});
         p.clearAvailability();
 
         assert(p.exists());
         assert(p.courses().size() == 1);
         assert(p.availability().empty()); // cleared
     }
 
     std::cout << "[test_profile] All tests passed.\n";
     return 0;
 }