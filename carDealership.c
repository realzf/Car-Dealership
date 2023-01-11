/**
File: carDealership.c
Description: This program is a rental car service backend.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX 20

//struct used for linked list, each node represents a car
typedef struct carAttributes{
    char plate[MAX];
    int mileage;
    int returnDate;
    struct carAttributes *next;
}car;

int payment;
//function declarations
car * addCar();
void insertNode(car **carsList, car *newCar);
void printLists(car *available, car *rented, car *atWkShop);
void saveLists(car *available, car *rented, car *atWkShop);
void loadLists(car **available, car **rented, car **atWkShop);
bool searchLists(car *available, car *rented, car *atWkShop, car *temp);
void deleteFirstNode(car **available, car **rented, car **atWkShop);
void deleteNode(car ** list, car *nodeToDelete);
void sortLists(car **available, car **rented, car **atWkShop);
void searchRepairs(car *repair, car **available, car ** repairReal);
void rentCar(car *available, car **returns, car **availableReal);

int main(){
    car *available = NULL, *rented = NULL, *atWkShop = NULL, *temp;
    int input = -1;
    loadLists(&available, &rented, &atWkShop);                  //load lists from files
    deleteFirstNode(&available, &rented, &atWkShop);            

    //gets user input from menu items
    while(input != 7){
        printf("\nCAR RENTAL:\n1. Add a new car to the available-for-rent list\n2. Add a returned car to the available-for-rent list\n3. Add a returned car to the repair list\n4. Transfer a car from the repair list to the available-for-rent list\n5. Rent the first available car\n6. Print all the lists\n7. Quit\n\nTransaction Code: ");
        scanf("%d", &input);
        getchar();

        //user input is directed accordingly using switch
        switch(input){
            case 1:
                //gets user input and adds new node to available to rent list
                insertNode(&available, addCar());
                printf("\nCar added!\n");
                break;

            case 2:
                //gets user input, deletes node from rented list and adds to available list
                if(rented != NULL){
                    temp = addCar();
                    if(searchLists(available, rented, atWkShop, temp)){    
                        insertNode(&available, temp);
                        deleteNode(&rented, temp);
                        printf("\nCar with plate number: %s moved to available for rent list from rented list.\n", temp->plate);
                    }
                }
                else    
                    printf("\nThere are no cars currently rented.\n");
                break;

            case 3:
                //gets user input, deletes node from rented list and adds in to repair list
                if(atWkShop != NULL){
                    temp = addCar();
                    if(searchLists(available, rented, atWkShop, temp)){
                        insertNode(&atWkShop, temp);
                        deleteNode(&rented, temp);
                        printf("\nCar with plate number: %s moved to repair list from rented list.\n", temp->plate);
                    }
                }
                else{
                    printf("\nCurrrently no cars are rented.\n");
                }
                break;

            case 4:
            //gets user input, deletes node in repair, adds to available
                if(atWkShop != NULL){
                    searchRepairs(atWkShop, &available, &atWkShop);
                }
                else{
                    printf("\nCurrently no cars are in repair.\n");
                }
                break;
            
            case 5:
                //Rents first available car
                if(available != NULL){
                    rentCar(available, &rented, &available);
                }
                else{
                    printf("\nCurrently no cars are available to rent.\n");
                }
                break;

            case 6:
                //Sorts lists accordingly and prints all the lists
                sortLists(&available, &rented, &atWkShop);
                printLists(available, rented, atWkShop);
                break;

            case 7:
                //Sorts lists before saving to file
                sortLists(&available, &rented, &atWkShop);
                saveLists(available, rented, atWkShop);
                printf("\nHave a great day!\n");
                break;
                
            //error trapping for invalid input
            default:
                printf("\nIncorrect input, try again.\n");
        }
    }
    return 0;
}

/*
This function creates a node for a list by getting user input
*/
car * addCar(){
    car *temp = NULL;
    temp = malloc(sizeof(car));

    //gets user input
    printf("Enter plate number: ");
    fgets(temp->plate, MAX, stdin);
    temp->plate[strcspn(temp->plate, "\n")] = 0;

    printf("Enter mileage: ");
    scanf("%d", &temp->mileage);
    getchar();

    temp->returnDate = 0;
    temp->next = NULL;
    return temp;
}

