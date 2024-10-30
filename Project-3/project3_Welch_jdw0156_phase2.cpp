/*
 * Author: John Welch
 * ID: jdw0156
 * Filename: project3_Welch_jdw0156.cpp
 * Compile: g++ project3_Welch_jdw0156.cpp -o project3_Welch_jdw0156
 * Resources: Internet for C++ syntax, struct usage, merge sort help, and Dr Li slides
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct TimeMeasurement {
    float value;
    int hours;
    int minutes;
    int seconds;

    TimeMeasurement(float v, int h, int m, int s) : value(v), hours(h), minutes(m), seconds(s) {}
};

// Check if file is valid
bool isValidFloat(const string& str) {
    try {
        size_t pos;
        stof(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

// Parse a line with value and timestamp (HH.MM.SS)
bool parseLine(const string& line, TimeMeasurement& measurement) {
    stringstream ss(line);
    string valueStr, timeStr;
    getline(ss, valueStr, '\t');
    getline(ss, timeStr);         

    if (!isValidFloat(valueStr)) return false;

    int h, m, s;
    if (sscanf(timeStr.c_str(), "%d.%d.%d", &h, &m, &s) != 3) return false;

    measurement = TimeMeasurement(stof(valueStr), h, m, s);
    return true;
}

// Read file and store values in array
int readFile(vector<TimeMeasurement>& measurements, ifstream& instream) {
    string line;
    int fileCount = 0;  // Track the number of values read from this specific file
    while (getline(instream, line)) {
        TimeMeasurement measurement(0, 0, 0, 0);
        if (parseLine(line, measurement)) {
            measurements.push_back(measurement);
            fileCount++;
        } else {
            return -1;
        }
    }
    return fileCount;
}

// Sorting by value
void mergeByValue(vector<TimeMeasurement>& arr, int l, int m, int r) {
    int numLeft = m - l + 1;
    int numRight = r - m;

    vector<TimeMeasurement> L(arr.begin() + l, arr.begin() + m + 1);
    vector<TimeMeasurement> R(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < numLeft && j < numRight) {
        if (L[i].value <= R[j].value) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < numLeft) arr[k++] = L[i++];
    while (j < numRight) arr[k++] = R[j++];
}

// Sorting by timestamp
void mergeByTime(vector<TimeMeasurement>& arr, int l, int m, int r) {
    int numLeft = m - l + 1;
    int numRight = r - m;

    vector<TimeMeasurement> L(arr.begin() + l, arr.begin() + m + 1);
    vector<TimeMeasurement> R(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;
    while (i < numLeft && j < numRight) {
        if ((L[i].hours < R[j].hours) ||
            (L[i].hours == R[j].hours && L[i].minutes < R[j].minutes) ||
            (L[i].hours == R[j].hours && L[i].minutes == R[j].minutes && L[i].seconds <= R[j].seconds)) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < numLeft) arr[k++] = L[i++];
    while (j < numRight) arr[k++] = R[j++];
}

// Merge sort wrapper
void mergeSort(vector<TimeMeasurement>& arr, int l, int r, bool byValue = true) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, byValue);
        mergeSort(arr, m + 1, r, byValue);
        if (byValue) mergeByValue(arr, l, m, r);
        else mergeByTime(arr, l, m, r);
    }
}

// Find Mean of the array
float findMean(const vector<TimeMeasurement>& measurements) {
    float sum = 0;
    for (const auto& measurement : measurements) sum += measurement.value;
    return sum / measurements.size();
}

// Find Median of the array
float findMedian(const vector<TimeMeasurement>& measurements) {
    int size = measurements.size();
    return size % 2 == 0 ? (measurements[size / 2 - 1].value + measurements[size / 2].value) / 2.0 : measurements[size / 2].value;
}

// Find Mode of the array
float findMode(const vector<TimeMeasurement>& measurements) {
    int maxCount = 1, currentCount = 1;
    vector<float> modes = {measurements[0].value};
    for (size_t i = 1; i < measurements.size(); ++i) {
        if (measurements[i].value == measurements[i - 1].value) {
            currentCount++;
        } else {
            currentCount = 1;
        }
        if (currentCount > maxCount) {
            maxCount = currentCount;
            modes.clear();
            modes.push_back(measurements[i].value);
        } else if (currentCount == maxCount) {
            modes.push_back(measurements[i].value);
        }
    }
    float modeSum = 0;
    for (float m : modes) modeSum += m;
    return modeSum / modes.size();
}

// Write file with statistics
void writeFile(const vector<TimeMeasurement>& measurements, const vector<TimeMeasurement>& timeSorted, float mean, float median, float mode, float meanTime, float medianTime, float modeTime, const string& outputFilename) {
    ofstream outFile(outputFilename);
    if (outFile.is_open()) {
        outFile << "*** Summarized Statistics ***\n\n";
        outFile << "The orderly sorted list of values is:\n";
        for (const auto& measurement : measurements) outFile << measurement.value << " ";
        outFile << "\n\nThe mean is " << mean << "\nThe median is " << median << "\nThe mode is " << mode << "\n\n";

        outFile << "The chronologically sorted list of values is:\n";
        for (const auto& measurement : timeSorted) outFile << measurement.value << " ";
        outFile << "\n\nThe mean is " << meanTime << "\nThe median is " << medianTime << "\nThe mode is " << modeTime << "\n";
        outFile.close();
    } 
    else {
        cerr << "Unable to open file";
    }
}

int main() {
    int count = 0;
    cout << "*** Welcome to John's Data Analyzer ***\nEnter the number of files to read: ";
    int numFiles;
    cin >> numFiles;
    cout << "\n";

    vector<TimeMeasurement> measurements;
    ifstream instream;

    int i = 0;  
        while (i < numFiles) {
        string filename;
        cout << "Enter the filename for file " << i + 1 << ": ";
        cin >> filename;
        if (filename == "quit") {
            if (i == 0) {
                cout << "Input cancelled. Proceeding to calculation...\n";
                cout << "\n*** Goodbye. ***" << endl;
                return 0;  
            } else {
                cout << "Input cancelled. Proceeding to calculation...\n\n";
                break; 
            }
    }


        instream.open(filename);
        if (!instream) {
            cout << "File not found. Please try again.\n\n";
            instream.clear();
            instream.close();
            continue;
        }

        vector<TimeMeasurement> fileMeasurements;  
        int count = readFile(fileMeasurements, instream); 

        if (count == -1) {
            cout << "Not an input file. Illegal content/structure detected. Please try again.\n\n";
            instream.clear();
            instream.close();
            continue;
        }

        cout << "The list of " << count << " values in file " << filename << " is:\n";
        for (const auto& measurement : fileMeasurements) {
            cout << measurement.value << "\t" << measurement.hours << "." << measurement.minutes << "." << measurement.seconds << endl;
        }
        measurements.insert(measurements.end(), fileMeasurements.begin(), fileMeasurements.end());
        cout << "\n";
        instream.close();
        i++;
    }

    vector<TimeMeasurement> valueSorted = measurements;
    vector<TimeMeasurement> timeSorted = measurements;

    mergeSort(valueSorted, 0, valueSorted.size() - 1, true);
    mergeSort(timeSorted, 0, timeSorted.size() - 1, false);

    cout << "*** Summarized Statistics ***\n";

    float mean = findMean(valueSorted);
    float median = findMedian(valueSorted);
    float mode = findMode(valueSorted);
    float modeTime = findMode(timeSorted);
    float medianTime = findMedian(timeSorted);
    float meanTime = findMean(timeSorted);

    cout << "\nThe orderly sorted list of " << measurements.size() << " values is:\n";
    for (const auto& measurement : valueSorted) cout << measurement.value << " ";
    cout << "\n\nThe mean is " << mean << "\nThe median is " << median << "\nThe mode is " << mode << "\n\n";
    cout << "The chronologically sorted list of " << measurements.size() << " values is:\n";
    for (const auto& measurement : timeSorted) cout << measurement.value << " ";
    cout << "\n\nThe mean is " << meanTime << "\nThe median is " << medianTime << "\nThe mode is " << modeTime << "\n\n";

    string outputFilename;
    cout << "Enter the output filename: ";
    cin >> outputFilename;
    writeFile(valueSorted, timeSorted, mean, median, mode, meanTime, medianTime, modeTime, outputFilename);
    cout << "\n*** File " << outputFilename << " has been written to disk ***\n*** Goodbye. ***" << endl;
    return 0;
}
