// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <queue>


// Types for IDs
using StopID = long int;
using RegionID = std::string;
using RouteID = std::string;
using Name = std::string;


// Return values for cases where required thing was not found
RouteID const NO_ROUTE = "!!NO_ROUTE!!";
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Type for time of day in minutes from midnight (i.e., 60*hours + minutes)
using Time = int;

// Type for a distance (in metres)
using Distance = int;

// Type for a duration of time (in minutes)
using Duration = int;

// Time depart from on stop and arrive to next adjacenet stop.
using TimeTravel = std::pair<Time, Time>;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Used for infinite weight in finding shortest path alogrithms.
int const INF = std::numeric_limits<int>::max();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Return value for cases where coordinates were not found
Coord const NO_COORD = { NO_VALUE, NO_VALUE };

// Return value for cases where color was not found
Time const NO_TIME = std::numeric_limits<Time>::min();

// Return value for cases where Duration is unknown
Duration const NO_DURATION = NO_VALUE;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;

// TripID that include the routeID of the trip and the index number denote the
// order in which the trip is added in the route.
struct TripID
{
    RouteID route_id = NO_ROUTE;
    int trip_index = NO_VALUE;
};

// Edge uses for graph algorithms.
struct Edge
{
    StopID from_stop_ID = NO_STOP;
    StopID to_stop_ID = NO_STOP;

    RouteID route_ID = NO_ROUTE;
    Distance dist = 0;

    // List of all trips that pass through this edge and information about time
    // travel between both ends of edge and trip information to detect bus
    // change.
    std::vector<std::pair<TripID, TimeTravel>> bus_travel = { };

    Time walking_time = NO_TIME;
};

struct Trip
{
    StopID start_stop;
    std::unordered_map<StopID, Time> timetable;
};

// Type for a stop.
struct Stop
{
    StopID id = NO_STOP;
    Name stop_name = NO_NAME;
    Coord location;
    RegionID regionID;

    std::unordered_map<RouteID, Edge> in_edges;
    std::unordered_map<RouteID, Edge> out_edges;

    std::vector<Edge> walking_to = { };
};

// Type for a region.
struct Region
{
    RegionID id = NO_REGION;
    Name region_name = NO_NAME;
    Region* parent = NULL;
    std::vector<Region*> children = { };
    std::vector<Stop*> stop_in_region = { };
};

// Type for a route.
struct Route
{
    RouteID id = NO_ROUTE;
    std::vector<StopID> stops_on_route = { };
    std::vector<Trip> trip_list = { };
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1 == c2); } // Not strictly necessary

// Example: Defining < for Coord so that it can be used
// as key for std::map/set

// Use distance in square to avoid using sqrt().
inline bool operator<(Coord c1, Coord c2)
{
    long long int distance1 = (long long int)c1.x * c1.x +
        (long long int)c1.y * c1.y;
    long long int distance2 = (long long int)c2.x * c2.x +
        (long long int)c2.y * c2.y;

    if (distance1 < distance2)
    {
        return true;
    }

    if (distance1 == distance2 && c1.y < c2.y)
    {
        return true;
    }

    return false;
}


// Used in Dijkstra algorithm for finding shortest distance between stops.
using MinWeightHeap = std::priority_queue<std::pair<int, StopID>,
    std::vector<std::pair<int, StopID>>,
    std::greater<std::pair<int, StopID>>>;

// Used in Dijkstra algorithm for storing temporarily found minimum weigh.
using Label = std::unordered_map<StopID, int>;

// Used for storing where a stop is already discovered by Dijkstra algorithm
// and the edge that lead to that stop. Except for the last stop with route ID
// is NO_ROUTE since the journey doesn't continue.
using TravelHistory = std::unordered_map<StopID, Edge>;

// Journey information with stop ID, route ID to take to the next stop and
// total distance from startting stop to current stop. Except for the last stop
// the route ID is NO_ROUTE and time information is the arrival time since the
// journey doesn't continue.
using JourneyDistance = std::vector<std::tuple<StopID, RouteID, Distance>>;

// Journey information with stop ID, route ID to take to the next stop and
// departure time from current stop.
using JourneyTime = std::vector<std::tuple<StopID, RouteID, Time>>;

