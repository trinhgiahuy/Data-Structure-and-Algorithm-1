1. CHOOSING DATA STRUCTURE FOR WAY

1.1. STRUCT OF WAY
   
	+ "coords_" : Vector of Coord type. It represents the coordinate of the way included the mid point
	
	+ "crossRoads_vct" : vector of Coord. It represents the coordinate of 2 crossroads of that way.
	
	+ 
1.2. STRUCT OF CoordMapWay

	+ "ways" : Connection type: It present a WayID with a pair of adjacent coordinate of that coordinate. 
								If the coordinate is crossroad. either of its pair element is NO_COORD
	
	+ "crossRoadWays_" : Connection type: Same as ways with different that included only the map of crossroad.

2.1. DATA MEMBER OF CLASS "Datastructures"

Datastructures class has private date members:

				   
	+ "ways_map":  An unordered map : It map the wayID to a struct of Way. The reason for choosing unordered map is that
	in some operation such as remove element or search element, its time complexcity in average case is lower than other 
	struct.
	
	+ "coords_map": An unordered map : It take each coordinate as a key and map its to struct CoordMapWay. The reason for choosing 
	unordered map is that in some operation such as remove element or search element, its time complexcity in average case is lower
	than other struct.
	
2.2 CHOOSING IMPLEMENTING OF PUBLIC MEMBER FUNCTION

	+ "BFS": A function that do the breadth_first_search for 2 given coordinate.
	
	+ "DFS": A function do Depth-first search for function route_with_cycle.
	
	+ "isIntersecting" : A function will take 2 vector of forward and backward visited coordinate and find its intersection between
	them. If no intersection found, it will return NO_COORD;
	
	+ "bidirPath": A function will return a return_tuple type which take 2 unordered map of parent contain visited vertex, the destination 
	and starting Way and interscting way of 2 given ways. It will combined 2 forward and backward search from BFS and return into one given type
	
	+ "return_tuple": A function wall take path_vect type combined with distance calculated using function getDistance to return a return_tuple type
	
	+ "return_tuple_without_dist" : A same with return_tuple function except it does not return a distance.
	
	+ "getDistance": A function will calculate the distance from a given wayID input.
	
	