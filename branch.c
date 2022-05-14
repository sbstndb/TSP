/**
 * - File : branch.c
 * - Description : This is a short branch.c code which is doing a Branch and
 *Bound solving on the hamiltonain path enigme.
 * - Author : Sebastien DUBOIS
 **/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// number of total nodes
#define N 5
#define INF INT_MAX

struct s_node;

struct s_segment {
  int from;
  int to;
};

struct s_data {
  struct s_node *nodes;
  int len;
};

struct s_node {
  struct s_segment *path;
  int len_path;
  double *reduced_matrix;
  double cost;
  int vertex;
  int level;
};

struct s_problem {
  double *raw_matrix;
  char **raw_data;
  int size;
};

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
  printf("Size of graph : %d\n", p->size);
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

void allocate_node(struct s_node *node, double *raw_matrix, int size) {
  node->cost = 0.0;
  node->vertex = 0;
  node->level = 0;
  node->reduced_matrix = (double *)malloc(size * size * sizeof(double));
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      node->reduced_matrix[i * size + j] = raw_matrix[i * size + j];
    }
  }
  node->path = (struct s_segment *)malloc(size * sizeof(struct s_segment));
  node->len_path = 0;
}

void set_path(struct s_node *node, int size, int level, int i, int j) {
  if (level != 0) {
    node->path[node->len_path].from = i;
    node->path[node->len_path].to = j;
    node->len_path += 1;
    for (int k = 0; k < size; k++) {
      node->reduced_matrix[i * size + k] = DBL_MAX;
      node->reduced_matrix[k * size + j] = DBL_MAX;
    }
  }
  node->reduced_matrix[j * size + 0] = DBL_MAX;
  node->level = level;
  node->vertex = j;
}

void copy_path(struct s_node *master_node, struct s_node *node) {
  node->len_path = master_node->len_path;
  for (int i = 0; i < node->len_path; i++) {
    node->path[i].from = master_node->path[i].from;
    node->path[i].to = master_node->path[i].to;
  }
}

void reduce_row(double *reduced_matrix, double *acc_row, int size) {
  double min;
  int index = -1;
  for (int i = 0; i < size; i++) {
    min = DBL_MAX;
    acc_row[i] = min;
    index = -1;
    for (int j = 0; j < size; j++) {
      if (reduced_matrix[i * size + j] < min) {
        min = reduced_matrix[i * size + j];
        acc_row[i] = min;
        index = j;
      }
    }

    for (int j = 0; j < size; j++) {
      if ((reduced_matrix[i * size + j] < DBL_MAX) && acc_row[i] < DBL_MAX) {
        reduced_matrix[i * size + j] -= acc_row[i];
      }
    }
  }
}

void reduce_column(double *reduced_matrix, double *acc_column, int size) {
  double min;
  int index = -1;
  for (int j = 0; j < size; j++) {
    min = DBL_MAX;
    acc_column[j] = min;
    index = -1;
    for (int i = 0; i < size; i++) {
      if (reduced_matrix[i * size + j] < min) {
        min = reduced_matrix[i * size + j];
        acc_column[j] = min;
        index = j;
      }
    }

    for (int i = 0; i < size; i++) {
      if ((reduced_matrix[i * size + j] < DBL_MAX) &&
          (acc_column[j] < DBL_MAX)) {
        reduced_matrix[i * size + j] -= acc_column[j];
      }
    }
  }
}

double compute_cost(double *reduced_matrix, int size) {
  double cost = 0;
  double *acc_column = (double *)malloc(size * sizeof(double));
  double *acc_row = (double *)malloc(size * sizeof(double));
  reduce_row(reduced_matrix, acc_row, size);
  reduce_column(reduced_matrix, acc_column, size);

  for (int i = 0; i < size; i++) {
    if (acc_column[i] < DBL_MAX) {
      cost += acc_column[i];
    }
    if (acc_row[i] < DBL_MAX) {
      cost += acc_row[i];
    }
  }
  return cost;
}

void add_node(struct s_data *data, struct s_node node) {
  // data->nodes = (struct s_node *)realloc(data->nodes, data->len + 1);
  data->nodes[data->len] = node;
  data->len += 1;
}

void remove_node(struct s_data *data, int index) {
  for (int i = index; i < data->len - 1; i++) {
    data->nodes[i] = data->nodes[i + 1];
  }
  data->len -= 1;
}

