#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>


char test[][100] = {"Hej","tomte","lol","knas","boll","skumtomte","skum","badskum","dator","mupp","mat","läsk","läskig","jacka","packa","telefon","Merry","Erika","Fredrik","Nicole","hylla","bord","stol","bänk","ponny", "ko", "mu", "hjälm", "jojjo", "låda", "katt", "hund" "apa", "måttband", "nycklar"};

typedef struct ware
{
  char *name;
  char *description;
  char location_char;
  int location_int;
  int price;
  int amount;
}kul; // konstigt namn.

struct database
{
  kul wares[120];
  int size;
};

struct database lager;


struct action_t
{
  enum { NOTHING, ADD, REMOVE, EDIT } type;
  union
  {
    struct { kul saved; };      // REMOVE
    struct { kul *edited; kul original; }; // EDIT
  };
};

void init_warehouse()
{
  for(int i = 0; i<120; i++)
    {
      lager.wares[i].name = strndup(test[i],128); //frigörs aldrig
    }
  lager.size = 34;
}


struct action_t action_memory = {.type = NOTHING};






#define Clear_stdin while (getchar() != '\n')
  ;

 char ask_question_char(char *q, char *alt)
 {
   char input = 0;

   printf("%s [%s]\n", q , alt);
   do {

     if (input)
       {
	 printf("Inget alternativ '%c', försök igen! [%s]\n", input, alt);
	 Clear_stdin;
       }

     input = getchar();
   } while (strchr(alt, input) == NULL);

   Clear_stdin;

   return input;
 }



 int ask_question_int(char *q)
 {
   char input[120];
   bool tillbaka = true;
   while (tillbaka)
     {
       printf("%s",q);
       tillbaka = false;
       scanf ("%s",input);
       for (int i = 0; input[i] != '\0'; i++)
	 {
	   if (!isdigit(input[i]))
	     {
	       puts("Ogiltigt värde, försök igen!");
	       tillbaka = true;
	       break;
	     }
	 }
       Clear_stdin;
     }
   return atoi(input);
 }





void string_fix (char *input)
{
  int längd = strlen(input)-1; // variabel namn med ä
  if (input[längd] == '\n')
    {
      input[längd] = '\0';
    }
}

char add_goods()     //TODO: En lägg till funktion
{
  kul ny_vara;
  bool loop = true;
  char buf[120];
  bool tillbaka = true;

  while (loop){

  puts("Namn:");
  fgets(buf, 120, stdin);
  string_fix(buf);
  ny_vara.name = strndup(buf,120); // frigörs aldrig

  puts("Description:");
  fgets(buf, 120, stdin);
  string_fix(buf);
  ny_vara.description = strndup(buf,120); // frigörs aldrig

  while (tillbaka)
    {
      tillbaka = false;

      char lager_char = toupper(ask_question_char ("Vilken plats (bokstav):", "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZzÅåÄäÖö"));
      ny_vara.location_char = lager_char;


      int lager_int = ask_question_int("Vilken plats (nummer):\n"); // kontrollerar inte om indata är mer än 2 siffror.
      ny_vara.location_int = lager_int;



      for (int i = 0; i < 120; i++)
	{

	  if (ny_vara.location_int == toupper(lager.wares[i].location_int) && toupper(ny_vara.location_char) == toupper(lager.wares[i].location_char))
	    {
	      puts("Lagerplatsen är tyvärr upptagen försök igen");
	      tillbaka = true;
	    }
	}
    }

  int lager_pris = ask_question_int("Hur mycket kostar varan:\n");
  ny_vara.price = lager_pris;


  int lager_antal = ask_question_int("Antal:\n");
  ny_vara.amount = lager_antal;


  puts("=======================");

  printf("Namn: %s \n", ny_vara.name);
  printf("Beskrivning: %s \n", ny_vara.description);
  printf("Plats: %c", ny_vara.location_char);
  printf("%i\n", ny_vara.location_int);
  printf("Pris: %i\n", ny_vara.price);
  printf("Antal: %i\n", ny_vara.amount);


  char yesnoedit = toupper(ask_question_char("Vill du verkligen lägga till varan?(Ja[J], Nej[N], Redigera[R]", "JjNnRr"));
  // finns inget case för R.
 if (yesnoedit == 'J')
   {
     lager.wares[lager.size] = ny_vara;
     action_memory.type = ADD;
     action_memory.edited = &lager.wares[lager.size];
     lager.size++;
     return 0;
   }
 else if (yesnoedit == 'N')
   {
     return 0;
     puts("");
     loop = false;
   }
   //else saknas
  }

  return 0;
}




