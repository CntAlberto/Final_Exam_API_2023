# Final Exam API-2023

This piece of code has inside my solution of one of the three final exams to reach Bachelor's Degree at Politecnico di Milano.
Specially this repository has the solution of API's Final Exam wich owner's Prof. Alessandro Barenghi <br> <br>

Consider a highway described as a sequence of service stations. Every gas station is located
at a distance from the start of the motorway expressed in kilometers by a positive or zero integer. They do not exist
two service stations having the same distance: each service station is therefore uniquely identified by
its distance from the start of the motorway. <br>
Each service station is equipped with a fleet of electric rental vehicles. Each vehicle is distinctive
by the autonomy given by one battery charge, expressed in kilometers, by a positive integer. The park
vehicles of a single station includes at most 512 vehicles. Renting a car from a station is possible
reach all those whose distance from you is less than or equal to the car's range. <br>
A trip is identified by a sequence of petrol stations where the driver stops. It then begins
at one service station and ends at another, passing through zero or more intermediate stations. Assume that the
driver cannot turn back during the trip, and rent a new car every time he stops in
a petrol station. Therefore, given two consecutive stages s and t, t must be further and further from the start
with respect to s, and t must be reachable using one of the vehicles available in s. <br>
The objective of the project is the following: given a pair of stations, plan the route with the fewest number
of stages between them. If there are multiple routes with the same minimum number of stages (i.e. equal merit),
the route that favors the stages with the shortest distance from the start of the motorway must be chosen. In other words,
consider the set of n equal paths P = {p1, p2, . . . pn} where each path is a tuple of m elements pi = ⟨pi,1, pi,2, . . . pi,m⟩
which are equivalent to the distance from the start of the motorway of each stage in order of travel.
The only path pi must be chosen such that there is no other pj with the same k final stages preceded by one
stage with shorter distance. <br>

## Input file format and expected printouts:

The input text file contains a sequence of commands, one per line, with the following format. All values
positive or null integers can be encoded in 32 bits. <br>

+ add-station distance car-number range-car-1 ... range-car-n:
Adds a station, located at the indicated distance, equipped with car number, with the indicated autonomy. <br>
Print expected as response: "aggiunta" or "non aggiunta"

+ demolish-station distance: 
Removes the station located at the indicated distance, if it exists. <br>
Print expected as response: "demolita" or "non demolita"

+ add-car station-distance car-range-to-add:
If the station exists, add a car to it. It is possible to have multiple cars with the same range. <br>
Print expected as response: "aggiunta" or "non aggiunta".

+ scrap-car distance-station autonomy-car-to-scrap:
Removes a car from the indicated station, if the station exists and is equipped with at least one car
with the indicated autonomy. <br>
Print waiting for response: "rimossa" or "non rimossa"

+ plan-route distance-station-departure distance-station-arrival:
It requires you to plan your route with the above constraints.
Print expected as response: the stages in order of travel, represented with the distance to the stations
from the beginning of the motorway, separated by spaces and at the end followed by a new line. Departure and must be included departure 
and arrival; if they match the station is printed only once. If the path does not exist, print "nessun percorso". <br>
The planning action does not alter the stations or their vehicle fleets. The given stations are
definitely present.


