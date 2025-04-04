#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 128
#define SEP ';'
#define movie_db "struct-data.txt"
#define director_db "struct-data2.txt"

/* Structure ------------------------------------------------------------------------------------------------------ */

struct movie;                                                       /* Define structure of Movie */

struct movie_head;                                                  /* Define structure of Movie List Head*/

struct director;                                                    /* Define structure of Director */

struct director_head;                                               /* Define structure of Director List Head */

typedef struct movie MOV;                                           /* Define Structured Type of Movie */

typedef struct movie_head MHD;                                      /* Define Structured Type of Movie List Head*/

typedef struct director DIR;                                        /* Define Structured Type of Director */

typedef struct director_head DHD;                                   /* Define Structured Type of Director List Head*/

/* Input ---------------------------------------------------------------------------------------------------------- */

void new_gets(char s[], int lim);                                   /*Read the string and write it in s[]*/

int enter_integer(char *message, int a, int b);                     /* Enter integer number in [a,b] and return it */

float enter_float(char *message, float a, float b);                 /* Enter float number in [a,b] and return it */

void enter_date(char *message, int *day, int *month, int *year);    /* Enter date */

int menu();                                                         /* Output main menu and enter option */

int detail();                                                       /* Action will be done in movie or director card-file */

int movie_by_what(int i);                                           /* Output name of movie fields (0 without id, 1 with id) */

int director_by_what(int i);                                        /* Output name of director fields (0 without id, 1 with id */

/* Output --------------------------------------------------------------------------------------------------------- */

void clear_screen();                                                /* Clear the console */

void print_head(int n);                                             /* Output head of sheet (0 - for MOVIE LIST, 1 - for DIRECTOR LIST) */

void print_tail(int n);                                             /* Output tail of sheet (0 - for MOVIE LIST, 1 - for DIRECTOR LIST) */

void reference();                                                   /* Information about program and card-file */

void output_movie(MHD *mph);                                        /* Output movie list */

void output_director(DHD *dph);                                     /* Output director list */

/* Heads of Lists ------------------------------------------------------------------------------------------------- */

MHD *make_movie_head();                                             /* Movie Head Initialization */

void update_movie_cnt(MHD *mph);                                    /* Update counter in movie head on max(id) to correct work */

DHD *make_director_head();                                          /* Director Head Initialization */

void update_director_cnt(DHD *dph);                                 /* Update counter in director head on max(id) to correct work */

/* Create Node of List -------------------------------------------------------------------------------------------- */

DIR *find_director_by_id(DHD *dph, int id);                         /* Get pointer to Node in Director List by ID */

DIR *find_director_by_name(DHD *dph, char *movie_director);         /* Get pointer to Node in Director List by name */
                                                                    /* Movie Node Initialization */
MOV *create_movie(DHD *dph, int id_mov, char *movie_name, int id_dir, int movie_year, int movie_duration, float movie_kpr, float movie_plr, int watch_date[3]);

void add_movie(MHD *mph, MOV *new_node, MOV *current_node);         /* Add new Movie Node to List */

DIR *create_director(int id, char *movie_director);                 /* Director Node Initialization */

void add_director(DHD *dph, DIR *new_node, DIR *current_node);      /* Add new Director Node to List */

/* Supporting ----------------------------------------------------------------------------------------------------- */

void clear_input_buffer();                                          /* Clear input buffer */

void to_lowercase(char *str);                                       /* Converts string to lowercase */

void rewrite_file(MHD *mph, DHD *dph);                              /* Rewrite file using lists */

void split_string(char *inputString, char **words, int *wordCount, char sep);
                                                                    /* Split string by separator */
/* Fill the lists and Create their elements ----------------------------------------------------------------------- */
                                                                    /* Adding the movie data of file to list */
void add_movie_from_file(MHD *mph, DHD *dph, char *filename, char sep);

void add_movie_to_list(MHD *mph, DHD *dph);                         /* Enter new data -> Create new element -> Add to list */

void add_director_from_file(DHD *dph, char *filename, char sep);    /* Adding the director data of file to list */

void add_director_to_list(DHD *dph);                                /* Enter new data -> Create new element -> Add to list */

/* Find in movie -------------------------------------------------------------------------------------------------- */
                                                                    /* Enter number in [a;b] and output relevant records */
void find_number_in_movie(MHD *mph, char *message, int a, int b, int i);

void find_string_in_movie(MHD *mph, char *message, int i);          /* Enter string (or substring) and output relevant records */

void find_date_in_movie(MHD *mph);                                  /* Enter date and output relevant records */

void find_in_movie(MHD *mph);                                       /* Chose the field of list to find record */

/* Find in director ----------------------------------------------------------------------------------------------- */

void find_id_in_director(DHD *dph);                                 /* Enter ID and output relevant records */

void find_name_in_director(DHD *dph);                               /* Enter string (or substring) and output relevant records */

void find_in_director(DHD *dph);                                    /* Chose the field of list to find record */

/* Edit Element of the List --------------------------------------------------------------------------------------- */

void edit_in_movie(MHD *mph, DHD *dph);                             /* Edit the data of record in Movie list */

void edit_in_director(DHD *dph);                                    /* Edit the data of record in Director list */

/* Delete Element of the List ------------------------------------------------------------------------------------- */

void delete_movie(MHD *mph, MOV *current_movie);                    /* Delete Movie Node */

void delete_in_movie(MHD *mph);                                     /* Enter ID and delete relevant record */

