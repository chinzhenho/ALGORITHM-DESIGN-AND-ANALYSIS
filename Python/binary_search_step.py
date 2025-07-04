import sys
import csv

def binary_search_with_steps(data, target):
    """
    Performs a binary search on the data and records the comparison steps.

    Args:
        data (list): A list of tuples, where each tuple is (integer, string).
                     The list must be sorted by the integer value.
        target (int): The integer value to search for.

    Returns:
        list: A list of strings representing the search path.
        bool: True if the target was found, False otherwise.
    """
    low = 0
    high = len(data) - 1
    steps_log = []     # To store all the comparison steps
    found = False      # A flag to indicate whether the target is found

    while low <= high:
        mid = (low + high) // 2
        mid_value, mid_string = data[mid]

        # Log the current step in the format: row_number: integer/string
        # Add 1 to index to match human-readable row numbers (starting from 1)
        steps_log.append(f"{mid + 1}: {mid_value}/{mid_string}")

        # Compare the current value with the target
        if mid_value == target:
            found = True
            break  # Stop searching since the target is found
        elif mid_value < target:
            low = mid + 1  # Search in the right half
        else:
            high = mid - 1  # Search in the left half

    # If the loop ends without finding the target, add -1 to the log
    if not found:
        steps_log.append("-1")  # This means target was not found in the data

    return steps_log, found

def main():
    """
    Main function to execute the binary search step demonstration.
    """
    # Ask the user to provide the CSV file that contains the sorted data
    sorted_filename = input("Enter CSV file name : ")

    # Ask the user to input the target number they want to search for
    try:
        target_str = input("Enter the target integer to search for: ")
        target = int(target_str)
    except ValueError:
        print("Error: The target must be an integer.")
        sys.exit(1)

    # Read the data from the CSV file
    dataset = []
    try:
        with open(sorted_filename, 'r', newline='') as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                # Each row must contain two elements: an integer and a string
                dataset.append((int(row[0]), row[1]))
    except FileNotFoundError:
        print(f"Error: The file '{sorted_filename}' was not found.")
        sys.exit(1)
    except (ValueError, IndexError):
        print(f"Error: The file '{sorted_filename}' is not formatted correctly.")
        sys.exit(1)

    # Perform the binary search and get the list of search steps
    search_path, _ = binary_search_with_steps(dataset, target)

    # Write all the recorded steps to an output file named after the target
    output_filename = f"binary_search_step_{target}.txt"
    with open(output_filename, 'w') as f:
        for step in search_path:
            f.write(step + '\n')

    # Notify the user that the output file has been created
    print(f"Search steps have been written to '{output_filename}'.")

if __name__ == "__main__":
    main()
