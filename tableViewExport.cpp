//
// Created by chernovda on 26.01.2020.
//

#include "tableViewExport.h"

tableViewExport::tableViewExport(const std::string& filepath, FILE_MODE mode) {
    // TODO: Проверка существования файла
    setFilePath(filepath);

    switch (mode) {
//        case CREATE: {
//            doc. CreateDocument(filepath);
//            break;
//        }
        case OPEN: {
            doc.OpenDocument(getFilePath());
            if (doc.Workbook().SheetCount()) {
                setSheetName(doc.Workbook().SheetNames().front());
            }
            break;
        }
        default: {
            break;
        }
    }
}

tableViewExport::~tableViewExport() {
    //doc.CloseDocument();
}

void tableViewExport::putDayStat(std::string, size_t) {

}

void tableViewExport::putDayStat(const std::string& sheet, const std::string& col, int value) {
    XLWorksheet wks = doc.Workbook().Worksheet(getSheetName());
    wks.Cell(col + std::to_string(ROW)).Value() = value;
    doc.SaveDocument();
}

size_t tableViewExport::incRow() {
    return ++ROW;
}

void tableViewExport::setRow(size_t Row) {
    ROW = Row;
}

void tableViewExport::setCol(ABC Col) {
    COL = Col;
}

void tableViewExport::sheetbookClose() {
    doc.CloseDocument();
}

const std::string &tableViewExport::getFilePath() const {
    return filePath;
}

void tableViewExport::setFilePath(const std::string &filePath) {
    tableViewExport::filePath = filePath;
}

const std::string &tableViewExport::getSheetName() const {
    return sheetName;
}

void tableViewExport::setSheetName(const std::string &sheetName) {
    tableViewExport::sheetName = sheetName;
}
