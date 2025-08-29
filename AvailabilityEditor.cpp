/***************************************************************************************
 * AvailabilityEditor.cpp — implementation
 ****************************************************************************************/
#include "AvailabilityEditor.hpp"
#include <algorithm>
#include <iostream>

namespace sb {

bool AvailabilityEditor::editSlot(Profile& prof, int oneBasedIndex, Day day,
                                  int newStartMin, int newEndMin) {
    if (!prof.exists()) { std::cout << "Create a profile first.\n"; return false; }
    auto& v = prof.availabilityMutable();
    if (oneBasedIndex < 1 || static_cast<size_t>(oneBasedIndex) > v.size()) {
        std::cout << "Index out of range.\n"; return false;
    }
    if (newEndMin <= newStartMin) {
        std::cout << "End must be after start.\n"; return false;
    }
    // Update slot
    auto& slot = v[oneBasedIndex - 1];
    slot.day = day;
    slot.start = newStartMin;
    slot.end = newEndMin;

    // Sort and merge
    mergeAll(prof);
    std::cout << "Availability edited (merged where necessary).\n";
    return true;
}

bool AvailabilityEditor::removeSlot(Profile& prof, int oneBasedIndex) {
    if (!prof.exists()) { std::cout << "Create a profile first.\n"; return false; }
    auto& v = prof.availabilityMutable();
    if (v.empty()) { std::cout << "No availability to remove.\n"; return false; }
    if (oneBasedIndex < 1 || static_cast<size_t>(oneBasedIndex) > v.size()) {
        std::cout << "Index out of range.\n"; return false;
    }
    v.erase(v.begin() + (oneBasedIndex - 1));
    std::cout << "Availability removed.\n";
    return true;
}

void AvailabilityEditor::mergeAll(Profile& prof) {
    auto& v = prof.availabilityMutable();
    std::sort(v.begin(), v.end(), [](const AvailabilitySlot& x, const AvailabilitySlot& y){
        if (x.day != y.day) return static_cast<int>(x.day) < static_cast<int>(y.day);
        return x.start < y.start;
    });
    std::vector<AvailabilitySlot> merged;
    for (const auto& s : v) {
        if (merged.empty() ||
            merged.back().day != s.day ||
            merged.back().end < s.start) {
            merged.push_back(s);
        } else {
            merged.back().end = std::max(merged.back().end, s.end);
        }
    }
    v.swap(merged);
}

} // namespace sb