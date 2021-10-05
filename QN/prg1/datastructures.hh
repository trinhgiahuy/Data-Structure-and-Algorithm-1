// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <map>
#include <unordered_map>
#include <algorithm>

// If you cannot find, the document file of this program has name "document".
// The notation of performance of functions are written in document file.


// Types for IDs
using StopID = long int;
using RegionID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y).
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Type for a stop.
struct Stop
{
    StopID id = NO_STOP;
    Name stop_name = NO_NAME;
    Coord location;
    RegionID regionID;
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

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1 == c2); } // Not strictly necessary

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    // Use distance in square to avoid using sqrt().

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

// Return value for cases where coordinates were not found
Coord const NO_COORD = { NO_VALUE, NO_VALUE };


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
};

#endif // DATASTRUCTURES_HH
