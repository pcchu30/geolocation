#include "PositionTable.h"
#include "Position.h"
#include "Position2.h"
#include "utils.h"


#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/assign.hpp>

using namespace std;
using namespace boost::assign;

typedef boost::geometry::model::d2::point_xy<double> point;

void PositionTable::setTable(){
        
    ifstream myFile(filePath);
        
    if (!myFile.is_open()) throw runtime_error("Could not open file");
        
    string line;
        
    if(myFile.good()) {
        // Skip the first line in the file
        getline(myFile, line);
    }
        
    while (getline(myFile,line)) {
        
        istringstream iss(line);
        string token;
        vector< string > vec;
            
        while(getline(iss, token, '\t')) {
            vec.push_back(token);
        }            
        Position p(vec[0], stod(vec[1]), stod(vec[2]));
        vecPosition.push_back(p);
    }
    // Close file
    myFile.close();
}


void PositionTable::sortTable() {
    sort(vecPosition.begin(), vecPosition.end(), [](Position&a, Position&b)->bool {return a.getTimestamp()<b.getTimestamp();});
}


// Overloading operator< for lower_bound
static bool operator<(const Position& left, unsigned long int right) {
    return (left.getTimestamp() < right);
}



// Overloading operator< for upper_bound
static bool operator<(unsigned long int left, const Position& right) {
    return (left < right.getTimestamp());
}


vector< Position > PositionTable::lessThan(unsigned long int upperTimestamp){
    vector<Position>::iterator up;
    up = upper_bound (vecPosition.begin(), vecPosition.end(), upperTimestamp);
    return vector< Position >(vecPosition.begin(), up);
}


vector< Position >  PositionTable::greaterThan(unsigned long int lowerTimestamp){
    vector<Position>::iterator up;
    up = upper_bound (vecPosition.begin(), vecPosition.end(), lowerTimestamp);
    return vector< Position >(up, vecPosition.end());
}




vector< Position > PositionTable::between(unsigned long int start, unsigned long int end){
    if (end > start){    
        vector<Position>::iterator st, ed;
        st = upper_bound (vecPosition.begin(), vecPosition.end(), start);
        ed = upper_bound (vecPosition.begin(), vecPosition.end(), end);
        return vector< Position >(st, ed);
    } else {
        throw "The end time must be more than the start time";
    }
}


// prepare data for Kalman filter
// each time m points are sent to kalman filter, and each point has x and y coordinates, so we have 2*m measurements.
vector< vector< Position > > PositionTable::getKalmanMatrix(int m){
    vector< vector< Position > > output;
    int sz = vecPosition.size(); 
    int row = sz/m;    
    int rem = sz % m;
    int column = m;
    cout << "row of input = " << sz << endl;
    cout << "row of output = " << row << ", column = " << m << ", " << rem << " points are left" << endl;
    int l = 0;
    for (int i = 0; i < row; i++){
        vector< Position > r;
        for (int j = 0; j < column; j++){
            r.push_back(vecPosition[l]);
            //cout << l << "\n";
            l++;
        }
        output.push_back(r);         
    }
    
    return output;      
}

vector< Position2 > PositionTable::resample(unsigned long int seconds){
    vector< Position2 > vecP2;     
    unsigned long int start = vecPosition[0].getTimestamp();
    Position2 p(start, start, vecPosition[0].getLongitude(), vecPosition[0].getLatitude());  
    vecP2.push_back(p);
    unsigned long int deltaT = seconds;
    unsigned long int index = start;
    while (true){
        index += deltaT;
        vector<Position>::iterator low;
        low = lower_bound (vecPosition.begin(), vecPosition.end(), index);
        Position2 p2(index, (low-1)->getTimestamp(), (low-1)->getLongitude(), (low-1)->getLatitude());
        vecP2.push_back(p2);
        if (index > vecPosition.back().getTimestamp()){
            break;
        }
    }
    /*
    for (unsigned long int index = (start + deltaT); index <= vecPosition.back().getTimestamp(); index += deltaT ){
        vector<Position>::iterator low;
        low = lower_bound (vecPosition.begin(), vecPosition.end(), index);
        Position2 p2(index, (low-1)->getTimestamp(), (low-1)->getLongitude(), (low-1)->getLatitude());
        vecP2.push_back(p2);
    }
    */
    return vecP2;
}

//go through every point from database linestring, and then find the closest point from vecPosition, and add it to new polyline.
void PositionTable::writeAdjustedLineString(std::string path, std::string output){ 
    // Declare linestring
    boost::geometry::model::linestring<point> polyline; 
    boost::geometry::model::linestring<point> b;
    // read linestring from wkt file, function readWkt is located in utils.h
    string lineString = readWkt(path);
    // convert string to geometry type
    boost::geometry::read_wkt(lineString, b);
    point minPoint; 
    // outer loop go through every point from database linestring, and then find the closest point from vecPosition, and add it to new polyline.
    for (auto p = b.begin(); p != b.end(); ++p ){
        double minDistance = 100000;     
        for (auto element = vecPosition.begin(); element != vecPosition.end(); ++element ){
            point p1(element->getLongitude(), element->getLatitude());
            double d = boost::geometry::distance(*p, p1);          
            if (d < minDistance){
                minDistance = d;
                minPoint = p1;
            }
        }
        // Use Boost.Assign to initialize a linestring
        polyline += minPoint;
        //cout << minPoint.get<0>() << ", " << minPoint.get<1>() << endl;
    }
    //cout << boost::geometry::wkt(polyline) << endl;
    cout << "size of database linestring = " << b.size() << ", " << "size of the new closest linestring = " << polyline.size() << endl;
    // function writeLinestringWkt is located in utils.h    
    writeLinestringWkt(output, polyline);
}


