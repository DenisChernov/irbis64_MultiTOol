cmake_minimum_required(VERSION 3.12)
project(irbis64BookImport)

set(CMAKE_CXX_STANDARD 17)

add_executable(irbis64BookImport main.cpp XRF.cpp XRF.h MST.cpp MST.h parsing.cpp parsing.h)

include(PVS-Studio.cmake)

pvs_studio_add_target(TARGET irbis64BookImport.analyze ALL
        OUTPUT FORMAT errorfile
        ANALYZE ${PROJECT_NAME}
        MODE GA:1,2 OP
        LOG target.err)