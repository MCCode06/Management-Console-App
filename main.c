#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int printMenu(FILE *file);
int addUser(FILE *file);
int showUsers(FILE *file);
FILE *deleteUser(FILE *file);
FILE *updateUser(FILE *file);
void quit();

int searchUser(FILE *file, char userId[]);
int continuePrompt(FILE *file);
char *capitalizeName(char *str);

int main(void)
{
    FILE *file = fopen("users.txt", "a+"); // append and read mode
    if (file == NULL)
    {
        printf("Can't open file.\n");
        return 1;
    }
    srand(time(NULL));

    printf("Welcome to the User Management System!\n");
    while (1)
    {
        int option = printMenu(file);
        switch (option)
        {
        case 1:
            addUser(file);
            break;
        case 2:
            showUsers(file);
            break;
        case 3:
            file = updateUser(file);
            break;
        case 4:
            file = deleteUser(file);
            break;
        case 5:
            quit();
            break;
        case 9: // hidden option for search
        {
            char userId[10];
            printf("Enter user ID to search: ");
            scanf("%s", userId);
            int lineNumber = searchUser(file, userId);
            if (lineNumber == 0)
            {
                printf("User with ID %s not found.\n", userId);
            }
            else
            {
                printf("User with ID %s found at line %d.\n", userId, lineNumber);
            }
            break;
        }
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    }
    fclose(file);
    return 0;
}

int printMenu(FILE *file)
{
    int option;
    printf("\n-------------------------\n");
    printf("1. Add User\n");
    printf("2. Show Users\n");
    printf("3. Update User\n");
    printf("4. Delete User\n");
    printf("5. Quit\n");
    printf("Enter your choice (1-5): ");
    scanf("%d", &option);

    return option;
}

int addUser(FILE *file)
{
    char userId[5];
    char name[50];
    char surname[50];
    char age[10];

    do
    {
        userId[0] = 'U';
        for (int i = 1; i < 4; i++)
        {
            userId[i] = '0' + rand() % 10;
        }
        userId[4] = '\0';
    } while (searchUser(file, userId) != 0);

    printf("Enter user name: ");
    scanf("%s", name);
    printf("Enter user surname: ");
    scanf("%s", surname);
    printf("Enter user age: ");
    scanf("%s", age);
    capitalizeName(name);
    capitalizeName(surname);

    for (int i = 0; i < strlen(age); i++)
    {
        if (age[i] < '0' || age[i] > '9' || atoi(age) < 6 || atoi(age) > 80)
        {
            printf("Invalid age.\n");
            printf("Enter VALID user age: ");
            scanf("%s", age);
            i = -1; // reset for loop
        }
    }
    fprintf(file, "%s | %s %s | %s\n", userId, name, surname, age);
    fflush(file); // force writing to file immediately

    return 0;
}

int showUsers(FILE *file)
{
    rewind(file); // takes the pointer to the beginnig of the file
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        putchar(c);
    }

    return 0;
}

FILE *deleteUser(FILE *file)
{
    rewind(file);
    char userId[10];
    printf("Enter user ID to delete: ");
    scanf("%s", userId);

    int lineNum = searchUser(file, userId);
    if (lineNum == 0)
    {
        printf("User with ID %s not found.\n", userId);
        return file;
    }

    int currentLine = 1;
    char line[256];
    rewind(file);

    FILE *tempFile = fopen("temp.txt", "w");

    while (fgets(line, sizeof(line), file))
    {
        if (currentLine != lineNum)
        {
            fputs(line, tempFile);
        }
        currentLine++;
    }

    fclose(file);
    fclose(tempFile);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    file = fopen("users.txt", "a+");
    return file;
}
FILE *updateUser(FILE *file)
{
    char searchId[10];
    printf("Enter user ID to update: ");
    scanf("%s", searchId);
    while (getchar() != '\n')
        ; // Clear buffer

    int lineNum = searchUser(file, searchId);
    if (lineNum == 0)
    {
        printf("User with ID %s not found.\n", searchId);
        return file;
    }

    rewind(file);
    FILE *tempFile = fopen("temp.txt", "w");
    char line[256];
    int currentLine = 1;

    while (fgets(line, sizeof(line), file))
    {
        if (currentLine != lineNum)
        {
            fputs(line, tempFile);
        }
        else
        {
            char oldId[10], oldName[50], oldSurname[50], oldAge[10];
            char newName[50], newSurname[50], newAge[10];

            sscanf(line, "%s | %s %s | %s", oldId, oldName, oldSurname, oldAge);

            printf("New Name: ", oldName);
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = 0;
            if (strlen(newName) == 0)
            {
                strcpy(newName, oldName);
            }
            printf("New Surname: ", oldSurname);
            fgets(newSurname, sizeof(newSurname), stdin);
            newSurname[strcspn(newSurname, "\n")] = 0; // removes newline character
            if (strlen(newSurname) == 0)
            {
                strcpy(newSurname, oldSurname);
            }

            printf("New Age: ", oldAge);
            fgets(newAge, sizeof(newAge), stdin);
            newAge[strcspn(newAge, "\n")] = 0;
            if (strlen(newAge) == 0)
            {
                strcpy(newAge, oldAge);
            }
            for (int i = 0; i < strlen(newAge); i++)
            {
                if (newAge[i] < '0' || newAge[i] > '9' || atoi(newAge) < 6 || atoi(newAge) > 80)
                {
                    printf("Invalid age. Please enter a VALID age: ");
                    fgets(newAge, sizeof(newAge), stdin);
                    newAge[strcspn(newAge, "\n")] = 0;
                    i = -1; // reset for loop
                }
            }
            capitalizeName(newName);
            capitalizeName(newSurname);

            fprintf(tempFile, "%s | %s %s | %s\n", oldId, newName, newSurname, newAge);
        }
        currentLine++;
    }

    fclose(file);
    fclose(tempFile);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    return fopen("users.txt", "a+");
}

int searchUser(FILE *file, char userId[])
{
    rewind(file);
    int lineNumber = 1;
    char line[256];
    char currentUserId[10];
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%s", currentUserId);
        if (strcmp(currentUserId, userId) == 0)
        {
            return lineNumber;
        }
        lineNumber++;
    }

    return 0;
}

void quit()
{
    printf("Exiting the program. Goodbye!\n");
    exit(0); // terminate the entire program
}

int continuePrompt(FILE *file)
{
    printf("\nDo you want to continue? y/n: ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y')
    {
        return 1;
    }
    return 0;
}

char *capitalizeName(char *str)
{
    if (str[0] >= 'a' && str[0] <= 'z')
    {
        str[0] = str[0] - ('a' - 'A');
    }
    for (int i = 1; i < strlen(str); i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = str[i] + ('a' - 'A');
        }
    }
    return str;
}