void PositionTable::displayContents(vector< Position >& vecP){
    cout.precision(6);
    int id = 0;
    for (auto element = vecP.cbegin(); element != vecP.cend(); ++element ){
        cout << id << " " <<  element->getDateTime() << " " << element->getTimestamp() << 
        " " << fixed << (*element).getLongitude() << " " << (*element).getLatitude() << 
        " " << (*element).getTZone() << endl;
        id++;
    }
}
/////////////////////////////////
//
//  Tests
//  
/////////////////////////////////


static void testSetTable(){
    PositionTable pt("data/test_data_1.csv");
    pt.setTable();
    pt.displayContents();
    vector< Position > vecP = pt.getTable();
    ASSERT( vecP[8].getDateTime()=="2017-11-23 08:59:44" );
    ASSERT( vecP[7].getTimestamp()==1511398799 );
    ASSERT( vecP[3].getLongitude()==121.427994 );
    ASSERT( vecP[4].getLatitude()==25.028442 );
    ASSERT( vecP[5].getTZone()=="Asia/Taipei" );
}

static void testSortTable(){
    PositionTable pt("data/test_data_1.csv");
    pt.setTable();
    pt.sortTable();
    pt.displayContents();
    vector< Position > vecP = pt.getTable();
    ASSERT( vecP[6].getDateTime()=="2017-11-23 08:59:44" );
    ASSERT( vecP[6].getTimestamp()==1511398784 );
    ASSERT( vecP[6].getLongitude()==121.485184 );
    ASSERT( vecP[6].getLatitude()==25.054560 );
    ASSERT( vecP[6].getTZone()=="Asia/Taipei" );
}


static void testLessThan(){
    PositionTable pt("data/test_data_1.csv");
    pt.setTable();
    pt.sortTable();
    
    string bound("2017-11-23 08:02:34");
    vector<string> twoParts = dateTimeSplit(bound);
    string tZone="Asia/Taipei";
    unsigned long int upperTimestamp = stringToTimestamp(twoParts[0], twoParts[1], tZone); 
    
    vector< Position > vecP = pt.lessThan(upperTimestamp);
    ASSERT( vecP[1].getDateTime()=="2017-11-23 08:02:34" );
    pt.displayContents(vecP);

    // extreme value test, "2017-11-22 08:02:34" is less than the first element in the range. 
    bound = "2017-11-22 08:02:34";
    twoParts = dateTimeSplit(bound);
    tZone="Asia/Taipei";
    upperTimestamp = stringToTimestamp(twoParts[0], twoParts[1], tZone);
    vecP = pt.lessThan(upperTimestamp);
    ASSERT( vecP.size()==0 );
    //cout << "size of vecP = " << vecP.size() << ", " << twoParts[0] << ", " <<  twoParts[1] << endl;

    // extreme value test, "2017-11-24 08:02:34" is more than the end element in the range. 
    bound = "2017-11-24 08:02:34";
    twoParts = dateTimeSplit(bound);
    tZone="Asia/Taipei";
    upperTimestamp = stringToTimestamp(twoParts[0], twoParts[1], tZone);
    vecP = pt.lessThan(upperTimestamp);
    ASSERT( vecP.size()==9 );
    ASSERT( vecP[8].getDateTime()=="2017-11-23 08:59:59" );
    //cout << "size of vecP = " << vecP.size() << ", " << twoParts[0] << ", " <<  twoParts[1] << endl;
    //pt.displayContents(vecP);
}


static void testGreaterThan(){
    PositionTable pt("data/test_data_1.csv");
    pt.setTable();
    pt.sortTable();
    
    string bound("2017-11-23 08:54:10");
    vector<string> twoParts = dateTimeSplit(bound);
    string tZone="Asia/Taipei";
    unsigned long int lowerTimestamp = stringToTimestamp(twoParts[0], twoParts[1], tZone);
 
    vector< Position > vecP = pt.greaterThan(lowerTimestamp);
    ASSERT( vecP.size()==3 );
    ASSERT( vecP[2].getDateTime()=="2017-11-23 08:59:59" );
    pt.displayContents(vecP);
}


static void testBetween(){
    PositionTable pt("data/test_data_1.csv");
    pt.setTable();
    pt.sortTable();
    
    string b1("2017-11-23 08:02:34");
    vector<string> twoParts = dateTimeSplit(b1);
    string tZone="Asia/Taipei";
    unsigned long int start = stringToTimestamp(twoParts[0], twoParts[1], tZone);

    string b2("2017-11-23 08:54:10");
    vector<string> twoParts2 = dateTimeSplit(b2);
    unsigned long int end = stringToTimestamp(twoParts2[0], twoParts2[1], tZone);
    
    try{
        vector< Position > vecP = pt.between(start, end);
        ASSERT( vecP.size()==4 );
        ASSERT( vecP[3].getDateTime()=="2017-11-23 08:54:10" );
        pt.displayContents(vecP);
    }
    catch (const char* message) {
        cout << message << endl;
    }
    
}

void testPositionTable(){
    TEST( testSetTable );
    TEST( testSortTable );
    TEST( testLessThan );
    TEST( testGreaterThan );
    TEST( testBetween );
}


