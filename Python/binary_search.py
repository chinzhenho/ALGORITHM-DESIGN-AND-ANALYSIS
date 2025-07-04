import sys
import csv
import time
import os
import random

def binary_search(data, target):
    low = 0
    high = len(data) - 1
    while low <= high:
        mid = (low + high) // 2
        mid_value = data[mid][0]
        if mid_value == target:
            return mid
        elif mid_value < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1

def main():
    sorted_filename = input("Enter CSV file name : ")

    dataset = []
    try:
        with open(sorted_filename, 'r', newline='') as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                dataset.append((int(row[0]), row[1]))
    except FileNotFoundError:
        print(f"Error: The file '{sorted_filename}' was not found.")
        sys.exit(1)

    n = len(dataset)
    if n == 0:
        print("Error: The dataset is empty.")
        sys.exit(1)

    # --- Best Case ---
    #If the dataset has 2 or more items (n >= 2), it selects the value at the middle index ((n // 2) - 1) as the best_case_target.
    #If the dataset has only 1 item, it selects the value of that single item (dataset[0][0]) as the best_case_target.
    best_case_target = dataset[(n // 2) - 1][0] if n >= 2 else dataset[0][0]
    start_time = time.perf_counter()
    #The code runs the binary_search function n times (where n is the number of items in the dataset).
    #Each time, it searches for the same value (best_case_target) in the dataset.
    #The _ is a throwaway variable, used when the loop variable is not needed.
    for _ in range(n):
        binary_search(dataset, best_case_target)
    end_time = time.perf_counter()
    best_case_time_ms = (end_time - start_time) * 1000

    # --- Worst Case ---
    worst_case_target = -1
    start_time = time.perf_counter()
    for _ in range(n):
        binary_search(dataset, worst_case_target)
    end_time = time.perf_counter()
    worst_case_time_ms = (end_time - start_time) * 1000

    # --- Average Case (10% random targets) ---
    sample_size = max(n // 10, 1) #It calculates 10% of the dataset size (n // 10).
                                  #If that value is less than 1 (for very small datasets), it uses 1 instead.
    start_time = time.perf_counter()
    for _ in range(sample_size):
        random_index = random.randint(0, n - 1)  #picks a random index in the dataset
        random_target = dataset[random_index][0] #gets the value at that random index 
        binary_search(dataset, random_target)    #performs a binary search for that random value in the dataset.
    end_time = time.perf_counter()
    average_case_time_ms = (end_time - start_time) * 1000

    # --- Output Results to File ---
    base_name = os.path.basename(sorted_filename) #Gets just the filename (without the path) from the user’s input.
    pos1 = base_name.rfind("_")                   #Finds the position of the last underscore () in the filename.
    pos2 = base_name.rfind(".")                   #Finds the position of the last dot (.) in the filename
    output_filename = "binary_search_result.txt"

    if pos1 != -1 and pos2 != -1 and pos1 < pos2: #If both an underscore and a dot are found, and the underscore comes before the dot
        size_str = base_name[pos1 + 1:pos2]       #Extracts the text between the last underscore and the dot
        output_filename = f"binary_search_{size_str}.txt" #Sets the output filename to include that extracted size

    with open(output_filename, 'w') as f:
        f.write(f"Best case target: {best_case_target}\n")
        f.write(f"Best case time: {best_case_time_ms:.3f} ms\n\n")
        f.write(f"Average case time (10% random targets): {average_case_time_ms:.3f} ms\n\n")
        f.write(f"Worst case target: {worst_case_target}\n")
        f.write(f"Worst case time: {worst_case_time_ms:.3f} ms\n")

    print(f"Timing analysis with targets has been written to '{output_filename}'.")

if __name__ == "__main__":
    main()
