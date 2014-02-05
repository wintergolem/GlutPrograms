#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <vector>

class weapon{
public:
	char type[100];
	int damage;
	int armor;
};

class person{
public:
	int ID;
	char name[1000];
	int health;
	weapon w;
	int position[2];
	char taunt[1000];
	
};

std::vector<person> persons;

person ReadPerson(FILE *file)
{
	person p;
	fscanf(file, "%i%s%i%s%i%i%i%i", &p.ID, p.name, &p.health, &p.w.type, &p.w.damage, &p.w.armor, &p.position[0], &p.position[1]);
	//getchar();
	fseek(file, 2, SEEK_CUR); // skip line-feed, carriage-return
	fgets(p.taunt, 999, file);
	
	return p;
}

int EditPerson(person &p)
{
	char g, line[1000];
	do{
		printf("\nEnter 'n' for Name or 'h' for Health or 'q' to Quit\n");
		// scanf("%c", &g);
		// getchar();
		gets(line);
		sscanf(line, "%c", &g);
		switch(g){
			case 'n':
				printf("New Name = ");
				scanf("%s",&p.name);
				getchar();
				break;
			case 'h':
				printf("New health = ");
				scanf("%i",&p.health);
				getchar();
				break;
			}//end switch

	} while(g != 'q');
	return 0;
}//EditPerson
int SavePerson(FILE *file){
	//FILE *file("char.txt", "w");
	fseek(file,0,SEEK_SET);
	for(unsigned int i = 0; i < persons.size(); i++){
		person &p = persons[i];
		fprintf(file, "%i\n%s\n%i\n%s\n%i\n%i\n%i\n%i", p.ID, p.name, p.health, p.w.type, p.w.damage, p.w.armor, p.position[0], p.position[1]);
	}
	return 0;
}
void main() {
	char *filename = "char.txt";
	FILE *file = fopen(filename, "r+");
	if(!file){
		printf("file %s not found\n", filename);
		getchar();
		getchar();
		return;
	}
	char c = 'q';
	person p;
	//read file
	do {
		p = ReadPerson(file);
		persons.push_back(p);
	}
	while (!feof(file));
	//print to screen
	for(unsigned int i = 0; i < persons.size(); i++){
		printf("\n\n%i\n%s\n%i\n%s\n%i\n%i\n%i\n%i\n%s", persons[i].ID, persons[i].name, persons[i].health, persons[i].w.type, persons[i].w.damage, persons[i].w.armor, persons[i].position[0], persons[i].position[1], persons[i].taunt);
	}
	//edit
	int id = 0;
	while (id < 5){
		printf("\nEdit?");
		scanf("%i", &id);
		getchar();
		if(id <4){
			system("cls");
			printf("\n\n%i\n%s\n%i\n%s\n%i\n%i\n%i\n%i\n%s", persons[id].ID, persons[id].name, persons[id].health, persons[id].w.type, persons[id].w.damage, persons[id].w.armor, persons[id].position[0], persons[id].position[1], persons[id].taunt);
			EditPerson(persons[id]);
		}
	}
	//save to file
	SavePerson(file);
	fclose(file);
}