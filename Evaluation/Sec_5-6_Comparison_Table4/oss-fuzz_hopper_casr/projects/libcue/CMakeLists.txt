SET(PACKAGE libcue)
SET(PACKAGE_NAME libcue)
SET(PACKAGE_VERSION 2.3.0)
SET(PACKAGE_SOVERSION 2)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(libcue C CXX)

include(GNUInstallDirs)


SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
ADD_DEFINITIONS(-DPACKAGE_VERSION="${PACKAGE_VERSION}")
ADD_DEFINITIONS(-DPACKAGE_STRING="${PACKAGE_STRING}")

SET (CUE_SOURCES
	cd.c
	cdtext.c
	rem.c
	time.c)

CONFIGURE_FILE(libcue.pc.in libcue.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/libcue.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}) # for generated files

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)
BISON_TARGET(CueParser
	cue_parser.y
	${CMAKE_BINARY_DIR}/cue_parser.c)
FLEX_TARGET(CueScanner
	cue_scanner.l
	${CMAKE_BINARY_DIR}/cue_scanner.c)
ADD_FLEX_BISON_DEPENDENCY(CueScanner CueParser)

ADD_LIBRARY(cue ${CUE_SOURCES}
	${BISON_CueParser_OUTPUTS}
	${FLEX_CueScanner_OUTPUTS})

SET_TARGET_PROPERTIES (cue
	PROPERTIES VERSION ${PACKAGE_VERSION}
	SOVERSION ${PACKAGE_SOVERSION}
	C_VISIBILITY_PRESET hidden)

INSTALL(TARGETS cue
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(FILES ${CMAKE_SOURCE_DIR}/libcue.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
if(CMAKE_HOST_UNIX)
	INSTALL(DIRECTORY DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libcue)
	INSTALL(CODE "
	EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E create_symlink ../libcue.h
		\$ENV{DESTDIR}/${CMAKE_INSTALL_FULL_INCLUDEDIR}/libcue/libcue.h)
	")
endif(CMAKE_HOST_UNIX)

ENABLE_TESTING()
ADD_EXECUTABLE(standard_cue ${CMAKE_SOURCE_DIR}/t/standard_cue.c)
TARGET_LINK_LIBRARIES(standard_cue cue)

ADD_EXECUTABLE(single_file_idx_00 ${CMAKE_SOURCE_DIR}/t/single_idx_00.c)
TARGET_LINK_LIBRARIES(single_file_idx_00 cue)

ADD_EXECUTABLE(multiple_files ${CMAKE_SOURCE_DIR}/t/multiple_files.c)
TARGET_LINK_LIBRARIES(multiple_files cue)

ADD_EXECUTABLE(noncompliant ${CMAKE_SOURCE_DIR}/t/noncompliant.c)
TARGET_LINK_LIBRARIES(noncompliant cue)

ADD_EXECUTABLE(issue10 ${CMAKE_SOURCE_DIR}/t/issue10.c)
TARGET_LINK_LIBRARIES(issue10 cue)

ADD_EXECUTABLE(99_tracks ${CMAKE_SOURCE_DIR}/t/99_tracks.c)
TARGET_LINK_LIBRARIES(99_tracks cue)

if(BUILD_FUZZER)
	ADD_EXECUTABLE(fuzz ${CMAKE_SOURCE_DIR}/oss-fuzz/fuzz.cpp)
	TARGET_LINK_LIBRARIES(fuzz cue $ENV{LIB_FUZZING_ENGINE})

ADD_EXECUTABLE(libcue__cd_get_cdtext__0 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_cdtext__0.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_cdtext__0 cue $ENV{LIB_FUZZING_ENGINE})
ADD_EXECUTABLE(libcue__cd_get_ntrack__0 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_ntrack__0.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_ntrack__0 cue $ENV{LIB_FUZZING_ENGINE})
ADD_EXECUTABLE(libcue__cd_get_ntrack__1 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_ntrack__1.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_ntrack__1 cue $ENV{LIB_FUZZING_ENGINE})
ADD_EXECUTABLE(libcue__cd_get_rem__0 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_rem__0.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_rem__0 cue $ENV{LIB_FUZZING_ENGINE})
ADD_EXECUTABLE(libcue__cd_get_track__0 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_track__0.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_track__0 cue $ENV{LIB_FUZZING_ENGINE})
ADD_EXECUTABLE(libcue__cd_get_track__1 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_track__1.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_track__1 cue $ENV{LIB_FUZZING_ENGINE})
ADD_EXECUTABLE(libcue__cd_get_track__2 ${CMAKE_SOURCE_DIR}/oss-fuzz/libcue__cd_get_track__2.cpp)
	TARGET_LINK_LIBRARIES(libcue__cd_get_track__2 cue $ENV{LIB_FUZZING_ENGINE})


endif()

INCLUDE(Dart)

SET(DART_TESTING_TIMEOUT 5)
ADD_TEST(standard_cue_test standard_cue)
ADD_TEST(single_file_idx_00_test single_file_idx_00)
ADD_TEST(multiple_files_test multiple_files)
ADD_TEST(noncompliant_test noncompliant)
ADD_TEST(NAME issue10_test COMMAND issue10 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/t)
ADD_TEST(NAME 99_tracks_test COMMAND 99_tracks WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/t)


SET(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${PACKAGE_VERSION}")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_SOURCE_IGNORE_FILES ".*~;\\\\.swp$;/\\\\.git/;build/")
INCLUDE(CPack)

INCLUDE(CheckCSourceCompiles)
check_c_source_compiles("
void log_fn(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
int main(void) { return 0; }
" HAVE_FUNC_ATTRIBUTE_FORMAT)