void delete_director(MHD *mph, DHD *dph, DIR *current_director);    /* Delete Director Node */

void delete_in_director(MHD *mph, DHD *dph);                        /* Enter ID and delete relevant record */

/* Sort the Movie List -------------------------------------------------------------------------------------------- */

int swap_movie(MHD *mph, MOV *current);                             /* Swap movie nodes */

void sort_movie_by_number(MHD *mph, int i, int direction);          /* Sort movie list by number field (0 - FORWARD / 1 - BACKWARD direction) */

void sort_movie_by_string(MHD *mph, int i, int direction);          /* Sort movie list by string field (0 - A-Z / 1 - Z-A direction) */

void sort_movie_by_date(MHD *mph, int direction);                   /* Sort movie list by date (0 - FORWARD / 1 - BACKWARD direction) */

void sort_in_movie(MHD *mph);                                       /* Chose the field to sort */

/* Sort the Director List ----------------------------------------------------------------------------------------- */

int swap_director(DHD *dph, DIR *current);                          /* Swap director nodes */

void sort_director_by_id(DHD *dph, int direction);                  /* Sort director list by id (0 - FORWARD / 1 - BACKWARD direction) */

void sort_director_by_name(DHD *dph, int direction);                /* Sort director list by name (0 - A-Z / 1 - Z-A direction) */

void sort_in_director(DHD *dph);                                    /* Chose the field to sort */

/* Main program --------------------------------------------------------------------------------------------------- */

int main(){
    MHD *mph = NULL;
    DHD *dph = NULL;
    int option, choise;
    dph=make_director_head();
    mph=make_movie_head();
    add_director_from_file(dph, director_db, SEP);
    add_movie_from_file(mph, dph, movie_db, SEP);
    update_director_cnt(dph);
    update_movie_cnt(mph);

    do{
        option = menu();

        switch(option){
        case 0: puts("\nYour selection is EXIT"); break;

        case 1:{
            puts("\nYour selection is SHOW THE DATA");
            choise=detail();
            if(choise==0) output_movie(mph);
            else {puts(""); output_director(dph);}
            break;
        }

        case 2:{
            puts("\nYour selection is ADD THE DATA");
            choise=detail();
            if(choise==0) add_movie_to_list(mph, dph);
            else add_director_to_list(dph);
            break;
        }

        case 3:{
            puts("\nYour selection is FIND THE DATA BY PARAMETERS");
            choise=detail();
            if(choise==0) find_in_movie(mph);
            else find_in_director(dph);
            break;
        }

        case 4:{
            puts("\nYour selection is EDIT THE DATA");
            choise=detail();
            if(choise==0) edit_in_movie(mph, dph);
            else edit_in_director(dph);
            break;
        }

        case 5:{
            puts("\nYour selection is DELETE THE DATA");
            choise=detail();
            if(choise==0) delete_in_movie(mph);
            else delete_in_director(mph, dph);
            break;
        }

        case 6:{
            puts("\nYour selection is SORT THE DATA");
            choise=detail();
            if(choise==0) sort_in_movie(mph);
            else sort_in_director(dph);
            break;
        }

        case 7:{
            puts("\nYour selection is REFERENCE");
            reference();
            break;
        }
        }
    rewrite_file(mph, dph);
    puts("\nPress ENTER to continue");
    clear_input_buffer();
    clear_screen();
    } while (option!=0);
    return 0;
}

/* Functions ------------------------------------------------------------------------------------------------------ */

/* Structure ------------------------------------------------------------------------------------------------------ */

struct movie{
    int id;                     /* ID of movie */
    char *name;                 /* Name of movie */
    struct director *director;  /* Director of movie */
    int year;                   /* Year of movie release */
    int duration;               /* Duration of movie in minutes*/
    float kpr;                  /* Movie rating on KinoPoisk */
    float plr;                  /* Movie rating on my opinion */
    int date[3];                /* Day/Month/Year of watch the movie */
    struct movie *prev;         /* Link to previous node */
    struct movie *next;         /* Link to next node */
};

struct movie_head{
    int cnt;                    /* Number of all existing elements */
    struct movie *first;        /* Link to first element of list */
    struct movie *last;         /* Link to last element of list */
};

struct director{
    int id;                     /* ID of the director */
    char *name;                 /* Director name */
    struct director *next;      /* Link to previous node */
    struct director *prev;      /* Link to next node */
};

struct director_head{
    int cnt;                    /* Number of all existing elements */
    struct director *first;     /* Link to first element of list */
    struct director *last;      /* Link to last element of list */
};

/* Input ---------------------------------------------------------------------------------------------------------- */

void new_gets(char s[], int lim){
    char c;
    int i;
    i=0;

    while (((c=getchar())!='\n') && (i < lim-1)){ /* Entering symbols to the end of the string or until the limit is reached */
        s[i]=c;
        i++;
    }
    s[i]='\0';
}

int enter_integer(char *message, int a, int b){
    char input[MAXLEN];
    int number, flag;
    flag = 0;

    do{
        printf(message);
        new_gets(input, MAXLEN);
        if (sscanf(input, "%d", &number) == 1 && number >= a && number <= b) flag = 1; /* If entered number is number and in [a, b] */
        else puts("Entered value is not correct. Please try again!");
    } while (flag == 0); /* While entered number is not correct */
    return number;
}

