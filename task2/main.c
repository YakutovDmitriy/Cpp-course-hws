#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define max_size 12312

char *read_string(FILE *f)
{
    int sz = 1;
    char *a = (char*) malloc(sz);
    char c;
    do {
        c = fgetc(f);
    } while(isspace(c));
    while (!isspace(c)) {
        a = (char*) realloc(a, ++sz);
        a[sz - 2] = c;
        c = fgetc(f);
    }
    a[sz - 1] = '\0';
    return a;
}

struct user_t
{
    int id;
    char *name;
    char *number;
    char *real_number;
    struct user_t *next;
};

void make_real_number(char *number, char **real_number)
{
    if (*real_number != NULL)
        free(*real_number);
    int digits = 0;
    char *s;
    for (s = number; *s; ++s)
        if (isdigit(*s))
            ++digits;
    *real_number = (char*) malloc(digits + 1);
    int i = 0;
    for (s = number; *s; ++s)
        if (isdigit(*s))
            (*real_number)[i++] = *s;
    (*real_number)[i] = '\0';
}

void load_user(struct user_t **user, char *a)
{
    *user = (struct user_t*) malloc(sizeof(struct user_t));
    
    while (isspace(*a))
        ++a;
    
    int id = 0;
    while (!isspace(*a))
        id = 10 * id + *a++ - '0';
    (*user)->id = id;
    
    while (isspace(*a))
        ++a;

    char *b = a;
    while (!isspace(*b))
        ++b;

    (*user)->name = (char*) malloc(b - a + 1);
    memcpy((*user)->name, a, b - a);
    (*user)->name[b - a] = '\0';
    
    a = b;
    while (isspace(*a))
        ++a;
        
    b = a;
    while (!isspace(*b))
        ++b;
    
    (*user)->number = (char*) malloc(b - a + 1);
    memcpy((*user)->number, a, b - a);
    (*user)->number[b - a] = '\0';
    
    (*user)->next = NULL;
    (*user)->real_number = NULL;
    make_real_number((*user)->number, &((*user)->real_number));
}

struct user_t *first_user;
struct user_t *last_user;
int size = 0;

void load_book(char *filename)
{
    size = 0;
    FILE *file = fopen(filename, "r");
    first_user = NULL;
    last_user = NULL;
    char *a = (char*) malloc(max_size);
    while (fgets(a, max_size, file))
    {
        if (first_user == NULL)
        {
            load_user(&first_user, a);
            last_user = first_user;
        }
        else
        {
            load_user(&last_user->next, a);
            last_user = last_user->next;
        }
        ++size;
    }
    fclose(file);
    free(a);
}

void print_users(char *filename)
{
    FILE *file = fopen(filename, "w");
    
    struct user_t *user;
    for (user = first_user; user != NULL; user = user->next)
        fprintf(file, "%d %s %s\n", user->id, user->name, user->number);
    
    fclose(file);
}

int correct_name(char *name)
{
    return 1;
}

int correct_number(char *number)
{
    return 1;
}

void create()
{
    char *name = read_string(stdin);
    char *number = read_string(stdin);
    
    if (!correct_name(name) || !correct_number(number))
    {
        free(number);
        free(name);
        return;
    }
    
    struct user_t *user = (struct user_t*) malloc(sizeof(struct user_t));
    user->id = ++size;
    
    int nlen = strlen(name);
    user->name = (char*) malloc(nlen + 1);
    memcpy(user->name, name, nlen);
    user->name[nlen] = 0;
    
    nlen = strlen(number);
    user->number = (char*) malloc(nlen + 1);
    memcpy(user->number, number, nlen);
    user->number[nlen] = 0;
    
    user->next = NULL;
    user->real_number = NULL;
    make_real_number(user->number, &user->real_number);
    
    if (first_user == NULL)
    {
        first_user = user;
        last_user = user;
    }
    else
    {
        last_user->next = user;
        last_user = last_user->next;
    }
    free(number);
    free(name);
}

void free_all()
{
    struct user_t *cur = first_user;
    while (cur != NULL)
    {
        struct user_t *next = cur->next;
        free(cur->name);
        free(cur->number);
        free(cur->real_number);
        free(cur);
        cur = next;
    }
    first_user = NULL;
    last_user = NULL;
}

int main(int argc, char *argv[])
{
    while (1)
    {
        load_book(argv[1]);
        char *com = (char*) malloc(max_size);
        scanf("%s", com);
        if (!strcmp(com, "create"))
        {
            create();
            print_users(argv[1]);
        }
        free_all();
        free(com);
    }
}
    