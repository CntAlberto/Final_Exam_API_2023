#include <stdio.h>
#include <stdlib.h>

#define maxNumberOfCars 512

struct station {
    int distance;
    int cars[maxNumberOfCars];
    struct station *right;
    struct station *left;
    struct station *father;
};
typedef struct station Station;
Station nullStation = {-1, {0}, NULL, NULL, NULL};

Station *createStation(int distanceToInsert) {
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->left = &nullStation;
    newStation->right = &nullStation;
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
        printf("\nstation: %d ", firstStation->distance);
        printf("left sister: %d ", firstStation->left->distance);
        printf("right sister: %d ", firstStation->right->distance);
        inorderVisitToTheHighway(firstStation->right);
    }
}

struct station *nextStation(Station *currentStation) {
    //TODO from cormen

    return currentStation;
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void removeStationAtDistance(int distance, Station *highway) {

    // struct station *foundStation = findStation(distance);
    // stationRemove(highway, foundStation);
}


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {

    Station *highway = &nullStation;

    Station *station1 = createStation(10);
    Station *station2 = createStation(1);
    Station *station3 = createStation(5);
    Station *station4 = createStation(1234);
    Station *station5 = createStation(754);

    insertNewStation(&highway, station1);
    insertNewStation(&highway, station2);
    insertNewStation(&highway, station3);
    insertNewStation(&highway, station4);
    insertNewStation(&highway, station5);


inorderVisitToTheHighway(highway);


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
