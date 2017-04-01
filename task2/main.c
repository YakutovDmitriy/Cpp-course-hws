#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define out(...) {printf(__VA_ARGS__); fflush(stdout);}


typedef struct
{
    int id;
    char *name;
    char *tel;
    char *act_tel;
} user_t;

user_t **users;
size_t size;
size_t cap;
char *filename;

void check_cap()
{
    out("start check cap\n");
    if (cap == 0 || size == cap)
    {
        size_t new_cap = cap == 0 ? 1 : cap * 2;
        user_t **new_users = malloc(sizeof(user_t*) * new_cap);
        memcpy(new_users, users, size * sizeof(user_t*));
        free(users);
        cap = new_cap;
        users = new_users;
    }
    out("end check cap\n");
}

char *read_string(FILE *file)
{
    out("start read string\n");
    if (feof(file))
    {
        out("return null read string\n");
        return NULL;
    }
    size_t sz = 1;
    char *a = malloc(sizeof(char));
    char c;
    do
    {
        c = fgetc(file);
        if (feof(file))
        {
            out("return null read string while reading spaces\n");
            return NULL;
        }
    }
    while (isspace(c));
    while (!feof(file) && !isspace(c))
    {
        ++sz;
        char *b = malloc(sizeof(char) * sz);
        memcpy(b, a, sz - 1);
        free(a);
        a = b;
        a[sz - 2] = c;
        c = fgetc(file);
    }
    a[sz - 1] = '\0';
    out("end read string\n");
    return a;
}

char *make_act_tel(char *tel)
{
    out("start make act tel\n");
    size_t len = 0;
    size_t i;
    for (i = 0; tel[i]; ++i)
        if (isdigit(tel[i]))
            ++len;
    char *ret = malloc(sizeof(char) * (len + 1));
    ret[len] = 0;
    len = 0;
    for (i = 0; tel[i]; ++i)
        if (isdigit(tel[i]))
            ret[len++] = tel[i];
    out("ok make act tel\n");
    return ret;
}

void read_users()
{
    out("start read users\n");
    size = 0;
    cap = 0;
    int id;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        file = fopen(filename, "a");
    if (file == NULL)
    {
        out("read users: file %s == NULL\n", filename);
        exit(0);
    }
    while (fscanf(file, "%d", &id) >= 0)
    {
        check_cap();
        users[size] = malloc(sizeof(user_t));
        users[size]->id = id;
        users[size]->name = read_string(file);
        users[size]->tel = read_string(file);
        users[size]->act_tel = make_act_tel(users[size]->tel);
        ++size;
    }
    fclose(file);
    out("end read users\n");
}

void write_users()
{
    out("start write users\n");
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        out("write users: file %s == NULL\n", filename);
        exit(0);
    }
    size_t i;
    for (i = 0; i != size; ++i)
        fprintf(file, "%d %s %s\n", users[i]->id, users[i]->name, users[i]->tel);
    fclose(file);
    out("end write users\n");
}

void free_users()
{
    out("start free users\n");
    size_t i;
    for (i = 0; i != size; ++i)
    {
        free(users[i]->name);
        free(users[i]->tel);
        free(users[i]->act_tel);
        free(users[i]);
    }
    free(users);
    cap = 0;
    size = 0;
    out("end write users\n");
}

void create_user(char *name, char *tel, char *act_tel)
{
    out("start create user\n");
    user_t *user = malloc(sizeof(user_t));
    user->id = size + 1;
    user->name = name;
    user->tel = tel;
    user->act_tel = act_tel;
    check_cap();
    users[size] = user;
    ++size;
    out("end create user\n");
}

int main(int argc, char *argv[])
{
    out("start main\n");
    assert(argc > 1);
    out("ok assert\n");
    filename = argv[1];
    while (1)
    {
        out("start iteration");
        read_users();
        char *cmd = read_string(stdin);
        if (cmd == NULL)
        {
            out("cmd is null");
            free(cmd);
            free_users();
            break;
        }
        out("cmd = %s\n", cmd);
        if (strcmp(cmd, "create") == 0)
        {
            char *name = read_string(stdin);
            char *tel = read_string(stdin);
            char *act_tel = make_act_tel(tel);
            create_user(name, tel, act_tel);
            write_users();
        }
        else if (!strcmp(cmd, "exit") == 0)
        {
            free(cmd);
            free_users();
            break;
        }
        free(cmd);
        free_users();
    }
    return 0;
}
