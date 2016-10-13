/*
 * Copyright 2016 Andrii Maksai
 */
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "hungarian_assignment.cc"
HungarianAssignment hungarian_assignment;
const int kMaxN = 5000;
const int kMaxTest = 10;
int n, m;
double a[kMaxN][kMaxN];
FILE *fid;

void generate_test(int input_n, int input_m) {
  fid = fopen("input.txt", "w");
  fprintf(fid, "%d %d\n", input_n, input_m);
  for (int i = 0; i < input_n; ++i) {
    for (int j = 0; j < input_m; ++j) {
      a[i][j] = rand() % 100 - rand() % 100 + (rand() % 100) / 100.;
      fprintf(fid, "%0.3lf ", a[i][j]);
    }
    fprintf(fid, "\n");
  }
  fclose(fid);
}

double run_hungarian() {
  fid = fopen("input.txt", "r");
  hungarian_assignment.ReadCostMatrix(fid);
  fclose(fid);
  fid = fopen("output.txt", "w");
  hungarian_assignment.PrintAssignment(fid);
  fclose(fid);

  fid = fopen("output.txt", "r");
  int k;
  fscanf(fid, "%d", &k);
  double reported_score = 0;
  while (k--) {
    int i, j;
    fscanf(fid, "%d%d", &i, &j);
    reported_score += a[i][j];
  }
  fclose(fid);
  return reported_score;
}
double solve_test() {
  double best_score = -1e9;
  int perm[kMaxTest], k;
  if (n > m) {
    k = n;
  } else {
    k = m;
  }

  for (int i = 0; i < k; ++i) {
    perm[i] = i;
  }
  do {
    double current_score = 0;
    for (int i = 0; i < k; ++i) {
      if (i < n && perm[i] < m) {
        current_score += a[i][perm[i]];
      }
    }
    if (current_score > best_score) {
      best_score = current_score;
    }
  } while (std::next_permutation(perm, perm + k));
  return best_score;
}
int main() {
  srand(1);
  printf("Correctness testing...");
  for (int iters = 0; iters < 10; ++iters) {
    n = 1 + rand() % kMaxTest;
    m = 1 + rand() % kMaxTest;
    generate_test(n, m);
    double reported_score = run_hungarian();
    double best_score = solve_test();
    if (best_score - reported_score > 1e-6 ||
        reported_score - best_score > 1e-6) {
      printf("ERROR test %d\n", iters);
      printf("Optimal: %lf Hungarian: %lf\n", best_score, reported_score);
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
          printf("%0.3lf ", a[i][j]);
        }
        printf("\n");
      }
      exit(0);
    }
    printf(" %d", iters);
    fflush(stdout);
  }
  printf("\n");

  printf("Running time testing(test until 4096x4096, should take ~2min)...\n");
  n = 1;
  for (int iters = 0; iters < 13; ++iters) {
    generate_test(n, n);
    printf("%d x %d: ", n, n);
    double t_start = clock();
    run_hungarian();
    double t_end = clock();
    printf("%0.3f\n", (t_end - t_start) / CLOCKS_PER_SEC);
    n *= 2;
  }
}
