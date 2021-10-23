#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Movie Struct
struct movie
{
    char title[1000];
    int year;
    char languages[1000];
    float rating;
    // struct movie* next;
};

int main(int argc, char** argv) {  

// Declare counter for number of movies:
    int counter = -1;

// Validate Argument:
    if (argc == 1){
        printf("Please enter a file for the program to process\n");
        // return 0;
    }

    else if(argc > 2){
        printf("Please enter just one file for the program to process\n");
        return 0;
    }

    // Initialize char** argv into array of movie structs:
    struct movie m[1000];
    // Access name of file with char* argv[1], the string value of the file that points to the contents of the file:
    char* csvName = argv[1];
    // Read the file:
    FILE* csv = fopen(csvName, "r");

    // Validate csv:
    if ( csv == NULL){
        printf("%s file cannot be opened.\n", csvName);
        return 0;
    }
    char row[4096];

// Processing file line by line:
    while(fgets(row, sizeof(row), csv)){
        if (counter >= 1){ // Checks if this is not the first row of the file (titles row).
            char* hdr = strdup(row);
            char* toke = strtok(hdr, ",\n");
            int j = 1;

            // Processing each column from a single row (tapping out when the strtok_r returns a NULL operator):
            while (toke != NULL){ 

                if(j == 1){ 
                    strcpy(m[counter - 1].title, toke);
                };
                if(j == 2){
                    m[counter - 1].year = atoi(toke);
                };
                if(j == 3){
                    strcpy(m[counter - 1].languages, toke);
                };
                if(j == 4){
                    m[counter - 1].rating = atof(toke);
                };
                toke = strtok(NULL, ",\n");
                j++;
            }
            free(hdr);
        }
        counter++;
    }

    int year;
    char userLang[1000];
    int uInput;

// Takeing user input until input asks to exit the program:
    while(1){
    // Printing the start of program interactivity with news that the file has processed:
        printf("Processed file %s and parsed data for %d movies\n\n", csvName, counter);

        printf("1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n");

        printf("\nEnter a choice from 1 to 4: \n");

    // Asking for user input:
        scanf("%d", &uInput);
    
    // Reacting to user input:
        if (uInput < 1){
            printf("Please don't do that. The choices are 1 to 4 inclusive.\n");
        }
        if (uInput > 4){
            printf("Please don't do that. The choices are 1 to 4 inclusive.\n");
        }
        switch(uInput){
        ////
            case 1:
                printf("Enter the year for which you want to see movies: ");
                scanf("%d", &year);
                int flag = 0;
            // Looping to find a movie struct where year matches user requested year:
                for(int i = 0; i < counter ;i++){
                    if(m[i].year == year){
                        printf("%s\n", m[i].title);
                        flag = 1; // Indicates that at least one movie of user requested year is in csv.
                    }
                }
            // Printing a message for when no movies of that year are in the file:
                if(flag == 0){
                    printf("\nNo data about movies released in year %d\n\n", year);
                }
                break;
        ////
            case 2:    
            // Looping through every year from 1800-2100:
                for (int awardYear = 1800; awardYear < 2100; awardYear++){
                    double highest = 0;
                    int winner = -1;
                // Looping through every movie in the csv and comparing it to current highest, if it is in the given year:
                    for(int i = 0; i < counter - 1; i++){
                        if (m[i].year == awardYear){
                            if (highest == 0){
                                highest = m[i].rating;
                                winner = i;
                            }
                            else if (m[i].rating > highest){
                                highest = m[i].rating;
                                winner = i;
                            }
                        }
                    }
                // Printing the winner of each year if one exists:
                    if(winner >= 0){
                        printf("%d %.2f %s\n", m[winner].year, m[winner].rating, m[winner].title);
                    }
                }
                break;
        ////
            case 3:
                printf("Enter the language for which you want to see movies: ");
                scanf("%s", userLang);
                
                int flagg = 0;
                for(int i = 0; i < counter ;i++){
                    
                    char* mLangos = strdup(m[i].languages);
                    char* lango = strtok(mLangos, "[];");
                   

                    while ( lango != NULL ){
                        if(strcmp(lango, userLang) == 0){
                            printf("%d %s\n",m[i].year, m[i].title);
                            flagg = 1;
                        }
                        lango = strtok(NULL, "];");
                        
                    }
                    
                }
            // Printing a message for when no movies are in that language:
                if(flagg == 0)printf("No data about movies released in %s\n\n", userLang);
                printf("\n");
                break;
        ////
            case 4:
                exit(0);
        }
    }
    return 0;
}

