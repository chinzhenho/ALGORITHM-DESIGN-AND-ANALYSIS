#include <iostream>     
#include <fstream>     
#include <sstream>      
#include <vector>       
#include <string>       
#include <limits>       

using namespace std;     


class TestMergeSortStep {
public:
    void main() {
        string input_filename;
        cout << "Enter CSV file name : ";          // Ask user to input the CSV filename
        cin >> input_filename;                     // Store input filename

        // Check if file has .csv extension
        if (input_filename.size() < 4 || input_filename.substr(input_filename.size() - 4) != ".csv") {
            cerr << "Error: Please provide a valid CSV file." << endl;
            return;                                
        }

        // Load all data from the CSV file
        vector<pair<int, string>> full_data = read_data_from_csv(input_filename);
        if (full_data.empty()) {
            cerr << "Error: Could not read data from " << input_filename << endl;
            return;                                
        }

        int start_row, end_row;
        while (true) {
            cout << "Enter start row : ";           // Ask user for the starting row
            if (!(cin >> start_row)) {              // Validate input is an integer
                cout << "Invalid input. Please enter integers." << endl;
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                continue;
            }
            cout << "Enter end row : ";             // Ask user for the ending row
            if (!(cin >> end_row)) {                // Validate input is an integer
                cout << "Invalid input. Please enter integers." << endl;
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                continue;
            }
            --start_row;                            // Convert to 0-based index
            --end_row;
            if (start_row < 0 || end_row >= full_data.size() || start_row > end_row) {
                cout << "Invalid row range. Please enter values between 1 and " << full_data.size() << ", with start <= end." << endl;
                continue;                           
            }
            break;                               
        }

        // Extract the selected subset of data
        vector<pair<int, string>> data_subset(full_data.begin() + start_row, full_data.begin() + end_row + 1);

        // Create output filename with selected row range
        string output_filename = "merge_sort_step_" + to_string(start_row + 1) + "_" + to_string(end_row + 1) + ".txt";

        // Open the output file to log sorting steps
        ofstream output_file(output_filename);
        if (!output_file.is_open()) {
            cerr << "Error opening output file." << endl;
            return;
        }

        // Write the initial state of the subset to the output file
        output_file << "[" << format_output_list(data_subset) << "]\n";

        // Perform merge sort with step logging
        _mergeSort(data_subset, 0, data_subset.size() - 1, output_file);

        // Notify user sorting is completed
        cout << "Sorting steps written to " << output_filename << endl;
    }

private:
    // Read and parse data from a CSV file into a vector of (int, string) pairs
    vector<pair<int, string>> read_data_from_csv(const string& filename) {
        vector<pair<int, string>> data;
        ifstream file(filename);          // Open file for reading
        string line;

        if (!file.is_open()) {           // Check if file opened successfully
            cerr << "File '" << filename << "' not found." << endl;
            return data;
        }

        int row_num = 0;
        while (getline(file, line)) {    // Read each line
            ++row_num;
            stringstream ss(line);
            string num_str, word;
            if (getline(ss, num_str, ',') && getline(ss, word)) { // Parse number and word
                try {
                    int number = stoi(num_str);      // Convert to integer
                    data.emplace_back(number, word); // Store valid row
                } catch (...) {
                    cerr << "Skipping row " << row_num << " due to non-integer value: " << line << endl;
                }
            } else {
                cerr << "Skipping row " << row_num << " due to malformed row: " << line << endl;
            }
        }

        return data;  
    }

    // Format the vector into a readable string for logging
    string format_output_list(const vector<pair<int, string>>& data_list) {
        stringstream ss;
        for (size_t i = 0; i < data_list.size(); ++i) { //This loop formats the contents of the data_list vector into a readable string.
            ss << data_list[i].first << "/" << data_list[i].second; //each element, it adds the integer and string (separated by a slash) to the stringstream ss.
            if (i < data_list.size() - 1) ss << ", "; //If the element is not the last one, it also adds a comma 
        }
        return ss.str();  
    }

    // Recursive merge sort function with output logging
    void _mergeSort(vector<pair<int, string>>& S, int left, int right, ofstream& output_file) {
        if (left < right) {
            int mid = (left + right) / 2;                  // Find midpoint
            _mergeSort(S, left, mid, output_file);         // Sort left half
            _mergeSort(S, mid + 1, right, output_file);    // Sort right half
            merge(S, left, mid, right, output_file);       // Merge both halves
        }
    }

    // Merge two sorted halves of the array with logging
    void merge(vector<pair<int, string>>& S, int left, int mid, int right, ofstream& output_file) {
        int n1 = mid - left + 1;       // Size of left subarray
        int n2 = right - mid;          // Size of right subarray

        vector<pair<int, string>> L(n1), R(n2);  // Temporary arrays

        // part of the merge step in merge sort.
        // It works by copying elements from the original array S, starting at index left, 
        // into the temporary array L. The loop runs n1 times, 
        // so it copies all elements from S[left] up to S[mid] into L[0] up to L[n1-1].
        for (int i = 0; i < n1; ++i) L[i] = S[left + i];        // Copy left half

        //This line copies elements from the right half of the array S (from index mid + 1 to right) into the temporary array R. 
        // The loop runs n2 times, so it fills R[0] to R[n2-1] with the elements S[mid + 1] to S[right].
        for (int j = 0; j < n2; ++j) R[j] = S[mid + 1 + j];     // Copy right half


        //i is used to track the current position in the left temporary array (L).
        //j is used to track the current position in the right temporary array (R).
        //k = left: index for placing merged elements back into the original array S, starting at the left boundary. 
        // These variables help merge the two sorted halves back into S in the correct order.
        int i = 0, j = 0, k = left;

        // Merge L and R back into S in sorted order
        while (i < n1 && j < n2) {          // Continue while both subarrays have elements left
            if (L[i].first <= R[j].first) { // If current element in left subarray is less than or equal to right
                S[k++] = L[i++];            // Copy element from left subarray to S and move to next in left
            } else {
                S[k++] = R[j++];            // Otherwise, copy element from right subarray to S and move to next in right
            }
        }

        // Copy any remaining elements in L
        while (i < n1) S[k++] = L[i++];
        //This line copies any remaining elements from the left temporary array L into the original array S. 
        // It runs after the main merge loop, in case there are still elements left in L (because all elements in R have already been merged). 
        // For each remaining element, it assigns L[i] to S[k], 
        // then increments both i and k to move to the next positions.

        // Copy any remaining elements in R
        while (j < n2) S[k++] = R[j++];
        //This line copies any remaining elements from the right temporary array R into the original array S. 
        // It runs after the main merge loop, in case there are still elements left in R (because all elements in L have already been merged). 
        // For each remaining element, it assigns R[j] to S[k], 
        // then increments both j and k to move to the next positions.

        // Log current array state after merging
        output_file << "[" << format_output_list(S) << "]\n";
    }
};

int main() {
    TestMergeSortStep sorter;  
    sorter.main();             
    return 0;                  
}
