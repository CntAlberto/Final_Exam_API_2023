#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


struct station {
    long distance;
    long maxNumberOfCars;
    long maxCar;
    long *cars;
    struct station *right;
    struct station *left;
    struct station *father;
};
typedef struct station Station;
Station *firsStation;
long numberOfStations = 0;
long *distances;
long *positions;
Station **stations;
Station **checkPoints;

Station *createStation(long distanceToInsert, long numberOfCarsToInsert, long carsToInsert[]) {
    long i;
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distanceToInsert;
    newStation->maxNumberOfCars = numberOfCarsToInsert;
    newStation->left = NULL;
    newStation->right = NULL;
    newStation->father = NULL;
    newStation->cars = (long *) malloc(512 * sizeof(long));
    newStation->maxCar = 0;
    if (numberOfCarsToInsert == 0) {
        return newStation;
    }
    for (i = 0; i < numberOfCarsToInsert; i++) {
        newStation->cars[i] = carsToInsert[i];
        if (carsToInsert[i] > newStation->maxCar)newStation->maxCar = carsToInsert[i];
    }
    return newStation;
}

void insertNewStation(Station *stationToInsert) {
    Station *rootStation = firsStation;
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
        firsStation = stationToInsert;
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
        free(firsStation);
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

void transplant(Station *firstStationToRemove, Station *secondStationToRemove) {
    if (firstStationToRemove->father == NULL)
        firsStation = secondStationToRemove;
    else if (firstStationToRemove == firstStationToRemove->father->left)
        firstStationToRemove->father->left = secondStationToRemove;
    else
        firstStationToRemove->father->right = secondStationToRemove;

    if (secondStationToRemove != NULL)
        secondStationToRemove->father = firstStationToRemove->father;
}

void removeStation(Station *firstStation, long distanceToRemove) {
    Station *stationToRemove = searchStation(firstStation, distanceToRemove);

    if (stationToRemove == NULL) {
        printf("non demolita\n");
        return;
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
    }
    if (stationToRemove != NULL) {
        free(stationToRemove->cars);
        free(stationToRemove);
        printf("demolita\n");
    }
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
            stationOfArrival = successorStation(stationOfArrival);
        }
    }
    numberOfStations++;
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void addStation() {
    long numberOfCars;
    long insertingDistance;
    Station *station;
    if (scanf("%ld", &insertingDistance) == 0) return;
    if (feof(stdin) == true)return;
    if (scanf("%ld", &numberOfCars) == 0) return;
    if (feof(stdin) == true)return;
    if (numberOfCars > 0) {
        long *car = (long *) malloc(numberOfCars * sizeof(long));
        for (long i = 0; i < numberOfCars; i++) {
            if (scanf("%ld", &car[i]) == 0) return;
            if (feof(stdin) == true)return;
        }
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
    if (feof(stdin) == true)return;
    removeStation(firsStation, removingDistance);
}

void addAutoAtDistance() {
    long distanceOfTheStation, carToInsert;
    if (scanf("%ld", &distanceOfTheStation) == 0) return;
    if (feof(stdin) == true)return;
    fflush(stdin);
    if (scanf("%ld", &carToInsert) == 0) return;
    if (feof(stdin) == true)return;
    fflush(stdin);
    Station *stationToFind = searchStation(firsStation, distanceOfTheStation);
    if (stationToFind == NULL) {
        printf("non aggiunta\n");
        return;
    }
    stationToFind->maxNumberOfCars++;
    stationToFind->cars[stationToFind->maxNumberOfCars - 1] = carToInsert;
    if (carToInsert > stationToFind->maxCar)stationToFind->maxCar = carToInsert;
    printf("aggiunta\n");
}

void removeAutoAtDistance() {
    long distanceOfTheStation, carToRemove, i, removedCar;
    if (scanf("%ld", &distanceOfTheStation) == 0) return;
    if (feof(stdin) == true)return;
    fflush(stdin);
    if (scanf("%ld", &carToRemove) == 0) return;
    if (feof(stdin) == true)return;
    fflush(stdin);
    Station *stationToFind = searchStation(firsStation, distanceOfTheStation);
    if (stationToFind == NULL) {
        printf("non rottamata\n");
        return;
    }
    for (i = 0; i < stationToFind->maxNumberOfCars; i++) {
        if (stationToFind->cars[i] == carToRemove) {
            removedCar = stationToFind->cars[i];
            stationToFind->cars[i] = stationToFind->cars[stationToFind->maxNumberOfCars - 1];
            stationToFind->cars[stationToFind->maxNumberOfCars - 1] = removedCar;
            stationToFind->maxNumberOfCars--;
            stationToFind->maxCar = 0;
            for (i = 0; i < stationToFind->maxNumberOfCars; i++) {
                if (stationToFind->cars[i] > stationToFind->maxCar)stationToFind->maxCar = stationToFind->cars[i];
            }
            printf("rottamata\n");
            return;
        }

        printf("non rottamata\n");
    }
}

    void planTheTrip() {
        long distanceOfLeaving, distanceOfArrival;
        if (scanf("%ld", &distanceOfLeaving) == 0) return;
        if (feof(stdin) == true)return;
        if (scanf("%ld", &distanceOfArrival) == 0) return;
        if (feof(stdin) == true)return;
        Station *leavingStation = searchStation(firsStation, distanceOfLeaving);
        Station *arrivalStation = searchStation(firsStation, distanceOfArrival);
        Station *successor;
        long i;
        if (leavingStation == NULL) {
            printf("nessun percorso\n");
            return;
        }
        if (arrivalStation == NULL) {
            printf("nessun percorso\n");
            return;
        }
        if (leavingStation->distance == arrivalStation->distance) {
            printf("%ld\n", arrivalStation->distance);
            return;
        }
        if (distanceOfLeaving > distanceOfArrival) {
            countStations(arrivalStation, leavingStation);
            long calculatedDistance, j;
            for (i = numberOfStations - 1; i >= 0; i--) {
                stations[i] = arrivalStation;
                arrivalStation = successorStation(arrivalStation);
                positions[i] = -1;
                checkPoints[i] = NULL;
            }
            for (i = 0; i < numberOfStations; i++) {
                if (i == 0)distances[i] = 0;
                else distances[i] = INT_MAX;
            }
            for (i = 0; i < numberOfStations; i++) {
                if (i + 1 < numberOfStations) {
                    successor = stations[i + 1];

                    if (successor == NULL)break;
                    j = i + 1;
                    while (stations[i]->distance - stations[i]->maxCar <= successor->distance) {
                        calculatedDistance = distances[i] + 1;
                        if (distances[j] >= calculatedDistance) {
                            distances[j] = calculatedDistance;
                            positions[j] = i;
                        }
                        if (j + 1 < numberOfStations) {
                            successor = stations[j + 1];
                            j++;
                        } else break;

                    }
                }
            }
            if (distances[numberOfStations - 1] == INT_MAX) {
                printf("nessun percorso\n");
                return;
            }
            i = numberOfStations - 1;
            j = 0;
            while (i >= 0) {
                checkPoints[j] = stations[i];
                i = positions[i];
                j++;
            }
            j--;
            while (j > 0) {
                if (checkPoints[j] != NULL) {
                    printf("%ld ", checkPoints[j]->distance);
                }
                j--;
            }
            printf("%ld\n", checkPoints[0]->distance);
            return;
        } else if (distanceOfLeaving < distanceOfArrival) {
            countStations(leavingStation, arrivalStation);
            long *predecessors = (long *) malloc(30000 * sizeof(long)); //VETTORE
            long calculatedDistance, j;
            for (i = 0; i < numberOfStations; i++) {
                stations[i] = leavingStation;
                leavingStation = successorStation(leavingStation);
                positions[i] = -1;
                predecessors[i] = -1;
                checkPoints[i] = NULL;
            }
            for (i = 0; i < numberOfStations; i++) {
                if (i == 0)distances[i] = 0;
                else distances[i] = INT_MAX;
            }
            for (i = 0; i < numberOfStations; i++) {
                successor = successorStation(stations[i]);
                if (successor == NULL)break;
                j = i + 1;
                while (stations[i]->distance + stations[i]->maxCar >= successor->distance) {
                    calculatedDistance = distances[i] + 1;
                    if (distances[j] > calculatedDistance) {
                        distances[j] = calculatedDistance;
                        predecessors[j] = stations[i]->distance;
                        positions[j] = i;
                    }
                    successor = successorStation(successor);
                    if (successor == NULL)break;
                    j++;
                }
            }
            if (distances[numberOfStations - 1] == INT_MAX) {
                printf("nessun percorso\n");
                return;
            }
            i = numberOfStations - 1;
            j = 0;
            while (i >= 0) {
                checkPoints[j] = stations[i];
                i = positions[i];
                j++;
            }
            j--;
            while (j > 0) {
                if (checkPoints[j] != NULL) {
                    printf("%ld ", checkPoints[j]->distance);
                }
                j--;
            }
            printf("%ld\n", checkPoints[0]->distance);
            return;
        }
    }


//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
    int main() {
        firsStation = (Station *) malloc(sizeof(Station));
        firsStation = NULL;
        distances = (long *) malloc(30000 * sizeof(long));
        positions = (long *) malloc(30000 * sizeof(long));
        stations = (Station **) malloc(30000 * sizeof(Station));
        checkPoints = (Station **) malloc(3500 * sizeof(Station));
        char command[20];

        while (feof(stdin) == false) {
            if (scanf("%s", command) != 0) {
                if (strcmp(command, "aggiungi-stazione") == 0) addStation();
                else if (strcmp(command, "demolisci-stazione") == 0) removeStationAtDistance();
                else if (strcmp(command, "aggiungi-auto") == 0) addAutoAtDistance();
                else if (strcmp(command, "rottama-auto") == 0) removeAutoAtDistance();
                else if (strcmp(command, "pianifica-percorso") == 0) planTheTrip();
                else if (feof(stdin) == false) return 0;
            }

        }

    }