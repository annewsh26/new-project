#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Event {
    int eventID;
    char eventName[50];
    char location[50];
    char date[15];
    int seats;
    int upcoming;
    struct Event *prev, *next;
} Event;

Event *head = NULL;

/* ---------- Utility ---------- */

int dateToInt(const char *date) {
    int y, m, d;
    sscanf(date, "%d-%d-%d", &y, &m, &d);
    return y * 10000 + m * 100 + d;
}

Event* createEvent(int id, char name[], char loc[], char date[], int seats, int upcoming) {
    Event* e = malloc(sizeof(Event));
    e->eventID = id;
    strcpy(e->eventName, name);
    strcpy(e->location, loc);
    strcpy(e->date, date);
    e->seats = seats;
    e->upcoming = upcoming;
    e->prev = e->next = NULL;
    return e;
}

int isDuplicate(char name[], char loc[], char date[]) {
    Event *cur = head;
    while (cur) {
        if (!strcmp(cur->eventName, name) &&
            !strcmp(cur->location, loc) &&
            !strcmp(cur->date, date))
            return 1;
        cur = cur->next;
    }
    return 0;
}

/* ---------- Core Operations ---------- */

void insertEvent(int id, char name[], char loc[], char date[], int seats, int upcoming) {

    if (isDuplicate(name, loc, date)) {
        printf("ERROR|Duplicate event rejected\n");
        return;
    }

    Event *newEvent = createEvent(id, name, loc, date, seats, upcoming);

    if (!head) {
        head = newEvent;
        printf("OK|Inserted\n");
        return;
    }

    Event *cur = head, *prev = NULL;
    while (cur && dateToInt(cur->date) < dateToInt(date)) {
        prev = cur;
        cur = cur->next;
    }

    if (!prev) {
        newEvent->next = head;
        head->prev = newEvent;
        head = newEvent;
    } else {
        newEvent->next = cur;
        newEvent->prev = prev;
        prev->next = newEvent;
        if (cur) cur->prev = newEvent;
    }

    printf("OK|Inserted\n");
}

void searchEvent(int id) {
    Event *cur = head;
    while (cur) {
        if (cur->eventID == id) {
            printf("%d|%s|%s|%s|%d|%d\n",
                   cur->eventID, cur->eventName,
                   cur->location, cur->date,
                   cur->seats, cur->upcoming);
            return;
        }
        cur = cur->next;
    }
    printf("ERROR|Not Found\n");
}

void displayAll() {
    Event *cur = head;
    while (cur) {
        printf("%d|%s|%s|%s|%d|%d\n",
               cur->eventID,
               cur->eventName,
               cur->location,
               cur->date,
               cur->seats,
               cur->upcoming);
        cur = cur->next;
    }
}

/* ---------- MAIN ---------- */

int main() {
    char cmd[20];
    scanf("%s", cmd);

    if (!strcmp(cmd, "INSERT")) {
        int id, seats, upcoming;
        char name[50], loc[50], date[15];
        scanf("%d %s %s %s %d %d", &id, name, loc, date, &seats, &upcoming);
        insertEvent(id, name, loc, date, seats, upcoming);
    }
    else if (!strcmp(cmd, "SEARCH")) {
        int id;
        scanf("%d", &id);
        searchEvent(id);
    }
    else if (!strcmp(cmd, "DISPLAY_ALL")) {
        displayAll();
    }

    return 0;
}