char delete_goods(paj) //TODO: En ta bort funktion
{
  switch(toupper(ask_question_char("Vill du ta bort denna vara?","JjNn")))
    {
    case 'J':
      action_memory.type = REMOVE;
      action_memory.saved = lager.wares[paj];
      for (int i = paj; i < 120; i++)
	{
	  lager.wares[i] = lager.wares[i+1];
	}
      lager.wares[lager.size-1].name = NULL;
      lager.size--;
      break;

    case 'N':
      return 0;
      break;
// defult saknas.
    }

  return 0;
}




char regret_aux() // TODO: Ångra funktion
{

  struct ware emptyware = (struct ware){};

  switch (action_memory.type){

  case  0:
    printf("Det finns ingenting nytt att ångra!\n");
    break;
  case  1:
    *action_memory.edited = emptyware; action_memory.type = NOTHING;
    break;
  case  2:
    lager.wares[lager.size] = action_memory.saved; lager.size++; action_memory.type = NOTHING;
    break;
  case  3:
    *action_memory.edited = action_memory.original; action_memory.type = NOTHING;
    break;
  default :
    printf(" Något konstigt har hänt \n");
  }
  return 0;
}




int regret()
{
  switch(toupper(ask_question_char("Vill du ångra din senaste ändring?","JjNn")))
    {
    case 'J':
      regret_aux();
      break;
    case 'N':
      break;
      //defult saknas
    }
  return 0;
}







void visa_vara(int paj)  //Visa funktion
{
  puts("Namn:");
  printf("%s\n",lager.wares[paj].name);
  puts("Beskrivning:");
  printf("%s\n",lager.wares[paj].description);
  puts("Lagerplats:");
  printf("%c",lager.wares[paj].location_char);
  printf("%i\n",lager.wares[paj].location_int);
  puts("Pris:");
  printf("%i kr\n",lager.wares[paj].price);
  puts("Antal:");
  printf("%i\n",lager.wares[paj].amount);
  puts("");
  }


 char edit_goods(int i) //TODO: En redigera funktion

  {
    bool tillbaka = true;
    char lagra[120];
    kul typ;
    action_memory.type = EDIT;
    action_memory.original = lager.wares[i];
    action_memory.edited = &lager.wares[i];

    switch(toupper(ask_question_char("Vad vill du redigera? \n"
				     "[N]amn \n"
				     "[B]eskrivning \n"
				     "[L]agerplats \n"
				     "[P]ris \n"
				     "An[T]al \n"
				     "[A]vbryt \n", "NnBbLlPpTtAa")))
      {
      case 'N':
	puts("Nuvarande Namn:");
	printf("%s\n",lager.wares[i].name);
	puts("--------------------");
	puts("Nytt Namn:");
	fgets(lagra, 120, stdin);
	lager.wares[i].name = (lagra,120);
	break;
      case 'B':
	puts("Nuvarande Beskrivning:");
	printf("%s\n",lager.wares[i].description);
	puts("--------------------");
	puts("Ny beskrivning:");
	fgets(lagra, 120, stdin);
	lager.wares[i].description = strndup(lagra,120); //frigörs aldrig
	break;

      case 'L':
	while (tillbaka)
	{
	  tillbaka = false;
	  puts("Nuvarande Lagerplats (bokstav):");
	  printf("%c\n",lager.wares[i].location_char);
	  puts("--------------------");

	  char ny_lager_char = toupper(ask_question_char ("Ny lagerplats (bokstav):", "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZzÅåÄäÖö"));
	  typ.location_char =ny_lager_char;


	  puts("Nuvarande Lagerplats (siffra):");
	  printf("%i\n",lager.wares[i].location_int);
	  puts("--------------------");

	  int ny_lager_int = ask_question_int("Ny lagerplats (siffra):\n");
	  typ.location_int =ny_lager_int;


	  for (int i = 0; i < 120; i++)
	    {
	      if (typ.location_int == toupper(lager.wares[i].location_int) && toupper(typ.location_char) == toupper(lager.wares[i].location_char))
		{
		  puts("Lagerplatsen är tyvärr upptagen, försök igen!\n");
		  tillbaka = true;
		}
	    }
	  if (tillbaka == false)
	    {
	      lager.wares[i].location_char = ny_lager_char;
	      lager.wares[i].location_int = ny_lager_int;
	    }
	}
	break;
	
      case 'P':
	puts("Nuvarande Pris:");
	puts("--------------------");
	int ny_lager_pris = ask_question_int("Nytt pris:");
	typ.price = ny_lager_pris;
       	lager.wares[i].price = ny_lager_pris;
	break;


      case 'T':
       	puts("Nuvarande Antal:");
	puts("--------------------");
	int ny_lager_antal = ask_question_int("Nytt antal:");
	typ.amount = ny_lager_antal;
	lager.wares[i].amount = ny_lager_antal;
	break;
      case 'A':
	return 0;
  // defult saknas
      }
    return 0;
  }





