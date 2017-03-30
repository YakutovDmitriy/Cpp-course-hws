#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
    int id;
    char *number;
    char *name;
} contact;

char *filename;

char *read_string(FILE *in) {
    char c;
    int sz = 1;
    char *ans;
    ans = (char *) malloc(sizeof(char));
    do {
        c = fgetc(in);
    } while(c == EOF || c == ' ' || c == '\n' || c == '\0');
    while (c != EOF && c != ' ' && c != '\n' && c != '\0') {
        ans = (char *) realloc(ans, (++sz) * sizeof(char));
        ans[sz - 2] = c;
        c = fgetc(in);
    }
    ans[sz - 1] = '\0';
    return ans;
}


void read_contact(FILE *in, contact *now) {
    //printf("%s ", "read_con_started");
    fscanf(in, "%d", &(now->id));
    //printf("%d ", now->id);
    char *name = read_string(in);
    //printf("%s ", name);
    char *number = read_string(in);
    //printf("%s\n", number);
    now->name = (char *) malloc(strlen(name) + 1);
    now->number = (char *) malloc(strlen(number) + 1);
    strcpy(now->name, name);
    strcpy(now->number, number);
    free(name);
    free(number);
}

void print_contact(contact *a) {
    printf("%d %s %s", a->id, a->name, a->number);
    fflush(stdout);
}

bool substring(char *str, char *substr) {
    if(strlen(substr) == 0) return true;
    for(int i = 0; i < strlen(str); i++) {
        int j = 0;
        while(i + j < strlen(str) && tolower(str[i + j]) == tolower(substr[j])) {
            j++;
            if(j == strlen(substr)) return true;
        }
    }
    return false;
}

bool cmp(char *s, char *t) {
    int cnt1 = 0, cnt2 = 0;
    for(int i = 0; i < strlen(s); i++)
        if(s[i] >= '0' && s[i] <= '9') cnt1++;
    for(int i = 0; i < strlen(t); i++)
        if(t[i] >= '0' && t[i] <= '9') cnt2++;
    if(cnt1 != cnt2) return false;
    char *ss = (char *) malloc((cnt1 + 1) * sizeof(char));
    char *tt = (char *) malloc((cnt2 + 1) * sizeof(char));
    int id1 = 0, id2 = 0;
    for(int i = 0; i < strlen(s); i++) {
        if(s[i] >= '0' && s[i] <= '9') ss[id1++] = s[i];
    }
    for(int i = 0; i < strlen(t); i++) {
        if(t[i] >= '0' && t[i] <= '9') tt[id2++] = t[i];
    }
    ss[id1] = '\0';
    tt[id2] = '\0';
    for(int i = 0; i < cnt1; i++) {
        if(ss[i] != tt[i]) {
            free(ss);
            free(tt);
            return false;
        }
    }
    free(ss);
    free(tt);
    return true;
}

void find(char *str) {
    FILE *f;
    f = fopen(filename, "r");
    while(!feof(f)) {
        contact *now = (contact *) malloc(sizeof(contact));
        read_contact(f, now);
        if(substring(now->name, str) || cmp(now->number, str)) {
            print_contact(now);
            printf("\n");
        }
        free(now);
    }
    fclose(f);
}


int count() {
    FILE *f;
    int ans = 0;
    f = fopen(filename, "r");
    while(!feof(f)) {
        contact *now = (contact *) malloc(sizeof(contact));
        read_contact(f, now);
        free(now->name);
        free(now->number);
        free(now);
        ans++;
    }
    fclose(f);
    return ans;
}

void create() {
    //printf("%s\n", "creating");
    contact *new_con = (contact *) malloc(sizeof(contact));
    char *name = read_string(stdin);
    char *number = read_string(stdin);
    new_con->name = (char *) malloc(strlen(name) + 1);
    new_con->number = (char *) malloc(strlen(number) + 1);
    strcpy(new_con->name, name);
    strcpy(new_con->number, number);
    //printf("%s\n", new_con->name);
    //printf("%s\n", new_con->number);
    free(name);
    free(number);
    FILE *f;
    int id = 0;
    f = fopen(filename, "r");
    if(f == 0) {
        //fclose(f);
        f = fopen(filename, "w");
        fprintf(f, "%d %s %s", 1, new_con->name, new_con->number);
    } else {
        //printf("%s\n", "contacts:");
        while(!feof(f)) {
            contact *now = (contact *) malloc(sizeof(contact));
            read_contact(f, now);
            if(id < now->id)
                id = now->id;
            free(now->number);
            free(now->name);
            free(now);
        }
        fclose(f);
        f = fopen(filename, "a");
        fprintf(f, "\n%d %s %s", id + 1, new_con->name, new_con->number);
    }
    fclose(f);
    //printf("%s\n", "created");
    free(new_con->number);
    free(new_con->name);
    free(new_con);
}


