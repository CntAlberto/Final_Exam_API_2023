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
Station *highway;
int maxAuto = 0;

Station *createStation(int distanceToInsert, int numberOfCarsToInsert, int carsToInsert[]) {
    int i;
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->maxNumberOfCars = numberOfCarsToInsert;
    newStation->left = NULL;
    newStation->right = NULL;
    newStation->father = NULL;
    newStation->cars = (int *) malloc(512 * sizeof(int));
    for (i = 0; i < 512; i++)newStation->cars[i] = 0;
    if (numberOfCarsToInsert == 0) {
        return newStation;
    }
    for (i = 0; i < numberOfCarsToInsert; i++) {
        newStation->cars[i] = carsToInsert[i];
    }
    return newStation;
}

int insertNewStation(Station *stationToInsert) {
    struct station *currentStation = NULL;
    struct station *rootStation = highway;

    while (rootStation != NULL) {
        currentStation = rootStation;
        if (stationToInsert->distance < rootStation->distance) {
            rootStation = rootStation->left;
        } else if (stationToInsert->distance > rootStation->distance) {
            rootStation = rootStation->right;
        } else if (stationToInsert->distance == rootStation->distance) {
            printf("non aggiunta\n");
            return 0;
        }
    }
    if (currentStation == NULL) {
        highway = stationToInsert;
        printf("aggiunta\n");
        return 1;
    } else if (stationToInsert->distance < currentStation->distance) {
        currentStation->left = stationToInsert;
        printf("aggiunta\n");
        return 1;
    } else if (stationToInsert->distance > currentStation->distance) {
        currentStation->right = stationToInsert;
        printf("aggiunta\n");
        return 1;
    } else {
        printf("non aggiunta\n");
        return 0;
    }
}


void clearUp(Station *firstStation) {
    if (highway != NULL) {
        clearUp(firstStation->left);
        clearUp(firstStation->right);
        free(highway);
    }
}

Station *minimumStation(Station *currentStation) {
    while (currentStation->left != NULL) {
        currentStation = currentStation->left;
    }
    return currentStation;
}

Station *searchStation(Station *firstStation, int distanceToFind) {
    if (firstStation == NULL || distanceToFind == firstStation->distance) {
        return firstStation;
    }
    if (distanceToFind < firstStation->distance) {
        return searchStation(firstStation->left, distanceToFind);
    } else {
        return searchStation(firstStation->right, distanceToFind);
    }
}

void transplant(Station *firstStation, Station *firstStationToTransplant, Station *secondStationToTransplant) {
    if (firstStationToTransplant->father == NULL)
        firstStation = secondStationToTransplant;
    else if (firstStationToTransplant == firstStationToTransplant->father->left)
        firstStationToTransplant->father->left = secondStationToTransplant;
    else
        firstStationToTransplant->father->right = secondStationToTransplant;

    if (secondStationToTransplant != NULL)
        secondStationToTransplant->father = firstStationToTransplant->father;
}

Station *removeStation(Station *firstStation, int distanceToRemove) {
    Station *stationToRemove = searchStation(firstStation, distanceToRemove);

    if (stationToRemove == NULL) {
        printf("non demolita\n");
        return firstStation;
    }
    if (stationToRemove->left == NULL) {
        transplant(firstStation, stationToRemove, stationToRemove->right);
    } else if (stationToRemove->right == NULL) {
        transplant(firstStation, stationToRemove, stationToRemove->left);
    } else {
        Station *successor = minimumStation(stationToRemove->right);
        if (successor != stationToRemove->right) {
            transplant(firstStation, successor, successor->right);
            successor->right = stationToRemove->right;
            successor->right->father = successor;
        }
        transplant(firstStation, stationToRemove, successor);
        successor->left = stationToRemove->left;
        successor->left->father = successor;
    }
    printf("demolita\n");
    return firstStation;
}


Station *predecessorStation(int distance) {
    Station *predecessor = NULL;
    while (highway != NULL) {
        if (highway->distance < distance) {
            predecessor = highway;
            highway = highway->right;
        } else highway = highway->left;
    }
    return predecessor;
}

