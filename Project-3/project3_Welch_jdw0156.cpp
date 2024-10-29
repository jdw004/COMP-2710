#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_SIZE = 1000;

// Function to read file and store values in array
int readFile(float userArray[], ifstream& instream) {
    int index = 0;
    instream >> userArray[index];
    while (!instream.eof()) {
        index++;
        instream >> userArray[index];
    }
    return index + 1;  
}

// Function to write file
void writeFile(float outputArray[], int outputArray_size, float mean, float median, float mode, const string& outputFilename) {
    ofstream outFile(outputFilename);
    if (outFile.is_open()) {
        outFile << "*** Summarized Statistics ***\n";
        outFile << "The orderly sorted list of " << outputArray_size << " values is:\n";
        for (int i = 0; i < outputArray_size; ++i) {
            outFile << outputArray[i] << " ";
        }
        outFile << "\nThe mean is " << mean << "\n";
        outFile << "The median is " << median << "\n";
        outFile << "The mode is " << mode << "\n";
        outFile.close();
    } else {
        cerr << "Unable to open file";
    }
}

// Function to convert 2D array to 1D and bubble sort
int sort2DArray(float array[][MAX_SIZE], int arraySizes[], int numFiles, float sortedArray[], int totalNumCount) {
    int idx = 0;
    for (int i = 1; i <= numFiles; i++) {
        for (int j = 0; j < arraySizes[i]; j++) {
            sortedArray[idx++] = array[i][j];
        }
    }

    // Bubble Sorting 1D Array
    for (int i = 0; i < totalNumCount - 1; i++) {
        for (int j = 0; j < totalNumCount - i - 1; j++) {
            if (sortedArray[j] > sortedArray[j + 1]) {
                float temp = sortedArray[j];
                sortedArray[j] = sortedArray[j + 1];
                sortedArray[j + 1] = temp;
            }
        }
    }
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
    int count = 1;
    int totalNumCount = 0;
    bool validFile;
    ifstream instream;

    int arraySizes[numFiles + 1];
    float array[numFiles + 1][MAX_SIZE];
    float tempArray[MAX_SIZE];

    // Loop until all files are read
    while (numFiles > 0) {
        string filename;
        cout << "Enter the filename for file " << count << ": ";
        cin >> filename;

        // Check if user wants to quit
        if (filename == "quit") {
            if (count == 1) {
                cout << "Input cancelled. Proceeding to calculation..." << endl;
                return 0;
            } else {
                cout << "Input cancelled. Proceeding to calculation..." << endl;
                break;
            }
        }

        instream.open(filename.c_str());
        validFile = instream.good();

        if (!validFile) {
            cout << "Error: Invalid filename, please try again\n";
            instream.clear();
            instream.close();
            continue;
        }

        // Read values from file
        arraySizes[count] = readFile(tempArray, instream);
        totalNumCount += arraySizes[count];
        instream.close();

        cout << "The list of " << arraySizes[count] << " numbers in file " << filename << " is:\n";
        for (int i = 0; i < arraySizes[count]; i++) {
            cout << tempArray[i] << "\n";
        }

        for (int i = 0; i < arraySizes[count]; i++) {
            array[count][i] = tempArray[i];
        }
        cout << endl;
        count++;
        numFiles--;
    }

    // Find and display statistics
    float sortedArray[totalNumCount];
    int size = sort2DArray(array, arraySizes, count - 1, sortedArray, totalNumCount);
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
    writeFile(sortedArray, size, mean, median, mode, outputFilename);

    cout << "*** Goodbye. ***" << endl;
    return 0;
}
