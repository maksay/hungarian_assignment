/*
 * Copyright 2016 Andrii Maksai
 */
#include "hungarian_assignment.cc"
HungarianAssignment hungarian_assignment;
int main() {
  hungarian_assignment.ReadCostMatrix(fopen("input.txt", "r"));
  hungarian_assignment.PrintAssignment(stdout);
}