void visa_lager(char inputta) //TODO: Lista varor
{

  int index = 0;
  char alt;
  int sida = 1;
  char efter_visa;
  for (int i = 0; i < 120; i++)
    {
      index++;
      printf("%d %s\n", index , lager.wares[i].name);

      if (index == 20 || i == lager.size-1)
	     {
	  char next_page =  ask_question_char("\nVisa nästa sida med varor?", "JjNn");
	  index = 0;
	  if (toupper(next_page) == 'J')
	    {
	      puts("\n\n");
	      sida++;
	    }
	  else
	    {
	      alt = ask_question_char("Visa vara?","JjNn");
	      if(toupper(alt) == 'J')
		{
		  puts("");
		  int paj = ((ask_question_int("Välj vara[1-20]: ")-1)+((sida-1)*20));
		  visa_vara(paj);
		  i = ((sida*20)-21);
		  switch(toupper(inputta))
		    {
		    case 'H':
		      efter_visa = ask_question_char("Vill du [F]ortsätta titta i listan eller [A]vbryta?","FfAa");
		      if (toupper(efter_visa) == 'A')
			{
			  return;
			}
		      break;
		    case 'R':
		      edit_goods(paj);
		      break;
		    case 'T':
		      delete_goods(paj);
		      break;
		    }
		}
	      else
		{
		  return;
		}
	    }
	}
    }
}






 bool quit_action(void)
 {
   while (true)
     {
       char input = ask_question_char("Vill du verkligen avsluta? Ja/Nej", "JjNn");

       if (input == 'J' || input == 'j') return true;
       if (input == 'N' || input == 'n') return false;

       puts("Enbart J och N är giltiga svar, försök igen!");
     }

   return false;
 }







 int main(void)
 {
   init_warehouse();
   bool should_quit = false;

   while (!should_quit)

     {
       puts("\nVälkommen till lagerhantering 1.0!\n"
	    "====================================\n"

	    "[L]ägg till en vara\n"
	    "[T]a bort en vara\n"
	    "[R]edigera en vara\n"
	    "Ån[G]ra senaste ändringen\n"
	    "Lista [H]ela varukatalogen\n"
	    "[A]vsluta\n"
	    "\n"
	    );


       char inputta = toupper(ask_question_char ("Vad vill du göra idag?", "AaLlTtRrGgHh"));
       if (inputta == 'L')
	 {
	   puts("\n");
	   add_goods();
	 }
       else if (inputta == 'T')
	 {
	   visa_lager(inputta);
	 }
       else if (inputta == 'R')
	 {
	   visa_lager(inputta);
	 }
       else if (inputta == 'G')
	 {
	   regret();
	 }
       else if (inputta == 'H')
	 {
	   visa_lager(inputta);
	 }
       else if (inputta == 'A')
	 {
	   should_quit = quit_action();
	 }
       else
	 {
	   main(); //
	 }

     }
   return 0;
 }
