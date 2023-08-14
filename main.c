#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct station {
    int distance;
    int maxNumberOfCars;
    int cars[512];
    struct station *right;
    struct station *left;
    struct station *father;
};
typedef struct station Station;
Station *nullStation;
int maxAuto = 0;

Station *createStation(int distanceToInsert, int numberOfCarsToInsert, int carsToInsert[]) {
    int i;
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->maxNumberOfCars = numberOfCarsToInsert;
    newStation->left = nullStation;
    newStation->right = nullStation;
    for (i = 0; i < 512; i++)newStation->cars[i] = 0;
    for (i = 0; i < numberOfCarsToInsert; i++) {
        newStation->cars[i] = carsToInsert[i];
    }
    return newStation;
}

int insertNewStation(Station **firstStation, Station *stationToInsert) {
    struct station *currentStation = nullStation;
    struct station *rootStation = *firstStation;

    while (rootStation != nullStation) {
        currentStation = rootStation;
        if (stationToInsert->distance < rootStation->distance) {
            rootStation = rootStation->left;
        } else rootStation = rootStation->right;
    }
    if (currentStation == nullStation) {
        *firstStation = stationToInsert;
        return 1;
    } else if (stationToInsert->distance < currentStation->distance) {
        currentStation->left = stationToInsert;
        return 1;
    } else if (stationToInsert->distance > currentStation->distance) {
        currentStation->right = stationToInsert;
        return 1;
    } else return 0;
}

