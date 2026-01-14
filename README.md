# Management-Console-App

User Management Console App
A lightweight, file-based User Management System written in C. This application allows users to perform CRUD (Create, Read, Update, Delete) operations on user records stored in a persistent text file.

🚀 Features
Add User: Create new user records with a unique ID, full name, and age.

Show Users: Display all currently stored users directly in the console.

Update User: Modify existing user information by searching for their unique ID.

Delete User: Remove a specific user record from the system.

Search (Hidden): Use Option 9 in the menu to find the specific line number of a user ID.

Data Persistence: All data is saved to users.txt and persists even after the program closes.

🛠️ Technical Details
The application utilizes several core C programming concepts:

File I/O: Uses fopen, fprintf, fgets, and fgetc for data management.

Buffer Management: Employs fflush and rewind to ensure data integrity and proper file pointer positioning.

Temporary File Handling: Updates and deletions are performed by streaming data to a temp.txt file and renaming it, ensuring the database remains clean.

Input Validation: Age inputs are validated using ASCII checks to prevent non-numeric data entry.

💻 How to Run
Clone the repository:

git clone https://github.com/MCCode06/Management-Console-App.git
cd Management-Console-App

Compile the code:

gcc main.c -o user_management

Execute the program:

./user_management
