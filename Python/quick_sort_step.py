import csv 

class TestQuickSortStep:
    def format_output_list(self, data_list):
        """
        Takes a list of (number, string) pairs and formats them into strings like "123/label".
        This is mainly used to make the log output easier to read.
        """
        return [f"{item[0]}/{item[1]}" for item in data_list]

    def main(self):
        # Ask the user to provide the CSV file name (should be in the same folder)
        input_filename = input("Enter CSV file name : ")    
        if not input_filename.endswith('.csv'):
            print("Error: Please provide a valid CSV file.")
            return

        # Load all the data from the CSV file
        full_data = self.read_data_from_csv(input_filename)
        if full_data is None:
            print(f"Error: Could not read data from {input_filename}. Please ensure the file exists.")
            return

        # Ask the user which rows they want to sort (inclusive range)
        while True:
            try:
                start_row_input = input(f"Enter start row : ")
                end_row_input = input(f"Enter end row : ")

                # Convert user input from 1-based to 0-based index
                start_idx = int(start_row_input) - 1
                end_idx = int(end_row_input) - 1

                # Make sure the given range is within the data limits
                if not (0 <= start_idx <= end_idx < len(full_data)):
                    print(f"Invalid row range. Enter values between 1 and {len(full_data)}.")
                    continue
                break
            except ValueError:
                print("Invalid input. Please enter integers.")

        # Grab just the portion of data that the user wants to sort
        data_subset = full_data[start_idx : end_idx + 1]

        # Set the filename where we'll log the sorting steps
        output_filename = f"quick_sort_step_{start_row_input}_{end_row_input}.txt"

        try:
            # Open the output file and begin logging
            with open(output_filename, 'w', newline='') as output_file:
                # Write the initial state of the data before sorting
                output_file.write(f"[{', '.join(self.format_output_list(data_subset))}]\n")

                # Perform quick sort and log every step
                self.quick_sort(data_subset, 0, len(data_subset) - 1, output_file)

            print(f"Sorting steps written to {output_filename}")
        except Exception as e:
            print(f"Error during sorting or writing: {e}")

    def read_data_from_csv(self, filename):
        """
        Reads data from the CSV file and stores it as a list of (number, string) pairs.
        If a row is badly formatted or contains invalid data, it's skipped with a warning.
        """
        data = []
        try:
            with open(filename, 'r', newline='') as csvfile:
                reader = csv.reader(csvfile)
                for row_num, row in enumerate(reader, 1):  # Start counting from row 1
                    if len(row) == 2:
                        try:
                            number = int(row[0].strip())   # Convert first value to integer
                            label = row[1].strip()         # Clean up the second value (string)
                            data.append((number, label))
                        except ValueError:
                            print(f"Skipping row {row_num} (invalid integer): {row}")
                    else:
                        print(f"Skipping row {row_num} (malformed): {row}")
            return data
        except FileNotFoundError:
            print(f"File '{filename}' not found.")
            return None
        except Exception as e:
            print(f"Error reading CSV: {e}")
            return None

    def quick_sort(self, array, low, high, output_file):
        """
        The main recursive quick sort function.
        Uses the last element as the pivot, splits the list around it,
        and logs each step to a file.
        """
        if low < high:
            # Rearrange the array and find out where the pivot ends up
            pivot_index = self.partition(array, low, high, output_file)

            # Log the pivot position and the state of the array after this step
            output_file.write(f"pi={pivot_index} [{', '.join(self.format_output_list(array))}]\n")

            # Now sort the left and right halves
            self.quick_sort(array, low, pivot_index - 1, output_file)
            self.quick_sort(array, pivot_index + 1, high, output_file)

    def partition(self, array, low, high, output_file):
        """
        Helper function to do the actual partitioning around the pivot.
        It puts smaller values before the pivot and larger ones after it.
        Returns the final index where the pivot ends up.
        """
        pivot = array[high][0]  # Use the number part of the last tuple as pivot
        i = low - 1  # Keeps track of where to swap next

        for j in range(low, high): #Loops through the array from index low to high - 1.
            if array[j][0] <= pivot: #Checks if the number part of the current element is less than or equal to the pivot value.
                i += 1               #Moves the boundary for smaller elements to the right.
                array[i], array[j] = array[j], array[i]  # Swap values

        # Finally, move the pivot to its correct position
        array[i + 1], array[high] = array[high], array[i + 1]
        return i + 1  # Return pivot's final index

# If the script is run directly, start the sorting process
if __name__ == "__main__":
    sorter = TestQuickSortStep()
    sorter.main()
