cmake_minimum_required (VERSION 3.0)
project (sse_dfa_llvm)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pipe -O0 -g -march=native -DNDEBUG -rdynamic")

find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_path(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)
# find_path(LLVM_INCLUDE_DIR llvm)
find_library(SQLITE3_LIBRARY NAMES sqlite3)
find_library(CLANG_LIBRARIES NAMES clang)

set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_path(CLANG_LIB_PATH clang)

add_definitions(${LLVM_DEFINITIONS})

message(STATUS ${Boost_LIBRARIES})

set(STATIC_LINKING false)

set(CLANG_LIBRARIES
  clangTooling
  clangFrontend
  clangDriver
  clangSerialization
  clangCodeGen
  clangParse
  clangSema
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangAnalysis
  clangARCMigrate
  clangRewrite
  clangRewriteFrontend
  clangEdit
  clangAST
  clangASTMatchers
  clangLex
  clangBasic
  )

set(ALL_LIBS 
  ${SQLITE3_LIBRARY}
  ${Boost_LIBRARIES}
  ${llvm_libs}
  ${CLANG_LIBRARIES}
  )

include_directories(
  ${INCLUDES}
  ${SQLITE3_INCLUDE_DIR}
  ${BOOST_INCLUDE_DIR}
  ${LLVM_INCLUDE_DIRS}
  )

add_definitions(-DNDEBUG)

file(GLOB_RECURSE SRC_ANALYSIS src/analysis/*.h? src/analysis/*.cpp)
add_library( analysis ${SRC_ANALYSIS} src/configuration.hh)
target_link_libraries(analysis ${ALL_LIBS})

file(GLOB_RECURSE SRC_CLANG src/clang/*.h? src/clang/*.cpp)
add_library( clang ${SRC_CLANG} src/configuration.hh)
target_link_libraries(clang ${ALL_LIBS})

file(GLOB_RECURSE SRC_LIB src/lib/*.h? src/lib/*.cpp)
add_library( lib ${SRC_LIB} src/configuration.hh)
target_link_libraries(lib ${ALL_LIBS})

file(GLOB_RECURSE SRC_DB src/db/*.h? src/db/*.cpp)
add_library( db ${SRC_DB} src/configuration.hh)
target_link_libraries(db ${ALL_LIBS})

file(GLOB_RECURSE SRC_FLEX src/flex/*.h? src/flex/*.cpp)
add_library( flex ${SRC_FLEX} src/configuration.hh)
target_link_libraries(db ${ALL_LIBS})

file(GLOB_RECURSE SRC_UTILS src/utils/*.h? src/utils/*.cpp)
add_library( utils ${SRC_UTILS} src/configuration.hh)
target_link_libraries(utils ${ALL_LIBS})


add_executable(main  src/main.cpp src/configuration.hh)

target_link_libraries(main
  analysis clang db flex lib utils
  ${SQLITE3_LIBRARY}
  ${Boost_LIBRARIES}
  # ${llvm_libs}
  LLVM
  ${CLANG_LIBRARIES}
  )


# Test For Hexastore

add_executable(test_hexastore src/db/Test.cpp src/db/Hexastore.cpp src/db/Hexastore.hh)

target_link_libraries(test_hexastore
  ${SQLITE3_LIBRARY}
  )