// Path make by list of successive edges.
using PathEdge = std::vector<Edge>;


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1).
    // Short rationale for estimate: The main contribution to the complexity
    // of the function is map::size() function with complexity is constant.
    int stop_count();

    // Estimate of performance: O(n).
    // Short rationale for estimate: The complexity of the function is linear
    // of the number of stop, that is map::clear() and linear of the number
    // of the region, that is vector::<Region*>::clear().
    void clear_all();

    // Estimate of performance: O(n).
    // Short rationale for estimate: The main contribution to the complexitiy
    // of the function is loop through all key in the map to assign them into
    // the returned vector.
    std::vector<StopID> all_stops();

    // Estimate of performance: O(1) or O(log(n))
    // Short rationale for estimate: Before creation_finished() is call the
    // method do the work add new element to unordered map with take constant
    // time. After creation_finished() called the main contribution to the
    // complexity of the function is using map::insert() which has time
    //complexity of logarithm. Also map::find() has time complexity of log(n).
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Estimate of performance: O(1).
    // Short rationale for estimate: Retrieve element which a given key of a
    // map takes constant time.
    Name get_stop_name(StopID id);

    // Estimate of performance: O(1).
    // Short rationale for estimate: Retrieve element which a given key of a
    // map takes constant time.
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlog(n)) or O(n).
    // Short rationale for estimate: Before creation_finished() is called the
    // method sorts the stop in alphabetical order with take O(nlog(n)) on
    // average. After creation_finshed() is called function loops through all
    // stops in order of their name in map stop_name_order.
    std::vector<StopID> stops_alphabetically();

    // Estimate of performance: O(nlog(n) or O(n).
    // Short rationale for estimate: Before creation_finished() is called the
    // method sorts the stop in coordinates order with take O(nlog(n)) on
    // average. After creation_finshed() is called function loops through all
    // stops in order of their coordinates in map stop_coord_order.
    std::vector<StopID> stops_coord_order();

    // Estimate of performance: O(n) or O(1).
    // Short rationale for estimate: Before creation_finished() is called the
    // method loops through all the stop to find stop with min coordinates.
    // After creation_finished() is called function gets the first element from
    // the vector of pointer to stops with the minimum key value from a map and
    // then retrieve id of the stop take constant time.
    StopID min_coord();

    // Estimate of performance: O(n) or O(1).
    // Short rationale for estimate: Before creation_finished() is called the
    // method loops through all the stop to find stop with max coordinates.
    // After creation_finished() is called function gets the first element from
    // the vector of pointer to stops with the maximum key value from a map and
    // then retrieve id of the stop take constant time.
    StopID max_coord();

    // Estimate of performance: O(log(n)) or in worst case O(n).
    // Short rationale for estimate: find the pointer to Stop list take
    // logarithm time. In the worst case all stops have the same name
    // then copy all stops return takes linear time complexity.
    std::vector<StopID> find_stops(Name const& name);

    // Estimate of performance: O(1) or O(log(n)).
    // Short rationale for estimate: Before creation_finished() is called the
    // the method access element in an unordered map that take constant time.
    // After creation_finished() is called the method do more work that find
    // the name of stop in ordered map and delete pointer to stop that takes
    // O(log(n)) on average.
    bool change_stop_name(StopID id, Name const& newname);

    // Estimate of performance: O(1) or O(log(n))
    // Short rationale for estimate:  Before creation_finished() is called the
    // the method access element in an unordered map that take constant time.
    // After creation_finished() is called the method do more work that finds
    // the coordinates of stop in ordered map and delete pointer to stop that
    // takes O(log(n)) on average.
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1).
    // Short rationale for estimate: Find an element with given key and insert
    // new element to unordered_map take constant time.
    bool add_region(RegionID id, Name const& name);

    // Estimate of performance: O(1).
    // Short rationale for estimate: The function use unordered_map::find and
    // unordered_map::at with take constant time on average.
    Name get_region_name(RegionID id);

    // Estimate of performance: O(m).
    // Short rationale for estimate: Iterate all the region in the region list
    // and assign key value to a vector take linear time complexity.
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(1).
    // Short rationale for estimate: The function uses unordered_map::find and
    // unordered_map::at that take constant time on average.
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Estimate of performance: O(1)
    // Short rationale for estimate: The function uses unordered_map::find and
    // unordered_map::at that take constant time on average.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(m) which m is the number of regions that one
    // one stop can belong to.
    // Short rationale for estimate: The function iterate all region that the
    // given stop belonged to. If the stop belongs to one region and that
    // region does not belong to any region then the complexity is constant
    // time.
    std::vector<RegionID> stop_regions(StopID id);

    // Non-compulsory operations

    // Estimate of performance: O(nlog(n)).
    // Short rationale for estimate: Create two ordered maps with key is the
    // name of coordinates of the stops for further use in return the stop
    // in alphabetically order and coordinates order after creation finsihed.
    void creation_finished();

    // Estimate of performance: Worst case O(mn).
    // Short rationale for estimate: The complexity of the function is also the
    // complexity of the region_traveral() function.
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The function loops through all the stops
    // to find at most five nearest stop to the stop considered.s
    std::vector<StopID> stops_closest_to(StopID id);

    // Estimate of performance: O(m) or O(log(n)).
    // Short rationale for estimate: Before creation finished the function
    // delete the stop from the list stop_in_region of the region that the
    // stop belongs to in the worst case can be linear on the number of
    // stop. After creation finsihed the function do more work like delete
    // the pointer to stop in stop_name_order and stop_coord_order map that
    // take O(log(n)) to element with key and may take from O(1) to (n) to
    // delete that element.
    bool remove_stop(StopID id);

    // Estimate of performance: Worst case O(m).
    // Short rationale for estimate: The function find the path from each stop
    // to the highest region that contains it. The compare the path to find the
    // lowest common region. In the worst case functions loop through all the
    // region in the data structures.
    RegionID stops_common_region(StopID id1, StopID id2);

    // Remember we use the distance in square.
    // Estimate of performance: O(1).
    // Short rational for estimate: Calculate the square of euclidean distance.
    long long int distance_square(const Coord& xy1, const Coord& xy2);

    // Estimate of performance: O(1).
    // Short rationale for estimate: The check the coordinate of the current
    // upper left corner and lower right corner and compare them to coordinate
    // of the stop and update them.
    void update_bounding_box(const Coord& stop_location,
        Coord& lower_left, Coord& upper_right);

    // Estimate of performance: O(mn).
    // Short rationale for estimate: The function loops for all stops of a
    // region and all stops of its subregion. So depends on the distribution of
    // stop in a region. In the worst case function loops through all the stops
    // in the data structures.
    void region_traversal(Region& root, Coord& lower_right, Coord& upper_left);

    // Phase 2 operations

    // Estimate of performance: O(R).
    // Short rationale for estimate: Funtion makes a vector that contain all
    // routes that takes linear time.
    std::vector<RouteID> all_routes();

    // Estimate of performance: O(n_r).
    // Short rationale for estimate: Funtion make edge between stops on a route
    // and create route with stop information that take constant time on the
    // number of stops on route on average.
    bool add_route(RouteID id, std::vector<StopID> stops);

    // Estimate of performance: O(r_n * t). t is average number of trip per
    // route.
    // Short rationale for estimate: Funtion makes a vector that contain all
    // stops that can go directly with route that pass through current stop
    // that take linear time on the number of route on a stop.
    std::vector<std::pair<RouteID, StopID>> routes_from(StopID stopid);

    // Estimate of performance: O(1).
    // Short rationale for estimate: Funtion checks if the ID is valid and
    // return stop on route in a vector that already stored. Both operation
    // take constant time on average.
    std::vector<StopID> route_stops(RouteID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Funtion loops over all stops to remove all
    // connections between stops except walking connection and then clear all
    // information about route in route list. Both operation take linear time
    // on its element on average: O(n, r) approximates O(n).
    void clear_routes();

    // Estimate of performance: O(max(n, e)) in worst case.
    // Short rationale for estimate: Function has the same complexity as
    // journey_least_stop().
    JourneyDistance journey_any(StopID fromstop, StopID tostop);

    //    // Non-compulsory operations

    // Estimate of performance: O(max(n, e)) in worst case.
    // Short rationale for estimate: Complexity of function depends on
    // complexity of find operation in unordered map which is O(n) in worst case
    // complexity of path_breadth_first_search() which is O(e) in worst case
    // and complexity of make_journey_distance with is O(L) on average.
    JourneyDistance journey_least_stops(StopID fromstop, StopID tostop);

    // Estimate of performance: O(max(n, e)) in worst case.
    // Short rationale for estimate: Complexity of function depends on
    // complexity of find operation in unordered map which is O(n) in worst case
    // complexity of path_depth_first_search() which is O(e) in worst case
    // and complexity of make_journey_distance which is O(L) on average.
    JourneyDistance journey_with_cycle(StopID fromstop);

    // Estimate of performance: O((n + e)log(n)) in worst case.
    // Short rationale for estimate: Complexity of function depends on
    // complexity of shortest_path_distance() which is O((n + e)log(n)) in
    // worst case and complexity of make_journey_distance which is O(L) on
    // average.
    JourneyDistance journey_shortest_distance(StopID fromstop, StopID tostop);

    // Estimate of performance: O(n_r).
    // Short rationale for estimate: Function adding information about time
    // and depart to each stop. Then it also adding information to each edge
    // related. The complexity on average is linear in the number of stop on
    // route n_r.
    bool add_trip(RouteID routeid, const std::vector<Time>& stop_times);

    // Estimate of performance: O(r_n).
    // Short rationale for estimate: Function check if the stop is on the route
    // and if possible make a vector of all possilbe time depart from the stop.
    // Both ope
    std::vector<std::pair<Time, Duration> > route_times_from(RouteID routeid, StopID stopid);

    // Estimate of performance: O((n + e)log(n)) in worst case.
    // Short rationale for estimate: Complexity of function depends on
    // complexity of shortest_path_duration() which is O((n + e)log(n)) in
    // worst case and complexity of make_journey_time which is O(L) on
    // average.
    JourneyTime journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime);

    // Estimate of performance: O(n^2)
    // Short rationale for estimate: Function loops over all stops and for each
    // stop function call the method stops_closest_to() which has complexity
    // O(n) on average and make_edge_no_trip which has complexity O(1) on
    // average.
    void add_walking_connections(); // Note! This method is completely optional, and not part of any testing


    // Estimate of performance: O(e) in worst case.
    // Short rationale for estimate: Implementing breath first search algorithm
    // that takes linear time on the total number of edge in the worst case.
    std::vector<Edge> path_breadth_first_search(StopID fromstop, StopID tostop);

    // Estimate of performance: O(e) in worst case.
    // Short rationale for estimate: Implementing depth first search algorithm
    // that takes linear time on the total number of edge in the worst case.
    std::vector<Edge> path_depth_first_search(StopID fromstop);

    // Estimate of performance: O(e) in worst case.
    // Short rationale for estimate: Function loops over all edge in the path
    // and get information to get a jounrney with take linear of the total edge
    // in worst case.
    JourneyDistance make_journey_distance(PathEdge path_by_edge);

    // Estimate of performance: O(e) in worst case.
    // Short rationale for estimate: Function loops over all edge in the path
    // and get information to get a jounrney with take linear of the total edge
    // in worst case.
    JourneyTime make_journey_time(PathEdge path_by_edge, Time starttime);

    // Estimate of performance: O((n + e)log(n)).
    // Short rationale for estimate: Implement of Dijkstra algorithm with
    // weight between each connection is the distance between two stops that it
    // connects.
    std::vector<Edge> shortest_path_distance(StopID fromstop, StopID tostop);

    // Estimate of performance: O((n + e)log(n)).
    // Short rationale for estimate: Implement of Dijkstra algorithm with
    // the label of each stop is the arrival time to that stops.
    std::vector<Edge> shortest_path_duration(StopID fromstop, StopID tostop,
        Time starttime);

    // Estimate of performance: O(e) in worst case.
    // Short rationale for estimate: Function goes from tostop getting
    // information of edge that lead to tostop and repeat those steps until
    // arrive fromstop. This operation takes linear on the number of total edge
    // in the worst case.
    std::vector<Edge> make_path(StopID fromstop, StopID tostop,
        TravelHistory& stop_visited, bool with_cycle,
        Edge last_edge = Edge());

    // Estimate of performance: O(log(n)) in worst case.
    // Short rationale for estimate: Compare the current distance and the new
    // distance obtained from new path. Adding information to unordered map
    // that takes complexity of O(1) and a priority queue that takes complexity
    // of O(log(n)). So the complexity of function is O(log(n)).
    void relax_distance(Edge connected_edge, Label& stop_label,
        TravelHistory& stop_visited, MinWeightHeap& min_heap);

    // Estimate of performance: O(log(n)) in worst case.
    // Short rationale for estimate: Compare the arrival time and the new
    // arrival time obtained from new path. Adding information to unordered map
    // that takes complexity of O(1) and a priority queue that takes complexity
    // of O(log(n)). So the complexity of function is O(log(n)).
    void relax_time(Edge connected_edge, Label& stop_label,
        TravelHistory& stop_visited, MinWeightHeap& min_heap);

    // Estimate of performance: Average O(1). Worst case O(n).
    // Short rationale for estimate: Function check if the stop and route ID
    // are valid then create an edge. Find element with key in unordered_map
    // that take constant time on average and linear time in worst case.
    Edge make_edge_no_trip(StopID fromstop, StopID tostop, RouteID id);

    bool change_bus(Edge next_edge, Edge prev_edge);

    PathEdge find_path(StopID fromstop, StopID tostop, Time starttime, Time soonest_arrival_time);


private:

    // Store all stops by their unique ID.
    std::unordered_map<StopID, Stop> stop_list;

    // Marked the status of program.
    bool create_finished;

    // Store all stops by their name.
    std::map<Name, std::vector<Stop*>> stop_name_order;

    // Store all stops by their coordinates.
    std::map<Coord, std::vector<Stop*>> stop_coord_order;

    // Store all regions by their unique ID.
    std::unordered_map<RegionID, Region> region_list;

    // Store all route by their route ID.
    std::unordered_map<RouteID, Route> route_list;

    // Walking speed in m/s of a person to move between stops.
    const double walking_speed = 1;

};

#endif // DATASTRUCTURES_HH
