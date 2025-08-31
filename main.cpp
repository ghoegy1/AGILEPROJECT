/***************************************************************************************
 * main.cpp — Study Buddy (CLI) — ALL FEATURES WIRED
 *
 * This demo keeps everything in-memory:
 *  - "Me" (your profile) + a vector<Profile> "roster" (classmates)
 *  - NotificationCenter for inboxes
 *  - SessionRequests for sending/confirming/canceling sessions
 *
 * STANDARD LIBRARIES USED:
 *  <iostream>  : CLI I/O
 *  <string>    : input text
 *  <vector>    : roster of classmates, course lists
 *  <limits>    : input flushing
 *  <algorithm> : simple searches/sorts where needed
 *
 * MODULES USED (your headers):
 *  Utils.hpp, Profile.hpp, CourseManager.hpp, AvailabilityManager.hpp
 *  AvailabilityEditor.hpp, AvailabilityBrowser.hpp, MatchSuggester.hpp
 *  ClassmateSearch.hpp, NotificationCenter.hpp, SessionRequests.hpp, CalendarView.hpp
 *
 * Notes:
 *  - Identity uses email primarily (fallback to name if email blank).
 *  - Times are minutes since midnight; format "HH:MM" for input/output.
 *  - This is a CLI demo; persistence (save/load) is not included yet.
 ****************************************************************************************/

 #include <iostream>
 #include <string>
 #include <vector>
 #include <limits>
 #include <algorithm>
 
 #include "Utils.hpp"
 #include "Profile.hpp"
 #include "CourseManager.hpp"
 #include "AvailabilityManager.hpp"
 #include "AvailabilityEditor.hpp"
 #include "AvailabilityBrowser.hpp"
 #include "MatchSuggester.hpp"
 #include "ClassmateSearch.hpp"
 #include "NotificationCenter.hpp"
 #include "SessionRequests.hpp"
 #include "CalendarView.hpp"
 
 using namespace sb;
 
 /* -------------------------- Small CLI helpers -------------------------- */
 
 static void pauseEnter() {
     std::cout << "Press Enter to continue...";
     std::string dummy;
     std::getline(std::cin, dummy);
 }
 
 static void printMainMenu() {
     std::cout <<
 R"(==================== Study Buddy (CLI) ====================
 1)  Create/Reset MY Profile (name/email/courses)
 2)  View MY Profile
 3)  Add Courses (ME)
 4)  Remove Courses (ME)
 5)  Add Availability (ME)
 6)  Remove Availability (ME)
 7)  Edit Availability (ME)
 
 ------ Classmates (Roster) ------
 8)  Add a Classmate Profile (for testing)
 9)  List Classmates (names/emails/courses)
 
 ------ Search / Browse ------
 10) Search Classmates by Course
 11) Search Classmates by Name
 12) Browse Classmates’ Availability by Course
 13) Browse Classmates’ Availability by Course & Day
 
 ------ Requests / Notifications / Calendar ------
 14) Send Study Session Request
 15) View MY Pending Requests
 16) Confirm a Pending Request (ME)
 17) View MY Notifications (fetch & clear)
 18) View MY Calendar (confirmed sessions)
 19) Cancel a Confirmed Session (ME)
 
 ------ Match Suggestions ------
 20) Suggest Study Partners (shared courses + overlap)
 
 0)  Exit
 )";
 }
 
 /* Prompt helpers using Utils */
 static Day promptDay() {
     std::cout << "Pick a day:\n";
     for (int i = 0; i < 7; ++i) {
         std::cout << "  " << (i+1) << ") " << kDayNames[i] << "\n";
     }
     int choice = promptIntInRange("Day [1-7]: ", 1, 7);
     return static_cast<Day>(choice - 1);
 }
 
 static int promptTime(const std::string& label) {
     while (true) {
         std::cout << label << " (24h HH:MM): ";
         std::string s = trim(safeGetLine());
         int minutes = 0;
         if (parseHHMM(s, minutes)) return minutes;
         std::cout << "Invalid time. Try again (e.g., 13:30)\n";
     }
 }
 
 /* Pretty printing for roster */
 static void listClassmates(const std::vector<Profile>& roster, const Profile& me) {
     if (roster.size() <= 1) {
         std::cout << "(No classmates yet. Use option 8 to add some.)\n";
         return;
     }
     std::cout << "Classmates:\n";
     for (size_t i = 0; i < roster.size(); ++i) {
         const Profile& p = roster[i];
         // Skip ME (assume me is roster[0])
         if ((!p.email().empty() && p.email() == me.email()) ||
             (!p.name().empty()  && !me.name().empty() && p.name() == me.name())) {
             continue;
         }
         std::cout << "  - " << (p.name().empty() ? "(no name)" : p.name())
                   << " <" << (p.email().empty() ? "(no email)" : p.email()) << ">\n";
         std::cout << "    Courses:\n";
         p.printCourses();
     }
 }
 
 /* Find classmate by email (exact) */
 static const Profile* findByEmail(const std::vector<Profile>& roster, const std::string& email) {
     for (const auto& p : roster) {
         if (!email.empty() && trim(p.email()) == trim(email)) return &p;
     }
     return nullptr;
 }
 
 /* -------------------------- main() -------------------------- */
 
 int main() {
     // Core single-user + roster
     Profile me; // me will be added as roster[0] once created
     std::vector<Profile> roster; // [0] reserved for me when exists
 
     // Managers
     CourseManager courseMgr;
     AvailabilityManager availMgr;
     AvailabilityEditor availEditor;
     NotificationCenter notif;
     SessionRequests sessions(&notif);
     MatchSuggester matcher;
 
     while (true) {
         printMainMenu();
         int choice = promptIntInRange("Choose an option [0-20]: ", 0, 20);
 
         if (choice == 0) {
             std::cout << "Goodbye!\n";
             return 0;
         }
 
         switch (choice) {
         case 1: { // Create/Reset MY Profile
             std::cout << "\n--- Create/Reset MY Profile ---\n";
             std::cout << "Enter your name (or leave blank): ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string name = trim(safeGetLine());
             std::cout << "Enter your Clemson email (or leave blank): ";
             std::string email = trim(safeGetLine());
             std::cout << "Enter enrolled courses (comma-separated):\n> ";
             std::string raw = safeGetLine();
             auto tokens = split(raw, ',');
             auto normalized = CourseManager::normalizeDedup(tokens);
 
             me.createOrReset(name, email, normalized);
 
             // Put ME at roster[0]; if first time, push_back; else replace index 0
             if (roster.empty()) roster.push_back(me);
             else roster[0] = me;
 
             std::cout << "Profile created/reset successfully.\n";
             me.show();
             break;
         }
         case 2: { // View MY Profile
             me.show();
             pauseEnter();
             break;
         }
         case 3: { // Add Courses (ME)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Enter courses to ADD (comma-separated):\n> ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string line = trim(safeGetLine());
             courseMgr.addCourses(me, line);
             roster[0] = me;
             break;
         }
         case 4: { // Remove Courses (ME)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Your current courses:\n";
             me.printCourses();
             std::cout << "Enter indices to REMOVE (comma-separated), e.g., 2,4 :\n> ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string line = trim(safeGetLine());
             courseMgr.removeCourses(me, line);
             roster[0] = me;
             break;
         }
         case 5: { // Add Availability (ME)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             Day d = promptDay();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             int startMin = promptTime("Start time");
             int endMin   = promptTime("End time");
             if (endMin <= startMin) { std::cout << "End must be after start.\n"; break; }
             availMgr.addAvailability(me, d, startMin, endMin);
             roster[0] = me;
             break;
         }
         case 6: { // Remove Availability (ME)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Current availability:\n";
             AvailabilityManager::listIndexed(me);
             std::cout << "Enter an index to remove (or 0 to cancel): ";
             int idx;
             if (!(std::cin >> idx)) {
                 std::cin.clear();
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 std::cout << "Invalid input.\n"; break;
             }
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             if (idx == 0) { std::cout << "Cancelled.\n"; break; }
             availMgr.removeAvailability(me, idx);
             roster[0] = me;
             break;
         }
         case 7: { // Edit Availability (ME)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             if (me.availability().empty()) { std::cout << "You have no availability to edit.\n"; break; }
             std::cout << "Current availability:\n";
             AvailabilityManager::listIndexed(me);
             std::cout << "Enter index to EDIT: ";
             int idx; if (!(std::cin >> idx)) {
                 std::cin.clear();
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 std::cout << "Invalid input.\n"; break;
             }
             Day d = promptDay();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             int startMin = promptTime("New start");
             int endMin   = promptTime("New end");
             std::cin.clear();
             bool ok = availEditor.editSlot(me, idx, d, startMin, endMin);
             if (ok) roster[0] = me;
             break;
         }
         case 8: { // Add a Classmate Profile
             std::cout << "\n--- Add Classmate Profile ---\n";
             std::cout << "Name (or blank): ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string name = trim(safeGetLine());
             std::cout << "Email (or blank): ";
             std::string email = trim(safeGetLine());
             std::cout << "Courses (comma-separated):\n> ";
             auto courses = split(safeGetLine(), ',');
             auto normalized = CourseManager::normalizeDedup(courses);
 
             Profile p;
             p.createOrReset(name, email, normalized);
 
             // Optional: a quick availability add
             std::cout << "Add availability now? (y/n): ";
             std::string yn = trim(safeGetLine());
             if (!yn.empty() && (yn[0]=='y' || yn[0]=='Y')) {
                 Day d = promptDay();
                 int startMin = promptTime("Start");
                 int endMin   = promptTime("End");
                 if (endMin > startMin) {
                     AvailabilitySlot s{d, startMin, endMin};
                     p.availabilityMutable().push_back(s);
                     // Merge like in AvailabilityManager (quick sort/merge):
                     std::vector<AvailabilitySlot>& v = p.availabilityMutable();
                     std::sort(v.begin(), v.end(), [](const AvailabilitySlot& a, const AvailabilitySlot& b){
                         if (a.day != b.day) return static_cast<int>(a.day) < static_cast<int>(b.day);
                         return a.start < b.start;
                     });
                     std::vector<AvailabilitySlot> merged;
                     for (const auto& slot : v) {
                         if (merged.empty() || merged.back().day != slot.day || merged.back().end < slot.start) {
                             merged.push_back(slot);
                         } else {
                             merged.back().end = std::max(merged.back().end, slot.end);
                         }
                     }
                     v.swap(merged);
                 }
             }
 
             roster.push_back(p);
             std::cout << "Classmate added.\n";
             break;
         }
         case 9: { // List Classmates
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             listClassmates(roster, me);
             pauseEnter();
             break;
         }
         case 10: { // Search by Course
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Course code to search (e.g., CPSC 2150): ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string code = trim(safeGetLine());
             auto results = ClassmateSearch::byCourse(roster, me, code);
             if (results.empty()) { std::cout << "No classmates found for that course.\n"; break; }
             std::cout << "Found:\n";
             for (auto* p : results) {
                 std::cout << "  - " << (p->name().empty()? "(no name)" : p->name())
                           << " <" << p->email() << ">\n";
             }
             break;
         }
         case 11: { // Search by Name
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Name (or part of it): ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string q = trim(safeGetLine());
             auto results = ClassmateSearch::byName(roster, me, q);
             if (results.empty()) { std::cout << "No classmates matched that name.\n"; break; }
             std::cout << "Found:\n";
             for (auto* p : results) {
                 std::cout << "  - " << (p->name().empty()? "(no name)" : p->name())
                           << " <" << p->email() << ">\n";
             }
             break;
         }
         case 12: { // Browse availability by course
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Course code: ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string code = trim(safeGetLine());
             auto list = AvailabilityBrowser::browseByCourse(roster, me, code);
             if (list.empty()) { std::cout << "No classmates in that course.\n"; break; }
             for (auto& entry : list) {
                 std::cout << entry.first << ":\n";
                 if (entry.second.empty()) { std::cout << "  (no availability)\n"; continue; }
                 Day current = static_cast<Day>(-1);
                 for (const auto& s : entry.second) {
                     if (current != s.day) {
                         current = s.day;
                         std::cout << "  " << kDayNames[static_cast<int>(s.day)] << ": ";
                     } else {
                         std::cout << "                   ";
                     }
                     std::cout << formatHHMM(s.start) << "-" << formatHHMM(s.end) << "\n";
                 }
             }
             break;
         }
         case 13: { // Browse availability by course & day
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Course code: ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string code = trim(safeGetLine());
             Day d = promptDay();
             auto list = AvailabilityBrowser::browseByCourseAndDay(roster, me, code, d);
             if (list.empty()) {
                 std::cout << "No classmates have availability on "
                           << kDayNames[static_cast<int>(d)] << " for that course.\n";
                 break;
             }
             for (auto& entry : list) {
                 std::cout << entry.first << ":\n";
                 for (const auto& s : entry.second) {
                     std::cout << "  " << kDayNames[static_cast<int>(s.day)] << " "
                               << formatHHMM(s.start) << "-" << formatHHMM(s.end) << "\n";
                 }
             }
             break;
         }
         case 14: { // Send study request
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cout << "Invitee email: ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string email = trim(safeGetLine());
             const Profile* target = findByEmail(roster, email);
             if (!target) { std::cout << "No classmate found with that email.\n"; break; }
 
             std::cout << "Course code: ";
             std::string code = trim(safeGetLine());
             Day d = promptDay();
             int startMin = promptTime("Start");
             int endMin   = promptTime("End");
             const auto& s = sessions.sendRequest(me, *target, code, d, startMin, endMin);
             std::cout << "Sent request " << s.id << " to " << target->email() << ".\n";
             break;
         }
         case 15: { // View MY pending requests (as invitee)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             auto pending = sessions.pendingFor(me);
             if (pending.empty()) { std::cout << "No pending requests.\n"; break; }
             for (const auto& s : pending) {
                 std::cout << "[" << s.id << "] "
                           << kDayNames[static_cast<int>(s.day)] << " "
                           << formatHHMM(s.start) << "-" << formatHHMM(s.end)
                           << " " << s.course
                           << " from " << s.requester << "\n";
             }
             break;
         }
         case 16: { // Confirm a pending request (ME)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             auto pending = sessions.pendingFor(me);
             if (pending.empty()) { std::cout << "No pending requests.\n"; break; }
             std::cout << "Enter request ID to confirm: ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string id = trim(safeGetLine());
             bool ok = sessions.confirmRequest(id, me);
             std::cout << (ok ? "Confirmed.\n" : "Could not confirm (check ID or permissions).\n");
             break;
         }
         case 17: { // Notifications
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::string key = me.email().empty() ? me.name() : me.email();
             auto msgs = notif.fetchAndClear(trim(key));
             if (msgs.empty()) { std::cout << "(No notifications)\n"; break; }
             std::cout << "Notifications:\n";
             for (const auto& m : msgs) std::cout << "  - " << m << "\n";
             break;
         }
         case 18: { // Calendar list (confirmed)
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             auto list = CalendarView::list(me, sessions);
             if (list.empty()) { std::cout << "(No confirmed sessions)\n"; break; }
             std::string myKey = trim(me.email().empty() ? me.name() : me.email());
             std::cout << "Your confirmed sessions:\n";
             for (const auto& s : list) {
                 std::cout << "  " << CalendarView::pretty(s, myKey) << "\n";
             }
             break;
         }
         case 19: { // Cancel confirmed
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             auto list = CalendarView::list(me, sessions);
             if (list.empty()) { std::cout << "(No confirmed sessions)\n"; break; }
             std::cout << "Enter session ID to cancel: ";
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::string id = trim(safeGetLine());
             bool ok = CalendarView::cancel(id, me, sessions);
             std::cout << (ok ? "Canceled.\n" : "Could not cancel (check ID or permissions).\n");
             break;
         }
         case 20: { // Match suggestions
             if (!me.exists()) { std::cout << "Create your profile first.\n"; break; }
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::cout << "Minimum overlap minutes (default 30): ";
             std::string mins = trim(safeGetLine());
             int minOverlap = 30;
             if (!mins.empty()) {
                 try { minOverlap = std::max(0, std::stoi(mins)); } catch(...) {}
             }
             std::cout << "Max results (default 5): ";
             std::string maxr = trim(safeGetLine());
             size_t k = 5;
             if (!maxr.empty()) {
                 try { k = static_cast<size_t>(std::max(1, std::stoi(maxr))); } catch(...) {}
             }
             auto matches = matcher.suggest(me, roster, minOverlap, k);
             if (matches.empty()) { std::cout << "No suggestions at the moment.\n"; break; }
             std::cout << "Suggestions:\n";
             for (const auto& m : matches) {
                 const Profile* p = m.person;
                 std::cout << "  - " << (p->name().empty()? "(no name)" : p->name())
                           << " <" << p->email() << ">  overlap=" << m.overlapMinutes
                           << " min  courses: ";
                 for (size_t i=0;i<m.sharedCourses.size();++i) {
                     std::cout << m.sharedCourses[i] << (i+1<m.sharedCourses.size()? ", ":"");
                 }
                 std::cout << "\n";
             }
             break;
         }
         default:
             std::cout << "Unknown option.\n";
         }
 
         std::cout << "\n";
     }
 }