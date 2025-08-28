/***************************************************************************************
 * AvailabilityManager.cpp — implementation
 ****************************************************************************************/
#include "AvailabilityManager.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <iostream>

namespace sb {

bool AvailabilityManager::overlapsSameDay(const AvailabilitySlot& a, const AvailabilitySlot& b) {
    if (a.day != b.day) return false;
    return (a.start < b.end) && (a.end > b.start);
}

void AvailabilityManager::addMerged(Profile& prof, const AvailabilitySlot& s) {
    auto& v = prof.availabilityMutable();
    v.push_back(s);

    std::sort(v.begin(), v.end(), [](const AvailabilitySlot& x, const AvailabilitySlot& y){
        if (x.day != y.day) return static_cast<int>(x.day) < static_cast<int>(y.day);
        return x.start < y.start;
    });

    std::vector<AvailabilitySlot> merged;
    for (const auto& slot : v) {
        if (merged.empty() ||
            merged.back().day != slot.day ||
            merged.back().end < slot.start) {
            merged.push_back(slot);
        } else {
            merged.back().end = std::max(merged.back().end, slot.end);
        }
    }
    v.swap(merged);
}

void AvailabilityManager::addAvailability(Profile& prof, Day day, int startMin, int endMin) {
    if (!prof.exists()) { std::cout << "Create a profile first.\n"; return; }
    if (endMin <= startMin) {
        std::cout << "End must be after start.\n";
        return;
    }
    addMerged(prof, AvailabilitySlot{day, startMin, endMin});
    std::cout << "Availability added (merged if overlapping on the same day).\n";
}

void AvailabilityManager::removeAvailability(Profile& prof, int oneBasedIndex) {
    if (!prof.exists()) { std::cout << "Create a profile first.\n"; return; }
    auto& v = prof.availabilityMutable();
    if (v.empty()) { std::cout << "No availability to remove.\n"; return; }

    if (oneBasedIndex < 1 || static_cast<size_t>(oneBasedIndex) > v.size()) {
        std::cout << "Index out of range.\n";
        return;
    }
    v.erase(v.begin() + (oneBasedIndex - 1));
    std::cout << "Availability removed.\n";
}

Day AvailabilityManager::promptDay() {
    std::cout << "Pick a day:\n";
    for (int i = 0; i < 7; ++i) {
        std::cout << "  " << (i+1) << ") " << kDayNames[i] << "\n";
    }
    int choice = sb::promptIntInRange("Day [1-7]: ", 1, 7);
    return static_cast<Day>(choice - 1);
}

void AvailabilityManager::listIndexed(const Profile& prof) {
    const auto& v = prof.availability();
    if (v.empty()) {
        std::cout << "  (no availability yet)\n";
        return;
    }
    for (size_t i = 0; i < v.size(); ++i) {
        const auto& s = v[i];
        std::cout << "  [" << (i+1) << "] "
                  << kDayNames[static_cast<int>(s.day)] << " "
                  << sb::formatHHMM(s.start) << "-" << sb::formatHHMM(s.end) << "\n";
    }
}

} // namespace sb