// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

struct highestRatedMovie {
    char year[5];  // Year as a string
    char rating[4];  // Rating as a string
    char title[100];  // Title of the movie
};

/* struct for movie information */
struct movie
{
    char *title;
    char *year;
    char *language;
    char *rating;
    struct movie *next;
};

/* Parse the current line which is space delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the language
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->language = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->language, token);

    // The last token is the rating
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->rating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->rating, token);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of movies by parsing data from
* each line of the specified file.
*/


struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;
    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode=createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie){
  printf("%s, %s %s, %s\n", aMovie->title,
               aMovie->year,
               aMovie->language,
               aMovie->rating);
}
/*
* Print the linked list of movies
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

void displayMenu() {
    printf("Please choose an option between 1-4\n");
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");
}

void showMoviesInYear(struct movie *list)
{
    int year;

    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    struct movie *current = list;
    int found = 0;

    while (current != NULL)
    {
        if (atoi(current->year) == year)
        {
            printf("%s\n", current->title);
            found = 1;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("No movies found for the year %d.\n", year);
    }
}

void showHighestRatedMovies(struct movie *list) {
    // Create a temporary pointer to the head of the linked list
    struct movie *temp = list;

    // Create a map to store the highest rated movie for each year
    int year;
    float rating;
    char title[100];
    struct movie *highestRatedMovies[2022-1900];  // Map for years from 1900 to 2021
    for (int i = 0; i < 2022-1900; i++) {
        highestRatedMovies[i] = NULL;
    }

    // Iterate through the linked list and update the map with highest rated movies
    while (temp != NULL) {
        year = atoi(temp->year);
        rating = strtof(temp->rating, NULL);
        if (highestRatedMovies[year-1900] == NULL || strtof(highestRatedMovies[year-1900]->rating, NULL) < rating) {
            highestRatedMovies[year-1900] = temp;
        }
        temp = temp->next;
    }

    // Print the highest rated movie for each year
    for (int i = 0; i < 2022-1900; i++) {
        if (highestRatedMovies[i] != NULL) {
            printf("%d %.1f %s\n", i+1900, strtof(highestRatedMovies[i]->rating, NULL), highestRatedMovies[i]->title);
        }
    }
}

void showMoviesByLanguage(struct movie *list) {
    char language[100];
    printf("Enter a language: ");
    scanf("%99s", language);
    language[strcspn(language, "\n")] = '\0'; // Remove newline character

    struct movie *temp = list;
    int found = 0;

    // Loop through the linked list
    while (temp != NULL) {
        // Copy the language string to a temporary buffer for parsing
        char lang[100];
        strcpy(lang, temp->language);

        // Parse the language string into individual languages
        char *token = strtok(lang, "[];");
        while (token != NULL) {
            // Check if the current language matches the requested language
            if (strcmp(token, language) == 0) {
                // Print the movie information
                printf("%s (%s) %s\n", temp->title, temp->year, temp->rating);
                found = 1;
                break;
            }
            token = strtok(NULL, "[];");
        }
        temp = temp->next;
    }
    if (!found) {
        printf("No movies found for the given language.\n");
    }
}


/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movie main.c
*/

int main(int argc, char *argv[]){
        if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movie movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);

    while (1) {
        int choice = 0;
        int year;
        char lang[20];

        displayMenu();
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("1. Show movies released in the specified year\n");
                showMoviesInYear(list);
                break;
            case 2:
                printf("2. Show highest rated movie for each year\n");
                showHighestRatedMovies(list);
                break;
            case 3:
                printf("3. Show movies and their year of release for a specific language\n");
                showMoviesByLanguage(list);
                break;
            case 4:
                printf("4. Exit from the program\n");
                exit(0);
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
                break;
        }
    }

    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movie movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }
    //struct movie *list = processFile(argv[1]);
    //printMovieList(list);
    return EXIT_SUCCESS;
}
