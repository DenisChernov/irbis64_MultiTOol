cmake_minimum_required(VERSION 3.12)
project(irbis64_MultiTOol)

set(CMAKE_CXX_STANDARD 17)

include_directories("OpenXLSX/src")
include_directories("xlnt/src")

link_directories(OpenXLSX/lib)
link_directories(xlnt/lib)

add_executable(irbis64_MultiTOol main.cpp XRF.cpp XRF.h MST.cpp MST.h parsing.cpp parsing.h tableViewExport.cpp tableViewExport.h singleRegistrationCard.cpp singleRegistrationCard.h fileStarter.cpp fileStarter.h dailyStats.cpp dailyStats.h tags.h)
target_link_libraries(irbis64_MultiTOol "libOpenXLSX.so")
target_link_libraries(irbis64_MultiTOol "libxlnt.so.1.2")


include(PVS-Studio.cmake)

pvs_studio_add_target(TARGET irbis64_MultiTOol.analyze ALL
        OUTPUT FORMAT errorfile
        ANALYZE ${PROJECT_NAME}
        MODE GA:1,2 OP
        LOG target.err)