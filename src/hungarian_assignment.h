/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef HUNGARIAN_ASSIGNMENT_H_
#define HUNGARIAN_ASSIGNMENT_H_
#include <cstdio>
class HungarianAssignment {
  static const int kMaxRowsCols = 6000;
  static const double kInfty;
  static const int kNoMatching = -1;
  static const double kZeroPrecision;
  double **cost_matrix;
  bool *row_used;
  bool *col_used;
  int *row_match;
  int *col_match;
  int num_rows;
  int num_cols;
  int num_objs;
  bool dfs(int row_idx);

 public:
  HungarianAssignment();
  void ReadCostMatrix(FILE *fid, int input_num_rows, int input_num_cols);
  void PrintAssignment(FILE *fid);
  ~HungarianAssignment();
};
// To avoid warnings
const double HungarianAssignment::kInfty = 1e6;
const double HungarianAssignment::kZeroPrecision = 1e-6;
#endif  // HUNGARIAN_ASSIGNMENT_H_
