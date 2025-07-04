#include <iostream>    
#include <fstream>    
#include <sstream>     
#include <vector>      
#include <string>      
#include <chrono>  
#include <iomanip>


using namespace std;
using namespace std::chrono;

class TestMergeSort {
public:
    void main() {
        string input_filename;
        cout << "Enter CSV file name: ";  // Ask user to enter CSV file name
        cin >> input_filename;

        // Check if file name ends with ".csv"
        if (input_filename.size() < 4 || input_filename.substr(input_filename.size() - 4) != ".csv") {
            cerr << "Error: Please provide a valid CSV file." << endl; 
            return;  
        }

        // Read data from CSV file into vector of pairs (int, string)
        vector<pair<int, string>> data = read_data_from_csv(input_filename);

        // Check if data was loaded successfully
        if (data.empty()) {
            cerr << "Error: Could not read data from " << input_filename << endl; 
            return; 
        }

        // Record start time before sorting
        auto start_time = high_resolution_clock::now();
        
        // Perform merge sort on data vector
        mergeSort(data);
        
        // Record end time after sorting
        auto end_time = high_resolution_clock::now();

        // Calculate duration in milliseconds
        duration<double, milli> duration = end_time - start_time;
        cout << fixed << setprecision(3);
        cout << "Running time: " << duration.count() << " ms" << endl;  // Print running time with 3 decimal places
        
        // Create output filename based on data size
        string output_filename = "merge_sort_" + to_string(data.size()) + ".csv";
        
        // Write sorted data to output CSV file
        write_data_to_csv(data, output_filename);
        cout << "Sorted data written to " << output_filename << endl;  // Notify user
    }

private:
    // Function to read data from CSV file and return vector of (int, string) pairs
    vector<pair<int, string>> read_data_from_csv(const string& filename) {
        vector<pair<int, string>> data;  // Vector to store data
        ifstream file(filename);         // Open file for reading
        string line;                     // To store each line

        // Check if file opened successfully
        if (!file.is_open()) {
            cerr << "Error: File '" << filename << "' not found." << endl;  
            return data;  
        }

        // Read each line of the file
        while (getline(file, line)) {
            stringstream ss(line);       // Create string stream from line
            string num_str, word;        // Variables to hold split parts
            // Split line by comma into number string and word
            if (getline(ss, num_str, ',') && getline(ss, word)) {
                try {
                    int number = stoi(num_str);        // Convert number string to int
                    data.emplace_back(number, word);   // Add pair to data vector
                } catch (...) {
                    cerr << "Skipping row due to non-integer value: " << line << endl; 
                }
            } else {
                cerr << "Skipping malformed row: " << line << endl;  
            }
        }

        return data;  
    }

    // Function to write sorted data to CSV file
    void write_data_to_csv(const vector<pair<int, string>>& data, const string& filename) {
        ofstream file(filename);   

        // Check if file opened successfully
        if (!file.is_open()) {
            cerr << "An error occurred while writing the CSV: " << filename << endl;  // Error on failure
            return;
        }

        // Write each pair as "int,string" line to file
        for (const auto& item : data) {
            file << item.first << "," << item.second << "\n";
        }
        file.close(); 
    }

    // Public interface for merge sort; calls internal recursive _mergeSort if size > 1
    void mergeSort(vector<pair<int, string>>& S) {
        if (S.size() > 1)
            _mergeSort(S, 0, S.size() - 1);  // Sort entire vector range
    }

    // Recursive merge sort helper function
    void _mergeSort(vector<pair<int, string>>& S, int left, int right) {
        if (left < right) {
            int mid = (left + right) / 2;  // Calculate middle index
            _mergeSort(S, left, mid);       // Recursively sort left half
            _mergeSort(S, mid + 1, right);  // Recursively sort right half
            merge(S, left, mid, right);     // Merge sorted halves
        }
    }

    // Merge two sorted subarrays of S back into S
    void merge(vector<pair<int, string>>& S, int left, int mid, int right) {
        int leftSize = mid - left + 1;   // Size of left subarray
        int rightSize = right - mid;     // Size of right subarray

        // Temporary vectors to hold the subarrays
        vector<pair<int, string>> L(leftSize);
        vector<pair<int, string>> R(rightSize);

        // Copy left half into L
        // part of the merge step in merge sort.
        // It works by copying elements from the original array S, starting at index left, 
        // into the temporary array L. The loop runs n1 times, 
        // so it copies all elements from S[left] up to S[mid] into L[0] up to L[n1-1].
        for (int i = 0; i < leftSize; ++i)
            L[i] = S[left + i];

        // Copy right half into R
        //This line copies elements from the right half of the array S (from index mid + 1 to right) into the temporary array R. 
        // The loop runs n2 times, so it fills R[0] to R[n2-1] with the elements S[mid + 1] to S[right].
        for (int j = 0; j < rightSize; ++j)
            R[j] = S[mid + 1 + j];

        //i is used to track the current position in the left temporary array (L).
        //j is used to track the current position in the right temporary array (R).
        //k = left: index for placing merged elements back into the original array S, starting at the left boundary. 
        // These variables help merge the two sorted halves back into S in the correct order.
        int i = 0, j = 0, k = left;

        // Merge elements back into S by comparing L and R
        while (i < leftSize && j < rightSize) { // Continue while both subarrays have elements left
            if (L[i].first <= R[j].first) {     // If current element in left subarray is less than or equal to right
                S[k++] = L[i++];                // Copy element from left subarray to S and move to next in left
            } else {
                S[k++] = R[j++];                // Otherwise, copy element from right subarray to S and move to next in right
            }
        }

        // Copy any remaining elements from left half
        //This line copies any remaining elements from the left temporary array L into the original array S. 
        // It runs after the main merge loop, in case there are still elements left in L (because all elements in R have already been merged). 
        // For each remaining element, it assigns L[i] to S[k], 
        // then increments both i and k to move to the next positions.
        while (i < leftSize) {
            S[k++] = L[i++];
        }

        // Copy any remaining elements from right half
        //This line copies any remaining elements from the right temporary array R into the original array S. 
        // It runs after the main merge loop, in case there are still elements left in R (because all elements in L have already been merged). 
        // For each remaining element, it assigns R[j] to S[k], 
        // then increments both j and k to move to the next positions.
        while (j < rightSize) {
            S[k++] = R[j++];
        }
    }
};

int main() {
    TestMergeSort sorter;
    sorter.main();         
    return 0;         
}
