//
// Created by chernovda on 26.01.2020.
//

#include "singleRegistrationCard.h"

singleRegistrationCard::singleRegistrationCard(std::string path, std::string bdName) {
    std::cout << "Подсчет ЕРК." << std::endl;

    setLastYear();
    bdOper = new fileStarter(path);
}

singleRegistrationCard::~singleRegistrationCard() {
    free(bdOper);
}

void singleRegistrationCard::getERK() {
    parsing* parse = new parsing(getLastYear()-1);
    std::string regDate;
    std::string division;
    parsing::map__registeredFilials regInFilials = parsing::map__registeredFilials();

    for (auto &it: bdOper->getParsedRecords()) {
        for (auto &recordIt: it) {
            switch (recordIt.first) {
                case MST::REG: {
                    for (const auto& regField: recordIt.second) {
                        // Сравниваем искомый год с записью в поле регистрации
                        regDate = parse->getFieldIndexValue(MST::REG_DATE, regField);
                        if (getLastYear() == parse->getYearNumber(regDate)) {
                            /**
                             * Если год совпадает
                             */
                            division = parse->getFieldIndexValue(MST::REG_DIVISION, regField);
                            if (parse->notInArray(division, regInFilials)) { // проверка на вхождение в массив филиалов
                                if (strlen(division.c_str()) == 0) {
                                    std::cout << regDate << std::endl;
                                }
                                std::cout << "Новый филиал: <" << division << "> len: " << strlen(division.c_str()) << std::endl;
                                regInFilials[division] = 1;
                            } else {
                                regInFilials[division] = ++regInFilials[division]; // увеличение количества
                            }
                        }
                    }
                    break;
                }
                case MST::REREG: {
                    for (const auto& regField: recordIt.second) {
                        // Сравниваем искомый год с записью в поле регистрации
                        regDate = parse->getFieldIndexValue(MST::REG_DATE, regField);
                        if (getLastYear() == parse->getYearNumber(regDate)) {
                            /**
                             * Если год совпадает
                             */
                            division = parse->getFieldIndexValue(MST::REG_DIVISION, regField);
                            if (parse->notInArray(division, regInFilials)) { // проверка на вхождение в массив филиалов
                                std::cout << "Новый филиал: <" << division << ">"<< std::endl;
                                regInFilials[division] = 1;
                            } else {
                                regInFilials[division] = ++regInFilials[division]; // увеличение количества
                            }
                        }
                    }
                    break;
                }
            }
        }

    }

    free(parse);

    for (const auto& filial: regInFilials) {
        std::cout << filial.first << ": " << filial.second << std::endl;
    }
}

void singleRegistrationCard::setLastYear() {
    std::time_t result = std::time(nullptr);
    lastYear = 1900 + localtime(&result)->tm_year;
}

uint32_t singleRegistrationCard::getLastYear() const {
    return lastYear;
}
