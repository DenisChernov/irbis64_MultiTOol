//
// Created by chernovda on 26.01.2020.
//

#ifndef IRBIS64_MULTITOOL_TABLEVIEWEXPORT_H
#define IRBIS64_MULTITOOL_TABLEVIEWEXPORT_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "OpenXLSX.h"

using namespace OpenXLSX;

class tableViewExport {
private:
    enum ABC {
        A='A',B='B',C='C',D='D',E='E',F='F',G='G',H='H',I='I',J='J',K='K',L='L',M='M',N='N',O='O',P='P',Q='Q',R='R',S='S',T='T',U='U',V='V',W='W',X='X',Y='Y',Z='Z'
    };

    char COL = ABC::A;
    size_t ROW = 1;

    std::string filePath;
    std::string sheetName;
public:
    const std::string &getSheetName() const;

    void setSheetName(const std::string &sheetName);

public:
    const std::string &getFilePath() const;

    void setFilePath(const std::string &filePath);

public:
    XLDocument doc;
//    XLWorksheet wks;

    enum FILE_MODE {
            CREATE,
            OPEN
    };

    tableViewExport(const std::string& filepath, FILE_MODE mode);
    ~tableViewExport();
    void sheetbookClose();

    void putDayStat(std::string, size_t);
    void putDayStat(const std::string&, const std::string&, int);
    size_t incRow();
    void setRow(size_t);
    void setCol(ABC);
};


#endif //IRBIS64_MULTITOOL_TABLEVIEWEXPORT_H
