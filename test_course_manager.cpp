/***************************************************************************************
 * test_course_manager.cpp
 * Unit tests for Feature 2: sb::CourseManager (add/remove courses, normalization)
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>  : assertions for test expectations
 *  <iostream> : simple output when all tests pass
 *  <vector>   : building course lists
 *  <string>   : raw input strings
 ****************************************************************************************/

 #include <cassert>
 #include <iostream>
 #include <vector>
 #include <string>
 
 #include "Profile.hpp"
 #include "CourseManager.hpp"
 #include "Utils.hpp"
 
 using namespace sb;
 
 int main() {
     {
         // Test 1: addCourses adds unique courses in uppercase, ignores duplicates
         Profile p;
         CourseManager cm;
 
         // Create a base profile so CourseManager methods will operate
         auto norm = CourseManager::normalizeDedup({"CPSC 2150"});
         p.createOrReset("El", "el@clemson.edu", norm);
 
         // Try adding: a duplicate, and two new courses (mixed case)
         cm.addCourses(p, "cpsc 2150, Math 1080, ENGL 1030");
 
         const auto& cs = p.courses();
         // Expected: ["CPSC 2150", "MATH 1080", "ENGL 1030"]
         assert(cs.size() == 3);
         assert(cs[0] == "CPSC 2150");
         assert(cs[1] == "MATH 1080");
         assert(cs[2] == "ENGL 1030");
     }
 
     {
         // Test 2: removeCourses removes by 1-based indices (ignore invalid)
         Profile p;
         CourseManager cm;
 
         auto norm = CourseManager::normalizeDedup({"CPSC 2150", "MATH 1080", "ENGL 1030"});
         p.createOrReset("El", "el@clemson.edu", norm);
 
         // Remove indices "2, 99, 1" → should remove MATH 1080 and then CPSC 2150
         cm.removeCourses(p, "2, 99, 1");
 
         const auto& cs = p.courses();
         // Remaining should be only "ENGL 1030"
         assert(cs.size() == 1);
         assert(cs[0] == "ENGL 1030");
     }
 
     {
         // Test 3: normalizeDedup uppercases, trims, de-dupes, and preserves first occurrence order
         std::vector<std::string> raw = {"  cpsc 2150 ", "MATH 1080", "cPsC 2150", " ENGL 1030 "};
         auto norm = CourseManager::normalizeDedup(raw);
 
         assert(norm.size() == 3);
         assert(norm[0] == "CPSC 2150");
         assert(norm[1] == "MATH 1080");
         assert(norm[2] == "ENGL 1030");
     }
 
     std::cout << "[test_course_manager] All tests passed.\n";
     return 0;
 }