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

int main(void) {  

    int uInput;
    int uInput2;
// Open the current directory (from the directories exploration)
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    off_t lastBytesSize;
    struct stat dirStat;
    int i = 0;
    char entryName[256];
    char entryName2[256];
    char entryName3[256];
// Declare counter for number of movies:
    int counter;
    counter = 0;


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
                while(1){
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
                                    if((i == 0 || dirStat.st_size > lastBytesSize) && (dirStat.st_mode)){
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
                            long int rando = 0;
                            for (int i = 0; i < 2; i++){
                                rando = random();
                                if (0 > rando){
                                    i--;
                                }
                                if (99999 < rando){
                                    i--;
                                }
                            }
                            char madeDir[256];
                            sprintf(madeDir, "./933966715.movies.%ld",rando);
                            char* newFilePath = madeDir;
                            mkdir(newFilePath, 0750);
                        // Change name for printf later:
                            sprintf(madeDir, "933966715.movies.%ld",rando);
                        // Print that the directory has been made:
                            printf("Created directory with name %s\n", madeDir);
                        // Looping through every movie in the csv and comparing each ordered year:
                            for(int i = 0; i < counter - 1; i++){
                                
                                if (i == 0 || m[i].year != m[i-1].year){
                                    
                                // Create a file with year as name YYYY.txt
                                    char newFile[256];                                
                                    sprintf(newFile, "./%s/%d.txt", madeDir,m[i].year);
                                    int fd;
                                    fd = open(newFile,  O_RDWR | O_CREAT | O_APPEND, 0640);
                                // Append m[i].name to the file named m[i].year:
                                    char titl[256];
                                    sprintf(titl,"%s\n", m[i].title);
                                    
                                    write(fd, titl, strlen(titl));
                                    close(fd);
                                    
                                    }
                                else {
                                // Open the file-directory named m[i].year.txt:
                                    char nuevoFile[256];
                                    sprintf(nuevoFile, "./%s/%d.txt", madeDir,m[i].year);
                                    int fp;
                                    fp = open(nuevoFile, O_RDWR | O_APPEND, 0640);
                                // Append m[i].name to the directory named m[i].year:
                                    char titulo[50];
                                    sprintf(titulo,"%s\n", m[i].title);
                                    write(fp, titulo, strlen(titulo));
                                    close(fp);
                                    }
                            }
                            break;

                        case 2:
                        // Go through all the entries
                            while((aDir = readdir(currDir)) != NULL){

                                if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0){
                                // Get meta-data for the current entry
                                    stat(aDir->d_name, &dirStat);  
                                    
                                // Find the smallest file in the current directory:
                                    if((i == 0 || dirStat.st_size < lastBytesSize) && (dirStat.st_mode)){
                                        lastBytesSize = dirStat.st_size;
                                        memset(entryName2, '\0', sizeof(entryName2));
                                        strcpy(entryName2, aDir->d_name);
                                        }
                                    i++;
                                }
                            }
                        // Close the directory
                            closedir(currDir);
                            printf("Now processing the chosen file named %s\n", entryName2);
                        // Initialize a movie structs:
                            struct movie m2[1000];
                        // Access name of file with char* argv[1], the string value of the file that points to the contents of the file:
                            char* csvName2 = entryName2;
                        // Read the file:
                            FILE* csv2 = fopen(csvName2, "r");
                        // initialize row:
                            char row2[4096];
                        // Processing file line by line:
                            while(fgets(row2, sizeof(row2), csv2)){
                                if (counter >= 1){ // Checks if this is not the first row of the file (titles row).
                                    char* hdr2 = strdup(row2);
                                    char* toke2 = strtok(hdr2, ",\n");
                                    int j = 1;
                                    
                                    
                                    // Processing each column from a single row (tapping out when the strtok_r returns a NULL operator):
                                    while (toke2 != NULL){ 

                                        if(j == 1){ 
                                            strcpy(m2[counter - 1].title, toke2);
                                        };
                                        if(j == 2){
                                            m2[counter - 1].year = atoi(toke2);
                                        };
                                        if(j == 3){
                                            strcpy(m2[counter - 1].languages, toke2);
                                        };
                                        if(j == 4){
                                            m2[counter - 1].rating = atof(toke2);
                                        };
                                        toke2 = strtok(NULL, ",\n");
                                        j++;
                                        
                                    }
                                    free(hdr2);
                                }
                                counter++;
                            }
                            
                        // Create ONID directory for movie year files:
                            long int rando2 = 0;
                            for (int i = 0; i < 2; i++){
                                rando2 = random();
                                if (0 > rando2){
                                    i--;
                                }
                                if (99999 < rando2){
                                    i--;
                                }
                            }
                            char madeDir2[256];
                            sprintf(madeDir2, "./933966715.movies.%ld",rando2);
                            char* newFilePath2 = madeDir2;
                            mkdir(newFilePath2, 0750);
                        // Change name for printf later:
                            sprintf(madeDir2, "933966715.movies.%ld",rando2);
                        // Print that the directory has been made:
                            printf("Created directory with name %s\n", madeDir2);
                        // Looping through every movie in the csv and comparing each ordered year:
                            for(int i = 0; i < counter - 1; i++){
                                
                                if (i == 0 || m2[i].year != m2[i-1].year){
                                    
                                // Create a file with year as name YYYY.txt
                                    char newFile2[256];                                
                                    sprintf(newFile2, "./%s/%d.txt", madeDir2,m2[i].year);
                                    int fd2;
                                    fd2 = open(newFile2, O_RDWR | O_CREAT | O_APPEND, 0640);
                                // Append m2[i].name to the file named m2[i].year:
                                    char titl2[256];
                                    sprintf(titl2,"%s\n", m2[i].title);
                                    
                                    write(fd2, titl2, strlen(titl2));
                                    close(fd2);
                                    
                                    }
                                else {
                                // Open the file-directory named m2[i].year.txt:
                                    char nuevoFile2[256];
                                    sprintf(nuevoFile2, "./%s/%d.txt", madeDir2,m2[i].year);
                                    int fp2;
                                    fp2 = open(nuevoFile2, O_RDWR | O_APPEND, 0640);
                                // Append m2[i].name to the directory named m2[i].year:
                                    char titulo2[50];
                                    sprintf(titulo2,"%s\n", m2[i].title);
                                    write(fp2, titulo2, strlen(titulo2));
                                    close(fp2);
                                    }
                            }
                            break;

                        case 3:
                            printf("Enter the complete file name: ");
                        // Asking for user input:
                            char uInput3[100];
                            scanf("%s", uInput3);
                            // Go through all the entries
                            while((aDir = readdir(currDir)) != NULL){

                                if(strcmp(uInput3, aDir->d_name) == 0){
                                // Get meta-data for the current entry
                                    stat(aDir->d_name, &dirStat);
                                    memset(entryName3, '\0', sizeof(entryName3));
                                    strcpy(entryName3, aDir->d_name);
                                }
                            }
                        // Close the directory
                            closedir(currDir);
                            if (strlen(entryName3) == 0){
                                printf("\nThe file %s was not found. Try again\n", uInput3);
                                break;
                            }
                            else{
                                printf("\nNow processing the chosen file named %s", entryName3);
                            }
                            
                        // Initialize a movie structs:
                            struct movie m3[1000];
                        // Access name of file with char* argv[1], the string value of the file that points to the contents of the file:
                            char* csvName3 = entryName3;
                        // Read the file:
                            FILE* csv3 = fopen(csvName3, "r");
                        // initialize row:
                            char row3[4096];
                        // Processing file line by line:
                            while(fgets(row3, sizeof(row3), csv3)){
                                if (counter >= 1){ // Checks if this is not the first row of the file (titles row).
                                    char* hdr3 = strdup(row3);
                                    char* toke3 = strtok(hdr3, ",\n");
                                    int j = 1;
                                    
                                    
                                    // Processing each column from a single row (tapping out when the strtok_r returns a NULL operator):
                                    while (toke3 != NULL){ 

                                        if(j == 1){ 
                                            strcpy(m3[counter - 1].title, toke3);
                                        };
                                        if(j == 2){
                                            m3[counter - 1].year = atoi(toke3);
                                        };
                                        if(j == 3){
                                            strcpy(m3[counter - 1].languages, toke3);
                                        };
                                        if(j == 4){
                                            m3[counter - 1].rating = atof(toke3);
                                        };
                                        toke3 = strtok(NULL, ",\n");
                                        j++;
                                        
                                    }
                                    free(hdr3);
                                }
                                counter++;
                            }
                        // Create ONID directory for movie year files:
                            long int rando3 = 0;
                            for (int i = 0; i < 2; i++){
                                rando3 = random();
                                if (0 > rando3){
                                    i--;
                                }
                                if (99999 < rando3){
                                    i--;
                                }
                            }
                            char madeDir3[256];
                            sprintf(madeDir3, "./933966715.movies.%ld",rando3);
                            char* newFilePath3 = madeDir3;
                            mkdir(newFilePath3, 0750);
                        // Change name for printf later:
                            sprintf(madeDir3, "933966715.movies.%ld",rando3);
                        // Print that the directory has been made:
                            printf("\nCreated directory with name %s\n", madeDir3);
                        // Looping through every movie in the csv and comparing each ordered year:
                            for(int i = 0; i < counter - 1; i++){
                                
                                if (i == 0 || m3[i].year != m3[i-1].year){
                                    
                                // Create a file with year as name YYYY.txt
                                    char newFile3[256];                                
                                    sprintf(newFile3, "./%s/%d.txt", madeDir3,m3[i].year);
                                    int fd3;
                                    fd3 = open(newFile3, O_RDWR | O_CREAT | O_APPEND, 0640);
                                // Append m2[i].name to the file named m2[i].year:
                                    char titl3[256];
                                    sprintf(titl3,"%s\n", m3[i].title);
                                    
                                    write(fd3, titl3, strlen(titl3));
                                    close(fd3);
                                    
                                    }
                                else {
                                // Open the file-directory named m2[i].year.txt:
                                    char nuevoFile3[256];
                                    sprintf(nuevoFile3, "./%s/%d.txt", madeDir3,m3[i].year);
                                    int fp3;
                                    fp3 = open(nuevoFile3, O_RDWR | O_APPEND, 0640);
                                // Append m2[i].name to the directory named m2[i].year:
                                    char titulo3[50];
                                    sprintf(titulo3,"%s\n", m3[i].title);
                                    write(fp3, titulo3, strlen(titulo3));
                                    close(fp3);
                                    }
                            }

                            break;

                    } // switch(uInput2)

                } // while(1) (the 2nd one)
            case 2:
                exit(0);

        } // switch(uInput)

    } // while(1)

    return 0;
    
 }// int main(void)

