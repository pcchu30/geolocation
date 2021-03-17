#include "utils.h"
#include <boost/algorithm/string.hpp>
#include "boost/date_time/local_time/local_time.hpp"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/geometries/linestring.hpp>
#include <boost/assign.hpp>

using namespace boost::assign;

using namespace std;
using namespace boost::gregorian; 
using namespace boost::local_time;
using namespace boost::posix_time;

// Define a type
typedef boost::geometry::model::d2::point_xy<double> point;


unsigned long int stringToTimestamp(const string& date_str, const string& time_str,const string& timezone_str){
    tz_database tz_db;
    try {
        tz_db.load_from_file("data/date_time_zonespec.csv");
    }catch(data_not_accessible dna) {
        std::cerr << "Error with time zone data file: " << dna.what() << std::endl;
        exit(EXIT_FAILURE);
    }catch(bad_field_count bfc) {
        std::cerr << "Error with time zone data file: " << bfc.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    //time_zone_ptr nyc_tz = tz_db.time_zone_from_region("America/New_York");
    //date in_date(2004,10,04);
    //time_duration td(12,14,32);
    time_zone_ptr nyc_tz = tz_db.time_zone_from_region(timezone_str);
    date in_date(from_string(date_str));
    time_duration td(duration_from_string(time_str));
    // construct with local time value
    // create not-a-date-time if invalid (eg: in dst transition)
    local_date_time nyc_time(in_date, td, nyc_tz, local_date_time::NOT_DATE_TIME_ON_ERROR);

    //std::cout << nyc_time << std::endl;

    ptime time_t_epoch(date(1970,1,1)); 
    //std::cout << time_t_epoch << std::endl;

    // first convert nyc_time to utc via the utc_time() 
    // call and subtract the ptime.
    time_duration diff = nyc_time.utc_time() - time_t_epoch;

    //Expected 1096906472
    //std::cout << "Seconds diff: " << diff.total_seconds() << std::endl;

    return diff.total_seconds();
}



vector<string> dateTimeSplit(const string& dateTime) {
    string delimiters(" ");
    vector<string> parts;
    boost::split(parts, dateTime, boost::is_any_of(delimiters));
    return parts;
}


void displayVecP2Contents(vector< Position2 >& vecP2){
    cout.precision(6);
    for (auto element = vecP2.cbegin(); element != vecP2.cend(); ++element )
        cout <<  element->getTimestamp() << " " << element->getOriginalTimestamp() << 
        " " << fixed << (*element).getLongitude() << " " << (*element).getLatitude() << endl;
}


// 
void writeVecP2Csv(string filename, vector< Position2 >& vecP2){
    // Make a CSV file with one column of integer values
    // filename - the name of the file
    // vals - an integer vector of values
    
    // Create an output filestream object
    ofstream myFile(filename);
    
    // Send the column name to the stream
    myFile << "TIMESTAMP" << "," << "LON" << "," << "LAT" << "\n";
    
    int n = vecP2.size();

    // Send data to the stream
    cout.precision(6);
    for(int i = 0; i < n; ++i)
    {
        myFile << vecP2[i].getTimestamp() << "," << fixed << vecP2[i].getLongitude() << "," << vecP2[i].getLatitude() << "\n";
    }
    
    // Close the file
    myFile.close();
}


void vecP2ToLinestringOnScreen(vector< Position2 >& vecP2){
    // Declare a linestring
    boost::geometry::model::linestring<point> polyline;
    // Use Boost.Assign to initialize a linestring
    int n = vecP2.size();
    for (int i = 0; i < n; i++) { 
        polyline += point(vecP2[i].getLongitude(), vecP2[i].getLatitude());
    } 
    /*
    polyline += point(0, 0), point(3, 3), point(5, 1), point(6, 2),
                point(8, 0), point(4, -4), point(1, -1), point(3, 2);
    */
    cout << boost::geometry::wkt(polyline) << endl;

}


// convert 2D matrix to linestring, and then output it to screen.
void matrixToLinestringOnScreen(vector<vector<double>>& mtxD){
    // Declare a linestring
    boost::geometry::model::linestring<point> polyline;
    // Use Boost.Assign to initialize a linestring
    int n = mtxD.size();
    for (int i = 0; i < n; i++) { 
        polyline += point(mtxD[i][0], mtxD[i][1]);
    } 
    
    cout << boost::geometry::wkt(polyline) << endl;
}


std::string readWkt(std::string path){
    ifstream myFile;

    string fileContents;
    myFile.open(path, ios_base::in);
    if (myFile.is_open()){
        cout << "File open successful. It contains: " << endl;
        
        
        while (myFile.good()){
            getline (myFile, fileContents);
            //cout << fileContents << endl;
        }
        cout << "Finished reading file, will close now" << endl;
        myFile.close();
    }
    else
        cout << "open() failed: check if file is in right folder" << endl;

    return fileContents;      
}

// write linestring to a file.
void writeLinestringWkt(std::string path, boost::geometry::model::linestring<point> polyline){
    ofstream myFile;
    myFile.open(path, ios_base::out);

    cout.precision(6);    
    
    if (myFile.is_open()){
        cout << "File open successful" << endl;
        
        myFile << fixed << boost::geometry::wkt(polyline) << endl;

        cout << "Finished writing to file, will close now" << endl;
        myFile.close();                                
    }
}


// convert 2D matrix to linestring, and then write it to a file.
void writeLinestring(std::vector<std::vector<double>> kStates, std::string output){
    boost::geometry::model::linestring<point> polyline;
    int row = kStates.size();
    for (int i=0; i<row; i++){
        point p(kStates[i][0], kStates[i][1]);
        polyline.push_back(p);        
    }
    cout << "size of polyline = " << polyline.size() << endl;
    writeLinestringWkt(output, polyline);
}

// display measurements on screen.
void display2DMeasurements(std::vector< std::vector< Position > > measurements){
    int row = measurements.size();
    int column = measurements[0].size();
    int id = 0;
    for (int i=0; i<row; i++){
        for (int j=0; j<column; j++){
            cout << id << " " << measurements[i][j].getLongitude() << ", " << measurements[i][j].getLatitude() << endl;
            id++;        
        }        
    }
}

///////////////////////////////////////////////
//
//   Tests
//
///////////////////////////////////////////////

static void testStringToTimestamp() {

    string date_str("2004-10-04");
    string time_str("12:14:32");
    string timezone_str("America/New_York");

    unsigned long int res = stringToTimestamp(date_str, time_str, timezone_str);
    ASSERT( res==1096906472 );

    cout << res << endl;
    // ASSERT_APPROX_EQUAL( mean(v), 0.5, 0.1);
}

static void testDateTimeSplit() {
    string dateTime("2004-10-04 12:14:32"); 
    vector<string> twoParts = dateTimeSplit(dateTime);

    ASSERT( twoParts[0]=="2004-10-04" );
    ASSERT( twoParts[1]=="12:14:32" );
    cout << twoParts[0] << "\n" << twoParts[1] << endl;
}

void testUtils(){
    TEST( testStringToTimestamp );
    TEST( testDateTimeSplit );
}





