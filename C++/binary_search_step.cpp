#include <iostream>   
#include <fstream>   
#include <sstream>   
#include <vector>     
#include <string>    

using namespace std;  

// Function to perform binary search and log each step
pair<vector<string>, bool> binary_search_with_steps(const vector<pair<int, string>>& data, int target) {
    int low = 0;                                       // Start of search range
    int high = static_cast<int>(data.size()) - 1;      // End of search range
    vector<string> steps_log;                          // To store steps taken during search
    bool found = false;                                // Flag to indicate if the value is found

    // Standard binary search loop
    while (low <= high) {
        int mid = (low + high) / 2;                    // Find the midpoint
        int mid_value = data[mid].first;               // Get the integer at mid
        const string& mid_string = data[mid].second;   // Get the string at mid

        // Log the current comparison step (1-based index for user-friendliness)
        steps_log.push_back(to_string(mid + 1) + ": " + to_string(mid_value) + "/" + mid_string);

        // If the target is found
        if (mid_value == target) {
            found = true;                              // Set found flag to true
            break;                                     // Exit the loop
        } else if (mid_value < target) {               // If target is greater, search right half
            low = mid + 1;
        } else {                                       // If target is smaller, search left half
            high = mid - 1;
        }
    }

    // If not found, log -1 to indicate failure
    if (!found) {
        steps_log.push_back("-1");
    }

    // Return the search path and the result flag
    return {steps_log, found};
}

int main() {
    string sorted_filename;                            // Variable to hold CSV filename
    cout << "Enter CSV file name : ";                  // Prompt user for input
    cin >> sorted_filename;                            // Read filename from user

    int target;                                        // Variable for target value
    cout << "Enter the target integer to search for: "; // Prompt user for target integer
    if (!(cin >> target)) {                            // Validate input
        cerr << "Error: The target must be an integer." << endl;
        return 1;                                      // Exit with error code
    }

    vector<pair<int, string>> dataset;                 // Vector to hold the dataset
    ifstream file(sorted_filename);                    // Open the file for reading

    if (!file.is_open()) {                             // Check if file opened successfully
        cerr << "Error: The file '" << sorted_filename << "' was not found." << endl;
        return 1;                                      // Exit with error code
    }

    string line;                                       // To hold each line from the file
    while (getline(file, line)) {                      // Read line-by-line
        stringstream ss(line);                         // Create a stream from the line
        string num_str, word;                          // Temporary holders for number and string

        if (getline(ss, num_str, ',') && getline(ss, word)) {  // Split line at comma
            try {
                int number = stoi(num_str);            // Convert number string to integer
                dataset.emplace_back(number, word);    // Add parsed pair to dataset
            } catch (...) {                            // Catch invalid number format
                cerr << "Skipping invalid line: " << line << endl;
            }
        } else {                                       // Catch malformed rows (not 2 values)
            cerr << "Skipping malformed line: " << line << endl;
        }
    }
    file.close();                                     

    // Perform binary search with step logging
    auto [search_path, found] = binary_search_with_steps(dataset, target);

    // Prepare output filename using the target value
    string output_filename = "binary_search_step_" + to_string(target) + ".txt";

    ofstream output_file(output_filename);             // Open output file
    if (output_file.is_open()) {                       // Check if file opened successfully
        for (const auto& step : search_path) {         // Write each step to the file
            output_file << step << '\n';
        }
        output_file.close();                           // Close the file after writing
        cout << "Search steps have been written to '" << output_filename << "'." << endl;
    } else {
        cerr << "Failed to write to output file." << endl;  // If writing fails
    }

    return 0;                                         
}
