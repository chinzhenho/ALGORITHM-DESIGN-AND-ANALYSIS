#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

// Function to perform binary search on a vector of pairs
int binary_search(const vector<pair<int, string>> &data, int target)
{
    int low = 0;
    int high = static_cast<int>(data.size()) - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;
        int mid_value = data[mid].first; // Get the integer at mid

        if (mid_value == target)
        {
            return mid;
        }
        else if (mid_value < target) // If target is greater, search right half
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;  // If target is smaller, search left half
        }
    }

    return -1;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr))); //seeds the random number generator with the current time. 

    string sorted_filename; // Variable to hold CSV filename
    cout << "Enter CSV file name : ";
    cin >> sorted_filename;

    vector<pair<int, string>> dataset; // Vector to hold the dataset
    ifstream file(sorted_filename); // Open the file for reading

    if (!file.is_open())
    {
        cerr << "Error: The file '" << sorted_filename << "' was not found." << endl;
        return 1;
    }

    string line; // To hold each line from the file
    while (getline(file, line)) // Read line-by-line
    {
        stringstream ss(line); // Create a stream from the line
        string num_str, word; // Temporary holders for number and string
        if (getline(ss, num_str, ',') && getline(ss, word)) // Split line at comma
        {
            try
            {
                int number = stoi(num_str); // Convert number string to integer
                dataset.emplace_back(number, word); // Add parsed pair to dataset
            }
            catch (...) // Catch invalid number format
            {
                cerr << "Skipping invalid line: " << line << endl;
            }
        }
    }
    file.close();

    int n = static_cast<int>(dataset.size());
    if (n == 0)
    {
        cerr << "Error: The dataset is empty." << endl;
        return 1;
    }

    // --- Best Case Analysis ---
    int best_case_target = dataset[(n / 2) - 1].first;
    auto start_time = high_resolution_clock::now(); //records the current time using a high-resolution clock. 
                                                    // It is used to mark the start of a time interval
    for (int i = 0; i < n; ++i) //This loop repeatedly calls the binary_search function n times, 
                                // each time searching for the best_case_target in the dataset.
    {
        binary_search(dataset, best_case_target);
    }
    auto end_time = high_resolution_clock::now();
    duration<double, milli> best_case_time = end_time - start_time;

    // --- Worst Case Analysis ---
    int worst_case_target = -1;
    start_time = high_resolution_clock::now();
    for (int i = 0; i < n; ++i)
    {
        binary_search(dataset, worst_case_target);
    }
    end_time = high_resolution_clock::now();
    duration<double, milli> worst_case_time = end_time - start_time;

    // --- Average Case Analysis (sample 10% random targets) ---
    int sample_size = n / 10;
    if (sample_size == 0)
        sample_size = 1; // ensure at least one sample

    start_time = high_resolution_clock::now();
    for (int i = 0; i < sample_size; ++i) //It runs binary_search sample_size times, 
                                          // each time searching for a randomly selected target from the dataset.
    {
        int random_index = rand() % n;
        int random_target = dataset[random_index].first;
        binary_search(dataset, random_target);
    }
    end_time = high_resolution_clock::now();
    duration<double, milli> average_case_time = end_time - start_time;

    // --- Output Results to File ---
    string output_filename = "binary_search_result.txt";
    size_t pos1 = sorted_filename.find_last_of("/");
    string base_name = (pos1 != string::npos) ? sorted_filename.substr(pos1 + 1) : sorted_filename;
    size_t pos2 = base_name.find_last_of("_");
    size_t pos3 = base_name.find_last_of(".");

    if (pos2 != string::npos && pos3 != string::npos && pos2 < pos3)
    {
        string size_str = base_name.substr(pos2 + 1, pos3 - pos2 - 1);
        output_filename = "binary_search_" + size_str + ".txt";
    }

    ofstream output_file(output_filename);
    if (output_file.is_open())
    {
        output_file << fixed << setprecision(3);
        output_file << "Best case target: " << best_case_target << "\n";
        output_file << "Best case time: " << best_case_time.count() << " ms\n\n";

        output_file << "Average case time (10% random targets): " << average_case_time.count() << " ms\n\n";

        output_file << "Worst case target: " << worst_case_target << "\n";
        output_file << "Worst case time: " << worst_case_time.count() << " ms\n";

        output_file.close();
        cout << "Timing analysis with targets has been written to '" << output_filename << "'." << endl;
    }
    else
    {
        cerr << "Failed to write to " << output_filename << endl;
    }

    return 0;
}
