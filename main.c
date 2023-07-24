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
Station *nullStation;


void insertNewStation(Station *firstStation, Station *stationToInsert) {
    struct station *currentStation = nullStation;
    struct station *otherStation = firstStation;

    while (otherStation != nullStation) {
        currentStation = otherStation;
        if (stationToInsert->distance < otherStation->distance) {
            otherStation = otherStation->left;
        } else otherStation = otherStation->right;
    }
    stationToInsert->father = currentStation;
    if (currentStation == nullStation) {
        firstStation = stationToInsert;
    } else if (stationToInsert->distance < currentStation->distance) {
        currentStation->left = stationToInsert;
    } else currentStation->right = stationToInsert;
}

void inorderVisitToTheHighway(Station *firstStation) {
    if (firstStation == nullStation)return;
    inorderVisitToTheHighway(firstStation->left);
    printf("\nstation: %d ", firstStation->distance);
    printf("left sister: %d ", firstStation->left->distance);
    printf("right sister: %d ", firstStation->right->distance);
    inorderVisitToTheHighway(firstStation->right);
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
    int i;
    nullStation = malloc(sizeof(Station));
    nullStation->left = NULL;
    nullStation->right = NULL;
    nullStation->father=NULL;
    nullStation->distance = -1;
    Station *highway = nullStation;
    Station *temporaryStation;
    for (i = 0; i < 3; i++) {
        temporaryStation = (Station *) malloc(sizeof(Station));
        temporaryStation->distance = i;
        temporaryStation->cars[0] = 1;
        temporaryStation->left = nullStation;
        temporaryStation->right = nullStation;
        temporaryStation->father=nullStation;
        insertNewStation(highway, temporaryStation);
    }
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
