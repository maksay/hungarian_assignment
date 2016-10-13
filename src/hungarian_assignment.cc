/*
 * Copyright 2016 Andrii Maksai
 */
#include "hungarian_assignment.h"
#include <cstdio>
#include <cstdlib>
HungarianAssignment::HungarianAssignment() {}

void HungarianAssignment::ReadCostMatrix(FILE* fid, int input_num_rows,
                                         int input_num_cols) {
  num_rows = input_num_rows;
  num_cols = input_num_cols;
  if (num_rows > num_cols) {
    num_objs = num_rows;
  } else {
    num_objs = num_cols;
  }
  if (num_objs > HungarianAssignment::kMaxRowsCols) {
    printf("ERROR: #rows or #cols too large");
    exit(1);
  }

  cost_matrix = new double*[num_objs];
  for (int row_idx = 0; row_idx < num_objs; ++row_idx) {
    cost_matrix[row_idx] = new double[num_objs];
    if (row_idx < num_rows) {
      for (int col_idx = 0; col_idx < num_cols; ++col_idx) {
        fscanf(fid, "%lf", &cost_matrix[row_idx][col_idx]);
      }
    }
  }
}

void HungarianAssignment::PrintAssignment(FILE* fid) {
  // Turn maximization problem into minimization problem, with non-negative
  // weights
  double min_value = HungarianAssignment::kInfty;
  double max_value = -HungarianAssignment::kInfty;
  for (int row_idx = 0; row_idx < num_rows; ++row_idx) {
    for (int col_idx = 0; col_idx < num_cols; ++col_idx) {
      if (cost_matrix[row_idx][col_idx] < min_value) {
        min_value = cost_matrix[row_idx][col_idx];
      }
      if (cost_matrix[row_idx][col_idx] > max_value) {
        max_value = cost_matrix[row_idx][col_idx];
      }
    }
  }
  min_value -= 1;
  max_value -= min_value;
  max_value += 1;

  for (int row_idx = 0; row_idx < num_objs; ++row_idx) {
    for (int col_idx = 0; col_idx < num_objs; ++col_idx) {
      if (row_idx < num_rows && col_idx < num_cols) {
        cost_matrix[row_idx][col_idx] =
            max_value - (cost_matrix[row_idx][col_idx] - min_value);
      } else {
        cost_matrix[row_idx][col_idx] = max_value;
      }
    }
  }

  // Variables used in each dfs iteration and answer variables
  row_used = new bool[num_objs];
  col_used = new bool[num_objs];
  row_match = new int[num_objs];
  col_match = new int[num_objs];

  for (int obj_idx = 0; obj_idx < num_objs; ++obj_idx) {
    row_match[obj_idx] = HungarianAssignment::kNoMatching;
    col_match[obj_idx] = HungarianAssignment::kNoMatching;
  }

  int cnt_matched = 0;
  while (cnt_matched < num_objs) {
    // Clear used nodes and try to run dfs for all not yet matched nodes
    for (int obj_idx = 0; obj_idx < num_objs; ++obj_idx) {
      row_used[obj_idx] = false;
      col_used[obj_idx] = false;
    }

    bool update_on_iteration = false;
    for (int obj_idx = 0; obj_idx < num_objs; ++obj_idx) {
      if (row_used[obj_idx] == false &&
          row_match[obj_idx] == HungarianAssignment::kNoMatching) {
        if (dfs(obj_idx)) {
          cnt_matched++;
          update_on_iteration = true;
        }
      }
    }

    // If no new pairs were matched, compute minimum update that would add one
    // more admissible edge and apply appropriate modifications
    if (!update_on_iteration) {
      double adjustment_value = HungarianAssignment::kInfty;
      for (int row_idx = 0; row_idx < num_objs; ++row_idx) {
        for (int col_idx = 0; col_idx < num_objs; ++col_idx) {
          if (row_used[row_idx] && !col_used[col_idx]) {
            if (cost_matrix[row_idx][col_idx] < adjustment_value) {
              adjustment_value = cost_matrix[row_idx][col_idx];
            }
          }
        }
      }

      for (int row_idx = 0; row_idx < num_objs; ++row_idx) {
        for (int col_idx = 0; col_idx < num_objs; ++col_idx) {
          if (row_used[row_idx] && !col_used[col_idx]) {
            cost_matrix[row_idx][col_idx] -= adjustment_value;
          } else if (!row_used[row_idx] && col_used[col_idx]) {
            cost_matrix[row_idx][col_idx] += adjustment_value;
          }
        }
      }
    }
  }

  for (int row_idx = 0; row_idx < num_rows; ++row_idx) {
    if (row_match[row_idx] < num_cols) {
      fprintf(fid, "%d %d\n", row_idx, row_match[row_idx]);
    }
  }

  delete row_match;
  delete col_match;
  delete row_used;
  delete col_used;
}

bool HungarianAssignment::dfs(int row_idx) {
  row_used[row_idx] = true;
  for (int col_idx = 0; col_idx < num_objs; ++col_idx) {
    if (!col_used[col_idx] &&
        cost_matrix[row_idx][col_idx] < HungarianAssignment::kZeroPrecision) {
      col_used[col_idx] = true;
      if (col_match[col_idx] == HungarianAssignment::kNoMatching ||
          dfs(col_match[col_idx])) {
        row_match[row_idx] = col_idx;
        col_match[col_idx] = row_idx;
        return true;
      }
    }
  }
  return false;
}

HungarianAssignment::~HungarianAssignment() { delete[] cost_matrix; }
