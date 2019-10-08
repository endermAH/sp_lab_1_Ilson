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
  char *sentence = NULL; //Строка для обработки
  char *tooutput = NULL; //Строка для вывода
  int i = 1, oi = 1;
  char letter;

  while (!feof(globalArgs.inputFile)){
    fscanf(globalArgs.inputFile, "%c", &letter);

    if (letter == 10) { //Проверка на перенос строки
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
    //Если -l нужно добавлять в начале каждой строки номер строки в которой найден полином
    if (globalArgs.stringNumbers) {
      int number = *stringNumber;
      int numberLength = 0;

      //Определение длины числа
      while (number / 10 != 0) {
        numberLength++;
        number /= 10;
      }
      numberLength++;
      //printf("%d\n", numberLength);

      //Запись числа в начало строки
      for (int j = numberLength; j > 0; j--) {
        if ((j == numberLength) && (*stringNumber / pow(10, j) < 1)) continue;
        addCharToOutput(*stringNumber / pow(10, j) + '0');
      }
      addCharToOutput(*stringNumber % 10 + '0');
      addCharToOutput(':');
      addCharToOutput(' ');
    }

    //Переписываем полиндром во временный массив
    for (int j = 0; j < oi-1; j++) {
      addCharToOutput(tooutput[j]);
    }
    addCharToOutput('\n');
  }

  if (!feof(globalArgs.inputFile)) getSentence(stringNumber);
  return sentence;
}*/

void display_usage(char* name) {
  //Отображение странички с помощью
  printf("\nUSAGE:\n%s [-h] [-k <key word>] [-d] [-c] \n\nARGS: \n-k: Key word \n-c: Encoding\n-d: Decoding\n-h: Help\n\n", name);
  exit(EXIT_SUCCESS);
}

int getStartData(int argc, char** argv) {
  int opt = 0;

  //Распределение аргументов массива по переменным
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

  //Парсинг путей
  if (optind < argc) {
    globalArgs.inputPath = argv[optind++];
    if (optind < argc) {
      globalArgs.outputPath = argv[optind];
    }
  }

  //Вывод введенных данных

  /*
  printf("\nINPUT DATA:\n");
  if (globalArgs.maxLength != 0) printf(" - Max palindrome length: %d\n", globalArgs.maxLength);
  if (globalArgs.minLength != -1) printf(" - Min palindrome length: %d\n", globalArgs.minLength);
  printf(" - Enable string numeration: %s\n", boolValues[globalArgs.stringNumbers]);
  if (globalArgs.inputPath != NULL) printf(" - Input path: %s\n", globalArgs.inputPath);
  if (globalArgs.outputPath != NULL) printf(" - Output path: %s\n", globalArgs.outputPath);
  printf("\n"); */

  //Проверка на верно введенные данные
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

  //Инициализация структуры
  globalArgs.optionEncode = false;
  globalArgs.optionDecode = false;
  globalArgs.key = NULL;
  globalArgs.inputPath = NULL;
  globalArgs.outputPath = NULL;

  //Обработка ключей
  if (getStartData(argc, argv) == 0) printf("Error occured - programm has stopped\n");

  //Проверка путей
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

  //getSentence(&stringNumber);

  // if (endOfOutputString != 0) {
  //   for (int i = 0; i <= endOfOutputString; i++) {
  //     fprintf(globalArgs.outputFile, "%c", outputString[i]);
  //   }
  // }

  return 0;
}
