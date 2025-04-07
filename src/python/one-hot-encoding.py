# import csv

# # Define known categories in fixed order
# CATEGORIES = ["<1H OCEAN", "INLAND", "ISLAND", "NEAR BAY", "NEAR OCEAN"]

# def one_hot_encode_ocean_proximity(row, header_index):
#     category = row[header_index]
#     # Drop the first category ("<1H OCEAN") and encode the rest
#     encoded = [1 if category == cat else 0 for cat in CATEGORIES[1:]]
#     return encoded

# def process_file(input_path, output_path):
#     with open(input_path, 'r') as infile:
#         reader = csv.reader(infile)
#         header = next(reader)

#         try:
#             ocean_idx = header.index("ocean_proximity")
#         except ValueError:
#             print("ERROR: 'ocean_proximity' column not found.")
#             return

#         new_header = (
#             header[:ocean_idx] +
#             [f"ocean_{c.replace(' ', '_')}" for c in CATEGORIES[1:]] +
#             header[ocean_idx + 1:]
#         )

#         with open(output_path, 'w', newline='') as outfile:
#             writer = csv.writer(outfile)
#             writer.writerow(new_header)

#             for row in reader:
#                 one_hot = one_hot_encode_ocean_proximity(row, ocean_idx)
#                 new_row = row[:ocean_idx] + list(map(str, one_hot)) + row[ocean_idx + 1:]
#                 writer.writerow(new_row)

#         print(f"[DONE] Output written to {output_path}")

# if __name__ == "__main__":
#     input_file = "data/input/cali-housing.csv"           
#     output_file = "data/input/cali-housing-encoding2.csv"
#     process_file(input_file, output_file)

import csv

def drop_column(input_path, output_path, column_to_drop="ocean_proximity"):
    with open(input_path, 'r') as infile:
        reader = csv.reader(infile)
        header = next(reader)

        try:
            drop_idx = header.index(column_to_drop)
        except ValueError:
            print(f"[ERROR] Column '{column_to_drop}' not found.")
            return

        # New header without the dropped column
        new_header = header[:drop_idx] + header[drop_idx+1:]

        with open(output_path, 'w', newline='') as outfile:
            writer = csv.writer(outfile)
            writer.writerow(new_header)

            for row in reader:
                new_row = row[:drop_idx] + row[drop_idx+1:]
                writer.writerow(new_row)

        print(f"[DONE] Wrote output to {output_path}")

if __name__ == "__main__":
    input_file = "data/input/cali-housing.csv"              # adjust path as needed
    output_file = "data/input/cali-housing-no-ocean.csv"    # new file without column
    drop_column(input_file, output_file)

