#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>

void add_task();
void show_day_tasks();
void change_task();
void delete_task();
void my_total();

typedef struct {
char title[20];
char desc[100];
char date[20];
char timing[20];
char status[20];
} TASK;
 
typedef struct {
} NODE;

int date_to_days(const char *date_str) {
    int day, month, year;
    sscanf(date_str, "%d.%d.%d", &day, &month, &year);
    int days = 0;
    for(int y = 1970; y < year; y++) {
        days += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0) ? 366 : 365);
    }
    int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        days_in_months[1] = 29;
    for(int m = 0; m < month - 1; m++) {
        days += days_in_months[m];
    }
    days += day;
    return days;
}

float get_duration_from_timing(const char *timing) {
    int start_min, start_hours, end_min, end_hours;
    if (sscanf(timing, "%d:%d-%d:%d", &start_hours, &start_min, &end_hours, &end_min)) {
        int start_total = start_hours * 60 + start_min;
        int end_total = end_hours * 60 + end_min;
        int diff_minutes = end_total - start_total;
        return diff_minutes / 60.0f;
    }
    return 0.0f;
}

int sql() {
    sqlite3 *db;    // указатель на базу данных
    // открываем подключение к базе данных
    int result  = sqlite3_open("TaskTrack.db", &db);
    // если подключение успешно установлено
    if(result == SQLITE_OK) 
    {
        printf("Connection established\n");
    }
    else
    {
        // выводим сообщение об ошибке
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
    // закрываем подключение
    sqlite3_close(db);
}

int menu() {
    sql();
    printf("Welcome to taskTracker!\n");
    while(1) {
    int numb;
    printf("\nAvialable optins:\n1.Add task.\n2.Show day tasks.\n3.Change task.\n4.Set reminder.\n5.Show results in some time.\n6.Delete task.\n7.Exit\n");
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
        my_total();
    break;
        case 6:
        delete_task();
    break;
        case 7:
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

    printf("Enter time period (hh:mm-hh:mm): ");
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

void my_total() {
    TASK my;
    char user_period_start[20], user_period_end[20];
    printf("In what period of time would you like to see results?\n");
    printf("Please, enter start of period (day.month.year): \n");
    scanf("%19s", user_period_start);
    printf("Please, enter end of period (day.month.year): \n");
    scanf("%19s", user_period_end);

    int start_days = date_to_days(user_period_start);
    int end_days = date_to_days(user_period_end); 

    FILE *ttk = fopen("tasktracker.txt", "rb");
    if (ttk == NULL) {
        printf("Fail to open file\n");
        return;
    }

    int workout_count = 0, lesson_count = 0;
    float programm_hours = 0, job_hours = 0;

    char programming[20] = "Programming";
    char workout[20] = "Workout";
    char job[20] = "Job";
    char lessons[20] = "Lessons";

    int found = 0;
    while (fread(&my, sizeof(my), 1, ttk) == 1) {
            int task_days = date_to_days(my.date);
            if(start_days <= task_days && task_days <= end_days) {
            if(strcmp(my.title, programming) == 0)
            programm_hours += get_duration_from_timing(my.timing);
            else if (strcmp(my.title, workout) == 0)
            workout_count++;
            else if (strcmp(my.title, job) == 0)
            job_hours += get_duration_from_timing(my.timing);
            else if (strcmp(my.title, lessons) == 0)
            lesson_count++;
            found = 1;
        }
    }
    printf("Your efficiency for chosen period:\n");
    printf("Hours of programming: %.2f\nHours of job: %.2f\nAmount of workouts: %d\nAmount of lessons: %d\n", programm_hours, job_hours, workout_count, lesson_count);

    if (!found) {
        printf("No tasks found for this date period.\n");
    }
    fclose(ttk);
}


int main() {
    menu();
    return 0;
}