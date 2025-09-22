// theory assignment

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

// WeatherRecord ADT>>
class WeatherRecord {
public:
    string date;
    string city;
    double temperature;

    WeatherRecord(string d = "", string c = "", double t = 0.0) {
        date = d;
        city = c;
        temperature = t;
    }
};

// WeatherDataStorage Class>>
class WeatherDataStorage {
private:
    vector<string> cities;           // List of cities
    vector<int> years;               // List of years
    vector<vector<double>> data;     // 2D array for temperatures
    double sentinel = -999;          // Value for missing data

public:
    // Constructor
    WeatherDataStorage(vector<string> c, vector<int> y) {
        cities = c;
        years = y;
        data.resize(years.size(), vector<double>(cities.size(), sentinel));
    }

    // Map city name to index
    int getCityIndex(string city) {
        for (int i = 0; i < cities.size(); i++) {
            if (cities[i] == city) return i;
        }
        return -1;
    }

    // Map year to index
    int getYearIndex(int year) {
        for (int i = 0; i < years.size(); i++) {
            if (years[i] == year) return i;
        }
        return -1;
    }

    // Populate array (can be used to insert multiple records)
    void populateArray(vector<WeatherRecord> records) {
        for (auto rec : records) {
            int year = stoi(rec.date.substr(6,4)); // extract year from date
            insertRecord(rec.date, rec.city, year, rec.temperature);
        }
    }

    // Insert a new record
    void insertRecord(string date, string city, int year, double temp) {
        int row = getYearIndex(year);
        int col = getCityIndex(city);
        if (row != -1 && col != -1) {
            data[row][col] = temp;
            cout << "Inserted: " << city << " " << year << " -> " << temp << "°C"<<endl;
        } else {
            cout << "Invalid city or year!"<<endl;
        }
    }

    // Delete a record
    void deleteRecord(string city, int year) {
        int row = getYearIndex(year);
        int col = getCityIndex(city);
        if (row != -1 && col != -1) {
            data[row][col] = sentinel;
            cout << "Deleted record for " << city << " in " << year << endl;
        } else {
            cout << "Invalid city or year!"<<endl;
        }
    }

    // Retrieve data for a specific city and year
    void retrieveRecord(string city, int year) {
        int row = getYearIndex(year);
        int col = getCityIndex(city);
        if (row != -1 && col != -1) {
            if (data[row][col] != sentinel)
                cout << "Temperature for " << city << " in " << year << " = " << data[row][col] << "°C"<<endl;
            else
                cout << "No data available for " << city << " in " << year << endl;
        } else {
            cout << "Invalid city or year!"<<endl;
        }
    }

    // Row-major access
    void rowMajorAccess() {
        for (int i = 0; i < years.size(); i++) {
            for (int j = 0; j < cities.size(); j++) {
                cout << "Year " << years[i] << ", City " << cities[j]
                     << " -> " << data[i][j] << "\n";
            }
        }
    }

    // Column-major access
    void columnMajorAccess() {
        for (int j = 0; j < cities.size(); j++) {
            for (int i = 0; i < years.size(); i++) {
                cout << "City " << cities[j] << ", Year " << years[i]
                     << " -> " << data[i][j] << endl;
            }
        }
    }

    // Handle sparse data 
    void handleSparseData() {
        cout << "\nSparse Data Representation (non-missing records only):\n";
        for (int i = 0; i < years.size(); i++) {
            for (int j = 0; j < cities.size(); j++) {
                if (data[i][j] != sentinel) {
                    cout << "Year " << years[i] << ", City " << cities[j]
                         << " -> " << data[i][j] << "°C\n";
                }
            }
        }
    }

    // Analyze and display time & space complexity
    void analyzeComplexity() {
        cout << endl<<"Time Complexity Analysis:  "<<endl;
        cout <<endl<< "insertRecord/deleteRecord/retrieveRecord: O(1)"<<endl;
        cout << "rowMajorAccess/columnMajorAccess: O(Y * C)"<<endl;
        cout <<endl<<"Space Complexity Analysis: "<<endl;
        cout << "2D array storage: O(Y * C)"<<endl;
    }

    // Compare row-major and column-major performance
    void compareEfficiency() {
        auto start = chrono::high_resolution_clock::now();
        rowMajorAccess();
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> rowTime = end - start;

        start = chrono::high_resolution_clock::now();
        columnMajorAccess();
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> colTime = end - start;

        cout << endl<<"Row-major access time: " << rowTime.count() << " seconds"<<endl;
        cout << "Column-major access time: " << colTime.count() << " seconds"<<endl;
    }
};

// Main function
int main() {
    vector<string> cityList = {"Delhi", "Mumbai", "Chennai", "Bengaluru", "Kolkata", "Hyderabad"};
    vector<int> yearList = {2022, 2023, 2024, 2025};

    WeatherDataStorage storage(cityList, yearList);

    // Insert sample data
    storage.insertRecord("01/01/2022", "Delhi", 2022, 24.5);
    storage.insertRecord("01/01/2022", "Mumbai", 2022, 28.3);
    storage.insertRecord("01/01/2023", "Chennai", 2023, 31.2);
    storage.insertRecord("01/01/2023", "Bengaluru", 2023, 22.7);
    storage.insertRecord("01/01/2024", "Kolkata", 2024, 26.5);
    storage.insertRecord("01/01/2024", "Hyderabad", 2024, 27.1);
    storage.insertRecord("01/01/2025", "Delhi", 2025, 29.0);
    storage.insertRecord("01/01/2025", "Mumbai", 2025, 30.2);

    cout << endl<<" Retrieve Example :  "<<endl;
    storage.retrieveRecord("Delhi", 2025);
    storage.retrieveRecord("Chennai", 2023);

    cout <<endl<<" Row Major Access :  "<<endl;
    storage.rowMajorAccess();

    cout << endl<<" Column Major Access :  "<<endl;
    storage.columnMajorAccess();

    cout << endl<<" Sparse Data :  "<<endl;
    storage.handleSparseData();

    cout << endl<<" Efficiency Comparison :  "<<endl;
    storage.compareEfficiency();

    cout << endl<< "  Complexity Analysis :   "<<endl;
    storage.analyzeComplexity();

    cout <<endl<< " Delete Example :  "<<endl;
    storage.deleteRecord("Mumbai", 2025);
    storage.retrieveRecord("Mumbai", 2025);

    return 0;
}
