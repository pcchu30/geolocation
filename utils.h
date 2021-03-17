#pragma once

#include "stdafx.h"
#include "Position2.h"
#include "Position.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/geometries/linestring.hpp>

// Define a type
typedef boost::geometry::model::d2::point_xy<double> point;

// Using Boost.Date_Time(example, Seconds Since Epoch): convert string to Unix Timestamp
// Example of calculating seconds elapsed since epoch (1970-Jan-1) using local_date_time.

unsigned long int stringToTimestamp(const std::string& date_str, const std::string& time_str, const std::string& timezone_str);

//split dateTime into date and time

std::vector<std::string> dateTimeSplit(const std::string& dateTime);

// display vector< Position2 >
void displayVecP2Contents(std::vector< Position2 >& vecP2);


// write vector< Position2 > in a csv file
void writeVecP2Csv(std::string filename, std::vector< Position2 >& vecP2);

//
void vecP2ToLinestringOnScreen(std::vector< Position2 >& vecP2);

//
void matrixToLinestringOnScreen(std::vector<std::vector<double>>& mtxD);

// read wkt file.
std::string readWkt(std::string path);

// write geometry linestring to a file
void writeLinestringWkt(std::string path, boost::geometry::model::linestring<point> polyline);

// convert 2D matrix to linestring, and then write it to a file.
void writeLinestring(std::vector<std::vector<double>> kStates, std::string output);

// display vector< vector< Position > > measurements
void display2DMeasurements(std::vector< std::vector< Position > > measurements);

/*
 * Test function
 */

void testUtils();

