#include <stdio.h>
#include <stdlib.h>

#define maxNumberOfCars 512

enum color {
    RED,
    BLACK
};
struct station {
    int distance;
    int cars[maxNumberOfCars];
    enum color stationColor;
    struct station *father;
    struct station *right;
    struct station *left;
};

struct station nullStation = {
        .distance=-1,
        .cars[0]=0,
        .stationColor=BLACK,
        .father=NULL,
        .right=NULL,
        .left=NULL,
};


struct station *highway = NULL;
int i;



void insertNewStation(struct station *firstStation, struct station *stationToInsert) {
    struct station *currentStation = &nullStation;
    struct station *otherStation = firstStation;
    if (firstStation != NULL) {
        do {
            if (otherStation->distance != nullStation.distance) {
                currentStation = otherStation;
                if (stationToInsert->distance < otherStation->distance) {
                    otherStation = otherStation->left;
                } else otherStation = otherStation->right;
            }
        } while (otherStation->distance != nullStation.distance);
        stationToInsert->father = currentStation;
        if (stationToInsert->distance < currentStation->distance) {
            currentStation->left = stationToInsert;
        } else currentStation->right = stationToInsert;
    } else {
        otherStation = stationToInsert;
        otherStation->left = &nullStation;
        otherStation->right = &nullStation;
        otherStation->father = &nullStation;
    }
}


void rightRotate(struct station *firstStation, struct station *stationToRotate) {
    struct station *currentStation = stationToRotate->left;
    stationToRotate->left = currentStation->right;
    if (currentStation != &nullStation) {
        currentStation->right->father = stationToRotate;
    }
    currentStation->father = stationToRotate->father;
    if (stationToRotate->father == &nullStation) {
        firstStation = currentStation;
    } else if (stationToRotate == stationToRotate->father->right) {
        stationToRotate->father->right = currentStation;
    } else stationToRotate->father->left = currentStation;
    currentStation->right = stationToRotate;
    stationToRotate->father = currentStation;
}


void leftRotate(struct station *firstStation, struct station *stationToRotate) {
    struct station *currentStation = stationToRotate->right;
    stationToRotate->right = currentStation->left;
    if (currentStation != &nullStation) {
        currentStation->left->father = stationToRotate;
    }
    currentStation->father = stationToRotate->father;
    if (stationToRotate->father == &nullStation) {
        firstStation = currentStation;
    } else if (stationToRotate == stationToRotate->father->left) {
        stationToRotate->father->left = currentStation;
    } else stationToRotate->father->right = currentStation;
    currentStation->left = stationToRotate;
    stationToRotate->father = currentStation;
}

//TODO it doesn't build a RBTree, need to debug it
void insertStationAndFixItUp(struct station *firstStation, struct station *stationToAdjust) {
    insertNewStation(firstStation, stationToAdjust);
    struct station *currentStation = &nullStation;
    stationToAdjust->stationColor = RED;


    do {
        if (stationToAdjust == &nullStation) {
            if (stationToAdjust->father == &nullStation) {
                stationToAdjust->father->father = &nullStation;
                stationToAdjust->father->father->left = &nullStation;
                stationToAdjust->father->father->right = &nullStation;
                stationToAdjust->father->father->father = &nullStation;
            }
            stationToAdjust->father = &nullStation;
            stationToAdjust->father->father = &nullStation;
            stationToAdjust->father->left = &nullStation;
            stationToAdjust->father->right = &nullStation;
        }

        if (stationToAdjust->father == stationToAdjust->father->father->left) {
            currentStation = stationToAdjust->father->father->right;
            if (currentStation->stationColor == RED) {
                stationToAdjust->father->stationColor = BLACK;
                currentStation->stationColor = BLACK;
                stationToAdjust->father->father->
                        stationColor = RED;
                stationToAdjust = stationToAdjust->father->father;
            } else if (stationToAdjust == stationToAdjust->father->right) {
                stationToAdjust = stationToAdjust->father;
                leftRotate(firstStation, stationToAdjust
                );
            }
            stationToAdjust->father->stationColor = BLACK;
            stationToAdjust->father->father->stationColor = RED;
            rightRotate(firstStation, stationToAdjust->father->father);
        } else {
            if (stationToAdjust->father->father == &nullStation) {
                stationToAdjust->father->father->left = &nullStation;
                stationToAdjust->father->father->right = &nullStation;
                stationToAdjust->father->father->father = &nullStation;

            } else currentStation = stationToAdjust->father->father->left;

            if (currentStation->stationColor == RED) {
                stationToAdjust->father->
                        stationColor = BLACK;
                currentStation->stationColor = BLACK;
                stationToAdjust->father->father->
                        stationColor = RED;
                stationToAdjust = stationToAdjust->father->father;
            } else if (stationToAdjust == stationToAdjust->father->left) {
                stationToAdjust = stationToAdjust->father;
                rightRotate(firstStation, stationToAdjust
                );
            }
            stationToAdjust->father->
                    stationColor = BLACK;
            stationToAdjust->father->father->
                    stationColor = RED;
            leftRotate(firstStation, stationToAdjust
                    ->father->father);
        }

    } while ((stationToAdjust != firstStation) && (stationToAdjust->father->stationColor == RED));
}

