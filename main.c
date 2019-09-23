#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

const char* boolValues[2] = {"false", "true"};
const char* optString = "x:n:lh";

struct globalArgs_t {
  bool stringNumbers;
  int minLength;
  int maxLength;
  char* inputPath;
  char* outputPath;
  FILE* inputFile;
  FILE* outputFile;
} globalArgs;

char* getSentence(int *stringNumber) {
  char *sentence = NULL; //��ப� ��� ��ࠡ�⪨
  char *tooutput = NULL; //��ப� ��� �뢮��
  int i = 1, oi = 1;
  char letter;

  while (!feof(globalArgs.inputFile)){
    fscanf(globalArgs.inputFile, "%c", &letter);

    if (letter == 10) { //�஢�ઠ �� ��७�� ��ப�
      *stringNumber += 1;
      continue;
    }

    tooutput = (char*)realloc(tooutput, oi * sizeof(char));
    tooutput[oi-1] = letter;
    oi++;

    if ((letter >= 97 && letter <= 122) || (letter >= 65 && letter <= 90)) {
      sentence = (char*)realloc(sentence, i * sizeof(char));
      sentence[i-1] = tolower(letter);
      i++;
    }

    if (letter == ('.')) {
      break;
    }
  }

  bool polinomeFlag = true;
  for (int j = 0; j < (i-1) / 2; j++) {
    if (sentence[j] != sentence[i-j-2]) polinomeFlag = false;
  }

  if ((globalArgs.maxLength != 0) && (oi-1 > globalArgs.maxLength)) polinomeFlag = false;
  if ((globalArgs.minLength != -1) && (oi-1 < globalArgs.minLength)) polinomeFlag = false;
  if (oi == 1 || oi == 2) polinomeFlag = false;

  if (polinomeFlag) {
    if (globalArgs.stringNumbers) fprintf(globalArgs.outputFile, "%d: ", *stringNumber);
    for (int j = 0; j < oi-1; j++) fprintf(globalArgs.outputFile, "%c", tooutput[j]);
    fprintf(globalArgs.outputFile, "\n");
  }

  return sentence;
}

void display_usage(char* name) {
  //�⮡ࠦ���� ��࠭�窨 � �������
  printf("\nUSAGE:\n%s [-h] [-n <length>] [-x: <length>] [-l] [-I <path>] [-O <path>]\n\nARGS: \n-n: Min palindrome length \n-x: Max palindrome length\n-l: Enable string numeration\n-h: Help\n\n", name);
  exit(EXIT_SUCCESS);
}

int getStartData(int argc, char** argv) {
  int opt = 0;

  //���।������ ��㬥�⮢ ���ᨢ� �� ��६����
  opt = getopt(argc, argv, optString);
  while (opt != -1) {
    switch (opt) {
      case 'x':
        globalArgs.maxLength = atoi(optarg);
        break;
      case 'n':
        globalArgs.minLength = atoi(optarg);
        break;
      case 'l':
        globalArgs.stringNumbers = true;
        break;
      case 'h':
        display_usage(argv[0]);
        break;
      default:
        break;
    }
    opt = getopt(argc, argv, optString);
  }

  //���ᨭ� ��⥩
  if (optind < argc) {
    globalArgs.inputPath = argv[optind++];
    if (optind < argc) {
      globalArgs.outputPath = argv[optind];
    }
  }

  //�뢮� ��������� ������

  /*
  printf("\nINPUT DATA:\n");
  if (globalArgs.maxLength != 0) printf(" - Max palindrome length: %d\n", globalArgs.maxLength);
  if (globalArgs.minLength != -1) printf(" - Min palindrome length: %d\n", globalArgs.minLength);
  printf(" - Enable string numeration: %s\n", boolValues[globalArgs.stringNumbers]);
  if (globalArgs.inputPath != NULL) printf(" - Input path: %s\n", globalArgs.inputPath);
  if (globalArgs.outputPath != NULL) printf(" - Output path: %s\n", globalArgs.outputPath);
  printf("\n"); */

  //�஢�ઠ �� ��୮ �������� �����
  if (globalArgs.maxLength < 0) {
    fprintf(stderr, "Incorrect max palindrome length!\n");
    return 0;
  }
  if (globalArgs.minLength < -2) {
    fprintf(stderr, "Incorrect min palindrome length!\n");
    return 0;
  }
  if (globalArgs.minLength > globalArgs.maxLength && globalArgs.maxLength != 0) {
    fprintf(stderr, "Min palindrome lengthh is geather tham max!\n");
    return 0;
  }
  return 1;
}

int main(int argc, char** argv) {

  //���樠������ ��������
  globalArgs.stringNumbers = false;
  globalArgs.minLength = -1;
  globalArgs.maxLength = 0;
  globalArgs.inputPath = NULL;
  globalArgs.outputPath = NULL;

  //��ࠡ�⪠ ���祩
  if (getStartData(argc, argv) == 0) printf("Error occured - programm has stopped\n");

  //�஢�ઠ ��⥩
  if (globalArgs.outputPath != NULL) {
    if ((globalArgs.outputFile = fopen(globalArgs.outputPath, "w")) == NULL) {
      fprintf(stderr, "Can not open output file!\n");
      exit(EXIT_FAILURE);
    }
  } else {
    //TODO: stdout;
  }

  if (globalArgs.inputPath != NULL) {
    if ((globalArgs.inputFile = fopen(globalArgs.inputPath, "r")) == NULL) {
      fprintf(stderr, "Can not open input file!\n");
      exit(EXIT_FAILURE);
    } else {
      char* sentence;
      int stringNumber = 1;
      while (!feof(globalArgs.inputFile)) getSentence(&stringNumber);
    }
  } else {
    //TODO: stdin
  }

  return 0;
}