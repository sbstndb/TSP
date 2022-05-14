/**
 * - File : bruteforce.c
 * - Description : This is a short bruteforce.c code which is doing a Bruteforce
 * solving on the hamiltonain path enigme.
 * - Author : Sebastien DUBOIS
 **/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *read_file(char *filename) {
  FILE *file;
  file = fopen(filename, "r");
  if (file != NULL) {
    printf("Read file with success ! \n");
  } else {
    printf("Erreur : Cannot read file with success \n");
  }
  return file;
}

struct s_problem {
  double *raw_matrix;
  char **raw_data;
  int size;
};

struct s_data {
  unsigned long long size;
  char **raw_data;
  double **p_data;
  double **p_reduced_data;
};

struct s_force {
  double solution;
  unsigned long long *path;
};

void swap(unsigned long long *order, unsigned long long l,
          unsigned long long r) {
  unsigned long long tmp = order[l];
  order[l] = order[r];
  order[r] = tmp;
}

unsigned long long score(double *raw_matrix, unsigned long long *order,
                         int size) {
  // compute the length of the path
  double result = 0.0;
  unsigned long long dep = 0;
  unsigned long long arr = order[0];
  result += raw_matrix[dep * size + arr];
  for (unsigned long long i = 1; i < size - 1; i++) {
    dep = arr;
    arr = order[i];
    result += raw_matrix[dep * size + arr];
  }
  dep = arr;
  arr = 0;
  result += raw_matrix[dep * size + arr];
  return result;
}

void permutation(unsigned long long *order, double *raw_matrix,
                 unsigned long long l, unsigned long long r, int size,
                 struct s_force *force) {
  if (l == r) {
    // we need to compute the length !
    // proposition : use a
    double value = score(raw_matrix, order, size);
    // printf("value :  %f\n", value);
    if (value < force->solution) {
      force->solution = value;
      for (unsigned long long i = 0; i < size - 1; i++) {
        force->path[i] = order[i];
      }
    }
  } else {
    for (unsigned long long i = l; i <= r; i++) {
      swap(order, l, i);
      permutation(order, raw_matrix, l + 1, r, size, force);
      swap(order, l, i);
    }
  }
}

void print_bruteforce(struct s_force *force, int size) {

  printf("\n\033[0;33mSolution :\033[0m \n\033[0;33m - Length :\033[0m "
         "%f\n\033[0;33m - Path   : \033[0m",
         force->solution);

  printf("0 - ", force->path[0]);
  for (int i = 0; i < size - 1; i++) {
    printf("%llu - ", force->path[i], force->path[i + 1]);
  }
  printf("0\n\n", force->path[size - 2]);
}

void bruteforce(double *raw_matrix, int size) {
  // resolution par bruteforce pour commencer

  struct s_force force;
  force.solution = DBL_MAX;
  force.path = malloc((size - 1) * sizeof(unsigned long long));

  unsigned long long *path = malloc((size - 1) * sizeof(unsigned long long));
  // initialisation
  for (unsigned long long i = 0; i < size - 1; i++) {
    path[i] = i + 1;
  }
  permutation(path, raw_matrix, 0, size - 2, size, &force);

  print_bruteforce(&force, size);
}

void read_data(FILE *file, struct s_problem *p) {

  p->size = 0;
  char *buffer = (char *)malloc(1024 * sizeof(char));
  p->raw_data = (char **)malloc(1 * sizeof(char *));

  printf("read data ..\n");
  // read raw data
  size_t line_buf_size = 0;
  unsigned long long i = 0;
  while (getline(&buffer, &line_buf_size, file) > -1) {
    p->raw_data[i] = (char *)malloc((line_buf_size + 1) * sizeof(char));
    strcpy(p->raw_data[i], buffer);
    p->raw_data = (char **)realloc(p->raw_data, (i + 2) * sizeof(char *));
    i += 1;
  }
  p->size = i;

  // print raw data
  printf("Size of graph  : %d\n", p->size);
  for (int i = 0; i < p->size; i++) {
    // printf("%s\n", p->raw_data[i]);
  }

  // allocation of data->p_data
  p->raw_matrix = (double *)malloc(p->size * p->size * sizeof(double));

  // char *parsed_line;
  char *token;
  // char *token2;
  // char *token3;
  char delim[2] = " ";
  // char delim2[2] = "-";
  // char delim3[2] = ",";
  for (unsigned long long i = 0; i < p->size; i++) {
    for (unsigned long long j = 0; j < p->size; j++) {
      if (j == 0) {
        token = strtok(p->raw_data[i], delim);

      } else {
        token = strtok(NULL, delim);
      }
      // printf("i : %llu j : %llu\n", i, j ) ;
      if (strncmp(token, "inf", 3) == 0) {
        // infinity value
        p->raw_matrix[i * p->size + j] = DBL_MAX;
      } else {
        p->raw_matrix[i * p->size + j] = strtod(token, NULL);
      }
    }
  }
}