int search_min_cost(struct s_data *data) {
  double min = DBL_MAX;
  int index_min = -1;
  double cost;
  for (int i = data->len - 1; i >= 0; i--) {
    cost = data->nodes[i].cost;
    if (cost < min) {
      min = cost;
      index_min = i;
    }
  }
  // printf("search_min_cost : minimim %f\n", min);
  return index_min;
}

void print_nodes(struct s_data *data) {
  for (int i = 0; i < data->len; i++) {
    int len_p = data->nodes[i].len_path;
    double c = data->nodes[i].cost;
    int v = data->nodes[i].vertex;
    int l = data->nodes[i].level;
    // printf("\033[0;32mNode = \033[0;34m%d , \033[0;32mlen_path = \033[0;34m%d
    // "
    //        ", \033[0;32mcost = \033[0;34m%f , \033[0;32mvertex = \033[0;34m%d
    //        "
    //        ", \033[0;32mlevel = \033[0;34m%d \033[0m\n",
    //        i, len_p, c, v, l);
  }
}

struct s_node solve_bb(double *raw_matrix, int size) {

  // creation of root
  struct s_node root;
  allocate_node(&root, raw_matrix, size);
  set_path(&root, size, 0, -1, 0);
  root.cost = compute_cost(root.reduced_matrix, size);

  struct s_data data;
  data.nodes = (struct s_node *)malloc(size * size * sizeof(struct s_node));
  data.nodes[0] = root;
  data.len = 1;
  while (data.len > 0) {
    print_nodes(&data);
    // search the node with the minimum cost value
    int index_min = search_min_cost(&data);
    struct s_node min_node = data.nodes[index_min];
    remove_node(&data, index_min);
    int min_vertex = min_node.vertex;
    if (min_node.level == size - 1) {
      // end of the computation
      min_node.path[min_node.len_path].from = min_vertex;
      min_node.path[min_node.len_path].to = 0;
      min_node.len_path += 1;
      return min_node;
    }
    for (int j = 0; j < size; j++) {
      if (min_node.reduced_matrix[min_vertex * size + j] < DBL_MAX) {
        struct s_node new_node;
        allocate_node(&new_node, min_node.reduced_matrix, size);
        copy_path(&min_node, &new_node);
        set_path(&new_node, size, min_node.level + 1, min_vertex, j);
        double reduced_value = min_node.reduced_matrix[min_vertex * size + j];
        //        printf("reduced_value : %d\n", reduced_value);
        double computed_cost = compute_cost(new_node.reduced_matrix, size);
        //      printf("computed_cost : %d\n", computed_cost);
        new_node.cost = min_node.cost + reduced_value + computed_cost;

        // printf("reduced_value : %f , computed_cost : %f , new_node.cost :
        // %f\n",
        //       reduced_value, computed_cost, new_node.cost);
        add_node(&data, new_node);
      }
    }
  }
}

int print_info(void) {
  printf(
      "\033[0;35mCommand :\033[0m \n\n\033[0;35m    With custom file :\033[0m "
      "        ./branch -f "
      "values.txt\n\033[0;35m    With "
      "embedded file  :\033[0m      ./branch -e\n\033[0;35m    With n * n "
      "random "
      "values :\033[0m ./branch -n "
      "number\n\n");
  return 0;
}

int main(int argc, char **argv) {
  printf("\n \033[0;33m--- Branch And Bound method ---\033[0m \n\n");

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

  struct s_node min_node = solve_bb(p.raw_matrix, p.size);

  clock_gettime(CLOCK_MONOTONIC, &stop_clock);
  unsigned long long total_clock_nsec;
  double total_clock_sec, total_clock_msec;
  total_clock_nsec = 1000000000 * (stop_clock.tv_sec - start_clock.tv_sec) +
                     stop_clock.tv_nsec - start_clock.tv_nsec;
  total_clock_sec = (double)(total_clock_nsec / 1000000000.0);
  total_clock_msec = (double)(total_clock_nsec / 1000000.0);

  printf("\n\033[0;33mSolution :\033[0m\n\033[0;33m - Length :\033[0m "
         "%f\n\033[0;33m - Path   :\033[0m ",
         min_node.cost);
  for (int i = 0; i < p.size; i++) {
    printf("%d - ", min_node.path[i].from);
  }
  printf("0\n\n");

  printf("\033[0;33mElapsed time :\033[0m \n \033[0;33m- s      :\033[0m %f\n "
         "\033[0;33m- ms     :\033[0m %f\n\n\n",
         total_clock_sec, total_clock_msec);

  return 0;
}
