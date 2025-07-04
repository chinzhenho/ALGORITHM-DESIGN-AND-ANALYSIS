import random
import string
import csv

def generate_dataset(n, output_filename):
    # Generate n unique random integers between 1 and 2,000,000,000
    MAX_RANGE = 2_000_000_000
    MIN_RANGE = 1

    # Make sure the number of requested elements doesn't exceed the allowed range
    if n > MAX_RANGE - MIN_RANGE + 1:
        print(" Too many elements requested for this range.")
        return

    # Generate a list of n unique random integers
    integers = random.sample(range(MIN_RANGE, MAX_RANGE + 1), n)

    # Create n random 5-letter lowercase strings and pair them with the integers
    records = []
    for i in range(n):
        rand_str = ''.join(random.choices(string.ascii_lowercase, k=5))
        records.append((integers[i], rand_str))  # Combine number and string as a tuple

    # Save the generated data to a CSV file
    try:
        with open(output_filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            for record in records:
                writer.writerow(record)  # Each row is (number, string)
        print(f" Dataset with {n} records written to '{output_filename}'")
    except Exception as e:
        print(f" Error writing to file: {e}")

def main():
    try:
        # Ask the user how many records to generate
        n = int(input("Enter the number of elements (n): "))
        if n <= 0:
            print(" Please enter a positive integer.")
            return

        # Build the output file name based on the number of elements
        output_filename = f"dataset_{n}.csv"

        # Call the function to generate the dataset and save it
        generate_dataset(n, output_filename)
    except ValueError:
        print(" Invalid input. Please enter a valid integer.")


if __name__ == "__main__":
    main()
