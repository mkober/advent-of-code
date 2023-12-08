#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **lines;
  int lineCount;
} DataFile;

DataFile loadDataFile() {

  DataFile df;

  FILE *fp = fopen("data.txt","r");
  //FILE *fp = fopen("data-test.txt","r");
  //FILE *fp = fopen("part2-data-example.txt","r");

  if (!fp) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  df.lines = NULL;
  df.lineCount = 0;

  while ((nread = getline(&line, &len, fp)) != -1) {
    df.lineCount++;

    df.lines = realloc(df.lines, df.lineCount * sizeof(char*));
    if (!df.lines) {
      perror("Memory allocation failed");
      fclose(fp);
      free(line);
      exit(EXIT_FAILURE);
    }

    df.lines[df.lineCount - 1] = malloc(nread + 1);
    if (!df.lines[df.lineCount - 1]) {
      perror("Memory allocation failed");
      fclose(fp);
      free(line);
      exit(EXIT_FAILURE);

      for (int i; i < df.lineCount - 1; i++) {
        free(df.lines[i]); 
      }

      free(df.lines);
      exit(EXIT_FAILURE);
    }

    memcpy(df.lines[df.lineCount - 1], line, nread);
    df.lines[df.lineCount - 1][nread] = '\0';
  }

  free(line);
  fclose(fp);

  return df;
}


int main(void) {

  static char first = '\0', last = '\0';
  static char calibration[3];

  static int firstPartTotal; 
  static int secondPartTotal; 
 
  DataFile df = loadDataFile();

  // Part 1
  for (int a=0; a < df.lineCount; a++) {
    for (int b=0; df.lines[a][b] != '\0'; b++) {
      if (isdigit(df.lines[a][b])) {
        if (first == '\0') {
          first = df.lines[a][b];
        }
        else {
          last = df.lines[a][b];
        }
      }
    }

    if (last == '\0') {
      last = first;
    }

    calibration[0] = first;
    calibration[1] = last;
    calibration[2] = '\0';

    firstPartTotal = firstPartTotal + atoi(calibration);

    first = '\0';
    last = '\0';
  }
  printf("Total for Part 1: %d\n",firstPartTotal);

  // Part 2
  static char *number[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
  };

  int totalNumbers = sizeof(number) / sizeof(number[0]);

  static int len = 0;
  static int lineNum[100];

  for (int a=0; a < df.lineCount; a++) {

    char * pch;
    int static numPos;
    char numChr;

    // Search the whole line for number words
    for (int n=0; n < totalNumbers; n++) {
      pch = strstr(df.lines[a], number[n]);
      while (pch != NULL) {
        numPos = (pch - df.lines[a]);
        lineNum[numPos] = n+1;
        pch = strstr(pch + 1, number[n]);
      }
    }

    // Check for digits in the line
    for (int b=0; df.lines[a][b] != '\0'; b++) {
      if (isdigit(df.lines[a][b])) {
        lineNum[b] = df.lines[a][b] - '0';
      }
    }

    // Get the first and last
    for (int n=0; n < 100; n++) {
      if (lineNum[n] != 0) {
        if (first == '\0') {
          first = lineNum[n] + '0';
        }
        else {
          last = lineNum[n] + '0';
        }
      }
    }

    if (last == '\0') {
      last = first;
    }

    calibration[0] = first;
    calibration[1] = last;

    secondPartTotal = secondPartTotal + atoi(calibration);

    first = '\0';
    last = '\0';

    for (int n=0; n < 100; n++) {
      lineNum[n] = 0;
    }
  }

  printf("Total for Part 2: %d\n",secondPartTotal);

  for (int i = 0; i < df.lineCount; i++) {
    free(df.lines[i]);
  }

  free(df.lines);
}
