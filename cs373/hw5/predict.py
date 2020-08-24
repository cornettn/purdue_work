"""
predict.py - read_csv_file
This is run as 'python predict.py <training data filepath> <testing data filepath>'
"""

# imports
import sys, csv, knn
import decision_tree as dt

# define functions
def read_csv_file(filepath):
  """
  Turns the information from filepath into workable data
  :param filepath: The filepath to extract the data from
  :return: A list of lists that contain the data from the file
  """

  data = list()
  with open(filepath) as file:
    reader = csv.reader(file, delimiter=';')

    # iterate through the csv file
    first_row = True
    for row in reader:

      # skip the first row
      if first_row:
        first_row = not first_row
        continue

      # add the rows data into the list
      data.append(row)

  return data

# actual script
# Get the training and testing data files
arguments = sys.argv
raw_data_file = sys.argv[1]
test_file = sys.argv[2]

raw_data = read_csv_file(raw_data_file)
test_data = read_csv_file(test_file)

# leave 20% of data for validation
num_for_training = len(raw_data) * 0.8

# split into training and validation sets
train_data = list()
validation_data = list()

for i in range(len(raw_data)):
  if i < num_for_training:
    train_data.append(raw_data[i])
  else:
    validation_data.append(raw_data[i])

# knn.train_classifier(train_data, validation_data)
dt.train_classifier(train_data)

