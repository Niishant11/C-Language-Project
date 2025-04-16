#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Contact {
    char name[50];
    char phone[15];
    char email[50];
};

// Function declarations
void addContact();
void displayContacts();
void searchContact();
void deleteContact();
void updateContact();

int main() {
    int choice;

    while (1) {
        printf("\n--- Contact Management System ---\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts\n");
        printf("3. Search Contact by Name\n");
        printf("4. Delete Contact by Name\n");
        printf("5. Update Contact by Name\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline from buffer

        switch (choice) {
            case 1: addContact(); break;
            case 2: displayContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: updateContact(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addContact() {
    struct Contact c;
    FILE *fp = fopen("contacts.dat", "ab");

    printf("Enter Name: ");
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = '\0';

    printf("Enter Phone: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    c.phone[strcspn(c.phone, "\n")] = '\0';

    printf("Enter Email: ");
    fgets(c.email, sizeof(c.email), stdin);
    c.email[strcspn(c.email, "\n")] = '\0';

    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);
    printf("Contact added successfully!\n");
}

void displayContacts() {
    struct Contact c;
    FILE *fp = fopen("contacts.dat", "rb");

    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }

    printf("\n--- All Contacts ---\n");
    while (fread(&c, sizeof(c), 1, fp)) {
        printf("Name : %s\n", c.name);
        printf("Phone: %s\n", c.phone);
        printf("Email: %s\n", c.email);
        printf("---------------------------\n");
    }

    fclose(fp);
}

void searchContact() {
    char searchName[50];
    struct Contact c;
    int found = 0;

    FILE *fp = fopen("contacts.dat", "rb");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }

    printf("Enter Name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    while (fread(&c, sizeof(c), 1, fp)) {
        if (strcasecmp(c.name, searchName) == 0) {
            printf("Contact Found:\n");
            printf("Name : %s\n", c.name);
            printf("Phone: %s\n", c.phone);
            printf("Email: %s\n", c.email);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Contact not found.\n");

    fclose(fp);
}

void deleteContact() {
    char nameToDelete[50];
    struct Contact c;
    int found = 0;

    FILE *fp = fopen("contacts.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL) {
        printf("No contacts to delete.\n");
        return;
    }

    printf("Enter Name to delete: ");
    fgets(nameToDelete, sizeof(nameToDelete), stdin);
    nameToDelete[strcspn(nameToDelete, "\n")] = '\0';

    while (fread(&c, sizeof(c), 1, fp)) {
        if (strcasecmp(c.name, nameToDelete) != 0) {
            fwrite(&c, sizeof(c), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("contacts.dat");
    rename("temp.dat", "contacts.dat");

    if (found)
        printf("Contact deleted successfully.\n");
    else
        printf("Contact not found.\n");
}

void updateContact() {
    char nameToUpdate[50];
    struct Contact c;
    int found = 0;

    FILE *fp = fopen("contacts.dat", "rb+");

    if (fp == NULL) {
        printf("No contacts available.\n");
        return;
    }

    printf("Enter Name to update: ");
    fgets(nameToUpdate, sizeof(nameToUpdate), stdin);
    nameToUpdate[strcspn(nameToUpdate, "\n")] = '\0';

    while (fread(&c, sizeof(c), 1, fp)) {
        if (strcasecmp(c.name, nameToUpdate) == 0) {
            printf("Enter New Name: ");
            fgets(c.name, sizeof(c.name), stdin);
            c.name[strcspn(c.name, "\n")] = '\0';

            printf("Enter New Phone: ");
            fgets(c.phone, sizeof(c.phone), stdin);
            c.phone[strcspn(c.phone, "\n")] = '\0';

            printf("Enter New Email: ");
            fgets(c.email, sizeof(c.email), stdin);
            c.email[strcspn(c.email, "\n")] = '\0';

            fseek(fp, -sizeof(c), SEEK_CUR);
            fwrite(&c, sizeof(c), 1, fp);
            printf("Contact updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Contact not found.\n");

    fclose(fp);
}
