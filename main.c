#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct station {
    int distance;
    int maxNumberOfCars;
    int cars[512];
    struct station *right;
    struct station *left;
    struct station *father;
};
typedef struct station Station;
Station nullStation = {-1, 0, {0}, NULL, NULL, NULL};

Station *createStation(int distanceToInsert, int numberOfCarsToInsert, int carsToInsert[]) {
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->maxNumberOfCars = numberOfCarsToInsert;
    newStation->left = &nullStation;
    newStation->right = &nullStation;
    for (int i = 0; i < numberOfCarsToInsert; i++) {
        newStation->cars[i] = carsToInsert[i];
    }
    return newStation;
}

void insertNewStation(Station **firstStation, Station *stationToInsert) {
    struct station *currentStation = &nullStation;
    struct station *rootStation = *firstStation;

    while (rootStation != &nullStation) {
        currentStation = rootStation;
        if (stationToInsert->distance < rootStation->distance) {
            rootStation = rootStation->left;
        } else rootStation = rootStation->right;
    }
    if (currentStation == &nullStation) {
        *firstStation = stationToInsert;

    } else if (stationToInsert->distance < currentStation->distance) {
        currentStation->left = stationToInsert;
    } else currentStation->right = stationToInsert;
}

void inorderVisitToTheHighway(Station *firstStation) {
    if (firstStation != &nullStation) {
        inorderVisitToTheHighway(firstStation->left);
        printf("distance: %d, right: %d, left: %d\n", firstStation->distance, firstStation->right->distance,
               firstStation->left->distance);
        printf("cars: ");
        for (int i = 0; i < firstStation->maxNumberOfCars; i++) {
            printf("%d ", firstStation->cars[i]);
        }
        printf("\n");
        inorderVisitToTheHighway(firstStation->right);
    }
}

void clearUp(Station *firstStation) {
    if (firstStation != &nullStation) {
        clearUp(firstStation->left);
        clearUp(firstStation->right);
        free(firstStation);

    }
}

Station *minimumStation(Station *currentStation) {
    while (currentStation->left != &nullStation) {
        currentStation = currentStation->left;
    }
    return currentStation;
}

void removeStation(Station **firstStation, int distanceToRemove) {
    if (distanceToRemove < (*firstStation)->distance) {
        removeStation(&((*firstStation)->left), distanceToRemove);
    } else if (distanceToRemove > (*firstStation)->distance) {
        removeStation(&((*firstStation)->right), distanceToRemove);
    } else {
        if ((*firstStation)->left == &nullStation) {
            Station *currentStation = (*firstStation)->right;
            free(*firstStation);
            *firstStation = currentStation;
        } else if ((*firstStation)->right == &nullStation) {
            Station *currentStation = (*firstStation)->left;
            free(*firstStation);
            *firstStation = currentStation;
        } else {
            Station *currentStation = minimumStation((*firstStation)->right);
            (*firstStation)->distance = currentStation->distance;
            removeStation(&((*firstStation)->right), currentStation->distance);
        }
    }
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void addStation(Station **firstStation) {
    int numberOfCars;
    int insertingDistance;

    scanf("%d", &insertingDistance);
    scanf("%d", &numberOfCars);
    int car[numberOfCars];
    for (int i = 0; i < numberOfCars; i++) {
        scanf("%d", &car[i]);
    }
    Station *station = createStation(insertingDistance, numberOfCars, car);
    insertNewStation(firstStation, station);
}

void removeStationAtDistance(Station **firstStation) {
    int removingDistance;
    scanf("%d", &removingDistance);
    removeStation(firstStation, removingDistance);
}

void addAutoAtDistance(int distanceOfTheStation, int carToInsert) {
    //TODO
}

void removeAutoAtDistance(int distanceOfTheStation, int autoToRemove) {
    //TODO
}

void planTheTrip(int distanceOfLeaving, int distanceOfArrival) {

}


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {

    Station *highway = &nullStation;
    /*   int vettore1[5] = {1, 1, 1, 1, 1};
       int vettore2[7] = {2, 2, 2, 2, 2, 2, 2};
       int vettore3[3] = {3, 3, 3};

       Station *station1 = createStation(10, 5, vettore1);
       Station *station2 = createStation(5, 7, vettore2);
       Station *station3 = createStation(34, 3, vettore3);

       insertNewStation(&highway, station1);
       insertNewStation(&highway, station2);
       insertNewStation(&highway, station3);

       inorderVisitToTheHighway(highway);

       removeStation(&highway, station1->distance);
       inorderVisitToTheHighway(highway);
       */
    char command[20];
    scanf("%s", command);
        if (strcmp(command, "aggiungi-stazione") == 0) addStation(&highway);
        else if (strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance(&highway);
    fflush(stdin);
    scanf("%s", command);
    if (strcmp(command, "aggiungi-stazione") == 0) addStation(&highway);
    else if (strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance(&highway);
    fflush(stdin);
    scanf("%s", command);
    if (strcmp(command, "aggiungi-stazione") == 0) addStation(&highway);
    else if (strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance(&highway);
    fflush(stdin);

    inorderVisitToTheHighway(highway);
    clearUp(highway);

/*
   while(feof(stdin)==false){
    scanf("%s", command);
    if(strcmp(command, "aggiungi-stazione") == 0) addStation(&highway);
    else if(strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance(&highway);
    else if(strcmp(command, "aggiungi-auto") == 0) addAutoAdDistance();
    else if(strcmp(command, "rottama-auto") == 0) removeAutoAtDistance();
    else if(strcmp(command, "pianifica-percorso") == 0)
}
 */

}

/**
 * USARE LA SCANF RIPETUTA PER LEGGERE PIU PAROLE NELLA STESSA RIGA
 * AD ESEMPIO:
 * CIAO COME STAI
 * SCANF("%s"); PER LEGGERE CIAO
 * SCANF("%s"); PER LEGGERE COME
 * SCANF("%s"); PER LEGGERE STAI
 * ANCHE SE LE METTO UNA A CAPO DELL'ALTRA LA SCNF LEGGE OGNI PAROLA SULLA STESSA RIGA PERCHE SPOSTA IL PUNTATORE ALL STDIN E AL BUFFER DI LETTURA
 * ALGORITMO EFFICENTE DI POZZI
 * MI SALVO LA MACCHINA CON MASSIMA AUTONOMIA IN TUTto L'ALBERO(autmax) partendo dalla fine all'inizio mi fermo ad ogni stazione econtrollo se la distanza + autmax raggiunge la staione finale, se si controllo allora
 * che anche la distanza della stazione corrente + l'autonomia massima della macchina nella stazione attuale ci arrivi, se entrambi i vincoli sono soddisfatti allora la segno come check point temporaneo,
 * ricorsivamente ripeto con il precedente finche non trovo un nodo dove una delle 2 non è rispetttata, il nodo ultimo in cui è rispettatan è la stazione da stampare a video
 * ATTENZIONE FUNZIONA SOLO IN UN VERSO; USARE DIJKSTRA PER IL SECODNO VERSO
 */
