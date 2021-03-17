#pragma once

#include "stdafx.h"
#include "Position.h"
#include "Position2.h"

class PositionTable{
public:
    // constructor
    explicit PositionTable(std::string path):filePath(path){};
    // load csv file, and convert string dateTime to timestamp in a new column
    void setTable();
    // sorting table according to timestamp
    void sortTable();

    std::vector< Position > lessThan(unsigned long int timestamp);

    std::vector< Position > greaterThan(unsigned long int timestamp);

    std::vector< Position > between(unsigned long int start, unsigned long int end);

    // prepare data for Kalman filter
    std::vector< std::vector< Position > > getKalmanMatrix(int m);


    std::vector< Position2 > resample(unsigned long int seconds);


    void writeAdjustedLineString(std::string path, std::string output);

    
    std::vector< Position > getTable() const {
        return vecPosition;
    }

    void displayContents(std::vector< Position >& vecP);

    void displayContents(){
        displayContents(vecPosition);
    }

    

private:
    std::vector< Position > vecPosition;
    std::string filePath;
};




void testPositionTable();


