/***************************************************************************************
 * Utils.hpp
 * Shared helper functions for CLI and parsing.
 *
 * STANDARD LIBRARIES USED:
 *  <string>     : std::string for textual data.
 *  <vector>     : std::vector for token lists.
 *  <sstream>    : std::stringstream for tokenization.
 *  <algorithm>  : std::find_if, std::transform for trimming & case conversion.
 *  <cctype>     : std::isspace, std::toupper for character ops.
 *  <iostream>   : std::cout/std::cin used in prompt helpers (declared here, implemented in .cpp).
 ****************************************************************************************/
#pragma once
#include <string>
#include <vector>

namespace sb {

// Trim whitespace from both ends.
std::string trim(std::string s);

// Uppercase copy (useful for course codes like "CPSC 2150").
std::string upperCopy(std::string s);

// Split string by delimiter (e.g., "A, B, C").
std::vector<std::string> split(const std::string& s, char delim);

// Safely read a full line from std::cin (handles leftover newlines from operator>>).
std::string safeGetLine();

// Prompt for an integer in [lo, hi]; repeats until valid.
int promptIntInRange(const std::string& prompt, int lo, int hi);

// Time helpers (minutes since midnight), e.g., "13:30" ⇄ 810
bool parseHHMM(const std::string& s, int& minutesOut);
std::string formatHHMM(int minutes);

} // namespace sb