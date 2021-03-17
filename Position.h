#pragma once

#include "stdafx.h"
#include "PositionBase.h"

class Position : public PositionBase{
public:

    explicit Position(std::string dt, double lon, double lat, std::string tZone="Asia/Taipei");
   

    std::string getDateTime() const {
        return dateTime;
    }

    
    unsigned long int getTimestamp() const {
        return timestamp;
    }

    std::string getTZone() const {
        return tZone;
    }


    void setDateTime( std::string dateTime ) {
        this->dateTime = dateTime;
    }

   

    void setTimestamp( unsigned long int timestamp ) {
        this->timestamp = timestamp;
    }

    void setTZone( std::string tZone ) {
        this->tZone = tZone;
    }

private:
    std::string dateTime;
    unsigned long int timestamp;
    std::string tZone;
};



void testPosition();

