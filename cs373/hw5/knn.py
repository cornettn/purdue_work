"""
knn.py - attr_dist, dist, classify, train_classifier
This file contains helper functions that serve several purposes
"""

ERR = -1

def attr_dist(attr_num, attr1, attr2):
  """
  Calcualates the distance between two attributes
  :param attr_num: The index of the attribute
  :param attr1: First attribute value
  :param attr2: Second attribute value
  :return: The distance between the two
  """
  numerical_indices = [2, 6, 7, 12, 13, 14, 23, 24, 25, 26, 27, 28, 29]
  nominal_indices = [8, 9, 10, 11]
  binary_indices = [0, 1, 3, 4, 5, 15, 16, 17, 18, 19, 20, 21, 22]

  # There is no difference
  if attr1 is attr2:
    return 1
  if attr_num in binary_indices:
    return 2**2
  if attr_num in numerical_indices:
    return 1 + abs(int(attr1) - int(attr2))**2
  if attr_num in nominal_indices:
    return 2**2


def dist(point1, point2):
  """
  This function evaluates the distance between two points
  :param point1: First data point, should be a list of the attributes
  :param point2: Second data point, should be a list of the attributes
  :return: The distance between the two data points
  """
  total_dist = 0

  # ensure that we are dealing with same size rows
  if len(point1) - len(point2) is not 0:
    return ERR

  # calculate distance
  for i in range(len(point1) - 1):
    total_dist += attr_dist(i, point1[i], point2[i])

  return total_dist**(1/2)


def classify(datapoint, data, k):
  """
  This function will classify the datapoint using KNN on the data
  :param datapoint: The data point to classify
  :param data: the data to use to classify
  :param k: The number of neighbors to analyze in the training set
  :return: The classifier that the datapoint is
  """
  DIST_INDEX = 1
  CLASSIFIER = 30
  MAX_DIST = 10000
  STARTING_INDEX = -1

  # Ensure that k is an odd number so there are no ties
  if k % 2 is 0:
    k += 1

  # make a list of k neighbors, containing (index, distance)
  neighbors = list()
  for i in range(k):
    neighbors.append((STARTING_INDEX, MAX_DIST, None))

  for i in range(len(data)):
    d = dist(datapoint, data[i])
    max_neighbor = max(neighbors, key=lambda n: n[DIST_INDEX])
    if d < max_neighbor[DIST_INDEX]:
      neighbors.remove(max_neighbor)
      neighbors.append((i, d, data[i][CLASSIFIER]))

  pos = 0
  neg = 0
  for item in neighbors:
    if item[2] is '+':
      pos += 1
    else:
      neg += 1

  return '+' if pos > neg else '-'


def train_classifier(training, validation):
  """
  Try to find the best value of k given the training and validation sets
  :param training: Training data set as a list
  :param validation: Validation set as a list
  :return:
  """
  min_wrong = 10000
  best_k = -1

  for i in range(int(len(validation) / 2)):
    if i % 2 is 0:
      continue

    num_wrong = 0
    num_right = 0
    for item in validation:
      label = classify(item, training, i)
      if label is item[30]:
        num_right += 1
      else:
        num_wrong += 1
    if num_wrong < min_wrong:
      min_wrong = num_wrong
      best_k = i
  print("k {}, {} wrong".format(best_k, min_wrong))