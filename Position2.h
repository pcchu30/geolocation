#pragma once

#include "stdafx.h"
#include "PositionBase.h"

class Position2 : public PositionBase{
public:

    explicit Position2(unsigned long int timestamp, unsigned long int originalTimestamp, double lon, double lat);
   

    unsigned long int getTimestamp() const {
        return timestamp;
    }

    unsigned long int getOriginalTimestamp() const {
        return originalTimestamp;
    }


    void setTimestamp( unsigned long int timestamp ) {
        this->timestamp = timestamp;
    }


    void setOriginalTimestamp( unsigned long int originalTimestamp ) {
        this->originalTimestamp = originalTimestamp;
    }

    


private:
    unsigned long int timestamp;
    unsigned long int originalTimestamp;   
};





void testPosition2();

