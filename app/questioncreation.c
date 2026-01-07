#include "questioncreation.h"

char * types[18] = {"fire", "water", "grass", "electric", "ghost", "poison", "ice", "dragon", "bug", "normal", "fighting", "flying", "ground", "rock", "psychic", "dark", "steel", "fairy"};
char * stat_types[6] = {"hp", "attack", "defense", "special attack", "special defense", "speed"};
char * qtypes[12] = {"name", "gen", "height (dm)", "weight (dg)", "type", "hp", "attack", "defense", "special attack", "special defense", "speed", "base experience"};

int main(){
  char * answers[4];
  for(int i = 0; i < 4; i++)
  {
    answers[i] = calloc(15, 1);
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
        char temp_str[15];
        sprintf(temp_str, "%d", temp_gen);
        while(temp_gen == generation || isIn(answers,temp_str))
        {
          temp_gen = (rand() % 9) + 1;
          sprintf(temp_str, "%d", temp_gen);
        }

        int index = i + ans_position;
        if(index < 0){index += 4;}
        if(index > 3){index -= 4;}
      //  printf("%d, %d\n", temp_gen, index);


      //  printf("%s\n",temp_str);
      //  answers[index] = NULL;
        strcpy(answers[index],temp_str);
        //printf("%s\n",answers[index]);
      }

      break;
    case 2: case 3:
      int real_val;
      sscanf(qdata[rand_qtype], "%d", &real_val);
      // printf("%s-\n", double_str);
      strcpy(answers[ans_position],qdata[rand_qtype]);
      // printf("%s--\n", answers[ans_position]);

      for(int i = 1; i < 4; i++)
      {
        int temp_val = real_val;
        char temp_str[15];
        sprintf(temp_str, "%d", temp_val);
        while(temp_val == real_val || isIn(answers,temp_str) || temp_val == 0)
        {
          double rand_offset = 0;
          if( rand() % 2 == 0)
          {
            rand_offset = ((rand() % 20) + 65) / 100.0;
          }
          else
          {
            rand_offset = ((rand() % 20) + 15) / 100.0;
          }

          temp_val *= rand_offset;
          if(temp_val == 0){temp_val += 1;}
          if(real_val < 10){temp_val += rand() % 10;}
          sprintf(temp_str, "%d", temp_val);
        }
        int index = i + ans_position;
        if(index < 0){index += 4;}
        if(index > 3){index -= 4;}

      //  printf("%s\n",temp_str);

        strcpy(answers[index],temp_str);
      }

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

int isIn(char ** arr, char * str)
{
  int i = 0;

  int final = 0;
  while(i < 4)
  {
    //printf("%s--\n", arr[i]);
    if(strcmp(arr[i],str) == 0){final = 1;}
    i++;
  }
  return final;
}