float enter_float(char *message, float a, float b){
    char input[MAXLEN];
    float number;
    int flag;
    flag = 0;

    do{
        printf(message);
        new_gets(input, MAXLEN);
        if (sscanf(input, "%f", &number) == 1 && number >= a && number <= b) flag = 1; /* If entered number is number and in [a, b] */
        else puts("Entered value is not correct. Please try again!");
    } while (flag == 0); /* While entered number is not correct */
    return number;
}

void enter_date(char *message, int *day, int *month, int *year){
    char input[MAXLEN];
    int flag;
    flag=0;

    do {
        printf(message);
        new_gets(input, MAXLEN);
        if (sscanf(input, "%d %d %d", day, month, year) == 3 && *day >= 1 && *day <= 31 && *month >= 1 && *month <= 12 && *year >= 1895 && *year <= 2099) flag = 1;  /* If entered date is real and in [1895-...] year */
        else printf("Entered date is not correct. Please try again!\n");                                                                            /* First movie out in 1895 | flag := 1 */
    } while (flag==0); /* While entered date is not correct */
}

int menu(){
    int option;

    puts("Choose the option");
    puts("0 - for EXIT PROGRAM");
    puts("1 - for SHOW THE DATA");
    puts("2 - for ADD THE DATA");
    puts("3 - for FIND THE DATA BY PARAMETERS");
    puts("4 - for EDIT THE DATA");
    puts("5 - for DELETE THE DATA");
    puts("6 - for SORT THE DATA");
    puts("7 - for REFERENCE");

    option = enter_integer("Enter the option: ", 0, 7);
    return option;
}

int detail(){
    int option;

    puts("\nChoose the card-file");
    puts("0 - for MOVIE card-file");
    puts("1 - for DIRECTOR card-file");

    option = enter_integer("Enter the option: ", 0, 1);
    return option;
}

int movie_by_what(int i){
    int option;

    puts("\nChoose the parameter");
    if(i==1) puts("0 - by ID");
    else puts("0 - to EXIT");
    puts("1 - by Name");
    puts("2 - by Director");
    puts("3 - by Year");
    puts("4 - by Duration");
    puts("5 - by KPR");
    puts("6 - by PLR");
    puts("7 - by Date");

    option = enter_integer("Enter the option: ", 0, 7);
    return option;
}

int director_by_what(int i){
    int option;

    puts("\nChoose the parameter");
    if(i==0) puts("0 - to EXIT");
    else puts("0 - by ID");
    puts("1 - by Name");

    option = enter_integer("Enter the option: ", 0, 1);
    return option;
}

/* Output --------------------------------------------------------------------------------------------------------- */

