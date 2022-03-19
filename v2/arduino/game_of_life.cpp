#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define W 16
#define H 16

int matrix[H][W];
int neighbours_matrix[H][W];
int countNeighbours(int j, int i);

int min(int a, int b){
  return a <= b ? a : b;
}

int max(int a, int b){
  return a >= b ? a : b;
}

void printMatrix(){
  for(int i = 0; i < W; i++){
    for(int j = 0; j < H; j++){
      if(matrix[j][i] == 1){
        printf("# ");
      } else {
        printf(". ");
      }
    }
    printf("\n");
  }
}

void printNeighbours(){
  for(int i = 0; i < W; i++){
    for(int j = 0; j < H; j++){
      printf("%d ", neighbours_matrix[j][i]);
    }
    printf("\n");
  }
}

void calcNeighbours(){
  for(int i = 0; i < W; i++){
    for(int j = 0; j < H; j++){
      neighbours_matrix[j][i] = countNeighbours(j, i);
    }
  }
}

void initMatrix(){
  matrix[2][2] = 1;
  matrix[2][3] = 1;
  matrix[3][3] = 1;
  matrix[3][4] = 1;
  matrix[5][5] = 1;
  matrix[5][6] = 1;
  matrix[6][7] = 1;
  matrix[7][7] = 1;
  matrix[6][6] = 1;
  matrix[8][7] = 1;

  matrix[10][11] = 1;
  matrix[10][10] = 1;
  matrix[11][12] = 1;
  matrix[12][11] = 1;
  matrix[12][12] = 1;
  matrix[11][11] = 1;
  matrix[12][13] = 1;

}

int countNeighbours(int j, int i){
  int count = -matrix[j][i];
  for(int y = max(0, j-1); y <= min(H-1, j+1); y++){
    for(int x = max(0, i-1); x <= min(W-1, i+1); x++){
      count += matrix[y][x];
    }
  }
  return count;
}

void stepMatrix(){
  for(int i = 0; i < W; i++){
    for(int j = 0; j < H; j++){
      int neighBours = neighbours_matrix[j][i];
      if(matrix[j][i] == 1 && (neighBours < 2 || neighBours > 3)){
        matrix[j][i] = 0;
      } else if(matrix[j][i] == 0 && neighBours == 3){
        matrix[j][i] = 1;
      }
    }
  }
}

int main(){
  initMatrix();
  for(int t = 0; t < 30; t++){
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
    printMatrix();
    calcNeighbours();
    stepMatrix();
    usleep(300000);
  }
  return 0;
}