/*
This function inserts a node to the front of the list
*/
void insertNode(car ** carsList, car * newCar){
    if(*carsList == NULL){
        *carsList = newCar;
    }

    else{
        newCar->next = *carsList;
        *carsList = newCar;
    }
}

/*
This functions prints the lists to the user
*/
void printLists(car *available, car *rented, car *atWkShop){
    int count = 1;
    car *temp;
            
    printf("\nCARS AVAILABLE FOR RENT:");
    if(available == NULL){
        printf(" No cars are available to rent.");
    }

    else{
        temp = available;
        while(temp != NULL){                    
            printf("\nCar #%d - Plate: %s\t  Mileage: %d", count, temp->plate, temp->mileage);
            count++;
            temp = temp->next;
        }
    }
    count = 1;

    printf("\n\nCARS CURRENTLY RENTED:");
    if(rented == NULL){
        printf(" No cars are rented right now.");
    }

    else{
        temp = rented;

        while(temp != NULL){                    
            printf("\nCar #%d - Plate: %s\t  Mileage: %d\tExpected Return Date (YYMMDD): %d", count, temp->plate, temp->mileage, temp->returnDate);
            count++;
            temp = temp->next;
        }
    }
    count = 1;

    printf("\n\nCARS CURRENTLY IN REPAIR:");
    if(atWkShop == NULL){
        printf(" No cars are in repair right now.");
    }

    else{
        temp = atWkShop;
        while(temp != NULL){                    
            printf("\nCar #%d - Plate: %s\t  Mileage: %d", count, temp->plate, temp->mileage);
            count++;
            temp = temp->next;
        }
    }    
    printf("\n");
}

/*
This function saves lists to their files
*/
void saveLists(car *available, car *rented, car *atWkShop){
    car *tempAvailable = available;
    car *tempRented = rented;
    car *tempRepair = atWkShop;

    FILE *fptr = fopen("available.txt", "w");

    if(fptr != NULL && tempAvailable != NULL){
        while(tempAvailable != NULL){
            fprintf(fptr, "%s\n%d\n%d\n\n", tempAvailable->plate, tempAvailable->mileage, tempAvailable->returnDate);
            tempAvailable = tempAvailable->next;
        }
    }
    fclose(fptr);

    fptr = fopen("rented.txt", "w");
    if(fptr != NULL && tempRented != NULL){
        while(tempRented != NULL){
            fprintf(fptr, "%s\n%d\n%d\n\n", tempRented->plate, tempRented->mileage, tempRented->returnDate);
            tempRented = tempRented->next;
        }
    }
    fclose(fptr);

    fptr = fopen("repair.txt", "w");
    if(fptr != NULL && tempRepair != NULL){
        while(tempRepair != NULL){
            fprintf(fptr, "%s\n%d\n%d\n\n", tempRepair->plate, tempRepair->mileage, tempRepair->returnDate);
            tempRepair = tempRepair->next;
        }
    }
    fclose(fptr);
}

