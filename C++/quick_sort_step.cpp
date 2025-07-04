#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class TestQuickSortStep {
public:
    void main() {
        string input_filename;
        cout << "Enter CSV file name: ";  // Prompt user to input CSV filename
        cin >> input_filename;

        // Check if filename ends with ".csv"
        if (input_filename.size() < 4 || input_filename.substr(input_filename.size() - 4) != ".csv") {
            cerr << "Error: Please provide a valid CSV file." << endl; // If not CSV, print error
            return;
        }

        // Step 1: Load entire CSV file data into memory
        vector<pair<int, string>> full_data = read_data_from_csv(input_filename);
        if (full_data.empty()) {
            cerr << "Error: Could not read data from " << input_filename << ". Please ensure the file exists." << endl; // Error if file empty or unreadable
            return;
        }

        int start_idx, end_idx;
        while (true) {
            cout << "Enter start row: ";  // Ask user for start row (1-based)
            if (!(cin >> start_idx)) {   // Validate input is integer
                cout << "Invalid input. Please enter integers." << endl;
                cin.clear();              // Reset input stream error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                continue;                 // Retry input
            }

            cout << "Enter end row: ";   // Ask user for end row (1-based)
            if (!(cin >> end_idx)) {     // Validate input is integer
                cout << "Invalid input. Please enter integers." << endl;
                cin.clear();              // Reset input stream error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
                continue;                 // Retry input
            }

            start_idx -= 1; // Convert to 0-based index for vector slicing
            end_idx -= 1;

            // Check if indices are valid and in correct order
            if (start_idx < 0 || end_idx >= full_data.size() || start_idx > end_idx) {
                cout << "Invalid row range. Enter values between 1 and " << full_data.size() << "." << endl;
                continue;                
            }
            break;                       
        }

        // Extract the subset of data between start_idx and end_idx (inclusive)
        vector<pair<int, string>> data_subset(full_data.begin() + start_idx, full_data.begin() + end_idx + 1);

        // Prepare output filename to record sorting steps
        string output_filename = "quick_sort_step_" + to_string(start_idx + 1) + "_" + to_string(end_idx + 1) + ".txt";
        ofstream output_file(output_filename);

        if (!output_file.is_open()) {  // Check if output file can be opened
            cerr << "Error opening file for writing: " << output_filename << endl;
            return;
        }

        // Write initial state of subset to output file (formatted)
        output_file << "[" << join(data_subset) << "]\n";

        // Call quick_sort on subset, passing output file for logging steps
        quick_sort(data_subset, 0, data_subset.size() - 1, output_file);

        cout << "Sorting steps written to " << output_filename << endl;  // Notify user
    }

private:
    vector<pair<int, string>> read_data_from_csv(const string& filename) {
        vector<pair<int, string>> data;
        ifstream file(filename);
        string line;
        int row_num = 0;

        if (!file.is_open()) {  // Check if file opens successfully
            cerr << "File '" << filename << "' not found." << endl;
            return data;         // Return empty vector on failure
        }

        // Read file line by line
        while (getline(file, line)) {
            row_num++;          // Keep track of line number for error messages
            stringstream ss(line);
            string num_str, word;
            if (getline(ss, num_str, ',') && getline(ss, word)) {  // Parse integer and string separated by comma
                try {
                    int number = stoi(num_str);  // Convert string to integer
                    data.emplace_back(number, word);  // Store tuple in vector
                } catch (...) {
                    cerr << "Skipping row " << row_num << " (invalid integer): " << line << endl; // Error on conversion
                }
            } else {
                cerr << "Skipping row " << row_num << " (malformed): " << line << endl;  // If row format incorrect
            }
        }

        return data;  // Return parsed data vector
    }

    string join(const vector<pair<int, string>>& data) {
        stringstream ss;
        // Format vector data into "int/string, int/string, ..." string
        for (size_t i = 0; i < data.size(); ++i) {
            ss << data[i].first << "/" << data[i].second;
            if (i < data.size() - 1) ss << ", ";
        }
        return ss.str();  // Return formatted string
    }

    void quick_sort(vector<pair<int, string>>& array, int low, int high, ofstream& log) {
        if (low < high) {
            // Partition array and get pivot index
            int pivot_index = partition(array, low, high, log);

            // Log pivot index and current state of array after partition
            log << "pi=" << pivot_index << " [" << join(array) << "]\n";

            // Recursively sort elements before pivot
            quick_sort(array, low, pivot_index - 1, log);

            // Recursively sort elements after pivot
            quick_sort(array, pivot_index + 1, high, log);
        }
    }

    int partition(vector<pair<int, string>>& array, int low, int high, ofstream& log) {
        int pivot = array[high].first;  // Choose last element as pivot
        int i = low - 1;                // Index of smaller element

        for (int j = low; j < high; ++j) {
            // If current element is less than or equal to pivot
            if (array[j].first <= pivot) {
                ++i;
                swap(array[i], array[j]);  // Swap elements
            }
        }

        swap(array[i + 1], array[high]);  // Place pivot in correct position
        return i + 1;                     // Return pivot index
    }
};

int main() {
    TestQuickSortStep sorter;
    sorter.main();
    return 0;
}
