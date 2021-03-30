// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>

#include <unordered_map>
#include <memory>

// Types for IDs
using PlaceID = long int;
using AreaID = long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

//enum PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };
// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

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
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: the function .size() only takes constant time for unordered_map
    int place_count();

    // Estimate of performance:O(1)
    // Short rationale for estimate:function clear() takes constant time
    // set bool to false also constant time
    void clear_all();

    // Estimate of performance:Theta(n), O(n^2) - worst case rarely happens if hash function is good
    // Short rationale for estimate:a for loop obtaining element from unordered_map
    // takes linear time on average because accessing takes constant time.
    std::vector<PlaceID> all_places();

    // Estimate of performance:Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate:on average, function find() and insert()
    // on unordered_map takes constant time, but might be linear in worst case
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance:Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate:on average, function find() and at()
    // on unordered_map takes constant time, but might be linear in worst case

    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance:Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate:on average, function find() and at()
    // on unordered_map takes constant time, but might be linear in worst case
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:O(nlogn), Omega(1)
    // Short rationale for estimate:std::sort takes O(nlogn). However if we ran this function before
    // and nothing changes, we can just return it
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance:O(n)
    // Short rationale for estimate:We have to tranverse through all elements which takes linear time
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(n)
    // Short rationale for estimate:We have to tranverse through all elements which takes linear time
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate:find() and at() for unordered_map takes constant time on average
    // linear in the worst case
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate:find() and at() for unordered_map takes constant time on average
    // linear in the worst case
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate: on average, function find() and insert()
    // on unordered_map takes constant time, but might be linear in worst case
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance:Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate: on average, function find() and at()
    // on unordered_map takes constant time, but might be linear in worst case
    Name get_area_name(AreaID id);

    // Estimate of performance:Theta(1), O(n) - worst case rarely happens if hash function is good
    // Short rationale for estimate: on average, function find() and at()
    // on unordered_map takes constant time, but might be linear in worst case
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: Theta(n), O(n^2) - worst case rarely happens if hash function is good
    // Short rationale for estimate: a for loop obtaining element from unordered_map
    // takes linear time on average because accessing takes constant time
    std::vector<AreaID> all_areas();

    // Estimate of performance:O(1)
    //Short rationale for estimate: The function uses unordered_map::find and
    // unordered_map::at that take constant time on average.
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: Worst case O(n)
    // Short rationale for estimate: The function will go through a vector contains
    // all its direct and inderict parent area
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: O(n)
    // Short rationale for estimate:The function will go through a vector contains
    // all its direct and inderict children area. The first for loop will go through some
    // area only
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance:O(n)
    // Short rationale for estimate:The function loops through all the places
    // to find 3 nearest place to the place considered.s
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance:Worst case O(n),theta(1)
    // Short rationale for estimate:go through all element in unordered map to
    // find the given id,best case is the id is the first element
    bool remove_place(PlaceID id);

    // Estimate of performance: Worst case O(m)
    // Short rationale for estimate:The function find the path from each areaa
    // to the highest area that contains it. The compare the path to find the
    // lowest common area. In the worst case functions loop through all the
    // area in the data structures.
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // Remember we use the distance in square.
    // Estimate of performance: O(1).
    // Short rational for estimate: Calculate the square of euclidean distance.
    long long int distance_square(const Coord& xy1,const Coord& xy2);


private:
    // Add stuff needed for your class implementation here
    struct Place {
        PlaceID id;
        Name name;
        PlaceType type;
        Coord coord;
    };


    // A unordered map for mapping Place ID with pointer to its
    std::unordered_map<PlaceID, std::shared_ptr<Place>> id_data;

    //Vector that contain all places
    std::vector<PlaceID>all_places_vct;
    std::vector<AreaID>all_areas_vct;


    bool isNameSorted;
    bool isCoordSorted;

    //Vector sorted by name and coordinate that will be return
    std::vector<PlaceID> place_alpha;
    std::vector<PlaceID> place_coord;


    // Name here type string
    struct Area
    {
        AreaID id;
        Name name;
        std::vector<Coord> bounds_;
        AreaID parent_area_id;
        std::vector<AreaID> sources_nodes_;
    };

    // A unordered map for mapping Area ID with pointer to its
    std::unordered_map<AreaID,std::shared_ptr<Area>> area_data;

};

#endif // DATASTRUCTURES_HH
