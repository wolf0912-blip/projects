#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float marks;
    char mobile[15];
    char email[50];
};

struct Student *s = NULL;
int n = 0;

// 🔹 Clear buffer
void clearBuffer() {
    while (getchar() != '\n');
}

// 🔹 Convert to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
    }
}

// 🔹 Safe input (nil if empty)
void inputString(char *str, int size) {
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = 0;

    if (strlen(str) == 0)
        strcpy(str, "nil");
}

// 🔹 Name match (partial + case-insensitive)
int nameMatch(char *a, char *b) {
    char x[50], y[50];
    strcpy(x, a);
    strcpy(y, b);
    toLowerCase(x);
    toLowerCase(y);
    return strstr(x, y) != NULL;
}

// 🔹 Load
void loadFromFile() {
    FILE *fp = fopen("students.dat", "r");
    if (!fp) return;

    while (1) {
        s = realloc(s, (n + 1) * sizeof(struct Student));

        if (fscanf(fp, "%d,%49[^,],%f,%14[^,],%49[^\n]\n",
                   &s[n].roll, s[n].name, &s[n].marks,
                   s[n].mobile, s[n].email) != 5)
            break;

        n++;
    }
    fclose(fp);
}

// 🔹 Save
void saveToFile() {
    FILE *fp = fopen("students.dat", "w");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,%s,%.2f,%s,%s\n",
                s[i].roll, s[i].name, s[i].marks,
                s[i].mobile, s[i].email);
    }
    fclose(fp);
}

// 🔹 Add
void addStudent() {
    s = realloc(s, (n + 1) * sizeof(struct Student));

    printf("Roll: ");
    scanf("%d", &s[n].roll);
    clearBuffer();

    printf("Name: ");
    inputString(s[n].name, 50);

    printf("Marks: ");
    scanf("%f", &s[n].marks);
    clearBuffer();

    printf("Mobile: ");
    inputString(s[n].mobile, 15);

    printf("Email: ");
    inputString(s[n].email, 50);

    n++;
}

// 🔹 View
void viewStudents() {
    if (!n) { printf("No data\n"); return; }

    printf("\n%-5s %-20s %-8s %-15s %-25s\n",
           "Roll","Name","Marks","Mobile","Email");

    for (int i = 0; i < n; i++) {
        printf("%-5d %-20s %-8.2f %-15s %-25s\n",
               s[i].roll, s[i].name, s[i].marks,
               s[i].mobile, s[i].email);
    }
}

// 🔹 Sort
void sortMenu() {
    int ch;
    printf("1.Name 2.Marks 3.Roll: ");
    scanf("%d",&ch);

    struct Student t;
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){

            int cond=0;
            if(ch==1) cond = strcmp(s[i].name,s[j].name)>0;
            if(ch==2) cond = s[j].marks > s[i].marks;
            if(ch==3) cond = s[j].roll < s[i].roll;

            if(cond){
                t=s[i]; s[i]=s[j]; s[j]=t;
            }
        }
    }
}

// 🔹 Search
void searchStudent() {
    char key[50];
    clearBuffer();
    printf("Enter name: ");
    inputString(key,50);

    for(int i=0;i<n;i++){
        if(nameMatch(s[i].name,key)){
            printf("%d %s %.2f %s %s\n",
                   s[i].roll,s[i].name,s[i].marks,
                   s[i].mobile,s[i].email);
        }
    }
}

// 🔹 Delete
void deleteStudent() {
    char key[50];
    int idx[100],c=0;

    clearBuffer();
    printf("Name: ");
    inputString(key,50);

    for(int i=0;i<n;i++){
        if(nameMatch(s[i].name,key))
            idx[c++]=i;
    }

    if(!c){ printf("Not found\n"); return; }

    for(int i=0;i<c;i++){
        int k=idx[i];
        printf("%d. %s %.2f\n",i+1,s[k].name,s[k].marks);
    }

    int ch;
    printf("Select: ");
    scanf("%d",&ch);

    int d=idx[ch-1];

    for(int i=d;i<n-1;i++)
        s[i]=s[i+1];

    n--;
    s=realloc(s,n*sizeof(struct Student));
}

// 🔹 Update
void updateStudent() {
    int choice;
    printf("1.Roll 2.Name: ");
    scanf("%d",&choice);

    int idx[100],c=0;

    if(choice==1){
        int roll;
        printf("Roll: ");
        scanf("%d",&roll);

        for(int i=0;i<n;i++)
            if(s[i].roll==roll) idx[c++]=i;
    }
    else{
        char key[50];
        clearBuffer();
        printf("Name: ");
        inputString(key,50);

        for(int i=0;i<n;i++)
            if(nameMatch(s[i].name,key)) idx[c++]=i;
    }

    if(!c){ printf("Not found\n"); return; }

    for(int i=0;i<c;i++){
        int k=idx[i];
        printf("%d. %s %.2f\n",i+1,s[k].name,s[k].marks);
    }

    int ch;
    printf("Select: ");
    scanf("%d",&ch);

    int i=idx[ch-1];

    char temp[50];
    clearBuffer();

    printf("New Name: ");
    fgets(temp,50,stdin);
    temp[strcspn(temp,"\n")]=0;
    if(strlen(temp)) strcpy(s[i].name,temp);

    printf("Marks (-1 skip): ");
    float m;
    scanf("%f",&m);
    if(m!=-1) s[i].marks=m;
    clearBuffer();

    printf("Mobile: ");
    fgets(temp,50,stdin);
    temp[strcspn(temp,"\n")]=0;
    if(strlen(temp)) strcpy(s[i].mobile,temp);

    printf("Email: ");
    fgets(temp,50,stdin);
    temp[strcspn(temp,"\n")]=0;
    if(strlen(temp)) strcpy(s[i].email,temp);
}

// 🔹 Export
void exportToFile() {
    char name[100];
    clearBuffer();

    printf("File name: ");
    fgets(name,100,stdin);
    name[strcspn(name,"\n")]=0;
    strcat(name,".txt");

    FILE *fp=fopen(name,"w");

    for(int i=0;i<n;i++){
        fprintf(fp,"%d %s %.2f %s %s\n",
                s[i].roll,s[i].name,s[i].marks,
                s[i].mobile,s[i].email);
    }

    fclose(fp);
}

// 🔹 MAIN
int main() {
    int ch;
    loadFromFile();

    do{
        printf("\n1.Add\n2.View\n3.Sort\n4.Search\n5.Delete\n6.Update\n7.Export\n8.Exit\nCHOICE: ");
        scanf("%d",&ch);

        switch(ch){
            case 1:addStudent();break;
            case 2:viewStudents();break;
            case 3:sortMenu();break;
            case 4:searchStudent();break;
            case 5:deleteStudent();break;
            case 6:updateStudent();break;
            case 7:exportToFile();break;
            case 8:saveToFile();break;
        }

    }while(ch!=8);

    free(s);
    return 0;
}