/*
This function loads the lists from files
*/
void loadLists(car **available, car **rented, car **atWkShop){
    car *temp = NULL;
    char tempMileage[MAX], tempReturnDate[MAX], emptyLine[MAX], tempPlate[MAX];

    FILE *fptr = fopen("available.txt", "r");
    if(fptr != NULL){
        while(!feof(fptr)){
            temp = malloc(sizeof(car));
            fgets(tempPlate, MAX, fptr);
            fgets(tempMileage, MAX, fptr);
            fgets(tempReturnDate, MAX, fptr);
            fgets(emptyLine, MAX, fptr);
            
            //deletes \n at the end of each string
            tempPlate[strcspn(tempPlate, "\n")] = 0;
            tempMileage[strcspn(tempMileage, "\n")] = 0;
            tempReturnDate[strcspn(tempReturnDate, "\n")] = 0;

            strcpy(temp->plate, tempPlate);
            temp->mileage = atoi(tempMileage);
            temp->returnDate = atoi(tempReturnDate);
            temp->next = NULL;

            insertNode(available, temp);      //sends node to bee added to the list
        }
    }
    fclose(fptr);

    fptr = fopen("rented.txt", "r");
    if(fptr != NULL){
        while(!feof(fptr)){
            temp = malloc(sizeof(car));
            fgets(tempPlate, MAX, fptr);
            fgets(tempMileage, MAX, fptr);
            fgets(tempReturnDate, MAX, fptr);
            fgets(emptyLine, MAX, fptr);

            //deletes \n at the end of each string
            tempPlate[strcspn(tempPlate, "\n")] = 0;
            tempMileage[strcspn(tempMileage, "\n")] = 0;
            tempReturnDate[strcspn(tempReturnDate, "\n")] = 0;

            strcpy(temp->plate, tempPlate);
            temp->mileage = atoi(tempMileage);
            temp->returnDate = atoi(tempReturnDate);
            temp->next = NULL;

            insertNode(rented, temp);       //sends node to be added to list
        }
    }
    fclose(fptr);

    fptr = fopen("repair.txt", "r");
    if(fptr != NULL){
        while(!feof(fptr)){
            temp = malloc(sizeof(car));
            fgets(tempPlate, MAX, fptr);
            fgets(tempMileage, MAX, fptr);
            fgets(tempReturnDate, MAX, fptr);
            fgets(emptyLine, MAX, fptr);

            //deletes \n at the end of each string
            tempPlate[strcspn(tempPlate, "\n")] = 0;
            tempMileage[strcspn(tempMileage, "\n")] = 0;
            tempReturnDate[strcspn(tempReturnDate, "\n")] = 0;

            strcpy(temp->plate, tempPlate);
            temp->mileage = atoi(tempMileage);
            temp->returnDate = atoi(tempReturnDate);
            temp->next = NULL;

            insertNode(atWkShop, temp);         //sends node to be added to list
        }
    }
    fclose(fptr);
}

/*
This function searches a list for if the user input of plate number is actually in the list and if the mileage
entered is greater than original mileage.
*/
bool searchLists(car *available, car *rented, car *repair, car *temp){
    car * temp1 = available, *temp2 = rented, *temp3 = repair, *new = temp;
    int countPlate = 0;
    
    //checks if the plate entered by user exists
    while(temp2 != NULL){
        if(strcmp(temp2->plate, new->plate) == 0)
            countPlate++;
        temp2 = temp2->next;
    }
    
    if(countPlate != 1){
        printf("\nThis plate number does not match any car.\n");
        free(new);
        return false;
    }
    
    else{
        countPlate = 0;

        while(temp1 != NULL){
            if(strcmp(temp1->plate, new->plate) == 0)
                countPlate++;
            temp1 = temp1->next;
        }

        while(temp3 != NULL){
            if(strcmp(temp3->plate, new->plate) == 0)
                countPlate++;
            temp3 = temp3->next;
        }

        if(countPlate > 0){
            printf("\nThe car with this plate number is in another list.\n");
            free(new);
            return false;
        }
    }
    //checks if mileage is higher
    temp2 = rented;
    while(temp2 != NULL){
        if(strcmp(temp2->plate, new->plate) == 0){
            if(new->mileage <= temp2->mileage){
                printf("\nThe mileage you entered is not valid.\n");
                free(new);
                return false;
            }
        }
        temp2 = temp2->next;
    }
    return true;
}

/*
This function deletes only the first node in a linked list
*/
void deleteFirstNode(car **available, car **rented, car **atWkShop){
    car * temp;

    if(*available != NULL){
        temp = *available;
        *available = temp->next;
        free(temp);
    }

    if(*rented != NULL){
        temp = *rented;
        *rented = temp->next;
        free(temp);
    }

    if(*atWkShop != NULL){
        temp = *atWkShop;
        *atWkShop = temp->next;
        free(temp);
    }
}

/*
This function deletes a node from a linked list
*/
void deleteNode(car ** list, car *nodeToDelete){
    car *temp = *list, *previous;
    if(strcmp(temp->plate, nodeToDelete->plate) == 0){
        *list = temp->next;
        free(temp);
    }

    else{
        while(temp != NULL && strcmp(temp->plate, nodeToDelete->plate) != 0){
            previous = temp;
            temp = temp->next;
        }

        previous->next = temp->next;
        free(temp);
    }
}

