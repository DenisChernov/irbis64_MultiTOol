//
// Created by chernovda on 26.01.2020.
//

#ifndef IRBIS64_MULTITOOL_TABLEVIEWEXPORT_H
#define IRBIS64_MULTITOOL_TABLEVIEWEXPORT_H

#include <iostream>
#include <string>
#include <OpenXLSX.h>
using namespace OpenXLSX;

class tableViewExport {
//    enum FILE_MODE {
//            CREATE,
//            OPEN
//    };

private:
    XLDocument doc;

    tableViewExport(std::string/*, FILE_MODE*/);
    ~tableViewExport();

};


#endif //IRBIS64_MULTITOOL_TABLEVIEWEXPORT_H
