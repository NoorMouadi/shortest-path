#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 100
typedef struct QueueRecord *Queue;
char cities[max][100];
int matrix[max][max];
int numVertices = 0;
int numEdges = 0;
int totalCost = 0;
int INF = 999999;
int cost[max];
int parent[max];
bool visited[max];
int pathLong = 0;
char path[max][2 * max];
int pathbfs[max];
int pathLengthbfs = 0;
int totalCostbsf = 0;
void menu();
void readFile();
void addEdge(char source[], char dis[], int distance);
int findIndex(char v[]);
void getPath(char source[], char destination[]);
int isExistInPath(char city[]);
int isExist(char v[]);
void dijkstra(char source[], bool visited[], int cost[], int parent[]);
int find_minimum(bool visited[], int cost[]);
void makeEmpty();
void bfs(char source[], char detionation[]);
void findTotalCost(char source[], char destination[]);
int main(void) {
  FILE *out;
  int choose;
  int readOnce = 0;
  char sr[max];
  char destination[max];
  makeEmpty();

  while (1) {
    menu();
    scanf("%d", &choose);

    switch (choose) {
    case 1:
      // this to prevent read file more than once
      if (readOnce == 0) {
        readFile();
        readOnce = 1;
      } else {
        printf("The file has already been read.\n");
      }
      break;
    case 2:
      if (readOnce == 0) {
        printf("You should read the file first.\n");
      } else {
        printf("Enter the source city:\n");
        scanf("%s", sr);
        // check if the entered source city exist
        if (!isExist(sr)) {
          printf("This city does not exist.\n");
          continue; // Continue to the next iteration of the loop
        }
      }
      break;
    case 3:
      if (readOnce == 0) {
        printf("You should read the file first.\n");
      } else {
        printf("Enter the destination city:\n");
        scanf("%s", destination);
        getPath(sr, destination);
        // check if the destination city exist
        if (!isExist(destination)) {
          printf("This city does not exist.\n");
          continue; // Continue to the next iteration of the loop
        }
        printf("the shortest way from %s to %s using Dijkestra:\n", sr,
               destination);
        for (int i = 0; i < pathLong - 1; i++) {
          printf("%s --> %dKm --> ", path[i],
                 matrix[findIndex(path[i])][findIndex(path[i + 1])]);
          totalCost += matrix[findIndex(path[i])][findIndex(path[i + 1])];
        }

        printf("%s\n", path[pathLong - 1]);
        pathLong = 0;
      }
      printf("\n");
      printf("total cost: %d\n\n", totalCost);
      pathLengthbfs = 0;
      bfs(sr, destination);
      break;

    case 4:
      // print the result at the file and exist
      out = fopen("shortest_distance.txt", "w");
      getPath(sr, destination);
      for (int i = 0; i < pathLong - 1; i++) {
        // path[i] ->all the cities in the path are saved in this array
        fprintf(out, "%s --> %dKm --> ", path[i],
                matrix[findIndex(path[i])][findIndex(path[i + 1])]);
      }
      fprintf(out, "%s", destination);
      fprintf(out, "\n");
      fprintf(out, "total cost: %d\n", totalCost);
      fprintf(out, "path from %s to %s in bfs: ", sr, destination);
      for (int i = pathLengthbfs - 1; i >= 0; i--) {
        if (strcmp(cities[pathbfs[i]], destination) != 0)
          fprintf(out, "%s-->%d", cities[pathbfs[i]],
                  matrix[pathbfs[i]][pathbfs[i - 1]]);
        if (i > 0) {
          fprintf(out, " --> ");
        }
      }
      fprintf(out, "%s", destination);
      fprintf(out, "\n");
      fprintf(out, "total cost using bfs: %d\n", totalCostbsf);
      exit(1);
      break;

    default:
      printf("Invalid choice. Please try again.\n");
    }
  }
  return 0;
}

void readFile() {
  // reading the file contents
  FILE *file;
  file = fopen("cities.txt", "r");
  if (file == NULL) {
    printf("Unable to open the file.\n");
    exit(1);
  }

  char line[100];
  char source[100], destination[100];
  int distance;
  int emptyFile = 1;

  while (fgets(line, sizeof(line), file) != NULL) {
    emptyFile = 0;
    // sscanf returns the number of input items successfully matched and
    // assigned
    if (sscanf(line, "%s %s %dkm", source, destination, &distance) == 3) {
      if (!isExist(source)) {
        strcpy(cities[numVertices], source);
        numVertices++;
      }

      if (!isExist(destination)) {
        strcpy(cities[numVertices], destination);
        numVertices++;
      }
      // adding edge between the source and the destination
      addEdge(source, destination, distance);
    }
  }

  if (emptyFile) {
    printf("The file is empty.\n");
    exit(1);
  }

  fclose(file);
}

void makeEmpty() {
  // build the graph
  int i, j;
  for (i = 0; i < max; i++) {
    for (j = 0; j < max; j++) {
      matrix[i][j] = INF;
    }
  }
}

void addEdge(char source[], char dis[], int distance) {
  // add edges between the cities
  int index_i = findIndex(source);
  int index_j = findIndex(dis);
  matrix[index_i][index_j] = distance;
  matrix[index_j][index_i] = distance;
  numEdges++;
}

