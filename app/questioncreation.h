#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#ifndef QUESTIONCREATION_H
  #define QUESTIONCREATION_H
  int questioncreation(int rand_dexnum, int rand_qtype, char ** answers);
  int isIn(char ** arr, char * str);
  void getName(int dexnum, char * pkmn_name);

#endif
