//
// Created by chernovda on 26.01.2020.
//

#ifndef IRBIS64_MULTITOOL_SINGLEREGISTRATIONCARD_H
#define IRBIS64_MULTITOOL_SINGLEREGISTRATIONCARD_H

#include "MST.h"
#include "XRF.h"
#include "fileStarter.h"
#include "parsing.h"

class singleRegistrationCard {
private:
    /**
     * Текущий год
     */
    uint32_t lastYear;
public:
    uint32_t getLastYear() const;

public:
    void setLastYear();

public:
    fileStarter* bdOper;

    singleRegistrationCard(std::string path, std::string bdName);
    virtual ~singleRegistrationCard();

    /**
     * Получаем ЕРК
     * @param parsedRecords
     */
    void getERK();
};


#endif //IRBIS64_MULTITOOL_SINGLEREGISTRATIONCARD_H
