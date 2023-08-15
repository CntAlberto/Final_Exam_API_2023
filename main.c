#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct station {
    int distance;
    int maxNumberOfCars;
    int *cars;
    struct station *right;
    struct station *left;
    struct station *father;
};
typedef struct station Station;
Station *nullStation;
Station *highway;
int maxAuto = 0;

Station *createStation(int distanceToInsert, int numberOfCarsToInsert, int carsToInsert[]) {
    int i;
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->maxNumberOfCars = numberOfCarsToInsert;
    newStation->left = nullStation;
    newStation->right = nullStation;
    newStation->cars=(int *) malloc(512*sizeof(int));
    for (i = 0; i < 512; i++)newStation->cars[i] = 0;
    for (i = 0; i < numberOfCarsToInsert; i++) {
        newStation->cars[i] = carsToInsert[i];
    }
    return newStation;
}

int insertNewStation(Station *stationToInsert) {
    struct station *currentStation = nullStation;
    struct station *rootStation = highway;

    while (rootStation != nullStation) {
        currentStation = rootStation;
        if (stationToInsert->distance < rootStation->distance) {
            rootStation = rootStation->left;
        } else rootStation = rootStation->right;
    }
    if (currentStation == nullStation) {
        highway = stationToInsert;
        return 1;
    } else if (stationToInsert->distance < currentStation->distance) {
        currentStation->left = stationToInsert;
        return 1;
    } else if (stationToInsert->distance > currentStation->distance) {
        currentStation->right = stationToInsert;
        return 1;
    } else return 0;
}


void clearUp(Station *firstStation) {
    if (highway != nullStation) {
        clearUp(firstStation->left);
        clearUp(firstStation->right);
        free(highway);
    }
}

Station *minimumStation(Station *currentStation) {
    while (currentStation->left != nullStation) {
        currentStation = currentStation->left;
    }
    return currentStation;
}

Station* removeStation(Station *root, int distanceToRemove) {
    if (root == NULL) {
        printf("non demolita\n");
        return root;
    }
    if (distanceToRemove < root->distance)
        root->left = removeStation(root->left, distanceToRemove);
    else if (distanceToRemove > root->distance)
        root->right = removeStation(root->right, distanceToRemove);
    else {
        if (root->left == NULL) {
            Station* temp = root->right;
            free(root);
            printf("demolita\n");
            return temp;
        } else if (root->right == NULL) {
            Station* temp = root->left;
            free(root);
            printf("demolita\n");
            return temp;
        }

        Station* temp = minimumStation(root->right);
        root->distance = temp->distance;
        root->right = removeStation(root->right, temp->distance);
    }
    return root;
}

Station *searchStation(int distanceToFind) {
    if (highway == nullStation || distanceToFind == (highway)->distance) {
        return highway;
    }
    if (distanceToFind < highway->distance) {
        highway = highway->left;
        return searchStation(distanceToFind);
    } else {
        highway = highway->right;
        return searchStation(distanceToFind);
    }
}

Station *predecessorStation(int distance) {
    Station *predecessor = nullStation;
    while (highway != nullStation) {
        if (highway->distance < distance) {
            predecessor = highway;
            highway = highway->right;
        } else highway = highway->left;
    }
    return predecessor;
}

void findMaxAuto() {
    if (highway != nullStation) {
        highway = highway->left;
        findMaxAuto();
        for (int i = 0; i < highway->maxNumberOfCars; i++) {
            if (highway->cars[i] > maxAuto)maxAuto = highway->cars[i];
        }
        highway = highway->right;
        findMaxAuto();
    }
}

