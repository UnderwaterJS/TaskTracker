#include <stdio.h>
#include <string.h>
#include <time.h>

void add_task();
void show_day_tasks();
void change_task();
void delete_task();

typedef struct {
char title[20];
char desc[100];
char date[20];
char timing[20];
char status[20];
} TASK;

typedef struct {
} NODE;

int menu() {
    printf("Welcome to taskTracker!\n");
    while(1) {
    int numb;
    printf("Avialable optins:\n1.Add task.\n2.Show day tasks.\n3.Change task.\n4.Set reminder.\n5.Show results for the week.\n6.Show results for the month.\n7.Delete task.\n8.Exit\n");
    printf("Choose option:");
    scanf("%d", &numb);
    switch(numb) {
        case 1:
        add_task();
    break;
        case 2:
        show_day_tasks();
    break;
        case 3:
        change_task();
    break; 
        case 4:
        printf("Mind?\n");
    break;
        case 5:
        printf("Week?\n");
    break;
        case 6:
        printf("Month?\n");
    break;
        case 7:
        delete_task();
    break;
        case 8:
        return 0;
    break;
        default:
        printf("Incorrect number.\n");
    break;
    }
}
}

void add_task()
{
    TASK my;

    printf("Enter your task: ");
    getchar();
    fgets(my.title, sizeof(my.title), stdin);
    my.title[strcspn(my.title, "\n")] = '\0';

    printf("Enter your description: ");
    fgets(my.desc, sizeof(my.desc), stdin);
    my.desc[strcspn(my.desc, "\n")] = '\0';

    printf("Enter date (day.month.year): ");
    fgets(my.date, sizeof(my.date), stdin);
    my.date[strcspn(my.date, "\n")] = '\0';

    printf("Enter time period (00:00-00:00): ");
    fgets(my.timing, sizeof(my.timing), stdin);
    my.timing[strcspn(my.timing, "\n")] = '\0';

    printf("Enter complete status(Not started yet/In progress/Done): ");
    fgets(my.status, sizeof(my.status), stdin);
    my.status[strcspn(my.status, "\n")] = '\0';

    FILE* ttk = fopen("tasktracker.txt", "ab");
    if (ttk == NULL) {
        perror("Error opening file");
        return;
    }

    fwrite(&my, sizeof(TASK), 1, ttk);
    fclose(ttk);
}

void show_day_tasks() {
    TASK my;
    char user_date[20];
    printf("Please, enter needed date: ");
    scanf("%s", user_date);
    FILE *ttk = fopen("tasktracker.txt", "rb");
    if(ttk == NULL) {
        printf("Fail to open file");
        return;
    }
    int found = 0;
    while (fread(&my, sizeof(my), 1, ttk) == 1) {
        if (strcmp(user_date, my.date) == 0) {
            printf("\nTask: %s\nDescription: %s\nDate: %s\nTiming: %s\nStatus: %s\n", my.title, my.desc, my.date, my.timing, my.status);
            found = 1;
        }
    }
    if (!found) {
        printf("No tasks found for this date.\n");
    }
    fclose(ttk);
}

