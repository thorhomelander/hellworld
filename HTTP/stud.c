#include<stdio.h>
#define SIZE 300
#define SEM 2
#define N 2

typedef struct{
	int regno;
	int age;
	int sem;
	int marks[5];
	char name[SIZE];
	char gender;
}student;

int main(){
	student s[N];
	FILE *of;
	int i,j,k;
	of = fopen("stud.txt", "w");
	for (i = 0; i < N; i++){
		printf("Enter Regno, Name, Age, Gender\n");
		scanf("%d", &s[i].regno);
		//getchar();
		scanf("%s", s[i].name);
		scanf("%d", &s[i].age);
		getchar();
		scanf("%c", &s[i].gender);
		for ( k = 0; k < SEM; k++){
			s[i].sem = k + 1;
			printf("Enter marks for sem %d, 5 subjects: ", k+1);
			for(int j = 0; j < 5; j++)
				scanf("%d",&s[i].marks[j]);
			fwrite (&s[i], sizeof(student), 1, of);
		}
	}
	fclose(of);
	student stu;
	FILE *inf;
	inf = fopen("stud.txt", "r");
	while(fread(&stu, sizeof(student), 1, inf)){
		printf("Regno: %d\nName: %s\nAge: %d\nGender: %c\nSem:%d\nMarks: ", stu.regno, stu.name, stu.age, stu.gender,stu.sem);
		for (int j = 0; j < 5; j++)
			printf("%d ", stu.marks[j]);
		printf("\n");
	}
	fclose(inf);
	return 0;
}