void clear_screen(){
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void print_head(int n){
    if(n==0){               /* 0 for movie / 1 for director */
        printf("| %2s | %25s | %25s | %4s | %3s | %5s | %5s | %10s |\n","ID","Name","Director","Year","Dur","KPR","PLR","Watchdate");
        printf("+----+---------------------------+---------------------------+------+-----+-------+-------+------------+\n");
    } else {
        printf("| %2s | %25s |\n","ID","Director");
        printf("+----+---------------------------+\n");
    }
}

void print_tail(int n){     /* 0 for movie / 1 for director */
    if(n==0) printf("+----+---------------------------+---------------------------+------+-----+-------+-------+------------+\n");
    else printf("+----+---------------------------+\n");
}

void reference(){
    printf("\nThis program is card-file. Its functions are presented in the menu.\nSubject area of the program - movie\nDesignations:\nName - Name of movie\nDirector - Director of the movie\nYear - Year of movie release\nDuration - Duration of the movie in minutes\nKPR - Rating of the movie on KinoPoisk\nPLR - Personal rating of the movie\nDate - Date of viewing the movie\n");
}

void output_movie(MHD *mph){
    MOV *current;
    current = mph->first;

    puts("");
    if(current!=NULL){
        print_head(0);
        while (current!=NULL) {
                printf("| %2d | %25s | %25s | %d | %3d | %5.1f | %5.1f | %.2d.%.2d.%d |\n", current->id, current->name, current->director->name, current->year, current->duration, current->kpr, current->plr, current->date[0], current->date[1], current->date[2]);
                current = current->next;
        }
        print_tail(0);
    } else puts("The movie list is empty");
}

void output_director(DHD *dph){
    DIR *current;
    current = dph->first;

    if(current!=NULL){
    print_head(1);
    while (current!=NULL) {
        printf("| %2d | %25s |\n", current->id, current->name);
        current = current->next;
    }
    print_tail(1);
    } else puts("The director list is empty");
}

/* Heads of Lists ------------------------------------------------------------------------------------------------- */

MHD *make_movie_head(){
    MHD *mph=NULL; /* Define and init Head */
    mph=(MHD*)malloc(sizeof(MHD));
    mph->cnt = 0;
    mph->first=NULL;
    mph->last=NULL;
    return mph;
}

void update_movie_cnt(MHD *mph){
    MOV *current;
    int max_id;
    current = mph->first;
    max_id = -1;

    while (current != NULL) {
        if (current->id > max_id) max_id = current->id;
        current = current->next;
    }
    mph->cnt = max_id + 1;
}

DHD *make_director_head(){
    DHD *dph=NULL; /* Define and init Head */
    dph=(DHD*)malloc(sizeof(DHD));
    dph->cnt = 0;
    dph->first=NULL;
    dph->last=NULL;
    return dph;
}

void update_director_cnt(DHD *dph){
    DIR *current;
    int max_id;
    current = dph->first;
    max_id = -1;

    while (current != NULL) {
        if (current->id > max_id) max_id = current->id;
        current = current->next;
    }
    dph->cnt = max_id + 1;
}

/* Create Node of List -------------------------------------------------------------------------------------------- */

DIR *find_director_by_id(DHD *dph, int id){
    DIR *current = NULL;
    int flag;
    current = dph->first;
    flag = 0;

    while(current!=NULL && flag==0){
        if(current->id==id) flag = 1;
        else current=current->next;
    }
    return current;
}

DIR *find_director_by_name(DHD *dph, char *movie_director){
    DIR *current = NULL;
    char lower_movie_director[MAXLEN], lower_current_name[MAXLEN];
    int flag;
    strncpy(lower_movie_director, movie_director, MAXLEN);
    to_lowercase(lower_movie_director);
    current = dph->first;
    flag = 0;

    while (current!= NULL && flag == 0) {
        strncpy(lower_current_name, current->name, MAXLEN);
        to_lowercase(lower_current_name);
        if (strcmp(lower_current_name, lower_movie_director) == 0) flag = 1;
        else current = current->next;
    }
    return current;
}

MOV *create_movie(DHD *dph, int id_mov, char *movie_name, int id_dir, int movie_year, int movie_duration, float movie_kpr, float movie_plr, int watch_date[3]){
    MOV *new_movie = NULL; /* Pointer to new node */
    DIR *movie_director = NULL; /* Pointer to director of movie */
    char *name = NULL;

    new_movie = (MOV*)malloc(sizeof(MOV));
    name = (char*)malloc((strlen(movie_name) + 1) * sizeof(char));
    movie_director = (DIR *)malloc(sizeof(DIR));

    movie_director=find_director_by_id(dph, id_dir);

    new_movie->id = id_mov;
    new_movie->name = name;
    new_movie->director = movie_director;
    new_movie->year = movie_year;
    new_movie->duration = movie_duration;
    new_movie->kpr = movie_kpr;
    new_movie->plr = movie_plr;
    memcpy(name, movie_name, strlen(movie_name) + 1);
    memcpy(new_movie->date, watch_date, sizeof(new_movie->date));
    new_movie->next = NULL;
    new_movie->prev = NULL;
    return new_movie; /* Return address of node */
}

void add_movie(MHD *mph, MOV *new_node, MOV *current_node){
    if (mph && new_node){
        if (current_node == NULL) {     /* Add first node of list */
            mph->cnt = 1;
            mph->first = new_node;
            mph->last = new_node;
            new_node->prev = NULL;
            new_node->next = NULL;
        } else {
            mph->cnt++;
            current_node->next = new_node;
            new_node->prev = current_node;
            new_node->next = NULL;
            mph->last = new_node;
        }
    } else printf("\nError! The head of movie list is missing\n");
}

DIR *create_director(int id, char *movie_director){
    DIR *new_director = NULL; /* Pointer to new node */
    char *director = NULL;

    new_director = (DIR*)malloc(sizeof(DIR));
    director = (char*)malloc((strlen(movie_director) + 1) * sizeof(char));

    new_director->id = id;
    new_director->name = director;
    memcpy(director, movie_director, strlen(movie_director) + 1);
    new_director->prev = NULL;
    new_director->next = NULL;
    return new_director; /* Return address of node */
}

void add_director(DHD *dph, DIR *new_node, DIR *current_node){
    if (dph && new_node){
        if (current_node == NULL) { /* Add first node of list */
            dph->cnt = 1;
            dph->first = new_node;
            dph->last = new_node;
            new_node->prev = NULL;
            new_node->next = NULL;
        } else {
            dph->cnt++;
            current_node->next = new_node;
            new_node->prev = current_node;
            new_node->next = NULL;
            dph->last = new_node;
        }
    } else printf("\nError! The head of director list is missing\n");
}

/* Supporting ----------------------------------------------------------------------------------------------------- */

void clear_input_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void to_lowercase(char *str){
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void rewrite_file(MHD *mph, DHD *dph){
    MOV *mov_current;
    DIR *dir_current;
    FILE *file1 = fopen(movie_db, "w");
    FILE *file2 = fopen(director_db, "w");
    mov_current = mph->first;
    dir_current = dph->first;

    while(mov_current!=NULL){
        fprintf(file1, "%d;%s;%d;%d;%d;%.1f;%.1f;%d;%d;%d\n", mov_current->id, mov_current->name, mov_current->director->id, mov_current->year, mov_current->duration, mov_current->kpr, mov_current->plr, mov_current->date[0], mov_current->date[1], mov_current->date[2]);
        mov_current=mov_current->next;
    }
    while(dir_current!=NULL){
        fprintf(file2, "%d;%s\n", dir_current->id, dir_current->name);
        dir_current=dir_current->next;
    }
    fclose(file1);
    fclose(file2);
}

void split_string(char *input_string, char **words, int *word_count, char sep) {
    int word_index, word_start, word_length, in_word, i, len;
    len = strlen(input_string);
    word_index = 0;
    word_start = 0;
    word_length = 0;
    in_word = 0;

    for (i = 0; i <= len; i++) {
        if ((input_string[i] == sep || input_string[i] == '\0')&&(in_word==1)) {
            words[word_index] = (char *)malloc(word_length + 1);
            strncpy(words[word_index], input_string + word_start, word_length);
            words[word_index][word_length] = '\0';
            word_index++;
            in_word = 0;
        } else {
            if (in_word==0) {
                word_start = i;
                word_length = 1;
                in_word = 1;
            } else word_length++;
        }
    }
    *word_count = word_index;
    /* For correct work on OS MAC and OS LINUX */
    for (i = 0; i < *word_count; i++) {
        len = strlen(words[i]);
        if (words[i][len - 1] == '\r') words[i][len - 1] = '\0';
    }
}

/* Fill the lists and Create their elements ----------------------------------------------------------------------- */

void add_movie_from_file(MHD *mph, DHD *dph, char *filename, char sep){
    MOV *new_movie;
    char line[MAXLEN], *words[10];  /* [10] - count of columns in filename and that card-file */
    int word_count, date[3], i;
    FILE *file = fopen(filename, "r");

    if (file == NULL) printf("Error opening file.\n");
    else {
        while (fgets(line, sizeof(line), file)){
            line[strcspn(line, "\n")] = 0;
            split_string(line, words, &word_count, sep);

            if (word_count == 10){
                int id_mov = atoi(words[0]);
                char *name = words[1];
                int id_dir = atoi(words[2]);
                int year = atoi(words[3]);
                int duration = atoi(words[4]);
                float kpr = atof(words[5]);
                float plr = atof(words[6]);
                date[0] = atoi(words[7]);
                date[1] = atoi(words[8]);
                date[2] = atoi(words[9]);

                new_movie = create_movie(dph, id_mov, name, id_dir, year, duration, kpr, plr, date);
                add_movie(mph, new_movie, mph->last);
            }
            else printf("Invalid number of attributes in line: %s\n", line);

            for (i = 0; i < word_count; i++){
                free(words[i]);
            }
        }
        fclose(file);
    }
}

void add_movie_to_list(MHD *mph, DHD *dph){
    MOV *new_movie = NULL;
    DIR *current = NULL;
    char name[MAXLEN],director[MAXLEN];
    float kpr, plr;
    int id_dir, year, duration, date[3];

    printf("\nID of new movie set automatically on %d\n", mph->cnt);
    printf("Enter the name of movie: ");
    new_gets(name,MAXLEN);
    printf("Enter the director of movie: ");
    new_gets(director,MAXLEN);
    year = enter_integer("Enter the year of movie: ", 1985, 2099);
    duration = enter_integer("Enter the duration of movie: ", 1, 999);
    kpr = enter_float("Enter the KPR of movie: ", 0, 10);
    plr = enter_float("Enter the PLR of movie: ", 0, 10);
    enter_date("Enter the date of viewing (DD MM YYYY): ", &date[0],&date[1],&date[2]);

    current = find_director_by_name(dph,director);
    if(current == NULL) {
        id_dir = dph->cnt;
        current = create_director(id_dir, director);
        add_director(dph, current, dph->last);
    }
    else id_dir = current->id;

    new_movie = create_movie(dph, mph->cnt, name, id_dir, year, duration, kpr, plr, date);
    add_movie(mph, new_movie, mph->last);
    printf("\nMovie added successfully\n");
}

void add_director_from_file(DHD *dph, char *filename, char sep){
    DIR *new_director;
    char line[MAXLEN], *words[2];   /* [2] - count of columns in filename and that card-file */
    int wordCount, i;
    FILE *file = fopen(filename, "r");

    if (file == NULL) printf("Error opening file.\n");
    else {
        while (fgets(line, sizeof(line), file)){
            line[strcspn(line, "\n")] = 0;
            split_string(line, words, &wordCount, sep);
            if (wordCount == 2){
                int id = atoi(words[0]);
                char *name = words[1];

                new_director = create_director(id, name);
                add_director(dph, new_director, dph->last);
            }
            else printf("Invalid number of attributes in line: %s\n", line);

            for (i = 0; i < wordCount; i++){
                free(words[i]);
            }
        }
        fclose(file);
    }
}

void add_director_to_list(DHD *dph){
    DIR *new_director = NULL;
    char name[MAXLEN];

    printf("\nID of new director set automatically on %d\n", dph->cnt);
    printf("Enter the name of director: ");
    new_gets(name, MAXLEN);
    new_director = find_director_by_name(dph, name);
    if(new_director==NULL){
        new_director = create_director(dph->cnt, name);
        add_director(dph, new_director, dph->last);
        printf("\nDirector added successfully\n");
    } else printf("\nThe director you entered is already on the list\n");
}

/* Find in movie -------------------------------------------------------------------------------------------------- */

void find_number_in_movie(MHD *mph, char *message, int a, int b, int i){
    MOV *current;
    float record, number;
    int count;
    current = mph->first;
    count = 0;

    if (i<3) number=enter_integer(message, a, b);
    else number=enter_float(message, a, b);
    puts("");

    while(current!=NULL){
        switch(i){
        case 0: record = current->id; break;
        case 1: record = current->year; break;
        case 2: record = current->duration; break;
        case 3: record = current->kpr; break;
        case 4: record = current->plr; break;
        }
        if(record == number){
            count++;
            if (count==1) print_head(0);
            printf("| %2d | %25s | %25s | %d | %3d | %5.1f | %5.1f | %.2d.%.2d.%d |\n", current->id, current->name, current->director->name, current->year, current->duration, current->kpr, current->plr, current->date[0], current->date[1], current->date[2]);
        }
        current = current->next;
    }
    if (count==0) printf("Movie with the entered value was not found\n");
    else print_tail(0);
}

void find_string_in_movie(MHD *mph, char *message, int i){
    MOV *current;
    char input[MAXLEN], *string, lstring[MAXLEN];
    int count, result;
    printf(message);
    new_gets(input, MAXLEN); /* Read the string */
    to_lowercase(input);
    current = mph->first;
    count = 0;

    puts("");

    while(current!=NULL){
        switch(i){
        case 0: string = current->name; break;
        case 1: string = current->director->name; break;
        }
        strcpy(lstring, string);
        to_lowercase(lstring);
        result = strncmp(input, lstring, strlen(input));
        if(result == 0 || strstr(lstring, input) != NULL) {
            count++;
            if (count==1) print_head(0);
            printf("| %2d | %25s | %25s | %d | %3d | %5.1f | %5.1f | %.2d.%.2d.%d |\n", current->id, current->name, current->director->name, current->year, current->duration, current->kpr, current->plr, current->date[0], current->date[1], current->date[2]);
        }
        current = current->next;
    }
    if (count==0) printf("Movie with the entered String (or Substring) was not found\n");
    else print_tail(0);
}

void find_date_in_movie(MHD *mph){
    MOV *current;
    int date[3], count;
    current = mph->first;
    count=0;

    enter_date("Enter the date of viewing (DD MM YYYY): ", &date[0], &date[1], &date[2]);
    puts("");

    while (current != NULL) {
        if (current->date[0] == date[0] && current->date[1] == date[1] && current->date[2] == date[2]) {
            count++;
            if (count == 1) print_head(0);
            printf("| %2d | %25s | %25s | %d | %3d | %5.1f | %5.1f | %.2d.%.2d.%d |\n",
                   current->id, current->name, current->director->name, current->year,
                   current->duration, current->kpr, current->plr, current->date[0],
                   current->date[1], current->date[2]);
        }
        current = current->next;
    }

    if (count == 0) printf("Movie with the entered Date was not found\n");
    else print_tail(0);
}

void find_in_movie(MHD *mph){
    int option;

    if(mph->first!=NULL){
        option=movie_by_what(1);
        puts("");
        switch (option) {
            case 0: find_number_in_movie(mph, "Enter the ID: ", 0, mph->cnt, 0); break;
            case 1: find_string_in_movie(mph, "Enter the Name of Movie (or Substring): ", 0); break;
            case 2: find_string_in_movie(mph, "Enter the Name of Director (or Substring): ", 1); break;
            case 3: find_number_in_movie(mph, "Enter the Year: ", 1895, 2099, 1); break;
            case 4: find_number_in_movie(mph, "Enter the Duration: ", 1, 999, 2); break;
            case 5: find_number_in_movie(mph, "Enter the KPR: ", 0, 10, 3); break;
            case 6: find_number_in_movie(mph, "Enter the PLR: ", 0, 10, 4); break;
            case 7: find_date_in_movie(mph); break;
        }
    } else printf("\nThe movie list is empty\n");
}

/* Find in director ----------------------------------------------------------------------------------------------- */

void find_id_in_director(DHD *dph){
    DIR *current;
    int id, count;
    current = dph->first;
    count = 0;

    puts("");
    id = enter_integer("Enter the ID: ", 0, dph->cnt);
    puts("");
    while(current!=NULL){
        if (current->id == id){
            count++;
            if (count==1) print_head(1);
            printf("| %2d | %25s |\n", current->id, current->name);
        }
        current = current->next;
    }
    if (count==0) printf("Director with the entered ID was not found\n");
    else print_tail(1);
}

void find_name_in_director(DHD *dph){
    DIR *current;
    char input[MAXLEN], lstring[MAXLEN];
    int count, result;
    printf("\nEnter the Name (or Substring): ");
    new_gets(input, MAXLEN); /* Read the string */
    to_lowercase(input);
    current = dph->first;
    count = 0;

    puts("");

    while (current != NULL) {
        strcpy(lstring, current->name);
        to_lowercase(lstring);
        result = strncmp(input, lstring, strlen(input));
        if (result == 0 || strstr(lstring, input) != NULL) {
            count++;
            if (count == 1) print_head(1);
            printf("| %2d | %25s |\n", current->id, current->name);
        }
        current = current->next;
    }

    if (count == 0) printf("Director with the entered Name (or Substring) was not found\n");
    else print_tail(1);
}

void find_in_director(DHD *dph){
    int option;

    if(dph->first!=NULL){
        option=director_by_what(1);
        if(option==0) find_id_in_director(dph);
        else find_name_in_director(dph);
    } else printf("\nThe director list is empty\n");
}

/* Edit Element of the List --------------------------------------------------------------------------------------- */

void edit_in_movie(MHD *mph, DHD *dph){
    MOV *current;
    DIR *temp_dir;
    char input[MAXLEN];
    float kpr, plr;
    int option, mov_id, dir_id, year, duration, date[2], i, flag;
    flag=0;

    if(mph->first!=NULL){
        option=movie_by_what(0);
        if(option!=0){
            current=mph->first;
            output_movie(mph);

            mov_id = enter_integer("Enter the ID of the record you want to edit: ", 0, (mph->cnt)-1);
            while(current!=NULL && flag == 0){
                if(current->id == mov_id) flag = 1;
                else current = current->next;
            }
            if (current!=NULL){
                puts("");
                switch (option) {
                    case 1:
                        printf("Enter the new name of Movie: ");
                        new_gets(input,MAXLEN);
                        strcpy(current->name, input);
                        break;
                    case 2:
                        output_director(dph);
                        dir_id=enter_integer("Enter the new director ID of Movie: ", 0, (dph->cnt)-1);
                        temp_dir = find_director_by_id(dph, dir_id);
                        if(temp_dir!=NULL) current->director = temp_dir;
                        else printf("Director with entered ID was not found. Keeping the previous director\n");
                        break;
                    case 3:
                        year = enter_integer("Enter the new Year of Movie: ", 1895, 2099);
                        current->year = year;
                        break;
                    case 4:
                        duration = enter_integer("Enter the new Duration of Movie: ", 1, 999);
                        current->duration = duration;
                        break;
                    case 5:
                        kpr = enter_float("Enter the new KPR of Movie: ", 0, 10);
                        current->kpr = kpr;
                        break;
                    case 6:
                        plr = enter_float("Enter the new PLR of Movie: ", 0, 10);
                        current->plr = plr;
                        break;
                    case 7:
                        enter_date("Enter the new date of viewing (DD MM YYYY): ", &date[0],&date[1],&date[2]);
                        for(i=0; i<3; i++) current->date[i] = date[i];
                        break;
                }
                printf("\nThe data updated successfully\n");
            } else printf("\nMovie with entered ID was not found\n");
        }
    } else printf("\nThe movie list is empty\n");
}

void edit_in_director(DHD *dph){
    DIR *current = NULL;
    char input[MAXLEN];
    int option, dir_id, flag;

    if(dph->first!=NULL){
        option = director_by_what(0);
        if (option!=0){
            puts("");
            output_director(dph);
            dir_id = enter_integer("Enter the ID of the record you want to edit: ", 0, (dph->cnt)-1);
            current = dph->first;
            flag=0;
            while(current!=NULL && flag == 0){
                if(current->id == dir_id) flag = 1;
                else current = current->next;
            }
            if(current!=NULL){
                printf("Enter the new name Name of Director: ");
                new_gets(input, MAXLEN);
                strcpy(current->name, input);
            } else printf("\nDirector with entered ID was not found\n");
        }
    } else printf("\nThe director list is empty\n");
}

/* Delete Element of the List ------------------------------------------------------------------------------------- */

void delete_movie(MHD *mph, MOV *current_movie) {
    if (current_movie == mph->first) { /* If deleted node is the first in the list */
        mph->first = current_movie->next;
        if (mph->first) {
            mph->first->prev = NULL;
        } else { /* If deleted node is alone in the list */
            mph->last = NULL;
        }
    } else if (current_movie == mph->last) { /* If deleted node is the last in the list */
        mph->last = current_movie->prev;
        if (mph->last) {
            mph->last->next = NULL;
        } else { /* If deleted node is alone in the list */
            mph->first = NULL;
        }
    } else { /* If deleted node not first or last of the list */
        current_movie->prev->next = current_movie->next;
        current_movie->next->prev = current_movie->prev;
    }
    current_movie->next = NULL;
    current_movie->prev = NULL;
    free(current_movie);
}

void delete_in_movie(MHD *mph){
    MOV *current = NULL;
    int id_mov, flag;

    if(mph->first!=NULL){
        current=mph->first;
        flag = 0;

        puts("");
        id_mov = enter_integer("Enter the ID of the movie you want to delete: ", 0, (mph->cnt)-1);
        while(current!=NULL && flag==0){
            if(current->id == id_mov) flag=1;
            else current = current->next;
        }

        if(current==NULL) printf("\nMovie with entered ID was not found\n");
        else {delete_movie(mph, current); printf("\nMovie with entered ID was deleted\n");}
    } else printf("\nThe movie list is empty\n");
}

void delete_director(MHD *mph, DHD *dph, DIR *current_director){
    MOV *next_movie = NULL;
    MOV *current_movie = mph->first;

    while (current_movie != NULL) {
        next_movie = current_movie->next;
        if (current_movie->director == current_director) {
            delete_movie(mph, current_movie);
        }
        current_movie = next_movie;
    }

    if (current_director == dph->first) {
        dph->first = current_director->next;
        if (dph->first) {
            dph->first->prev = NULL;
        } else dph->last = NULL;
    } else if (current_director== dph->last) {
        dph->last = current_director->prev;
        if (dph->last) {
            dph->last->next = NULL;
        } else dph->first = NULL;
    } else {
        current_director->prev->next = current_director->next;
        current_director->next->prev = current_director->prev;
    }

    current_director->next = NULL;
    current_director->prev = NULL;

    free(current_director);
}

void delete_in_director(MHD *mph, DHD *dph){
    DIR *current = NULL;
    int id_dir;

    if(dph->first!=NULL){
        puts("");
        id_dir = enter_integer("Enter the ID of the director you want to delete: ", 0, (dph->cnt)-1);
        current=find_director_by_id(dph, id_dir);
        if(current != NULL) {delete_director(mph, dph, current); printf("\nDirector with entered ID was deleted\n");}
        else printf("\nDirector with entered ID was not found\n");
    } else printf("\nThe director list is empty\n");
}

/* Sort the Movie List -------------------------------------------------------------------------------------------- */

int swap_movie(MHD *mph, MOV *current){
    MOV *temp = current->next;

    if (current->prev != NULL) current->prev->next = temp;
    else mph->first = temp;

    if (temp->next != NULL) temp->next->prev = current;
    else mph->last = current;

    current->next = temp->next;
    temp->prev = current->prev;
    temp->next = current;
    current->prev = temp;

    return 1;
}

void sort_movie_by_number(MHD *mph, int i, int direction) {
    MOV *current = NULL;
    int swapped, need_to_swap;

    do {
        current = mph->first;
        swapped = 0;

        while (current->next != NULL) {
            need_to_swap = 0;
            switch (i) {
                case 0:
                    if ((direction == 0 && current->id > current->next->id) || (direction == 1 && current->id < current->next->id)) need_to_swap = 1;
                    break;
                case 1:
                    if ((direction == 0 && current->year > current->next->year) || (direction == 1 && current->year < current->next->year)) need_to_swap = 1;
                    break;
                case 2:
                    if ((direction == 0 && current->duration > current->next->duration) || (direction == 1 && current->duration < current->next->duration)) need_to_swap = 1;
                    break;
                case 3:
                    if ((direction == 0 && current->kpr > current->next->kpr) || (direction == 1 && current->kpr < current->next->kpr)) need_to_swap = 1;
                    break;
                case 4:
                    if ((direction == 0 && current->plr > current->next->plr) || (direction == 1 && current->plr < current->next->plr)) need_to_swap = 1;
                    break;
            }

            if (need_to_swap == 1) swapped = swap_movie(mph, current);
            else current = current->next;
        }
    } while (swapped);
}

void sort_movie_by_string(MHD *mph, int i, int direction) {
    MOV *current = NULL;
    int swapped, need_to_swap;

    do {
        swapped = 0;
        current = mph->first;

        while (current->next != NULL) {
            need_to_swap = 0;

            switch (i) {
                case 0:
                    if ((direction == 0 && strcmp(current->name, current->next->name) > 0)  || (direction == 1 && strcmp(current->name, current->next->name) < 0)) need_to_swap = 1;
                    break;
                case 1:
                    if ((direction == 0 && strcmp(current->director->name, current->next->director->name) > 0)  || (direction == 1 && strcmp(current->director->name, current->next->director->name) < 0)) need_to_swap = 1;
                    break;
            }

            if (need_to_swap == 1) swapped = swap_movie(mph, current);
            else current = current->next;
        }
    } while (swapped);
}

void sort_movie_by_date(MHD *mph, int direction) {
    MOV *current = NULL;
    int swapped, need_to_swap;

    do {
        swapped = 0;
        current = mph->first;

        while (current->next != NULL) {
            need_to_swap = 0;

            if ((direction == 0 && current->date[2] > current->next->date[2]) || (direction == 1 && current->date[2] < current->next->date[2])) need_to_swap = 1;
            else if (current->date[2] == current->next->date[2]) {
                if ((direction == 0 && current->date[1] > current->next->date[1]) || (direction == 1 && current->date[1] < current->next->date[1])) need_to_swap = 1;
                else if (current->date[1] == current->next->date[1]) {
                    if ((direction == 0 && current->date[0] > current->next->date[0]) || (direction == 1 && current->date[0] < current->next->date[0])) need_to_swap = 1;
                }
            }

            if (need_to_swap == 1) swapped = swap_movie(mph, current);
            else current = current->next;
        }
    } while (swapped);
}

void sort_in_movie(MHD *mph){
    int option, direction;

    if(mph->first!=NULL){
        option = movie_by_what(1);
        puts("");
        direction = enter_integer("Enter the direction of sort (0 - Forward / 1 - Backward): ", 0, 1);
        switch(option){
        case 0: sort_movie_by_number(mph, 0, direction); break;
        case 1: sort_movie_by_string(mph, 0, direction); break;
        case 2: sort_movie_by_string(mph, 1, direction); break;
        case 3: sort_movie_by_number(mph, 1, direction); break;
        case 4: sort_movie_by_number(mph, 2, direction); break;
        case 5: sort_movie_by_number(mph, 3, direction); break;
        case 6: sort_movie_by_number(mph, 4, direction); break;
        case 7: sort_movie_by_date(mph, direction); break;
        }
        printf("\nThe movie list was sorted\n");
    } else printf("\nThe movie list is empty\n");
}

/* Sort the Director List ----------------------------------------------------------------------------------------- */

int swap_director(DHD *dph, DIR *current) {
    DIR *temp = current->next;

    if (current->prev != NULL) current->prev->next = temp;
    else dph->first = temp;

    if (temp->next != NULL) temp->next->prev = current;
    else dph->last = current;

    current->next = temp->next;
    temp->prev = current->prev;
    temp->next = current;
    current->prev = temp;

    return 1;
}

void sort_director_by_id(DHD *dph, int direction){
    DIR *current = NULL;
    int swapped;

    do {
        swapped = 0;
        current = dph->first;

        while (current->next != NULL) {
            if ((direction == 0 && current->id > current->next->id) ||  (direction == 1 && current->id < current->next->id)) swapped = swap_director(dph, current);
            else current = current->next;
        }
    } while (swapped);
}

void sort_director_by_name(DHD *dph, int direction){
    DIR *current = NULL;
    int swapped;

    do {
        swapped = 0;
        current = dph->first;

        while (current->next != NULL) {
            if ((direction == 0 && strcmp(current->name, current->next->name) > 0) ||  (direction == 1 && strcmp(current->name, current->next->name) < 0)) swapped = swap_director(dph, current);
            else current = current->next;
        }
    } while (swapped);
}

void sort_in_director(DHD *dph){
    int option, direction;

    if(dph->first!=NULL){
        option = director_by_what(1);
        puts("");
        direction = enter_integer("Enter the direction of sort (0 - Forward / 1 - Backward): ", 0, 1);
        if(option==0) sort_director_by_id(dph, direction);
        else sort_director_by_name(dph, direction);
        printf("\nThe director list was sorted\n");
    } else printf("\nThe director list is empty\n");
}
