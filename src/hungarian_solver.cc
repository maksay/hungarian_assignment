/*
 * Copyright 2016 Andrii Maksai
 */
#include "hungarian_assignment.cc"
#include <cstdio>
#include <cstdlib>
HungarianAssignment hungarian_assignment;
int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Expected usage: hungarian_solver num_rows num_cols");
    exit(0);
  }
  int input_num_rows = atoi(argv[1]);
  int input_num_cols = atoi(argv[2]);
  hungarian_assignment.ReadCostMatrix(stdin, input_num_rows, input_num_cols);
  hungarian_assignment.PrintAssignment(stdout);
}

