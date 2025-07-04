import csv   
import time  

class TestQuickSort:
    def main(self):

        # Ask the user to enter the name of the CSV file (must be in the same folder)
        input_filename = input("Enter CSV file name : ")    
        if not input_filename.endswith('.csv'):
            print("Error: Please provide a valid CSV file.")
            return

        # Step 1: Read data from the CSV file
        records = self.load_csv_data(input_filename)
        if records is None:
            print(f"Error: Could not read data from {input_filename}")
            return

        # Step 2: Start timing (only for the sorting part)
        start = time.time()

        # Step 3: Sort the data using Quick Sort
        self.quick_sort(records, 0, len(records) - 1)

        # Step 4: Stop timing and calculate how long it took in milliseconds
        end = time.time()
        duration_ms = (end - start) * 1000
        print(f"Running time: {duration_ms:.3f} ms")

        # Step 5: Save the sorted data into a new CSV file
        output_filename = f"quick_sort_{len(records)}.csv"  
        self.save_to_csv(records, output_filename)
        print(f"Sorted data written to {output_filename}")

    def load_csv_data(self, filename):
        """
        Reads data from a CSV file.
        Each row should have two values: a number and a string.
        Returns a list of tuples like (number, string).
        """
        data = []
        try:
            with open(filename, 'r', newline='') as file:
                reader = csv.reader(file)
                for row in reader:
                    if len(row) == 2:
                        try:
                            number = int(row[0].strip())   # Convert the first value to an integer
                            label = row[1].strip()         # Use the second value as a string
                            data.append((number, label))   # Add the tuple to the list
                        except ValueError:
                            print(f"Skipping row (non-integer): {row}")
                    else:
                        print(f"Skipping malformed row: {row}")
            return data
        except FileNotFoundError:
            print(f"Error: File '{filename}' not found.")
            return None
        except Exception as e:
            print(f"An error occurred while reading the CSV: {e}")
            return None

    def save_to_csv(self, data, filename):
        """
        Saves the sorted list of tuples into a CSV file.
        Each tuple is written as a new row with two values.
        """
        try:
            with open(filename, 'w', newline='') as file:
                writer = csv.writer(file)
                for record in data:
                    writer.writerow(record)  # Write each (number, string) pair to the file
        except Exception as e:
            print(f"An error occurred while writing the CSV: {e}")

    def quick_sort(self, array, low, high):
    # If the current part of the list has more than one item
        if low < high:
            # Divide the list around a pivot and get its index
            pivot_index = self.partition(array, low, high)

            # Recursively sort everything before the pivot
            #After partitioning, it recursively calls quick_sort on the left part of the array (from index low up to pivot_index - 1).
            #This sorts all elements that are less than or equal to the pivot.
            self.quick_sort(array, low, pivot_index - 1)

            # Recursively sort everything after the pivot
            #After partitioning, it recursively calls quick_sort on the right part of the array (from index pivot_index + 1 to high).
            #This sorts all elements that are greater than the pivot.
            self.quick_sort(array, pivot_index + 1, high)


    def partition(self, array, low, high):
        # We'll use the last element as the pivot
        pivot = array[high][0]  # Get the number part from the tuple

        # 'i' keeps track of the last spot with a value less than or equal to the pivot
        i = low - 1

        # Go through all elements except the pivot
        for j in range(low, high):
            # If this number is smaller than or equal to the pivot
            if array[j][0] <= pivot:
                i += 1
                # Swap the current item with the one at index i
                array[i], array[j] = array[j], array[i]

        # Finally, move the pivot to its correct position
        array[i + 1], array[high] = array[high], array[i + 1]

        # Return the pivot's new position
        return i + 1


if __name__ == "__main__":
    sorter = TestQuickSort()
    sorter.main()
