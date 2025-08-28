/***************************************************************************************
 * main.cpp — Integrates the three features into a CLI.
 *
 * FEATURE ↔ CLASS MAPPING
 *  (1) Create a profile with enrolled courses  → sb::Profile (+ sb::CourseManager::normalizeDedup)
 *  (2) Update profile to add/remove courses    → sb::CourseManager
 *  (3) Set availability (+ remove)             → sb::AvailabilityManager
 *
 * STANDARD LIBRARIES USED:
 *  <iostream>   : CLI I/O (menus, feedback).
 *  <string>     : user input.
 ****************************************************************************************/
#include <iostream>
#include <string>

#include "Utils.hpp"
#include "Profile.hpp"
#include "CourseManager.hpp"
#include "AvailabilityManager.hpp"

using namespace sb;

static void printMainMenu() {
    std::cout << "================ Study Buddy (CLI) ================\n"
              << "1) Create/Reset Profile (name/email/courses)\n"
              << "2) View Profile\n"
              << "3) Add Courses\n"
              << "4) Remove Courses\n"
              << "5) Add Availability\n"
              << "6) Remove Availability\n"
              << "7) Exit\n";
}

int main() {
    Profile me;                // Single-user profile instance (prototype).
    CourseManager courses;     // Feature 2.
    AvailabilityManager avail; // Feature 3.

    while (true) {
        printMainMenu();
        int choice = promptIntInRange("Choose an option [1-7]: ", 1, 7);

        if (choice == 1) {
            // Feature 1: Create/Reset Profile
            std::cout << "\n--- Create/Reset Profile ---\n";
            std::cout << "Enter your name (or leave blank): ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string name = trim(safeGetLine());

            std::cout << "Enter your Clemson email (or leave blank): ";
            std::string email = trim(safeGetLine());

            std::cout << "Enter enrolled courses (comma-separated, e.g., CPSC 2150, MATH 1080):\n> ";
            std::string raw = safeGetLine();
            auto tokens = split(raw, ',');
            auto normalized = CourseManager::normalizeDedup(tokens);

            me.createOrReset(name, email, normalized);
            std::cout << "Profile created/reset successfully.\n";
            me.show();
        }
        else if (choice == 2) {
            me.show();
        }
        else if (choice == 3) {
            // Feature 2: Add courses
            if (!me.exists()) { std::cout << "Create a profile first.\n\n"; continue; }
            std::cout << "Enter course codes to ADD (comma-separated):\n> ";
            std::string line = trim(safeGetLine());
            courses.addCourses(me, line);
            std::cout << "\n";
        }
        else if (choice == 4) {
            // Feature 2: Remove courses
            if (!me.exists()) { std::cout << "Create a profile first.\n\n"; continue; }
            std::cout << "Your current courses:\n";
            me.printCourses();
            std::cout << "Enter indices to REMOVE (comma-separated), e.g., 2,4 :\n> ";
            std::string line = trim(safeGetLine());
            courses.removeCourses(me, line);
            std::cout << "\n";
        }
        else if (choice == 5) {
            // Feature 3: Add availability
            if (!me.exists()) { std::cout << "Create a profile first.\n\n"; continue; }
            Day d = AvailabilityManager::promptDay();
            std::cout << "Start time (24h HH:MM): ";
            std::string s1 = trim(safeGetLine());
            int startMin = 0;
            if (!parseHHMM(s1, startMin)) { std::cout << "Invalid start time.\n\n"; continue; }

            std::cout << "End time (24h HH:MM): ";
            std::string s2 = trim(safeGetLine());
            int endMin = 0;
            if (!parseHHMM(s2, endMin)) { std::cout << "Invalid end time.\n\n"; continue; }

            avail.addAvailability(me, d, startMin, endMin);
            std::cout << "\n";
        }
        else if (choice == 6) {
            // Feature 3: Remove availability
            if (!me.exists()) { std::cout << "Create a profile first.\n\n"; continue; }
            std::cout << "Current availability:\n";
            AvailabilityManager::listIndexed(me);
            std::cout << "Enter an index to remove (or 0 to cancel): ";
            int idx;
            if (!(std::cin >> idx)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input.\n\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (idx == 0) { std::cout << "Cancelled.\n\n"; continue; }
            avail.removeAvailability(me, idx);
            std::cout << "\n";
        }
        else if (choice == 7) {
            std::cout << "Goodbye!\n";
            return 0;
        }
    }
}