// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
// Created by chernovda on 19.11.2019.
//

#include "parsing.h"

bool parsing::regLastYears(const int& year, const std::vector<std::string>& tag) {
//    std::cout << "regLastYears tagSize: " << tag.size() << std::endl;
    bool toDelete = true;

    for (std::string part: tag) {
        //std::cout << "\t" << part << std::endl;
        record = mst->record(part);
        for (auto subIt = record.begin(); subIt != record.end(); ++subIt) {
            if (std::tolower(subIt->first.c_str()[0], loc) == 'd') {
                //std::cout << "\t\t\t" << subIt->first << ": " << subIt->second << std::endl;
                if (!getYear(subIt->second)) {
                    return false;
                }
            }
        }
    }
    return toDelete;
}

bool parsing::reRegLastYears(const std::vector<std::string>& tag) {
    //std::cout << "reRegLastYears tagSize: " << tag.size() << std::endl;

    return false;
}

/**
 *
 * @param date
 * @return bool
 */
bool parsing::getYear(const std::string& date) {
    std::regex reFront("^(\\d{4}).*");
    std::smatch result;
    if (std::regex_match(date, result, reFront)) {
//        std::cout << "\t\t\t" << std::stoi(result[1]) << std::endl;
        if (isYear(result[1])) {
            if (toLongOut(result[1])) {
                return true;
            }
        } else {
            std::regex reBack(".*(\\d{4})$");
            if (std::regex_match(date, result, reBack)) {
//                std::cout << "\t\t\t" << std::stoi(result[1]) << std::endl;
                if (isYear(result[1])) {
                    if (toLongOut(result[1])) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 *
 * @param date
 * @return uint32_t
 */
uint32_t parsing::getYearNumber(const std::string& date) {
    std::regex reFront("^(\\d{4}).*");
    std::smatch result;
    if (std::regex_match(date, result, reFront)) {
//        std::cout << "\t\t\t" << std::stoi(result[1]) << std::endl;
        if (isYear(result[1])) {
            return std::stoi(result[1]);
        } else {
            std::regex reBack(".*(\\d{4})$");
            if (std::regex_match(date, result, reBack)) {
//                std::cout << "\t\t\t" << std::stoi(result[1]) << std::endl;
                if (isYear(result[1])) {
                    return std::stoi(result[1]);
                }
            }
        }
    }
    return 0;
}

bool parsing::isYear(const std::string &year) {
    const uint32_t y = std::stoi(year);
    return 1940 <= y && y <= getCurYear();
}

parsing::parsing() {
    mst = new MST();
}

int parsing::getCurYear() const {
    return curYear;
}

void parsing::setCurYear(int curYear) {
    parsing::curYear = curYear;
}

parsing::parsing(uint32_t year) {
    setCurYear(year);
}

bool parsing::toLongOut(const std::string &year) {
    const int y = std::stoi(year);
    return getCurYear() - y > 3;
}

void parsing::showUser(const std::map<uint32_t, std::vector<std::string>>& record) {
    for (const auto& recordIt: record) {
        std::cout << recordIt.first << "#" << std::endl;
        for (const auto& tag: recordIt.second) {
            std::cout << "\t" << tag << std::endl;
//            std::cout << "\t\tразбор поля: " << std::endl;
//            for (auto subIt: recordIt) {
//                std::cout << "\t\t\t" << subIt->first << ": " << subIt->second << std::endl;
//            }
        }
    }
}

std::string parsing::getFieldIndexValue(MST::fieldIndexes field, const std::string &str) {
    switch (field) {
        case MST::REG_DATE: {
            std::regex re("\\^([Dd])([^\\^]*)");
            std::smatch match;
            if (std::regex_search(str, match, re)) {
                return match[2];
            }
            break;
        }
        case MST::REG_DIVISION: {
            std::regex re("\\^([Cc])([^\\^]*)");
            std::smatch match;

            if (std::regex_search(str, match, re)) {
                std::regex reDivision("(\\W{1,2}\\s(\\d{2}?|\\d{1}?)|(\\W{2,4}))");
                std::string division = match[2];
                if (std::regex_search(division, match, reDivision)) {
                    return match[1];
                }
            }
            break;
        }
        case MST::VISIT_FILIAL: {
            std::regex re("\\^([Vv])([^\\^]*)");
            std::smatch match;

            if (std::regex_search(str, match, re)) {
                return match[2];
            }
            break;
        }
    }
    return std::string();
}

std::vector<std::string> parsing::regFilials(uint32_t year, std::vector<std::string> &regField) {
    std::vector<std::string> filials;

    for (auto field: regField) {

    }

    return filials;
}

std::vector<std::string> parsing::regRegFilials(uint32_t, std::vector<std::string> &reRegField) {
    std::vector<std::string> filials;
    return filials;
}

bool parsing::notInArray(std::string value, map__registeredFilials &array) {
    return array.find(value) == array.end();
}
