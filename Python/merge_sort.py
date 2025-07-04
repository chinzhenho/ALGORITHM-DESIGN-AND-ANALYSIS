import collections
import csv  
import time  

class TestMergeSort:
    def main(self):
        # Ask the user to enter the name of the CSV file (should be in the same folder)
        input_filename = input("Enter CSV file name : ")    
        if not input_filename.endswith('.csv'):
            print("Error: Please provide a valid CSV file.")
            return
        
        # Read the data from the CSV file
        S = self.read_data_from_csv(input_filename)

        if S is None:
            print(f"Error: Could not read data from {input_filename}")
            return

        # Start measuring the time before sorting
        start_time = time.time()
        self.mergeSort(S)
        end_time = time.time()
        
        # Calculate how long it took in milliseconds
        running_time_ms = (end_time - start_time) * 1000
        
        # Show the user how long the sorting took
        print(f"Running time: {running_time_ms:.3f} ms")

        # Create a filename for the output based on the number of rows
        dataset_size = len(S)
        output_filename = f"merge_sort_{dataset_size}.csv"
        
        # Save the sorted result to a new CSV file
        self.write_data_to_csv(S, output_filename)
        print(f"Sorted data written to {output_filename}")

    def read_data_from_csv(self, filename):
        data = []
        try:
            with open(filename, 'r', newline='') as csvfile:
                reader = csv.reader(csvfile)
                # We expect each row to contain an integer and a string
                for row in reader:
                    if len(row) == 2:
                        try:
                            # Convert the first column to an integer, and clean up the second
                            data.append((int(row[0].strip()), row[1].strip()))
                        except ValueError:
                            print(f"Skipping row due to non-integer value: {row}")
                    else:
                        print(f"Skipping malformed row: {row}")
            return data
        except FileNotFoundError:
            print(f"Error: File '{filename}' not found.")
            return None
        except Exception as e:
            print(f"An error occurred while reading the CSV: {e}")
            return None

    def write_data_to_csv(self, data, filename):
        try:
            with open(filename, 'w', newline='') as csvfile:
                writer = csv.writer(csvfile)
                for item in data:
                    writer.writerow(item)  # Write each (number, string) pair to a row
        except Exception as e:
            print(f"An error occurred while writing the CSV: {e}")

    def mergeSort(self, S):
        # Call the recursive merge sort function on the full list
        self._mergeSort(S, 0, len(S) - 1)

    def _mergeSort(self, S, left, right):
        if left < right:
            mid = (left + right) // 2
            # Recursively sort the left half
            self._mergeSort(S, left, mid)
            # Recursively sort the right half
            self._mergeSort(S, mid + 1, right)
            # Merge the two sorted halves together
            self.merge(S, left, mid, right)

    def merge(self, S, left, mid, right):
        # Copy the left half into a deque
        L = collections.deque()
        for i in range(mid - left + 1):
            L.append(S[left + i])

        # Copy the right half into another deque
        R = collections.deque()
        for j in range(right - mid):
            R.append(S[mid + 1 + j])

        k = left  # Index for placing back into the original list

        # Merge the two deques back into the original list
        while L and R:
            if L[0][0] <= R[0][0]:  # Compare based on the number part
                S[k] = L.popleft()
            else:
                S[k] = R.popleft()
            k += 1

        # If anything is left in L, add it back
        while L:
            S[k] = L.popleft()
            k += 1

        # If anything is left in R, add it back
        while R:
            S[k] = R.popleft()
            k += 1


if __name__ == "__main__":
    sorter = TestMergeSort()
    sorter.main()