void findMaxAuto(Station *firstStation) {
    if (firstStation != NULL) {
        findMaxAuto(firstStation->left);
        for (int i = 0; i < highway->maxNumberOfCars; i++) {
            if (highway->cars[i] > maxAuto)maxAuto = highway->cars[i];
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

int compare(const void *firstCarToCompare, const void *secondCarToCompare) {
    int firstOrderingCar = *((int *) firstCarToCompare);
    int secondOrderingCar = *((int *) secondCarToCompare);

    if (firstOrderingCar == 0)
        return 1;
    if (secondOrderingCar == 0)
        return -1;

    if (firstOrderingCar == secondOrderingCar)
        return 0;
    else if (firstOrderingCar < secondOrderingCar)
        return -1;
    else
        return 1;
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void addStation() {
    int numberOfCars;
    int insertingDistance;
    Station *station;
    if (scanf("%d", &insertingDistance) == 0) return;
    if (scanf("%d", &numberOfCars) == 0) return;
    if (numberOfCars > 0) {
        int car[numberOfCars];
        for (int i = 0; i < numberOfCars; i++) {
            if (scanf("%d", &car[i]) == 0) return;
        }
        qsort(car, numberOfCars, sizeof(int), compare);
        station = createStation(insertingDistance, numberOfCars, car);
    } else {
        int nullVectorOfCar[1] = {0};
        station = createStation(insertingDistance, 0, nullVectorOfCar);
    }
    insertNewStation(station);
}

void removeStationAtDistance() {
    int removingDistance;
    if (scanf("%d", &removingDistance) == 0) return;
    removeStation(highway, removingDistance);
}

void addAutoAtDistance() {
    int distanceOfTheStation, carToInsert, i, addedCar = 0;
    if (scanf("%d", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%d", &carToInsert) == 0) return;
    fflush(stdin);
    Station *stationToFind = searchStation(highway, distanceOfTheStation);
    if (stationToFind == NULL) {
        printf("non aggiunta\n");
        return;
    }
    stationToFind->maxNumberOfCars++;
    for (i = 0; i < stationToFind->maxNumberOfCars; i++) {
        if (stationToFind->cars[i] == 0) {
            stationToFind->cars[i] = carToInsert;
            qsort(stationToFind->cars, stationToFind->maxNumberOfCars, sizeof(int), compare);
            addedCar = 1;
            break;
        }
    }
    if (addedCar == 1)printf("aggiunta\n");
    else printf("non aggiunta\n");
}

void removeAutoAtDistance() {
    int distanceOfTheStation, carToRemove, i;
    if (scanf("%d", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%d", &carToRemove) == 0) return;
    fflush(stdin);


    Station *stationToFind = searchStation(highway, distanceOfTheStation);
    if (stationToFind == NULL) {
        printf("non rottamata\n");
        return;
    }
    for (i = 0; i < stationToFind->maxNumberOfCars; i++) {
        if (stationToFind->cars[i] == carToRemove) {
            stationToFind->cars[i] = 0;
            printf("rottamata\n");
            stationToFind->maxNumberOfCars--;
            qsort(stationToFind->cars, stationToFind->maxNumberOfCars, sizeof(int), compare);
            return;
        }

    }
    printf("non rottamata\n");
}

void planTheTrip() {
    int distanceOfLeaving, distanceOfArrival;
    if (scanf("%d", &distanceOfLeaving) == 0) return;
    if (scanf("%d", &distanceOfArrival) == 0) return;
    Station *leavingStation = searchStation(highway, distanceOfLeaving);
    Station *arrivalStation = searchStation(highway, distanceOfArrival);
    //  Station *predecessor, *checkpointStation;
    //int actualAuto;
    if (leavingStation == NULL) {
        printf("nessun percorso\n");
        return;
    }
    if (arrivalStation == NULL) {
        printf("nessun percorso\n");
        return;
    }
    //findMaxAuto(highway);
    if (distanceOfLeaving > distanceOfArrival) {
        //TODO usare dijkstra
        printf("nessun percorso\n");
    } else if (distanceOfLeaving < distanceOfArrival) {
        /*   predecessor = predecessorStation(arrivalStation->distance);
           checkpointStation = arrivalStation;
           while (predecessor != leavingStation) {
               actualAuto = findActualMaxAuto(predecessor);
               if ((predecessor->distance + maxAuto) >= checkpointStation->distance) {
                   if ((predecessor->distance + actualAuto) >= checkpointStation->distance) {
                       predecessor = predecessorStation(predecessor->distance);
                   } else {
                       checkpointStation = predecessor;
                       printf("%d ", checkpointStation->distance);
                   }
               } else printf("nessun percorso");
           }
           printf("\n");
           */
        printf("nessun percorso\n");
    }
}


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {
    highway = (Station *) malloc(sizeof(Station));
    highway = NULL;
/*
    int vetoreDiMacchine1[13] = {445, 462, 416, 471, 473, 413, 492, 408, 396, 462, 388, 464, 480};
    Station *station1 = createStation(756, 13, vetoreDiMacchine1);
    insertNewStation(station1);

    int vetoreDiMacchine2[20] = {367, 313, 365, 393, 380, 343, 339, 307, 369, 350, 384, 334, 346, 344, 351, 332, 323,
                                 392, 369, 350};
    Station *station2 = createStation(2114, 20, vetoreDiMacchine2);
    insertNewStation(station2);

    removeStation(highway, 756);

    int vettoreDiMacchine3[1] = {0};
    Station *station3 = createStation(2764, 0, vettoreDiMacchine3);
    insertNewStation(station3);

    //TODO debug addAuto and removeAuto

*/
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

    //   clearUp(highway);
    //   free(highway);


}


/**
 * ALGORITMO EFFICENTE DI POZZI
 * MI SALVO LA MACCHINA CON MASSIMA AUTONOMIA IN TUTto L'ALBERO(autmax) partendo dalla fine all'inizio mi fermo ad ogni stazione econtrollo se la distanza + autmax raggiunge la staione finale, se si controllo allora
 * che anche la distanza della stazione corrente + l'autonomia massima della macchina nella stazione attuale ci arrivi, se entrambi i vincoli sono soddisfatti allora la segno come check point temporaneo,
 * ricorsivamente ripeto con il precedente finche non trovo un nodo dove una delle 2 non è rispetttata, il nodo ultimo in cui è rispettatan è la stazione da stampare a video
 * ATTENZIONE FUNZIONA SOLO IN UN VERSO; USARE DIJKSTRA PER IL SECODNO VERSO
 */
