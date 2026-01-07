#include "questioncreation.h"

char * types[18] = {"fire", "water", "grass", "electric", "ghost", "poison", "ice", "dragon", "bug", "normal", "fighting", "flying", "ground", "rock", "psychic", "dark", "steel", "fairy"};
char * stat_types[6] = {"hp", "attack", "defense", "special attack", "special defense", "speed"};
char * qtypes[12] = {"name", "gen", "height", "weight", "type", "hp", "attack", "defense", "special attack", "special defense", "speed", "base experience"};

int main(){
  char * answers[4];
  for(int i = 0; i < 4; i++)
  {
    answers[i] = malloc(15);
  }
  int ans = questioncreation(answers);
  printf("Ans Pos: %d\n", ans);
  for(int i = 0; i < 4; i++)
  {
    printf("%s, ", answers[i]);
  }
  printf("\n");
}

int questioncreation(char ** answers){

  time_t t;

  srand((unsigned) time(&t));
  int rand_types = rand() % 18;
  int rand_stat = rand() % 6;
  // printf("TypeRand: %d\nStatRand: %d\n", rand_types, rand_stat);
  // printf("Type: %s\nStat: %s\n", types[rand_types], stat_types[rand_stat]);

  int rand_qtype = rand() % 12;
  if(rand_qtype == 0){rand_qtype = 1;}
  // printf("Rand Qtype: %d\nQuestion: %s\n", rand_qtype, qtypes[rand_qtype]);

  int rand_dexnum = rand() % 1025;
  if(rand_dexnum <= 0){rand_dexnum = 1;}
  // printf("Rand Dexnum: %d\n",rand_dexnum);

  char pkmn_file[15] = "data/";
//  printf("%s\n",pkmn_file);
  char dexnum_str[10];
  sprintf(dexnum_str, "%d.txt", rand_dexnum);
//   printf("%s\n",dexnum_str);
  strcat(pkmn_file,dexnum_str);
//  printf("%s\n", pkmn_file);

  FILE * info_file = fopen(pkmn_file, "r");
  char info_buff[100];
  fgets(info_buff, 100, info_file);
  // printf("%s\n",info_buff);

  char * qdata[12];
  char * qdata_buff = info_buff;
  for(int i = 0; i < 12; i++)
  {
    qdata[i] = strsep(&qdata_buff, ";");
  }

  rand_qtype = 3;
  printf("What is %s's %s?\nAns: %s\n", qdata[0], qtypes[rand_qtype], qdata[rand_qtype]);


  int ans_position = rand() % 4;

  switch (rand_qtype){
    case 1:
      int generation;
      sscanf(qdata[rand_qtype], "%d", &generation);
      answers[ans_position] = qdata[rand_qtype];

      for(int i = 1; i < 4; i++)
      {
        int temp_gen = generation;
        if(i == 1){temp_gen -= 1;}
        if(i == 2){temp_gen += 1;}
        if(i == 3){temp_gen += 2;}
        if(temp_gen < 1){temp_gen += 9;}
        if(temp_gen > 9){temp_gen -= 9;}

        int index = i + ans_position;
        if(index < 0){index += 4;}
        if(index > 3){index -= 4;}
      //  printf("%d, %d\n", temp_gen, index);
        char temp_str[15];
        sprintf(temp_str, "%d", temp_gen);
      //  printf("%s\n",temp_str);
      //  answers[index] = NULL;
        strcpy(answers[index],temp_str);
        //printf("%s\n",answers[index]);
      }

      break;
    case 2: case 3:
      int real_val;
      sscanf(qdata[rand_qtype], "%d", &real_val);
      answers[ans_position] = qdata[rand_qtype];


      break;
    case 4:
      printf("not default\n");
      printf("%s\n", qtypes[rand_qtype]);
      break;
    case 11:
      printf("not default\n");
      printf("%s\n", qtypes[rand_qtype]);
      break;
    default:
      printf("%s\n", qtypes[rand_qtype]);
      break;
  }

  return ans_position;
}
