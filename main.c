#include <stdio.h>
#include <stdlib.h>
#define maxNumberOfCars 512


struct station {
    int distance;
    int cars[maxNumberOfCars];
    char stationColor;
    struct station* father;
    struct station* right;
    struct station* left;
};


struct station* highway = NULL;


struct station* insertNewStation(struct station* firstStation, struct station* currentStation)
{


    if (firstStation == NULL)
        return currentStation;


    if (currentStation->distance < firstStation->distance)
    {
        firstStation->left = insertNewStation(firstStation->left, currentStation);
        firstStation->left->father = firstStation;
    }
    else if (currentStation->distance > firstStation->distance)
    {
        firstStation->right = insertNewStation(firstStation->right, currentStation);
        firstStation->right->father = firstStation;
    }


    return firstStation;
}



void rightrotate(struct station* currentStation)
{
    struct station* left = currentStation->left;
    currentStation->left = left->right;
    if (currentStation->left)
        currentStation->left->father = currentStation;
    left->father = currentStation->father;
    if (!currentStation->father)
        highway = left;
    else if (currentStation == currentStation->father->left)
        currentStation->father->left = left;
    else
        currentStation->father->right = left;
    left->right = currentStation;
    currentStation->father = left;
}


void leftrotate(struct station* currentStation)
{
    struct station* right = currentStation->right;
    currentStation->right = right->left;
    if (currentStation->right)
        currentStation->right->father = currentStation;
    right->father = currentStation->father;
    if (!currentStation->father)
        highway = right;
    else if (currentStation == currentStation->father->left)
        currentStation->father->left = right;
    else
        currentStation->father->right = right;
    right->left = currentStation;
    currentStation->father = right;
}


void fixupStationInsideTheHighway(struct station* firstStation, struct station* currentStation)
{
    struct station* fatherCurrentStation = NULL;
    struct station* grandFatherCurrentStation = NULL;

    while ((currentStation != firstStation) && (currentStation->stationColor != 0)
           && (currentStation->father->stationColor == 1))
    {
        fatherCurrentStation = currentStation->father;
        grandFatherCurrentStation = currentStation->father->father;


        if (fatherCurrentStation == grandFatherCurrentStation->left)
        {

            struct station* uncle_currentStation = grandFatherCurrentStation->right;


            if (uncle_currentStation != NULL && uncle_currentStation->stationColor == 1)
            {
                grandFatherCurrentStation->stationColor = 1;
                fatherCurrentStation->stationColor = 0;
                uncle_currentStation->stationColor = 0;
                currentStation = grandFatherCurrentStation;
            }

            else {


                if (currentStation == fatherCurrentStation->right) {
                    leftrotate(fatherCurrentStation);
                    currentStation = fatherCurrentStation;
                    fatherCurrentStation = currentStation->father;
                }


                rightrotate(grandFatherCurrentStation);
                int t = fatherCurrentStation->stationColor;
                fatherCurrentStation->stationColor = grandFatherCurrentStation->stationColor;
                grandFatherCurrentStation->stationColor = t;
                currentStation = fatherCurrentStation;
            }
        }


        else {
            struct station* uncle_currentStation = grandFatherCurrentStation->left;


            if ((uncle_currentStation != NULL) && (uncle_currentStation->stationColor == 1))
            {
                grandFatherCurrentStation->stationColor = 1;
                fatherCurrentStation->stationColor = 0;
                uncle_currentStation->stationColor = 0;
                currentStation = grandFatherCurrentStation;
            }
            else {

                if (currentStation == fatherCurrentStation->left) {
                    rightrotate(fatherCurrentStation);
                    currentStation = fatherCurrentStation;
                    fatherCurrentStation = currentStation->father;
                }


                leftrotate(grandFatherCurrentStation);
                char currentStationColor = fatherCurrentStation->stationColor;
                fatherCurrentStation->stationColor = grandFatherCurrentStation->stationColor;
                grandFatherCurrentStation->stationColor = currentStationColor;
                currentStation = fatherCurrentStation;
            }
        }
    }
}


void inorderVisitToTheHighway(struct station* firstStation)
{
    if (firstStation == NULL)
        return;
    inorderVisitToTheHighway(firstStation->left);
    printf("station: %d ", firstStation->distance);
    printf("car: %d ",firstStation->cars[0]);
    inorderVisitToTheHighway(firstStation->right);
}

int main()
{
    for(int i=0;i<3;i++){
        struct station* temporaryStation = malloc(sizeof(struct station));
        temporaryStation->distance=1+i;
        temporaryStation->cars[0]=1;
        temporaryStation->stationColor='r';
        temporaryStation->father=NULL;
        temporaryStation->left=NULL;
        temporaryStation->right=NULL;
        highway=insertNewStation(highway,temporaryStation);
    }
    inorderVisitToTheHighway(highway);
}