void close_file(FILE *file) { fclose(file); }

int print_info(void) {
  printf(
      "\033[0;35mCommand :\033[0m \n\n\033[0;35m    With custom file :\033[0m "
      "        ./bruteforce -f "
      "values.txt\n\033[0;35m    With "
      "embedded file  :\033[0m      ./bruteforce -e\n\033[0;35m    With n * n "
      "random "
      "values :\033[0m ./bruteforce -n "
      "number\n\n");
  return 0;
}

int main(int argc, char **argv) {

  printf("\n \033[0;33m-----  Bruteforce method -----\033[0m \n\n");

  char *filename;
  int with_file = 1;
  int n = 1;

  if (argc == 1) {
    return print_info();
  }

  else if (argc == 2) {
    char nf[4] = "-e";
    if (strncmp(nf, argv[1], 3) == 0) {
      printf("You selected the \033[0;32membedded file\033[0m option.\n");
      filename = malloc(18 * sizeof(char));
      strncpy(filename, "configuration.txt\0", 18);
    } else {
      return print_info();
    }
  }

  else if (argc == 3) {
    char f[3] = "-f";
    if (strncmp(f, argv[1], 2) == 0) {
      printf("You selected the \033[0;32mfile\033[0m option.\n");
      size_t argv_len = strlen(argv[2]);
      filename = malloc((int)argv_len * sizeof(char));
      strncpy(filename, argv[2], (int)(argv_len));
    } else if (strncmp("-n", argv[1], 2) == 0) {
      printf("You selected the \033[0;32mrandom - n\033[0m option.\n");
      with_file = 0;
      n = atoi(argv[2]);
    } else {
      return print_info();
    }
  }
  struct s_problem p;
  if (with_file == 1) {
    FILE *file = NULL;
    file = read_file(filename);
    if (file == NULL) {
      printf("\033[0;31mThe file does not exist.\033[0m \n\033[0;34mHint : try "
             "the -e option !\033[0m\n\n");
      return print_info();
    }
    read_data(file, &p);
    close_file(file);
  } else {
    p.size = n;
    p.raw_matrix = (double *)malloc(n * n * sizeof(double));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        p.raw_matrix[i * n + j] = (double)rand() / 1000.0;
      }
    }
    for (int i = 0; i < n; i++) {
      p.raw_matrix[i * n + i] = DBL_MAX;
    }
  }

  struct timespec start_clock, stop_clock;
  clock_gettime(CLOCK_MONOTONIC, &start_clock);

  bruteforce(p.raw_matrix, p.size);

  clock_gettime(CLOCK_MONOTONIC, &stop_clock);
  unsigned long long total_clock_nsec;
  double total_clock_sec, total_clock_msec;
  total_clock_nsec = 1000000000 * (stop_clock.tv_sec - start_clock.tv_sec) +
                     stop_clock.tv_nsec - start_clock.tv_nsec;
  total_clock_sec = (double)(total_clock_nsec / 1000000000.0);
  total_clock_msec = (double)(total_clock_nsec / 1000000.0);

  printf("\033[0;33mElapsed time :\033[0m \n \033[0;33m- s      :\033[0m %f\n "
         "\033[0;33m- ms     :\033[0m %f\n\n\n",
         total_clock_sec, total_clock_msec);

  return 0;
}