int findIndex(char v[]) {
  // find the index that refers to city v
  for (int i = 0; i < numVertices; i++) {
    if (strcmp(cities[i], v) == 0)
      return i;
  }
  return -1;
}

int isExist(char v[]) {
  // check if a city is already exist
  for (int i = 0; i < numVertices; i++) {
    if (strcmp(cities[i], v) == 0)
      return 1;
  }
  return 0;
}

void menu() {
  printf("Choose a task:\n");
  printf("1. Load cities: Load the file and construct the graph.\n");
  printf("2. Enter source: Read the source city.\n");
  printf("3. Enter destination: Print the full route of the shortest path, "
         "including the distance between each two cities.\n");
  printf("4. Exit\n");
}

int find_minimum(bool visited[], int cost[]) {
  // find the minimium cost between the city and its adjacent
  int vertex = -1;
  int min_cost = INF;
  for (int i = 0; i < numVertices; i++) {
    if (!visited[i] && cost[i] < min_cost) {
      min_cost = cost[i];
      vertex = i;
    }
  }

  return vertex;
}

void dijkstra(char source[], bool visited[], int cost[], int parent[]) {
  int v = findIndex(source);
  // initial values
  for (int i = 0; i < numVertices; i++) {
    cost[i] = INF;
    visited[i] = false;
    parent[findIndex(source)] = -1;
  }
  // put the cost of the source zero
  cost[v] = 0;

  for (int i = 1; i <= numVertices; i++) {
    // at first min will =0 (cost of the source) all others is inf
    int min = find_minimum(visited,
                           cost); // find the minimum cost of the adjacent nodes
    for (int j = 0; j < numVertices; j++) {
      // check if there is an edge between the min cost and other nodes
      if (matrix[min][j] != INF) {
        if (visited[j] == false && cost[j] > matrix[min][j] + cost[min]) {
          // update the cost
          cost[j] = matrix[min][j] + cost[min];
          // update the parent
          parent[j] = findIndex(cities[min]);
        }
      }
    }
    // update visited to true
    visited[min] = true;
  }
}

void getPath(char source[], char destination[]) {
  // find the path between two cities
  int i = findIndex(destination); // i is the index refer to the destination
  int j = findIndex(source);      // i is the index refer to the source
  dijkstra(source, visited, cost, parent);

  if (cost[i] == INF) {
    printf("Source or destination city not found.\n");
    return;
  }

  while (i != j) {
    // checks if the current city i is not equal to the source city j
    strcpy(path[pathLong++], cities[i]);
    // let i refers to the parent of the city
    i = parent[i];
  }

  strcpy(path[pathLong++], source);
  // this for loop is used to reverse the order of elements in the path array
  for (int i = 0; i < pathLong / 2; i++) {
    char temp[100];
    strcpy(temp, path[i]);
    strcpy(path[i], path[pathLong - i - 1]);
    strcpy(path[pathLong - i - 1], temp);
  }
}

void bfs(char source[], char destination[]) {
  // Array to store the traversal order
  char traversalOrder[max][50];
  // Array to store the previous city on the shortest path
  int previous[max];

  // Initialize visited array
  for (int i = 0; i < numVertices; i++) {
    visited[i] = false;
    previous[i] = -1;
  }

  int sourceIndex = findIndex(source);
  int destinationIndex = findIndex(destination);

  // Mark the source city as visited
  visited[sourceIndex] = true;
  strcpy(traversalOrder[0], source);

  // Create a queue for BFS
  int queue[max];
  int front = 0;
  int rear = 0;

  // Enqueue the source city
  queue[rear++] = sourceIndex;

  while (front != rear) {
    // Dequeue a city from the queue
    int current = queue[front++];
    if (current == destinationIndex)
      break;

    // Visit all adjacent cities
    for (int i = 0; i < numVertices; i++) {
      if (matrix[current][i] != INF && !visited[i]) {
        // Mark the city as visited
        visited[i] = true;
        // Update the previous city
        previous[i] = current;
        // Enqueue the city
        queue[rear++] = i;
      }
    }
  }

  // Check if a path exists from source to destination
  if (!visited[destinationIndex]) {
    printf("No path exists from %s to %s.\n", source, destination);
    return;
  }

  // Reconstruct the shortest path

  int current = destinationIndex;
  while (current != -1) {
    pathbfs[pathLengthbfs++] = current;
    current = previous[current];
  }

  // Print the shortest path
  printf("Shortest path from %s to %s: ", source, destination);
  for (int i = pathLengthbfs - 1; i >= 0; i--) {
    if (strcmp(cities[pathbfs[i]], destination) != 0) {
      printf("%s-->%d", cities[pathbfs[i]],
             matrix[findIndex(cities[pathbfs[i]])]
                   [findIndex(cities[pathbfs[i - 1]])]);
      if (i > 0) {
        printf(" --> ");
      }
      totalCostbsf += matrix[findIndex(cities[pathbfs[i]])]
                            [findIndex(cities[pathbfs[i - 1]])];
    }
  }
  printf("%s", destination);
  printf("\n");
  printf("total cost using bfs: %d\n",totalCostbsf);
}
