#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int welcome(FILE *file);
int addUser(FILE *file);
int showUsers(FILE *file);
FILE *deleteUser(FILE *file);
FILE *updateUser(FILE *file);
void quit();

int searchUser(FILE *file, char userId[]);
int continuePrompt(FILE *file);

int main(void)
{
    FILE *file = fopen("users.txt", "a+"); // append and read mode
    if (file == NULL)
    {
        printf("Can't open file.\n");
        return 1;
    }
    int running = 1;

    while (running)
    {
        int option = welcome(file);
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
        running = continuePrompt(file);
    }
    fclose(file);
    return 0;
}

int welcome(FILE *file)
{
    int option;
    printf("\nWelcome to the User Management System!\n");
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
    char userId[10];
    char name[50];
    char surname[50];
    char age[10];

    printf("Enter user ID: ");
    scanf("%s", userId);
    printf("Enter user name and surname (ex: Sanani Zeynalli): ");
    scanf("%s %s", name, surname);
    printf("Enter user age: ");
    scanf("%s", age);
    for (int i = 0; i < strlen(age); i++)
    {
        if (age[i] < '0' || age[i] > '9')
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
    rewind(file);
    char userId[10];
    printf("Enter user ID to update: ");
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
        else // addUser logic
        {
            char name[50];
            char surname[50];
            char age[10];

            printf("Enter new user name and surname (ex: Sanani Zeynalli): ");
            scanf("%s %s", name, surname);
            printf("Enter new user age: ");
            scanf("%s", age);
            for (int i = 0; i < strlen(age); i++)
            {
                if (age[i] < '0' || age[i] > '9')
                {
                    printf("Invalid age.\n");
                    printf("Enter VALID user age: ");
                    scanf("%s", age);
                    i = -1; // reset for loop
                }
            }
            fprintf(tempFile, "%s | %s %s | %s\n", userId, name, surname, age);
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