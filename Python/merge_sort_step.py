import collections
import csv

class TestMergeSortStep:
    def format_output_list(self, data_list):
        """
        Converts each (number, string) pair into a 'number/string' format.
        This helps make the output easier to read when showing the sorting steps.
        """
        return [f"{item[0]}/{item[1]}" for item in data_list]

    def main(self):
        """
        Main function that controls the process:
        - Reads the CSV file
        - Asks the user which rows to sort
        - Performs Merge Sort with step-by-step logging
        - Saves all steps to a text file
        """
        # Ask the user to enter the name of the CSV file (should be in the same folder)
        input_filename = input("Enter CSV file name : ")    
        if not input_filename.endswith('.csv'):
            print("Error: Please provide a valid CSV file.")
            return
        
        # Read all rows from the file so we can extract the part the user wants
        full_data = self.read_data_from_csv(input_filename)
        
        if full_data is None:
            print(f"Error: Could not read data from {input_filename}. Please ensure the file exists and is correctly formatted.")
            return

        # Let the user select a range of rows to sort (e.g., from row 10 to 20)
        while True:
            try:
                start_row_input = input(f"Enter start row : ")
                end_row_input = input(f"Enter end row : ")
                
                # Convert to 0-based index (Python indexing starts from 0)
                start_idx = int(start_row_input) - 1
                end_idx = int(end_row_input) - 1

                # Make sure the selected range is valid
                if not (0 <= start_idx <= end_idx < len(full_data)):
                    print(f"Invalid row range. Please enter values between 1 and {len(full_data)} where start row <= end row.")
                    continue
                break
            except ValueError:
                print("Invalid input. Please enter integers for start and end rows.")
        
        # Get the portion of the data the user wants to sort
        # We make a copy of it to sort, so the original data stays unchanged
        data_subset = full_data[start_idx : end_idx + 1]

        # Generate the output filename based on selected row range
        output_filename = f"merge_sort_step_{start_row_input}_{end_row_input}.txt"
        
        try:
            # Open the output file and start logging the sorting steps
            with open(output_filename, 'w', newline='') as output_file:
                # Write the initial state of the array before any sorting
                output_file.write(f"[{', '.join(self.format_output_list(data_subset))}]\n")
                
                # Perform merge sort and record each step
                self._mergeSort(data_subset, 0, len(data_subset) - 1, output_file)
                
            print(f"Sorting steps written to {output_filename}")
        except Exception as e:
            print(f"An error occurred during sorting or writing output: {e}")

    def read_data_from_csv(self, filename):
        """
        Reads data from a CSV file. Each row should have two values: a number and a string.
        Returns a list of (number, string) pairs.
        If a row is missing data or contains invalid data, it's skipped with a warning.
        """
        data = []
        try:
            with open(filename, 'r', newline='') as csvfile:
                reader = csv.reader(csvfile)
                for row_num, row in enumerate(reader, 1):
                    if len(row) == 2:
                        try:
                            number = int(row[0].strip())  # Convert the first column to an integer
                            label = row[1].strip()        # Trim any extra spaces from the second column
                            data.append((number, label))
                        except ValueError:
                            print(f"Skipping row {row_num} due to non-integer value in first column: {row}")
                    else:
                        print(f"Skipping row {row_num} due to malformed row (expected 2 columns): {row}")
            return data
        except FileNotFoundError:
            print(f"Error: File '{filename}' not found. Please make sure the CSV file is in the same directory.")
            return None
        except Exception as e:
            print(f"An unexpected error occurred while reading the CSV: {e}")
            return None

    def _mergeSort(self, S, left, right, output_file):
        """
        Recursive Merge Sort function that breaks the list into halves,
        sorts them, and then merges them while logging each merge step.
        """
        if left < right:
            mid = (left + right) // 2
            self._mergeSort(S, left, mid, output_file)       # Sort the first half
            self._mergeSort(S, mid + 1, right, output_file)  # Sort the second half
            self.merge(S, left, mid, right, output_file)     # Merge the two halves

    def merge(self, S, left, mid, right, output_file):
        """
        This function merges two sorted parts of the array into one.
        It also logs the full state of the array after each merge.
        """
        # Copy the left part into a deque (so we can pop from the front efficiently)
        L = collections.deque()
        for i in range(mid - left + 1):
            L.append(S[left + i])

        # Copy the right part into another deque
        R = collections.deque()
        for j in range(right - mid):
            R.append(S[mid + 1 + j])

        k = left  # Index for writing merged results back into the array

        # Merge from both deques into the original list
        while L and R:
            if L[0][0] <= R[0][0]:  # Compare based on the integer part
                S[k] = L.popleft()
            else:
                S[k] = R.popleft()
            k += 1

        # If anything is left in L, copy it over
        while L:
            S[k] = L.popleft()
            k += 1

        # If anything is left in R, copy it over
        while R:
            S[k] = R.popleft()
            k += 1
        
        # Log the full array state after each merge
        output_file.write(f"[{', '.join(self.format_output_list(S))}]\n")


if __name__ == "__main__":
    sorter = TestMergeSortStep()
    sorter.main()
