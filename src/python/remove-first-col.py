import csv

def remove_first_column(input_path, output_path):
    with open(input_path, 'r', newline='') as infile, open(output_path, 'w', newline='') as outfile:
        reader = csv.reader(infile)
        writer = csv.writer(outfile)

        for row in reader:
            writer.writerow(row[1:]) 

remove_first_column('data/input/Salary_dataset.csv', 'data/input/Salary_dataset2.csv')