void inorderVisitToTheHighway(Station *firstStation) {
    if (firstStation != nullStation) {
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
    if (firstStation != nullStation) {
        clearUp(firstStation->left);
        clearUp(firstStation->right);
        free(firstStation);

    }
}

Station *minimumStation(Station *currentStation) {
    while (currentStation->left != nullStation) {
        currentStation = currentStation->left;
    }
    return currentStation;
}

//TODO deep debugging, isn't executed after one call
int removeStation(Station *firstStation, int distanceToRemove) {
    Station *currentStation;
    if (firstStation == nullStation) return 0;
    if (distanceToRemove < firstStation->distance) {
        removeStation(firstStation->left, distanceToRemove);
    } else if (distanceToRemove > firstStation->distance) {
        removeStation(firstStation->right, distanceToRemove);
    } else {
        if (firstStation->left == nullStation) {
            currentStation = firstStation->right;
            free(firstStation);
            firstStation = currentStation;
            return 1;
        } else if (firstStation->right == nullStation) {
            currentStation = firstStation->left;
            free(firstStation);
            firstStation = currentStation;
            return 1;
        } else if (firstStation->left != nullStation && firstStation->right != nullStation) {
            currentStation = minimumStation(firstStation->right);
            firstStation->distance = currentStation->distance;
            removeStation(firstStation->right, currentStation->distance);
        } else free(firstStation);
    }
    return 0;
}

Station *searchStation(Station *firstStation, int distanceToFind) {
    if (firstStation == nullStation || distanceToFind == (firstStation)->distance) {
        return firstStation;
    }
    if (distanceToFind < firstStation->distance) {
        return searchStation(firstStation->left, distanceToFind);
    } else return searchStation(firstStation->right, distanceToFind);
}

Station *predecessorStation(Station *firstStation, int distance) {
    Station *predecessor = nullStation;
    while (firstStation != nullStation) {
        if (firstStation->distance < distance) {
            predecessor = firstStation;
            firstStation = firstStation->right;
        } else firstStation = firstStation->left;
    }
    return predecessor;
}

void findMaxAuto(Station *firstStation) {
    if (firstStation != nullStation) {
        findMaxAuto(firstStation->left);
        for (int i = 0; i < firstStation->maxNumberOfCars; i++) {
            if (firstStation->cars[i] > maxAuto)maxAuto = firstStation->cars[i];
        }
        findMaxAuto(firstStation->right);
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
void addStation(Station **firstStation) {
    int numberOfCars = 0;
    int insertingDistance = 0;
    if (scanf("%d", &insertingDistance) == 0) return;
    if (scanf("%d", &numberOfCars) == 0) return;
    int car[numberOfCars];
    for (int i = 0; i < numberOfCars; i++) {
        if (scanf("%d", &car[i]) == 0) return;
    }
    Station *station = createStation(insertingDistance, numberOfCars, car);
    if (insertNewStation(firstStation, station) == 1)printf("aggiunta\n");
    else printf("non aggiunta\n");

}

void removeStationAtDistance(Station *firstStation) {
    int removingDistance;
    if (scanf("%d", &removingDistance) == 0) return;
    if (removeStation(firstStation, removingDistance) == 1)printf("demolita\n");
    else printf("non demolita\n");
}

void addAutoAtDistance(Station *firstStation) {
    int distanceOfTheStation, carToInsert, i;
    if (scanf("%d", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%d", &carToInsert) == 0) return;
    fflush(stdin);
    Station *stationToFind = searchStation(firstStation, distanceOfTheStation);
    stationToFind->maxNumberOfCars++;
    for (i = 0; i < 512; i++) {
        if (stationToFind->cars[i] == 0) {
            stationToFind->cars[i] = carToInsert;
            printf("aggiunta\n");
            break;
        }
    }
    if (i == 512)printf("non aggiunta\n");
}

void removeAutoAtDistance(Station *firstStation) {
    int distanceOfTheStation, carToRemove, i;
    if (scanf("%d", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%d", &carToRemove) == 0) return;
    fflush(stdin);
    Station *stationToFind = searchStation(firstStation, distanceOfTheStation);
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

void planTheTrip(Station *firstStation) {
    int distanceOfLeaving, distanceOfArrival;
    if (scanf("%d", &distanceOfLeaving) == 0) return;
    if (scanf("%d", &distanceOfArrival) == 0) return;
    Station *leavingStation = searchStation(firstStation, distanceOfLeaving);
    Station *arrivalStation = searchStation(firstStation, distanceOfArrival);
    Station *predecessor, *checkpointStation;
    int actualAuto;
    if (leavingStation == NULL) {
        printf("nessun percorso");
        return;
    }
    if (arrivalStation == NULL) {
        printf("nessun percorso");
        return;
    } else {
        findMaxAuto(firstStation);
        if (distanceOfLeaving > distanceOfArrival) {
            //TODO usare dijkstra
        } else if (distanceOfLeaving < distanceOfArrival) {
            predecessor = predecessorStation(firstStation, arrivalStation->distance);
            checkpointStation = arrivalStation;
            while (predecessor != leavingStation) {
                actualAuto = findActualMaxAuto(predecessor);
                if ((predecessor->distance + maxAuto) >= checkpointStation->distance) {
                    if (predecessor->distance + actualAuto >= checkpointStation->distance) {
                        predecessor = predecessorStation(firstStation, predecessor->distance);
                    } else {
                        checkpointStation = predecessor;
                        printf("%d->", checkpointStation->distance);
                    }
                } else printf("nessun percorso");
            }
        } else printf("%d ", distanceOfLeaving);
    }
}


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {
    nullStation = malloc(sizeof(Station));
    nullStation->distance = -1;
    nullStation->right = NULL;
    nullStation->left = NULL;
    nullStation->maxNumberOfCars = 0;
    nullStation->father = NULL;
    Station *highway = nullStation;
    int vettore1[5] = {1, 1, 5, 1, 1};
    int vettore2[7] = {2, 2, 2, 5, 2, 2, 2};
    int vettore3[3] = {3, 5, 3};

    Station *station1 = createStation(10, 5, vettore1);
    Station *station2 = createStation(5, 7, vettore2);
    Station *station3 = createStation(34, 3, vettore3);

    insertNewStation(&highway, station1);
    insertNewStation(&highway, station2);
    insertNewStation(&highway, station3);

    printf("aggiunte");

    if (removeStation(highway, station1->distance) == 1) printf("rimossa 1");
    if (removeStation(highway, station2->distance) == 1) printf("rimossa 2");
    if (removeStation(highway, station3->distance) == 1) printf("rimossa 3");
/*
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

           scanf("%s", command);
           if (strcmp(command, "pianifica-percorso") == 0) planTheTrip(highway);
       //    inorderVisitToTheHighway(highway);

             if(scanf("%s", command)==0)return;
              if (strcmp(command, "rottama-auto") == 0) removeAutoAtDistance(highway);
              else if (strcmp(command, "aggiungi-auto") == 0) addAutoAtDistance(highway);
              fflush(stdin);


              if(scanf("%s", command)==0)return;
              if (strcmp(command, "rottama-auto") == 0) removeAutoAtDistance(highway);
              else if (strcmp(command, "aggiungi-auto") == 0) addAutoAtDistance(highway);
              fflush(stdin);

              if(scanf("%s", command)==0)return;
              if (strcmp(command, "rottama-auto") == 0) removeAutoAtDistance(highway);
              else if (strcmp(command, "aggiungi-auto") == 0) addAutoAtDistance(highway);
              fflush(stdin);

             inorderVisitToTheHighway(highway);

    char command[20];

    while (feof(stdin) == false) {
        if (scanf("%s", command) != 0) {
            if (strcmp(command, "aggiungi-stazione") == 0) addStation(&highway);
            else if (strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance(highway);
            else if (strcmp(command, "aggiungi-auto") == 0) addAutoAtDistance(highway);
            else if (strcmp(command, "rottama-auto") == 0) removeAutoAtDistance(highway);
            else if (strcmp(command, "pianifica-percorso") == 0) planTheTrip(highway);
        }

    }
*/
    clearUp(highway);
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
