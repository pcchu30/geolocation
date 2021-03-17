#pragma once

#include "stdafx.h"


class PositionBase {
public:
    
    explicit PositionBase(double lon, double lat) : longitude(lon), latitude(lat){}    
    
    virtual ~PositionBase() {}
    
    double getLongitude() const {
        return longitude;
    }


    double getLatitude() const {
        return latitude;
    }

    void setLongitude( double longitude ) {
        this->longitude = longitude;
    }

    void setLatitude( double latitude ) {
        this->latitude = latitude;
    }


private:
    double longitude;
    double latitude;
};


void testPositionBase();