int findActualMaxAuto(Station *stationToFindMaxAuto) {
    int actualMaxAuto = 0;
    for (int i = 0; i < stationToFindMaxAuto->maxNumberOfCars; i++) {
        if (stationToFindMaxAuto->cars[i] > actualMaxAuto)actualMaxAuto = stationToFindMaxAuto->cars[i];
    }
    return actualMaxAuto;
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void addStation() {
    int numberOfCars = 0;
    int insertingDistance = 0;
    if (scanf("%d", &insertingDistance) == 0) return;
    if (scanf("%d", &numberOfCars) == 0) return;
    int car[numberOfCars];
    for (int i = 0; i < numberOfCars; i++) {
        if (scanf("%d", &car[i]) == 0) return;
    }
    Station *station = createStation(insertingDistance, numberOfCars, car);
    if (insertNewStation(station) == 1)printf("aggiunta\n");
    else printf("non aggiunta\n");

}

void removeStationAtDistance() {
    int removingDistance;
    if (scanf("%d", &removingDistance) == 0) return;
    removeStation(highway,removingDistance);
}

void addAutoAtDistance() {
    int distanceOfTheStation, carToInsert, i;
    if (scanf("%d", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%d", &carToInsert) == 0) return;
    fflush(stdin);
    Station *stationToFind = searchStation(distanceOfTheStation);
    if(stationToFind==NULL){
        printf("non aggiunta\n");
        return;
    }
    stationToFind->maxNumberOfCars++;
    for (i = 0; i < 512; i++) {
        if (stationToFind->cars[i] == 0) {
            stationToFind->cars[i] = carToInsert;
            printf("aggiunta\n");
            return;
        }
    }
    if (i == 512)printf("non aggiunta\n");
}

void removeAutoAtDistance() {
    int distanceOfTheStation, carToRemove, i;
    if (scanf("%d", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%d", &carToRemove) == 0) return;
    fflush(stdin);
    Station *stationToFind = searchStation(distanceOfTheStation);
    stationToFind->maxNumberOfCars--;
    for (i = 0; i < 512; i++) {
        if (stationToFind->cars[i] == carToRemove) {
            stationToFind->cars[i] = 0;
            printf("rottamata\n");
            break;
        }
    }
    if (i == 512)printf("non rottamata\n");
}

void planTheTrip() {
    int distanceOfLeaving, distanceOfArrival;
    if (scanf("%d", &distanceOfLeaving) == 0) return;
    if (scanf("%d", &distanceOfArrival) == 0) return;
    Station *leavingStation = searchStation(distanceOfLeaving);
    Station *arrivalStation = searchStation(distanceOfArrival);
    // Station *predecessor, *checkpointStation;
    //  int actualAuto;
    if (leavingStation == NULL) {
        printf("nessun percorso\n");
        return;
    }
    if (arrivalStation == NULL) {
        printf("nessun percorso\n");
        return;
    } else {
        findMaxAuto();
        if (distanceOfLeaving > distanceOfArrival) {
            //TODO usare dijkstra
            printf("nessun percorso");
        } else if (distanceOfLeaving < distanceOfArrival) {
            /*  predecessor = predecessorStation(arrivalStation->distance);
              checkpointStation = arrivalStation;
              while (predecessor != leavingStation) {
                  actualAuto = findActualMaxAuto(predecessor);
                  if ((predecessor->distance + maxAuto) >= checkpointStation->distance) {
                      if (predecessor->distance + actualAuto >= checkpointStation->distance) {
                          predecessor = predecessorStation(predecessor->distance);
                      } else {
                          checkpointStation = predecessor;
                          printf("%d->", checkpointStation->distance);
                      }
                  } else
                      */
            printf("nessun percorso\n");
        }else printf("nessun percorso\n");
    }
}


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {
    nullStation = (Station *) malloc(sizeof(Station));
    nullStation->distance = -1;
    nullStation->right = NULL;
    nullStation->left = NULL;
    nullStation->maxNumberOfCars = 0;
    nullStation->father = NULL;
    highway = (Station *) malloc(sizeof(Station));
    highway = nullStation;

    int vetoreDiMacchine1[9]={5,4,5,5,4,6,5,4,5};
    Station *station1= createStation(91,9,vetoreDiMacchine1);
    insertNewStation(station1);
    printf("aggiunta\n");

    int vetoreDiMacchine2[6]={5,4,5,4,4,5};
    Station *station2= createStation(24,6,vetoreDiMacchine2);
    insertNewStation(station2);
    printf("aggiunta\n");

    int vetoreDiMacchine3[6]={5,4,4,5,5,5};
    Station *station3= createStation(92,6,vetoreDiMacchine3);
    insertNewStation(station3);
    printf("aggiunta\n");

    removeStation(highway,59);

    int vetoreDiMacchine4[9]={13,14,13,17,17,15,18,15,16};
    Station *station4= createStation(52,9,vetoreDiMacchine4);
    insertNewStation(station4);
    printf("aggiunta\n");
//TODO debug addAuto and removeAuto

/*
   char command[20];

    while (feof(stdin) == false) {
        if (scanf("%s", command) != 0) {
            if (strcmp(command, "aggiungi-stazione") == 0) addStation();
            else if (strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance();
            else if (strcmp(command, "aggiungi-auto") == 0) addAutoAtDistance();
            else if (strcmp(command, "rottama-auto") == 0) removeAutoAtDistance();
            else if (strcmp(command, "pianifica-percorso") == 0) planTheTrip();
        }

    }
    */
    clearUp(highway);
    free(nullStation);
    free(highway);
    return 0;
}



/**
 * ALGORITMO EFFICENTE DI POZZI
 * MI SALVO LA MACCHINA CON MASSIMA AUTONOMIA IN TUTto L'ALBERO(autmax) partendo dalla fine all'inizio mi fermo ad ogni stazione econtrollo se la distanza + autmax raggiunge la staione finale, se si controllo allora
 * che anche la distanza della stazione corrente + l'autonomia massima della macchina nella stazione attuale ci arrivi, se entrambi i vincoli sono soddisfatti allora la segno come check point temporaneo,
 * ricorsivamente ripeto con il precedente finche non trovo un nodo dove una delle 2 non è rispetttata, il nodo ultimo in cui è rispettatan è la stazione da stampare a video
 * ATTENZIONE FUNZIONA SOLO IN UN VERSO; USARE DIJKSTRA PER IL SECODNO VERSO
 */