char printColor(struct station *node){
    if(node->stationColor==BLACK)return 'B';
    else return 'R';
}

void inorderVisitToTheHighway(struct station *firstStation) {
    if (firstStation == &nullStation)
        return;
    inorderVisitToTheHighway(firstStation->left);
    printf("\nstation: %d ", firstStation->distance);
    printf("parent: %d ", firstStation->father->distance);
    printf("left sister: %d ", firstStation->left->distance);
    printf("right sister: %d ", firstStation->right->distance);
    printf("color: %c ", printColor( firstStation));
    inorderVisitToTheHighway(firstStation->right);
}

struct station *nextStation(struct station *currentStation) {
    //TODO from cormen

    return currentStation;
}

void deleteFixupStationOnTheHighway(struct station *firstStation, struct station *stationToRemove) {
    struct station *currentStation;
    do {
        if (stationToRemove == stationToRemove->father->left) {
            currentStation = stationToRemove->father->right;
            if (currentStation->stationColor == RED) {
                currentStation->stationColor = BLACK;
                stationToRemove->stationColor = RED;
                leftRotate(firstStation, stationToRemove->father);
                currentStation = stationToRemove->father->right;
            }
            if (currentStation->left->stationColor == BLACK && currentStation->right->stationColor == BLACK) {
                currentStation->stationColor = RED;
                stationToRemove = stationToRemove->father;
            } else if (currentStation->right->stationColor == BLACK) {
                currentStation->left->stationColor = BLACK;
                currentStation->stationColor = RED;
                rightRotate(firstStation, currentStation);
                currentStation = stationToRemove->father->right;
            }
            currentStation->stationColor = stationToRemove->father->stationColor;
            stationToRemove->father->stationColor = BLACK;
            currentStation->right->stationColor = BLACK;
            leftRotate(firstStation, stationToRemove->father);
            stationToRemove = firstStation;

        }
    } while (stationToRemove != firstStation && stationToRemove->stationColor == BLACK);
    stationToRemove->stationColor = BLACK;
}

struct station *findStation(int positionByDistance) {
    //TODO from cormen
    struct station *ah = malloc(sizeof(struct station));
    return ah;
}

void stationRemove(struct station *firstStation, struct station *stationToRemove) {
    struct station *currentStation = &nullStation;
    struct station *otherStation;


    if (stationToRemove->left == &nullStation || stationToRemove->right == &nullStation) {
        *currentStation = *stationToRemove;
    } else currentStation = nextStation(stationToRemove);
    if (currentStation->left != &nullStation) {
        otherStation = currentStation->left;
    } else otherStation = currentStation->right;
    otherStation->father = currentStation->father;
    if (currentStation->father == &nullStation) {
        firstStation = otherStation;
    } else if (currentStation == currentStation->father->left) {
        currentStation->father->left = otherStation;
    } else currentStation->father->right = otherStation;
    if (currentStation != stationToRemove) {
        stationToRemove->distance = currentStation->distance;
        i=0;
        while (stationToRemove->cars[i] != -1) {
            stationToRemove->cars[i] = currentStation->cars[i];
            i++;
        }
        stationToRemove->father = currentStation->father;
        stationToRemove->left = currentStation->left;
        stationToRemove->right = currentStation->right;
    }
    if (currentStation->stationColor == BLACK) deleteFixupStationOnTheHighway(highway, otherStation);
}

//--------------------------------------------------------------------------Requested Functions--------------------------------------------------------------------------//
void removeStationAtDistance(int distance) {

    struct station *foundStation = findStation(distance);
    stationRemove(highway, foundStation);
}

//TODO implementare un sono nullStation da sostituire a tutti i null delle funzioni di apposggio, ricordarsi che la right, la left e la father vanno al null vero, il colore invece sempre black
//----------------------------------------------------------------------------Main Functions-----------------------------------------------------------------------------//
int main() {
    highway = malloc(sizeof(struct station));
    highway->distance = 0;
    highway->cars[0] = 15;
    highway->stationColor = BLACK;
    highway->father = &nullStation;
    highway->left = &nullStation;
    highway->right = &nullStation;

    for (i = 0; i < 3; i++) {
        struct station *temporaryStation = malloc(sizeof(struct station));
        temporaryStation->distance = 1 + i;
        temporaryStation->cars[0] = 1;
        temporaryStation->stationColor = RED;
        temporaryStation->father = &nullStation;
        temporaryStation->left = &nullStation;
        temporaryStation->right = &nullStation;
        insertStationAndFixItUp(highway, temporaryStation);

    }
    inorderVisitToTheHighway(highway);


}

/**
 * USARE LA SCANF RIPETUTA PER LEGGERE PIU PAROLE NELLA STESSA RIGA
 * AD ESEMPIO:
 * CIAO COME STAI
 * SCANF("%S"); PER LEGGERE CIAO
 * SCANF("%S"); PER LEGGERE COME
 * SCANF("%S"); PER LEGGERE STAI
 * ANCHE SE LE METTO UNA A CAPO DELL'ALTRA LA SCNF LEGGE OGNI PAROLA SULLA STESSA RIGA PERCHE SPOSTA IL PUNTATORE ALL STDIN E AL BUFFER DI LETTURA
 */
