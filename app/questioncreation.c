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

char * types[18] = {"fire", "water", "grass", "electric", "ghost", "poison", "ice", "dragon", "bug", "normal", "fighting", "flying", "ground", "rock", "psychic", "dark", "steel", "fairy"};
char * stat_types[6] = {"hp", "attack", "defense", "special attack", "special defense", "speed"};
char * qtypes[12] = {"name", "gen", "height", "weight", "type", "hp", "attack", "defense", "special attack", "special defense", "speed", "base experience"};

int main(){
  time_t t;

  srand((unsigned) time(&t));
  int rand_types = rand() % 18;
  int rand_stat = rand() % 6;
  printf("TypeRand: %d\nStatRand: %d\n", rand_types, rand_stat);
  printf("Type: %s\nStat: %s\n", types[rand_types], stat_types[rand_stat]);

  int rand_qtype = rand() % 12;
  printf("Rand Qtype: %d\nQuestion: %s\n", rand_qtype, qtypes[rand_qtype]);

  int rand_dexnum = rand() % 1025;
  printf("Rand Dexnum: %d\n",rand_dexnum);
}
