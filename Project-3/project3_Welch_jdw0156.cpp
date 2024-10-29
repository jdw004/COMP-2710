/*
 * Author: John Welch
 * ID: jdw0156
 * Filename: project2_Welch_jdw0156_v3.cpp
 * Compile: g++ project2_Welch_jdw0156_v3.cpp -o project2_Welch_jdw0156_v3
 * Resources: Internet for C++ syntax, merge sort help, and Dr Li slides
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>


using namespace std;

const int MAX_SIZE = 1000;

// Check if file is Valid
bool isValidFloat(const string& str) {
    try {
        size_t pos;
        stof(str, &pos);
        return pos == str.length(); 
    } catch (const invalid_argument&) {
        return false;
    } catch (const out_of_range&) {
        return false;
    }
}


// Function to read file and store values in array
int readFile(float userArray[], ifstream& instream) {
    int index = 0;
    string line;

    while (getline(instream, line)) {
        if (isValidFloat(line)) {
            userArray[index++] = stof(line);  
        } else {
            // If invalid return -1
            return -1;
        }
    }
    return index;
}


// Function to write file
void writeFile(float outputArray[], int outputArray_size, float mean, float median, float mode, const string& outputFilename) {
    ofstream outFile(outputFilename);
    if (outFile.is_open()) {
        outFile << "*** Summarized Statistics ***\n";
        outFile << "\n";
        outFile << "The orderly sorted list of " << outputArray_size << " values is:\n";
        for (int i = 0; i < outputArray_size; ++i) {
            outFile << outputArray[i] << " ";
        }
        outFile << "\n";

        outFile << "\nThe mean is " << mean << "\n";
        outFile << "The median is " << median << "\n";
        outFile << "The mode is " << mode << "\n";
        outFile.close();
    } else {
        cerr << "Unable to open file";
    }

    cout << "*** File " << outputFilename << " has been written to disk ***" << endl;
}

// Merge two sorted subarrays into a one array
void merge(float arr[], int l, int m, int r) {
    int numLeft = m - l + 1;
    int numRight = r - m;
    float L[numLeft], R[numRight];

    // Copy vals to temp arrays
    for (int i = 0; i < numLeft; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < numRight; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays together
    int i = 0, j = 0, k = l;
    while (i < numLeft && j < numRight) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements for left
    while (i < numLeft) {
        arr[k] = L[i];
        i++;
        k++;
    }
    // Copy remaining elements for right
    while (j < numRight) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Recursive Merge Sort function
void mergeSort(float arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Recursively sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

// Convert 2D array to 1D and sort using Merge Sort
int sort2DArray(float array[][MAX_SIZE], int arraySizes[], int numFiles, float sortedArray[], int totalNumCount) {
    // Convert 2D array to 1D array
    int idx = 0;
    for (int i = 1; i <= numFiles; i++) {
        for (int j = 0; j < arraySizes[i]; j++) {
            sortedArray[idx++] = array[i][j];
        }
    }

    // Sort 1D array using mergesort
    mergeSort(sortedArray, 0, totalNumCount - 1);
    return totalNumCount;
}


// Function to find Mean of the array
float findMean(float array[], int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum / size;
}

// Function to find Median of the array
float findMedian(float sorted_array[], int size) {
    if (size % 2 == 0) {
        return (sorted_array[size / 2 - 1] + sorted_array[size / 2]) / 2.0;
    }
    return sorted_array[size / 2];
}

// Function to find Mode of the array
float findMode(float sorted_array[], int size) {
    int maxCount = 1, currentCount = 1;
    float mode = sorted_array[0];
    vector<float> modes = {mode};

    for (int i = 1; i < size; i++) {
        if (sorted_array[i] == sorted_array[i - 1]) {
            currentCount++;
        } else {
            currentCount = 1;
        }

        if (currentCount > maxCount) {
            maxCount = currentCount;
            modes.clear();
            modes.push_back(sorted_array[i]);
        } else if (currentCount == maxCount) {
            modes.push_back(sorted_array[i]);
        }
    }

    float modeSum = 0;
    for (float m : modes) {
        modeSum += m;
    }
    return modeSum / modes.size();  // Average of multiple modes if needed
}

int main() {
    cout << endl << "*** Welcome to John's Data Analyzer ***\n";
    cout << "Enter the number of files to read: ";
    int numFiles;
    cin >> numFiles;
    cout << endl;

    int count = 1;
    int totalNumCount = 0;
    ifstream instream;
    int arraySizes[numFiles + 1];
    float array[numFiles + 1][MAX_SIZE];
    float tempArray[MAX_SIZE];

    // Loop until all files are read
    while (numFiles > 0) {
        string filename;
        bool fileReadSuccessfully = false;
        
        while (!fileReadSuccessfully) {
            cout << "Enter the filename for file " << count << ": ";
            cin >> filename;

            if (filename == "quit") {
                if (count == 1) {
                    cout << "Input cancelled. Proceeding to calculation..." << endl;
                    cout << endl;
                    cout << "*** Goodbye. ***" << endl;
                    return 0;
                } 
                else {
                    cout << "Input cancelled. Proceeding to calculation..." << endl;
                    break;
                }
            }

            instream.open(filename.c_str());
            if (instream.fail()) {
                cout << "File not found. Please try again.\n";
                instream.clear();
                continue;
            }

            // Read values from file and check if valid
            int numValidEntries = readFile(tempArray, instream);
            instream.close();

            if (numValidEntries == -1) {
                cout << "Not an input file. Illegal content/structure detected. Please try again.\n";
                cout << endl;
                continue; 
            } else {
                // File is valid
                fileReadSuccessfully = true;
                arraySizes[count] = numValidEntries;
                totalNumCount += arraySizes[count];

                cout << "The list of " << arraySizes[count] << " numbers in file " << filename << " is:\n";
                for (int i = 0; i < arraySizes[count]; i++) {
                    cout << tempArray[i] << "\n";
                    array[count][i] = tempArray[i];
                }
                cout << endl;
            }
        }

        if (filename == "quit") break;
        
        count++;
        numFiles--;
    }

    cout << endl;
    cout << "*** Summarized Statistics ***" << endl;
    cout << endl;
    cout << "The orderly sorted list of " << totalNumCount << " values is: " << endl;

    // Sort and print
    float sortedArray[totalNumCount];
    int size = sort2DArray(array, arraySizes, count - 1, sortedArray, totalNumCount);
    for (int i = 0; i < size; i++) {
        cout << sortedArray[i] << " ";
    }
    cout << endl;

    // Find and display statistics
    float mean = findMean(sortedArray, size);
    float median = findMedian(sortedArray, size);
    float mode = findMode(sortedArray, size);

    cout << "The mean is " << mean << endl;
    cout << "The median is " << median << endl;
    cout << "The mode is " << mode << endl;
    cout << endl;

    // Prompt for output filename and write to output file
    cout << "Enter the output filename: ";

    string outputFilename;
    cin >> outputFilename;
    cout << endl;

    writeFile(sortedArray, size, mean, median, mode, outputFilename);

    cout << "*** Goodbye. ***" << endl;
    return 0;
}


