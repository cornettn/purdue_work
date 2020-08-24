"""
decision_tree.py -
This file is responsible for all things decision tree related
"""

import math

NOT_BINARY = -1
binary_indices = [0, 1, 3, 4, 5, 15, 16, 17, 18, 19, 20, 21, 22]

class Tree(object):
  """
  This is used as the decision tree class
  """
  def __init__(self):
    """
    Init for the nodes in the decision tree
    """
    self.attr_index = None
    self.left_value = None
    self.left = None
    self.right = None
    self.used_attr = None # The parents

root = Tree()


def train_classifier(training_set):
  gain_list = list()
  max_gain = (0, info_gain(training_set, 0))
  for i in range(1, len(training_set)):
    gain = info_gain(training_set, i)
    if gain is not NOT_BINARY:
      gain_list.append((i, gain))

  gain_list = sorted(gain_list, key=lambda i: i[1])

  best = gain_list.pop()
  root.attr_index = best[0]

  return

def entropy_binary(data_set, attr_index):
  zero = data_set[0][attr_index]
  pos_count_zero = 0
  neg_count_zero = 0
  pos_count_one = 0
  neg_count_one = 0

  # count the number of pos classifications vs. neg
  for item in data_set:
    if item[attr_index] is zero:
      if item[30] is '+':
        pos_count_zero += 1
      else:
        neg_count_zero += 1
    else:
        if item[30] is '+':
          pos_count_one += 1
        else:
          neg_count_one += 1

  tot_zero = pos_count_zero + neg_count_zero
  tot_one = pos_count_one + neg_count_one

  prob_zero = float(tot_zero) / len(data_set)
  prob_one = float(tot_one) / len(data_set)
  zero_pos_ratio = float(pos_count_zero) / tot_zero
  zero_neg_ratio = float(neg_count_zero) / tot_zero
  one_pos_ratio = float(pos_count_one) / tot_one
  one_neg_ratio = float(neg_count_one) / tot_one

  if zero_neg_ratio == 0.0:
    zero_neg_ratio += 0.0000001

  if zero_pos_ratio == 0.0:
    zero_pos_ratio += 0.0000001

  if one_neg_ratio == 0.0:
    one_neg_ratio += 0.0000001

  if one_pos_ratio == 0.0:
    one_pos_ratio += 0.0000001

  e_one = -1 * one_pos_ratio * math.log2(one_pos_ratio) + -1 * one_neg_ratio * math.log2(one_neg_ratio)
  e_zero = -1 * zero_pos_ratio * math.log2(zero_pos_ratio) + -1 * zero_neg_ratio * math.log2(zero_neg_ratio)
  return prob_zero * e_zero + prob_one * e_one


def entropy(data_set, attr_index = None):
  pos_count = 0
  neg_count = 0

  # count the number of pos classifications vs. neg
  for item in data_set:
    if item[30] is '+':
      pos_count += 1
    else:
      neg_count += 1

  if attr_index is None:
    # Calculating the entropy of the entire set
    pos_ratio = float(pos_count) / len(data_set)
    neg_ratio = float(neg_count) / len(data_set)
    return -1 * pos_ratio * math.log2(pos_ratio) + -1 * neg_ratio * math.log2(neg_ratio)

  # Calculate entropy given an attribute
  if attr_index in binary_indices:
    return entropy_binary(data_set, attr_index)
  else:
    return NOT_BINARY


def info_gain(data_set, attr):
  if attr not in binary_indices:
    return NOT_BINARY
  return entropy(data_set) - entropy(data_set, attr_index=attr)
