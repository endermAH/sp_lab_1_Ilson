#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

const char *boolValues[2] = {"false", "true"};
const char *optString = "k:cdh";

char *outputString = NULL;
int endOfOutputString = 0;

struct globalArgs_t {
  bool optionEncode;
  bool optionDecode;
  char* key;
  char* inputPath;
  char* outputPath;
  FILE* inputFile;
  FILE* outputFile;
} globalArgs;

void addCharToOutput(char target_char) {
  endOfOutputString++;
  outputString = realloc(outputString, endOfOutputString * sizeof(char));
  outputString[endOfOutputString-1] = target_char;
}

/*char* getSentence(int *stringNumber) {
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
    //�᫨ -l �㦭� ��������� � ��砫� ������ ��ப� ����� ��ப� � ���ன ������ �������
    if (globalArgs.stringNumbers) {
      int number = *stringNumber;
      int numberLength = 0;

      //��।������ ����� �᫠
      while (number / 10 != 0) {
        numberLength++;
        number /= 10;
      }
      numberLength++;
      //printf("%d\n", numberLength);

      //������ �᫠ � ��砫� ��ப�
      for (int j = numberLength; j > 0; j--) {
        if ((j == numberLength) && (*stringNumber / pow(10, j) < 1)) continue;
        addCharToOutput(*stringNumber / pow(10, j) + '0');
      }
      addCharToOutput(*stringNumber % 10 + '0');
      addCharToOutput(':');
      addCharToOutput(' ');
    }

    //��९��뢠�� ������஬ �� �६���� ���ᨢ
    for (int j = 0; j < oi-1; j++) {
      addCharToOutput(tooutput[j]);
    }
    addCharToOutput('\n');
  }

  if (!feof(globalArgs.inputFile)) getSentence(stringNumber);
  return sentence;
}*/

void startJob() {
  char inChar;
  char outChar;
  int index = 0;
  int len;
  int n = 127 - 32 + 1;

  len = strlen(globalArgs.key);

  while (!feof(globalArgs.inputFile)){
    fscanf(globalArgs.inputFile,"%c",&inChar);
    if (globalArgs.optionEncode) {
        if (inChar >= 32 && inChar <= 127){
          outChar = (((inChar - 32) + (globalArgs.key[index] - 32)) % 96) + 32;
          //printf("%d -> %d with %d\n", inChar, outChar, globalArgs.key[index]);
          index++;
          if (index == len) index = 0;
        } else outChar = inChar;
        addCharToOutput(outChar);
    } else if (globalArgs.optionDecode) {
      if (inChar >= 32 && inChar <= 127){
        outChar = (((inChar - 32) - (globalArgs.key[index] - 32) + n) % n) + 32;
        index++;
        if (index == len) index = 0;
      } else outChar = inChar;
      addCharToOutput(outChar);
    }
  }
}

void display_usage(char* name) {
  //�⮡ࠦ���� ��࠭�窨 � �������
  printf("\nUSAGE:\n%s [-h] [-k <key word>] [-d] [-c] \n\nARGS: \n-k: Key word \n-c: Encoding\n-d: Decoding\n-h: Help\n\n", name);
  exit(EXIT_SUCCESS);
}

int getStartData(int argc, char** argv) {
  int opt = 0;

  //���।������ ��㬥�⮢ ���ᨢ� �� ��६����
  opt = getopt(argc, argv, optString);
  while (opt != -1) {
    switch (opt) {
      case 'k':
        globalArgs.key = optarg;
        break;
      case 'd':
        globalArgs.optionDecode = true;
        break;
      case 'c':
        globalArgs.optionEncode = true;
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
  if (globalArgs.optionDecode == globalArgs.optionEncode) {
    fprintf(stderr, "Encode and Decode options are same!\n");
    exit(EXIT_FAILURE);
  }
  if (globalArgs.key == NULL) {
    fprintf(stderr, "Can not run without key!\n");
    exit(EXIT_FAILURE);
  }
  // if (globalArgs.minLength > globalArgs.maxLength && globalArgs.maxLength != 0) {
  //   fprintf(stderr, "Min palindrome lengthh is geather tham max!\n");
  //   return 0;
  // }
  return 1;
}

int main(int argc, char** argv) {

  //���樠������ ��������
  globalArgs.optionEncode = false;
  globalArgs.optionDecode = false;
  globalArgs.key = NULL;
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
    globalArgs.outputFile = stdout;
  }

  bool setToStdout = false;

  if (globalArgs.inputPath != NULL) {
    if ((globalArgs.inputFile = fopen(globalArgs.inputPath, "r")) == NULL) {
      fprintf(stderr, "Can not open input file!\n");
      exit(EXIT_FAILURE);
    }
  } else {
    globalArgs.inputFile = stdin;
  }

  char* sentence;
  int stringNumber = 1;

  startJob();
  //getSentence(&stringNumber);


  if (endOfOutputString != 0) {
    for (int i = 0; i <= endOfOutputString; i++) {
      fprintf(globalArgs.outputFile, "%c", outputString[i]);
    }
  }

  return 0;
}
