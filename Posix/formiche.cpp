#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

struct ant {
    int pos; 
    int dir; // 0 ovest, 1 est
    pthread_t thid;
};

struct cell {
    pthread_mutex_t m;
    pthread_cond_t c;
    ant* ant;
};

ant* ants;
cell* cells;
int nAnts;
int nCells;
pthread_cond_t cond;
pthread_mutex_t mutex;
bool start = false;


void init() {
    cells = new cell[nCells];

    for(int i = 0; i < nCells; i++) {
        pthread_mutex_init(&cells[i].m, NULL);
        pthread_cond_init(&cells[i].c, NULL);
        for(int j = 0; j < nAnts; j++) {
            if(ants[j].pos == i)
                cells[i].ant = &ants[j];
        }
    }
}

void move(ant& a) {

    bool locked = false, moved = false;

    if(a.pos > 0 && a.dir == 0) {
        pthread_mutex_lock(&cells[a.pos-1].m);
        locked = true;
        if(cells[a.pos-1].ant == NULL) {
            pthread_mutex_lock(&cells[a.pos].m);
            ant* tmp = cells[a.pos].ant;
            cells[a.pos].ant = NULL;
            cells[a.pos-1].ant = tmp;
            pthread_mutex_unlock(&cells[a.pos].m);
            a.pos--;
            moved = true;
            printf("Formica %d con dir %d in nuova pos %d\n", a.thid, a.dir, a.pos);
            printf("Formica %d unlock01 di pos %d\n", a.thid, a.pos);
            pthread_mutex_unlock(&cells[a.pos].m); 
        }

        else {
            a.dir = 1;
            printf("Formica %d in pos %d incontra una formica e cambia dir in %d\n", a.thid, a.pos, a.dir);
            printf("Formica %d unlock0 di pos %d\n", a.thid, a.pos-1);
            pthread_mutex_unlock(&cells[a.pos-1].m);
        }
    }

    else if(a.pos < (nCells-1) && a.dir == 1) {
        pthread_mutex_lock(&cells[a.pos+1].m);
        locked = true;
        if(cells[a.pos+1].ant == NULL) {
            pthread_mutex_lock(&cells[a.pos].m);
            ant* tmp = cells[a.pos].ant;
            cells[a.pos].ant = NULL;
            cells[a.pos+1].ant = tmp;
            pthread_mutex_unlock(&cells[a.pos].m);
            a.pos++;           
            moved = true;
            printf("Formica %d con dir %d in nuova pos %d\n", a.thid, a.dir, a.pos);
            printf("Formica %d unlock01 di pos %d\n", a.thid, a.pos);
            pthread_mutex_unlock(&cells[a.pos].m); 
        }

        else {
            a.dir = 0;
            printf("Formica %d in pos %d incontra una formica e cambia dir in %d\n", a.thid, a.pos, a.dir);
            printf("Formica %d unlock1 di pos %d\n", a.thid, a.pos+1);
            pthread_mutex_unlock(&cells[a.pos+1].m);
        }
    }

    else if(a.dir == 0) {
        a.dir = 1;
        printf("Formica %d in pos %d incontra un muro e cambia dir in %d\n", a.thid, a.pos, a.dir);
    }

    else {
        a.dir = 0;
        printf("Formica %d in pos %d incontra un muro e cambia dir in %d\n", a.thid, a.pos, a.dir);
    }
}

void* antRun(void* arg) {
    ant* me = (ant*)arg;
    pthread_mutex_lock(&mutex);

    while(!start) {
        pthread_cond_wait(&cond, &mutex);
    }

    pthread_mutex_unlock(&mutex);

    while(true) {
        move(*me);
        Sleep(1000);
    }
}

void setVariables(int numAnts, int numCells) {
    nAnts = numAnts;
    nCells = numCells;
}

void setPosDir(int index, int p, int d) {
    ants[index].pos = p;
    ants[index].dir = d; 

    printf("Formica %d: pos %d --- dir %d\n", ants[index].thid, ants[index].pos, ants[index].dir);
}

int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    setVariables(2, 5); // nAnts, nCells
    ants = new ant[nAnts];

    for(int i = 0; i < nAnts; i++) {
        pthread_create(&ants[i].thid, NULL, &antRun, &ants[i]);
        setPosDir(i, i*3, i);
    }

    init();

    start = true;

    pthread_cond_broadcast(&cond);

    for(int i = 0; i < nAnts; i++)
        pthread_join(ants[i].thid, NULL);

    return 0;
}