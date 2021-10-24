#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
// From files exploration:
#include <fcntl.h>
// This prefix taken from directories exploration:
#define PREFIX "movies_"


// Movie Struct
struct movie
{
    char title[1000];
    int year;
    char languages[1000];
    float rating;
    // struct movie* next;
};

// Comparator function from exploration modified:
int comparator(const void* p, const void* q){
    if(((struct movie*)p)->year == ((struct movie*)q)->year){
        return ((struct movie*)p)->year < ((struct movie*)q)->year;
    }
}

int main(int argc, char** argv) {  

    int year;
    char userLang[1000];
    int uInput;
    int uInput2;
// Open the current directory (from the directories exploration)
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    off_t lastBytesSize;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
// Declare counter for number of movies:
    int counter = -1;

// Takeing user input until input asks to exit the program:
    while(1){
    // Printing the start of program interactivity:

        printf("1. Select file to process\n");
        printf("2. Exit from the program\n");

        printf("\nEnter a choice of 1 or 2: \n");

    // Asking for user input:
        scanf("%d", &uInput);
    
    // Reacting to user input:
        if (uInput < 1){
            printf("Please don't do that. The choices are 1 or 2.\n");
        }
        if (uInput > 2){
            printf("Please don't do that. The choices are 1 or 2.\n");
        }

        switch(uInput){
        ////

        
            case 1:
                printf("Enter 1 to pick the largest file\n");
                printf("Enter 2 to pick the smallest file\n");
                printf("Enter 3 to specify the name of a file\n");
                printf("Enter a choice from 1 to 3:\n");
                scanf("%d", &uInput2);

            // Reacting to user input:
                if (uInput < 1){
                    printf("Please don't do that. The choices are 1, 2, and 3.\n");
                }
                if (uInput > 3){
                    printf("Please don't do that. The choices are 1, 2, and 3.\n");
                }

                switch(uInput2){

                    /* The program finds the largest file with the extension csv in the current 
                    directory whose name starts with the prefix movies_ and automatically process it.
                    In case of tie, pick any of the files with the extension csv starting with movies_ 
                    that have the largest size. */
                    case 1:
                    // Go through all the entries
                        while((aDir = readdir(currDir)) != NULL){

                            if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0){
                        // Get meta-data for the current entry
                            stat(aDir->d_name, &dirStat);  
                            
                        // Find the largest file in the current directory:
                            if(i == 0 || dirStat.st_size > lastBytesSize){
                                lastBytesSize = dirStat.st_size;
                                memset(entryName, '\0', sizeof(entryName));
                                strcpy(entryName, aDir->d_name);
                                }
                            i++;
                            }
                        }
                    // Close the directory
                        closedir(currDir);
                        printf("Now processing the chosen file named %s\n", entryName);
                    // Initialize a movie structs:
                        struct movie m[1000];
                    // Access name of file with char* argv[1], the string value of the file that points to the contents of the file:
                        char* csvName = entryName;
                    // Read the file:
                        FILE* csv = fopen(csvName, "r");
                    // initialize row:
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
                    // Create ONID directory for movie year files:
                        char rando = random();
                        char madeDir[100];
                        sprintf(madeDir, "./933966715.movies.%d",rando);
                        char* newFilePath = madeDir;
                    // Change name for printf later:
                        sprintf(madeDir, "933966715.movies.%d",rando);

                        int file_descriptor;
                        file_descriptor = open(newFilePath, O_RDWR | O_CREAT, 0750);
                        if (file_descriptor == -1){
                            printf("open() failed on \"%s\"\n", newFilePath);
                            perror("Error");
                            exit(1);
                        }
                        else {
                        // Print that the directory has been made:
                            printf("Created directory with name %s\n", madeDir);
                        }
                    //Should I now close the file???????
                        close(file_descriptor);
                    // Looping through every movie in the csv and comparing each ordered year:
                    // Sort the movies by year:
                        qsort(m, counter + 1, sizeof(m[0]), comparator);
                        for(int i = 0; i < counter - 1; i++){
                            if (i = 0 || m[i].year != m[i-1].year){
                            // Create a file with year as name YYYY.txt
	                            char newFile[100];
                                sprintf(newFile, "./%s/%s.txt", madeDir,m[i].year);
                                int fd;
                                fd = open(newFile, O_RDWR | O_CREAT | O_APPEND, 0640);
                            // Append m[i].name to the directory named m[i].year:
                                char titl[50];
                                sprintf(titl,"%s\n", m[i].title);
                                int addTitl = write(fd, titl, strlen(titl));
                                close(fd);
                                }
                            else if (m[i].year == m[i-1].year){
                            // Open the file-directory named m[i].year.txt:
                                char newFile[100];
                                sprintf(newFile, "./%s/%s.txt", madeDir,m[i].year);
                                int fd;
                                fd = open(newFile, O_RDWR | O_APPEND, 0640);
                            // Append m[i].name to the directory named m[i].year:
                                char titl[50];
                                sprintf(titl,"%s\n", m[i].title);
                                int addTitl = write(fd, titl, strlen(titl));
                                close(fd);
                                }
                            }
                        }


                    case 2:
                    // Go through all the entries
                        while((aDir = readdir(currDir)) != NULL){

                            if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0){
                        // Get meta-data for the current entry
                            stat(aDir->d_name, &dirStat);  
                            
                        // Find the largest file in the current directory:
                            if(i == 0 || dirStat.st_size > lastBytesSize){
                                lastBytesSize = dirStat.st_size;
                                memset(entryName, '\0', sizeof(entryName));
                                strcpy(entryName, aDir->d_name);
                                }
                            i++;
                            }
                        }
                    // Close the directory
                        closedir(currDir);
                        printf("Now processing the chosen file named %s\n", entryName);
                    // Initialize a movie structs:
                        struct movie m[1000];
                    // Access name of file with char* argv[1], the string value of the file that points to the contents of the file:
                        char* csvName = entryName;
                    // Read the file:
                        FILE* csv = fopen(csvName, "r");
                    // initialize row:
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
                    // Create ONID directory for movie year files:
                        char rando = random();
                        char madeDir[100];
                        sprintf(madeDir, "./933966715.movies.%d",rando);
                        char* newFilePath = madeDir;
                    // Change name for printf later:
                        sprintf(madeDir, "933966715.movies.%d",rando);

                        int file_descriptor;
                        file_descriptor = open(newFilePath, O_RDWR | O_CREAT, 0750);
                        if (file_descriptor == -1){
                            printf("open() failed on \"%s\"\n", newFilePath);
                            perror("Error");
                            exit(1);
                        }
                        else {
                        // Print that the directory has been made:
                            printf("Created directory with name %s\n", madeDir);
                        }
                    //Should I now close the file???????
                        close(file_descriptor);
                    // Looping through every movie in the csv and comparing each ordered year:
                    // Sort the movies by year:
                        qsort(m, counter + 1, sizeof(m[0]), comparator);
                        for(int i = 0; i < counter - 1; i++){
                            if (i = 0 || m[i].year != m[i-1].year){
                            // Create a file with year as name YYYY.txt
	                            char newFile[100];
                                sprintf(newFile, "./%s/%s.txt", madeDir,m[i].year);
                                int fd;
                                fd = open(newFile, O_RDWR | O_CREAT | O_APPEND, 0640);
                            // Append m[i].name to the directory named m[i].year:
                                char titl[50];
                                sprintf(titl,"%s\n", m[i].title);
                                int addTitl = write(fd, titl, strlen(titl));
                                close(fd);
                                }
                            else if (m[i].year == m[i-1].year){
                            // Open the file-directory named m[i].year.txt:
                                char newFile[100];
                                sprintf(newFile, "./%s/%s.txt", madeDir,m[i].year);
                                int fd;
                                fd = open(newFile, O_RDWR | O_APPEND, 0640);
                            // Append m[i].name to the directory named m[i].year:
                                char titl[50];
                                sprintf(titl,"%s\n", m[i].title);
                                int addTitl = write(fd, titl, strlen(titl));
                                close(fd);
                                }
                            }

                    case 3:
                }


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

        } // switch(uInput)

    } // while(1)

    return 0;
    
} // int main(int argc, char** argv)

