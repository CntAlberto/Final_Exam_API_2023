#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


struct station {
    long distance;
    long maxNumberOfCars;
    long *cars;
    struct station *right;
    struct station *left;
    struct station *father;
};
typedef struct station Station;
Station *highway;
long maxAuto = 0;
long numberOfStations = 0;

int compare(const void *firstCarToCompare, const void *secondCarToCompare) {
    long firstOrderingCar = *((long *) firstCarToCompare);
    long secondOrderingCar = *((long *) secondCarToCompare);

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

Station *createStation(long distanceToInsert, long numberOfCarsToInsert, long carsToInsert[]) {
    long i;
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->maxNumberOfCars = numberOfCarsToInsert;
    newStation->left = NULL;
    newStation->right = NULL;
    newStation->father = NULL;
    newStation->cars = (long *) malloc(512 * sizeof(long));
    for (i = 0; i < 512; i++)newStation->cars[i] = 0;
    if (numberOfCarsToInsert == 0) {
        return newStation;
    }
    for (i = 0; i < numberOfCarsToInsert; i++) {
        newStation->cars[i] = carsToInsert[i];
    }
    qsort(newStation->cars, newStation->maxNumberOfCars, sizeof(long), compare);
    return newStation;
}

void insertNewStation(Station *stationToInsert) {
    Station *rootStation = highway;
    Station *currentStation = NULL;


    while (rootStation != NULL) {
        currentStation = rootStation;
        if (stationToInsert->distance < rootStation->distance) {
            rootStation = rootStation->left;
        } else if (stationToInsert->distance > rootStation->distance) {
            rootStation = rootStation->right;
        } else {
            printf("non aggiunta\n");
            return;
        }
    }
    stationToInsert->father = currentStation;
    if (currentStation == NULL) {
        highway = stationToInsert;
        printf("aggiunta\n");
        return;
    } else if (stationToInsert->distance < currentStation->distance) {
        currentStation->left = stationToInsert;
        printf("aggiunta\n");
        return;
    } else {
        currentStation->right = stationToInsert;
        printf("aggiunta\n");
    }
}


void clearUp(Station *firstStation) {
    if (firstStation != NULL) {
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

Station *searchStation(Station *firstStation, long distanceToFind) {
    while (firstStation != NULL) {
        if (firstStation->distance == distanceToFind)return firstStation;
        else if (distanceToFind < firstStation->distance) firstStation = firstStation->left;
        else if (distanceToFind > firstStation->distance) firstStation = firstStation->right;
    }
    return firstStation;
}

void transplant(Station *firstStationToTransplant, Station *secondStationToTransplant) {
    if (firstStationToTransplant->father == NULL)
        highway = secondStationToTransplant;
    else if (firstStationToTransplant == firstStationToTransplant->father->left)
        firstStationToTransplant->father->left = secondStationToTransplant;
    else
        firstStationToTransplant->father->right = secondStationToTransplant;

    if (secondStationToTransplant != NULL)
        secondStationToTransplant->father = firstStationToTransplant->father;
}

Station *removeStation(Station *firstStation, long distanceToRemove) {
    Station *stationToRemove = searchStation(firstStation, distanceToRemove);

    if (stationToRemove == NULL) {
        printf("non demolita\n");
        return firstStation;
    }
    if (stationToRemove->left == NULL) {
        transplant(stationToRemove, stationToRemove->right);
    } else if (stationToRemove->right == NULL) {
        transplant(stationToRemove, stationToRemove->left);
    } else {
        Station *successor = minimumStation(stationToRemove->right);
        if (successor != stationToRemove->right) {
            transplant(successor, successor->right);
            successor->right = stationToRemove->right;
            successor->right->father = successor;
        }
        transplant(stationToRemove, successor);
        successor->left = stationToRemove->left;
        successor->left->father = successor;
        printf("demolita\n");
        return firstStation;
    }
    printf("demolita\n");
    return firstStation;
}


Station *successorStation(Station *currentStation) {
    Station *successor = NULL;
    if (currentStation->right != NULL) {
        return minimumStation(currentStation->right);
    } else {
        successor = currentStation->father;
        while (successor != NULL && currentStation == successor->right) {
            currentStation = successor;
            successor = successor->father;
        }
        return successor;
    }

}

Station *predecessorStation(Station *currentStation) {
    Station *predecessor = NULL;
    if (currentStation->left != NULL) {
        return minimumStation(currentStation->left);
    } else {
        predecessor = currentStation->father;
        while (predecessor != NULL && currentStation == predecessor->left) {
            currentStation = predecessor;
            predecessor = predecessor->father;
        }
        return predecessor;
    }
}

void findMaxAuto(Station *firstStation) {
    if (firstStation != NULL) {
        findMaxAuto(firstStation->left);
        for (long i = 0; i < highway->maxNumberOfCars; i++) {
            if (highway->cars[i] > maxAuto)maxAuto = highway->cars[i];
        }
        findMaxAuto(firstStation->right);
    }
}

long findActualMaxAuto(Station *stationToFindMaxAuto) {
    long actualMaxAuto = stationToFindMaxAuto->cars[stationToFindMaxAuto->maxNumberOfCars - 1];
    return actualMaxAuto;
}

void countStations(Station *stationOfLeaving, Station *stationOfArrival) {
    numberOfStations = 0;
    if (stationOfLeaving->distance < stationOfArrival->distance) {
        while (stationOfLeaving != stationOfArrival) {
            numberOfStations++;
            stationOfLeaving = successorStation(stationOfLeaving);
        }
    } else if (stationOfLeaving->distance > stationOfArrival->distance) {
        while (stationOfArrival != stationOfLeaving) {
            numberOfStations++;
            stationOfLeaving = predecessorStation(stationOfLeaving);
        }
    }
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void addStation() {
    long numberOfCars;
    long insertingDistance;
    Station *station;
    if (scanf("%ld", &insertingDistance) == 0) return;
    if (scanf("%ld", &numberOfCars) == 0) return;
    if (numberOfCars > 0) {
        long *car = (long *) malloc(numberOfCars * sizeof(long));
        for (long i = 0; i < numberOfCars; i++) {
            if (scanf("%ld", &car[i]) == 0) return;
        }
        qsort(car, numberOfCars, sizeof(long), compare);
        station = createStation(insertingDistance, numberOfCars, car);
    } else {
        long *nullVectorOfCar = (long *) malloc(sizeof(long));
        nullVectorOfCar[0] = 0;
        station = createStation(insertingDistance, 0, nullVectorOfCar);
    }
    insertNewStation(station);
}

void removeStationAtDistance() {
    long removingDistance;
    if (scanf("%ld", &removingDistance) == 0) return;
    removeStation(highway, removingDistance);
}

void addAutoAtDistance() {
    long distanceOfTheStation, carToInsert, addedCar = 0;
    if (scanf("%ld", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%ld", &carToInsert) == 0) return;
    fflush(stdin);
    Station *stationToFind = searchStation(highway, distanceOfTheStation);
    if (stationToFind == NULL) {
        printf("non aggiunta\n");
        return;
    }
    stationToFind->maxNumberOfCars++;
    if (stationToFind->cars[stationToFind->maxNumberOfCars - 1] == 0) {
        stationToFind->cars[stationToFind->maxNumberOfCars - 1] = carToInsert;
        addedCar = 1;
        qsort(stationToFind->cars, stationToFind->maxNumberOfCars, sizeof(long), compare);
    }
    if (addedCar == 1)printf("aggiunta\n");
    else printf("non aggiunta\n");
}

void removeAutoAtDistance() {
    long distanceOfTheStation, carToRemove, i;
    if (scanf("%ld", &distanceOfTheStation) == 0) return;
    fflush(stdin);
    if (scanf("%ld", &carToRemove) == 0) return;
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
            qsort(stationToFind->cars, stationToFind->maxNumberOfCars, sizeof(long), compare);
            stationToFind->maxNumberOfCars--;
            return;
        }

    }
    printf("non rottamata\n");
}

void planTheTrip() {
    long distanceOfLeaving, distanceOfArrival;
    if (scanf("%ld", &distanceOfLeaving) == 0) return;
    if (scanf("%ld", &distanceOfArrival) == 0) return;
    Station *leavingStation = searchStation(highway, distanceOfLeaving);
    Station *arrivalStation = searchStation(highway, distanceOfArrival);
    Station *successor;
    long actualAuto, i;
    if (leavingStation == NULL) {
        printf("nessun percorso\n");
        return;
    }
    if (arrivalStation == NULL) {
        printf("nessun percorso\n");
        return;
    }
    if (distanceOfLeaving > distanceOfArrival) {


        printf("nessun percorso\n");
    } else if (distanceOfLeaving < distanceOfArrival) {
        countStations(leavingStation, arrivalStation);
        Station **stations = (Station **) malloc(numberOfStations * sizeof(Station));
        Station **predecessors = (Station **) malloc(numberOfStations * sizeof(Station));
        Station **carChanges = (Station **) malloc(numberOfStations * sizeof(Station));
        long *distances = malloc(numberOfStations * sizeof(long));
        long *positions = malloc(numberOfStations * sizeof(long));
        long calculatedDistance;
        for (i = 0; i < numberOfStations; i++) {
            stations[i] = leavingStation;
            leavingStation = successorStation(leavingStation);
            positions[i]=-1;
        }
        for (i = 0; i < numberOfStations; i++) {
            if (i == 0)distances[i] = 0;
            else distances[i] = INT_MAX;
            predecessors[i]=NULL;
        }
        for (i=0;i<numberOfStations;i++) {
            actualAuto= findActualMaxAuto(stations[i]);
            successor= successorStation(stations[i]);
            long j=i;
            while(stations[i]->distance + actualAuto >= successor->distance){
                calculatedDistance=distances[i]+1;
                if(distances[j]>calculatedDistance){
                    distances[j]=calculatedDistance;
                    predecessors[j]=stations[i];
                    positions[i]=i;
                }
                successor= successorStation(successor);
                j++;
            }
        }
        for(i=0;i<numberOfStations;i++){
            if(distances[i]==INT_MAX){
                printf("nessun percorso\n");
                return;
            }
        }
        long j=0;
        for(i=numberOfStations;i>0;i--){
            if(positions[i]!=-1){
                while(numberOfStations>=positions[i])arrivalStation= predecessorStation(arrivalStation);
                carChanges[j]=arrivalStation;
                j++;
            }
        }
        while(j!=0){
            printf("%ld ",carChanges[j]->distance);
            j--;
        }
    }
}
//TODO deep debugging planTrip


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {
    highway = (Station *) malloc(sizeof(Station));
    highway = NULL;

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
    //clearUp(highway);


}


/**
 * ALGORITMO EFFICENTE DI POZZI
 * MI SALVO LA MACCHINA CON MASSIMA AUTONOMIA IN TUTto L'ALBERO(autmax) partendo dalla fine all'inizio mi fermo ad ogni stazione econtrollo se la distanza + autmax raggiunge la staione finale, se si controllo allora
 * che anche la distanza della stazione corrente + l'autonomia massima della macchina nella stazione attuale ci arrivi, se entrambi i vincoli sono soddisfatti allora la segno come check polong temporaneo,
 * ricorsivamente ripeto con il precedente finche non trovo un nodo dove una delle 2 non è rispetttata, il nodo ultimo in cui è rispettatan è la stazione da stampare a video
 * ATTENZIONE FUNZIONA SOLO IN UN VERSO; USARE DIJKSTRA PER IL SECODNO VERSO
 */
