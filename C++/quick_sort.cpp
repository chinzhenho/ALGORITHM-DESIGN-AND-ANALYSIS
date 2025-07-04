#include <iostream>      
#include <fstream>        
#include <sstream>        
#include <vector>         
#include <string>          
#include <chrono>        
#include <iomanip>         

using namespace std;       
using namespace std::chrono; 


class TestQuickSort {
public:
    void main() {
        string input_filename;

        // Ask user for the CSV file name
        cout << "Enter CSV file name: ";
        cin >> input_filename;

        // Check if the input file has the '.csv' extension
        if (input_filename.substr(input_filename.find_last_of('.') + 1) != "csv") {
            cerr << "Error: Invalid file type. Please provide a valid CSV file ending with '.csv'." << endl;
            return; 
        }

        // Load data from CSV into a vector of integer-string pairs
        vector<pair<int, string>> records = load_csv_data(input_filename);

        // If data loading failed, display error and exit
        if (records.empty()) {
            cerr << "Error: Failed to read data from the file '" << input_filename << "'. Please check the file and try again." << endl;
            return;
        }

        // Start measuring the execution time for sorting
        auto start = high_resolution_clock::now();

        // Sort the data using Quick Sort
        quick_sort(records, 0, records.size() - 1);

        // Stop measuring time after sorting is complete
        auto end = high_resolution_clock::now();

        // Calculate the duration in milliseconds with high precision
        duration<double, milli> duration = end - start;
        cout << fixed << setprecision(3);
        cout << "Running time: " << duration.count() << " ms" << endl;

        // Construct output filename using the number of records
        string output_filename = "quick_sort_" + to_string(records.size()) + ".csv";

        // Write the sorted results to the output CSV file
        save_to_csv(records, output_filename);
        cout << "Sorted data has been saved to file: " << output_filename << endl;
    }

private:
    // Load CSV file data and return as a vector of integer-string pairs
    vector<pair<int, string>> load_csv_data(const string& filename) {
        vector<pair<int, string>> data;   // Vector to store parsed rows
        ifstream file(filename);          // Open the file for reading
        string line;                      // To store each line from file

        // If the file couldn't be opened, show an error
        if (!file.is_open()) {
            cerr << "Error: The file '" << filename << "' could not be found or opened." << endl;
            return data; 
        }

        // Read each line of the file
        while (getline(file, line)) {
            stringstream ss(line);       // Create a string stream from the line
            string numStr, word;         // To hold parsed values

            // Try to split the line by comma and read two values
            if (getline(ss, numStr, ',') && getline(ss, word)) {
                try {
                    int number = stoi(numStr);  // Try converting the first value to an integer
                    data.emplace_back(number, word);  // Store the pair in the vector
                } catch (...) {
                    // If conversion fails, skip the line and warn
                    cerr << "Warning: Skipping row with invalid integer value: " << line << endl;
                }
            } else {
                // If line is malformed (missing comma/column), skip it and warn
                cerr << "Warning: Skipping malformed row (expected 2 columns): " << line << endl;
            }
        }

        file.close();
        return data;   
    }

    // Write sorted data to a new CSV file
    void save_to_csv(const vector<pair<int, string>>& data, const string& filename) {
        ofstream file(filename);  

        // Check if file is ready to be written
        if (!file.is_open()) {
            cerr << "Error: Unable to write to the file '" << filename << "'." << endl;
            return;
        }

        // Write each record to the file in CSV format
        for (const auto& record : data) {
            file << record.first << "," << record.second << "\n";
        }

        file.close(); 
    }

    // Recursive Quick Sort function
    void quick_sort(vector<pair<int, string>>& arr, int low, int high) {
        if (low < high) {
            // Partition the array and get the pivot index
            //This line calls the partition function, which rearranges the array so that all elements 
            // less than or equal to the pivot are on the left, 
            // and returns the index where the pivot ends up. The result is stored in pivot_index.
            int pivot_index = partition(arr, low, high);

            // Recursively sort elements before the pivot
            //calls the quick_sort function recursively to sort the left part of the array, 
            // from the low index up to one position before the pivot. 
            // This ensures that all elements less than or equal to the pivot are sorted before the pivot in the array. 
            // It is a key step in the divide-and-conquer approach of quick sort.
            quick_sort(arr, low, pivot_index - 1);

            // Recursively sort elements after the pivot
            //This line recursively sorts the right part of the array, 
            // from just after the pivot (pivot_index + 1) to the high index, 
            // ensuring all elements greater than the pivot are sorted.
            quick_sort(arr, pivot_index + 1, high);
        }
    }

    // Partition function to rearrange elements around the pivot
    int partition(vector<pair<int, string>>& arr, int low, int high) {
        int pivot = arr[high].first;  // Choose last element's integer as pivot
        int i = low - 1;              // Index of smaller element

        // Rearrange elements based on pivot
        for (int j = low; j < high; ++j) { // Loop through the subarray from low to high - 1
            if (arr[j].first <= pivot) {  // If current element is less than or equal to the pivot
                ++i;                      // Move the boundary for smaller elements to the right
                swap(arr[i], arr[j]);  // Swap current element with the element at the new boundary
            }
        }

        swap(arr[i + 1], arr[high]);   // This line swaps the pivot element (at arr[high]) 
                                       // with the first element greater than the pivot (at arr[i + 1]), 
                                       // placing the pivot in its correct sorted position in the array.
        return i + 1;                  // Return the pivot index
    }
};


int main() {
    TestQuickSort sorter; 
    sorter.main();     
    return 0;              
}
