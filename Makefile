# --------------------------------------------
# Study Buddy — Makefile (C++17, stdlib only)
# Portable: explicit rules (no bash indirection)
# --------------------------------------------

# Compiler & flags
CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

# Sources shared by main and tests
CORE_SRC := \
	Utils.cpp \
	Profile.cpp \
	CourseManager.cpp \
	AvailabilityManager.cpp \
	AvailabilityEditor.cpp \
	AvailabilityBrowser.cpp \
	MatchSuggester.cpp \
	ClassmateSearch.cpp \
	NotificationCenter.cpp \
	SessionRequests.cpp \
	CalendarView.cpp

# Main program
MAIN_SRC := main.cpp
MAIN_BIN := study_buddy

# Test binaries
TEST_BINS := \
	test_profile \
	test_course_manager \
	test_availability_manager \
	test_availability_editor \
	test_availability_browser \
	test_match_suggester \
	test_classmate_search \
	test_notifications \
	test_session_requests \
	test_calendar_view

# Default target: build everything (main + tests)
.PHONY: all
all: build tests

# 1) Compile the main program
.PHONY: build
build: $(MAIN_BIN)

$(MAIN_BIN): $(MAIN_SRC) $(CORE_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 2) Run the main program executable
.PHONY: run
run: $(MAIN_BIN)
	./$(MAIN_BIN)

# 3) Compile all test suites
.PHONY: tests
tests: $(TEST_BINS)

# ----- Explicit test build rules (portable) -----
test_profile: $(CORE_SRC) test_profile.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_course_manager: $(CORE_SRC) test_course_manager.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_availability_manager: $(CORE_SRC) test_availability_manager.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_availability_editor: $(CORE_SRC) test_availability_editor.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_availability_browser: $(CORE_SRC) test_availability_browser.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_match_suggester: $(CORE_SRC) test_match_suggester.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_classmate_search: $(CORE_SRC) test_classmate_search.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# This test only needs NotificationCenter, but linking CORE_SRC is fine too.
test_notifications: NotificationCenter.cpp test_notifications.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_session_requests: $(CORE_SRC) test_session_requests.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_calendar_view: $(CORE_SRC) test_calendar_view.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# 4) Execute all test suites (builds first, then runs; stops on first failure)
.PHONY: test run-tests
test: run-tests

run-tests: tests
	@set -e; \
	for t in $(TEST_BINS); do \
		echo "== Running $$t =="; \
		./$$t; \
	done; \
	echo "All tests passed."

# 5) Clean: delete binaries and any Windows .exe artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -f $(MAIN_BIN) $(TEST_BINS) *.o *.obj *.exe

# Convenience aliases
.PHONY: rebuild
rebuild: clean build

.PHONY: help
help:
	@echo "Targets:"
	@echo "  all        : build main + tests"
	@echo "  build      : build main program ($(MAIN_BIN))"
	@echo "  run        : run main program"
	@echo "  tests      : build all test binaries"
	@echo "  test       : build and run all tests"
	@echo "  clean      : remove binaries and *.exe"
	@echo "  rebuild    : clean and build"