/*
This function sorts all the arrays in ascending order of mileages or expected return dates
*/
void sortLists(car **available, car **rented, car **repair){
    car *temp = *available, *temp2;
    int value;
    char tempPlate[MAX];

    //sorts available to rent list
    if(*available != NULL){
        while(temp != NULL){
            temp2 = temp->next;
            while(temp2 != NULL){
                if(temp->mileage > temp2->mileage){
                    value = temp->mileage;
                    temp->mileage = temp2->mileage;
                    temp2->mileage = value;

                    strcpy(tempPlate, temp->plate);
                    strcpy(temp->plate, temp2->plate);
                    strcpy(temp2->plate, tempPlate);
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }
    //sorts repair list
    temp = *repair;
    if(*repair != NULL){
        while(temp != NULL){
            temp2 = temp->next;
            while(temp2 != NULL){
                if(temp->mileage > temp2->mileage){
                    value = temp->mileage;
                    temp->mileage = temp2->mileage;
                    temp2->mileage = value;

                    strcpy(tempPlate, temp->plate);
                    strcpy(temp->plate, temp2->plate);
                    strcpy(temp2->plate, tempPlate);
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }

    //sorts rented list
    temp = *rented;
    if(*rented != NULL){
        while(temp != NULL){
            temp2 = temp->next;
            while(temp2 != NULL){
                if(temp->returnDate > temp2->returnDate){
                    value = temp->mileage;
                    temp->mileage = temp2->mileage;
                    temp2->mileage = value;
                    
                    value = temp->returnDate;
                    temp->returnDate = temp2->returnDate;
                    temp2->returnDate = value;

                    strcpy(tempPlate, temp->plate);
                    strcpy(temp->plate, temp2->plate);
                    strcpy(temp2->plate, tempPlate);
                }
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
    }
}

/*
This function transfers a car from the repair list to the available-for-rent list,
deletes node from repair list and adds in to available to rent list
*/
void searchRepairs(car *repair, car ** available, car ** repairReal){
    char input[MAX];
    int count = 0;
    car *temp;

    if(repair != NULL){
        car *tempRepair = repair;

        printf("\nEnter plate number: ");
        fgets(input, MAX, stdin);
        input[strcspn(input, "\n")] = 0;

        //checks if user inputted license plate number exists
        while(tempRepair != NULL){
            if(strcmp(tempRepair->plate, input) == 0){
                count++;
                break;  
            }    
            tempRepair = tempRepair->next;
        }
        
        //creates node to be added to the list
        if(count != 0){
            tempRepair = repair;
            temp = malloc(sizeof(car));
            strcpy(temp->plate, input);
            temp->returnDate = 0;

            while(tempRepair != NULL){
                if(strcmp(tempRepair->plate, input) == 0){
                    temp->mileage = tempRepair->mileage;
                    break;   
                }  
                tempRepair = tempRepair->next;
            }
            temp->next = NULL;
            printf("The car with plate number: %s has been moved to the available to rent list.\n", input);
            insertNode(available, temp);       //adds node to available to rent list
            deleteNode(repairReal, temp);      //deletes node from repair list
        }
        else{
            printf("The plate number entered does not match any car in the repair list.\n");
        }
    }
    else{
        printf("\nThere are no cars in the repair list.\n");
    }
}

/*
This function rents the first available car, deletes node from available to rent list,
add that node to the rented list.
*/
void rentCar(car *available, car **returns, car **availableReal){
    int input;
    car *temp = malloc(sizeof(car));
    car *tempAvailable = available;

    //gets user input
    printf("\nEnter expected return date (YYMMDD): ");
    scanf("%d", &input);
    getchar();

    //creates node
    temp->returnDate = input;
    strcpy(temp->plate, tempAvailable->plate);
    temp->mileage = tempAvailable->mileage;
    temp->next = NULL;

    printf("Car Rented!\n");
    insertNode(returns, temp);          //semds node to be added to rented list
    deleteNode(availableReal, temp);    //sends node to be deleted from available to rent list
}