void change_task() {
    TASK my;
    char user_title[20], user_date[20], user_timing[20];
    printf("Which task do you want to change?\n");
    printf("Enter task name: ");
    getchar();
    fgets(user_title, sizeof(user_title), stdin);
    user_title[strcspn(user_title, "\n")] = '\0';
    printf("Enter task date: ");
    fgets(user_date, sizeof(user_date), stdin);
    user_date[strcspn(user_date, "\n")] = '\0';
    printf("Enter task time period: ");
    fgets(user_timing, sizeof(user_timing), stdin);
    user_timing[strcspn(user_timing, "\n")] = '\0';
    FILE *ttk = fopen("tasktracker.txt", "rb");
    if(ttk == NULL) {
        printf("Fail to open file");
        return;
    }
    FILE *temp = fopen("temp.txt", "wb");
    if(temp == NULL) {
        printf("Fail to open this file");
        fclose(ttk);
        return;
    }
    int found = 0;
    while(fread(&my, sizeof(my), 1, ttk) == 1) {
        if (strcmp(user_title, my.title) == 0 && strcmp(user_date, my.date) == 0 && strcmp(user_timing, my.timing) == 0) {
            printf("\nTask: %s\nDescription: %s\nDate: %s\nTiming: %s\nStatus: %s\n", my.title, my.desc, my.date, my.timing, my.status);
            char choice;
            printf("Change task name?(y/n)\n");
            scanf(" %c", &choice);
            if(choice == 'y' || choice == 'Y') {
            while (getchar() != '\n');
            printf("Enter new task name: ");
            fgets(my.title, sizeof(my.title), stdin);
            my.title[strcspn(my.title, "\n")] = '\0';
        }

            printf("Change task description?(y/n)\n");
            scanf(" %c", &choice);
            if(choice == 'y' || choice == 'Y') {
            while (getchar() != '\n');
            printf("Enter new task decsription: ");
            fgets(my.desc, sizeof(my.desc), stdin);
            my.desc[strcspn(my.desc, "\n")] = '\0';
        }

         printf("Change task date?(y/n)\n");
            scanf(" %c", &choice);
            if(choice == 'y' || choice == 'Y') {
            while (getchar() != '\n');
            printf("Enter new task date: ");
            fgets(my.date, sizeof(my.date), stdin);
            my.date[strcspn(my.date, "\n")] = '\0';
        }

        printf("Change task timing?(y/n)\n");
            scanf(" %c", &choice);
            if(choice == 'y' || choice == 'Y') {
            while (getchar() != '\n');
            printf("Enter new task timing: ");
            fgets(my.timing, sizeof(my.timing), stdin);
            my.timing[strcspn(my.timing, "\n")] = '\0';
        }

        printf("Change task status?(y/n)\n");
            scanf(" %c", &choice);
            if(choice == 'y' || choice == 'Y') {
            while (getchar() != '\n');
            printf("Enter new task status: ");
            fgets(my.status, sizeof(my.status), stdin);
            my.status[strcspn(my.status, "\n")] = '\0';
        }
            found = 1;
     }
     fwrite(&my, sizeof(TASK), 1, temp);
  }
    fclose(ttk);
    fclose(temp);

    if(found) {
        remove("tasktracker.txt");
        rename("temp.txt", "tasktracker.txt");
        printf("Task updated successfully.\n");
    } else {
        remove("temp.txt");
        printf("Task not found.\n");
    }
}

void delete_task() {
    TASK my;
    char user_title[20], user_date[20], user_timing[20];
    printf("Which task do you want to delete?");
    printf("Enter task name: ");
    fgets(user_title, sizeof(user_title), stdin);
    user_title[strcspn(user_title, "\n")] = '\0';
    printf("Enter task date: ");
    fgets(user_date, sizeof(user_date), stdin);
    user_date[strcspn(user_date, "\n")] = '\0';
    printf("Enter task timing: ");
    fgets(user_timing, sizeof(user_timing), stdin);
    user_timing[strcspn(user_timing, "\n")] = '\0';
    FILE *ttk = fopen("tasktracker.txt", "rb");
    if(ttk == NULL) {
        printf("Fail to open this file");
        return;
    }
    FILE *temp = fopen("temp.txt", "wb");
    if(temp == NULL) {
        printf("Fail to open this file");
        fclose(ttk);
        return;
    }
    int found = 0;
    while(fread(&my, sizeof(my), 1, ttk) == 1) {
        if (strcmp(user_title, my.title) == 0 && strcmp(user_date, my.date) == 0 && strcmp(user_timing, my.timing) == 0) {
            printf("\nTask: %s\nDescription: %s\nDate: %s\nTiming: %s\nStatus: %s\n", my.title, my.desc, my.date, my.timing, my.status);
            found = 1;
        }
        else {
            fwrite(&my, sizeof(TASK), 1, temp); 
        }
    }
    fclose(ttk);
    fclose(temp);

    if(found) {
        remove("tasktracker.txt");
        rename("temp.txt", "tasktracker.txt");
        printf("Task deleted successfully.\n");
    } else {
        remove("temp.txt");
        printf("Task not found.\n");
    }
}

int main() {
    menu();
    return 0;
}