void delete(int id) {
    int cnt = count();
    FILE *f;
    f = fopen(filename, "r");
    contact **contacts = (contact **) malloc(cnt * sizeof(contact *));
    int was = 0;
    for(int i = 0; i < cnt; i++) {
        contacts[i] = (contact *) malloc(sizeof(contact));
        read_contact(f, contacts[i - was]);
        if(id == contacts[i - was]->id)
            was = 1;
    }
    fclose(f);
    f = fopen(filename, "w");
    for(int i = 0; i < cnt - was; i++) {
        print_contact(contacts[i]);
        if(i != cnt - 1 - was)
            printf("\n");
        free(contacts[i]->name);
        free(contacts[i]->number);
        free(contacts[i]);
    }
    free(contacts);
    fclose(f);
}


void change_number(int id, char *number) {
    int cnt = count();
    FILE *f;
    f = fopen(filename, "r");
    contact **contacts = (contact **) malloc(cnt * sizeof(contact *));
    for(int i = 0; i < cnt; i++) {
        contacts[i] = (contact *) malloc(sizeof(contact));
        read_contact(f, contacts[i]);
    }
    fclose(f);
    f = fopen(filename, "w");
    for(int i = 0; i < cnt; i++) {
        if(contacts[i]->id == id) {
            contacts[i]->number = (char *) realloc(contacts[i]->number, strlen(number) + 1);
            strcpy(contacts[i]->number, number);
        }
        print_contact(contacts[i]);
        if(i != cnt - 1)
            printf("\n");
        free(contacts[i]->name);
        free(contacts[i]->number);
        free(contacts[i]);
    }
    free(contacts);
    fclose(f);
}

void change_name(int id, char *name) {
    int cnt = count();
    FILE *f;
    f = fopen(filename, "r");
    contact **contacts = (contact **) malloc(cnt * sizeof(contact *));
    for(int i = 0; i < cnt; i++) {
        contacts[i] = (contact *) malloc(sizeof(contact));
        read_contact(f, contacts[i]);
    }
    fclose(f);
    f = fopen(filename, "w");
    for(int i = 0; i < cnt; i++) {
        if(contacts[i]->id == id) {
            contacts[i]->name = (char *) realloc(contacts[i]->name, strlen(name) + 1);
            strcpy(contacts[i]->name, name);
        }
        print_contact(contacts[i]);
        if(i != cnt - 1)
            printf("\n");
        free(contacts[i]->name);
        free(contacts[i]->number);
        free(contacts[i]);
    }
    free(contacts);
    fclose(f);
}


int main(int argc, char *argv[]) {
    filename = argv[1];
    while(0 == 0) {
        //printf("%s\n", "started");
        char *now = read_string(stdin);
        //printf("%s\n", "read");
        if(strcmp(now, "find") == 0) {
            char *need = read_string(stdin);
            find(need);
            free(need);
        }
        if(strcmp(now, "create") == 0) {
            create();
        }
        if(strcmp(now, "delete") == 0) {
            int id;
            scanf("%d", &id);
            delete(id);
        }
        if(strcmp(now, "change") == 0) {
            int id;
            scanf("%d", &id);
            char *s = read_string(stdin);
            if(strcmp(s, "number") == 0) {
                change_number(id, read_string(stdin));
            }
            if(strcmp(s, "name") == 0) {
                change_name(id, read_string(stdin));
            }
            free(s);
        }
        if(strcmp(now, "exit") == 0) {
            free(now);
            break;
        }
        printf("%s\n", "done");
        free(now);
    }
    return 0;
}
