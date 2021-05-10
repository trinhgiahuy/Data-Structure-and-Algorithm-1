// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH
#include <limits>
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <algorithm>
#include <functional>
#include <set>
#include <map>
#include <list>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <memory>
#include <algorithm>


// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
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
//Phase 2 using


using ways_vec = std::vector<WayID>;
using queue_list = std::list<Coord>;
using parent_map = std::unordered_map<Coord,std::pair<WayID,Coord>,CoordHash>;
using path_vec = std::vector<std::pair<WayID,Coord>>;
using return_tuple = std::vector<std::tuple<Coord,WayID,Distance>>;
using return_tuple_without_dist = std::vector<std::tuple<Coord,WayID>>;

using complex = std::unordered_map<WayID,std::pair<Coord,Coord>>;

struct Edge
{
    Coord fromscoord = NO_COORD;
    Coord tocoord = NO_COORD;
    WayID way_id = NO_WAY;
    Distance dist = 0;
};

struct CoordMapWay {
    //std::unordered_map<WayID,std::pair<Coord,Coord>> ways_ = {} ;
    complex ways_;
    complex crossRoadWays_;
    std::unordered_map<WayID,Edge,CoordHash> out_edges = {};
};
//WayID isIntersecting(ways_vec* s_visited, ways_vec *t_visited);
//void BFS(queue_list *queue, ways_vec* visited, parent_map *parent, bool flow);
//void DFS(ways_vec *visited, parent_map *parent, PlaceID cur_play,
//         std::tuple<WayID, PlaceID, PlaceID> *return_pair);
path_vec bidirPath(parent_map *s_parent, parent_map *t_parent,
                   PlaceID fromplace, PlaceID toplace, PlaceID intersectNode);

using pathEdge = std::vector<Edge>;

// Used for storing where a Coordinate is already discovered by Dijkstra algorithm
// and the edge that lead to that coord. Except for the last coord with
// NO_COORD since the journey doesn't continue.
using travelHistory = std::unordered_map<Coord,Edge,CoordHash>;

// Used in Dijkstra algorithm for storing temporarily found minimum weigh.
using Label = std::unordered_map<Coord, int,CoordHash>;

// Used for infinite weight in finding shortest path alogrithms.
///int const INF = std::numeric_limits<int>::max();
class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    int place_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> all_places();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_area_name(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_areas();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_place(PlaceID id);

    // Estimate of performance:
    // Short rationale for estimate:
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

    // Phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<WayID> all_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_way(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

    long long int distance_square(const Coord& xy1,const Coord& xy2);

    std::vector<Coord> getAllWayCoord();

    bool checkCoordExist(std::vector<Coord> coord_vct_, Coord coord_);

    void updateAllWayCoord(std::vector<Coord> coord_vct_);

    void BFS(std::list<Coord> *queue, std::vector<Coord> *visited, parent_map *parent,
             bool flow);

    bool DFS(std::vector<Coord> *visited,parent_map *parent,
             Coord cur_coord, std::tuple<WayID,Coord,Coord> *return_pair);

    Coord isIntersecting(std::vector<Coord> *fw_visited, std::vector<Coord> *bw_visited);

    std::vector<std::pair<WayID,Coord>> bidirPath(parent_map *fw_parent, parent_map *bw_parent, Coord frommcoord, Coord tocoord, Coord intersectNode);

    return_tuple getTuple(path_vec *path);

    return_tuple_without_dist getTupleWithoutDist(path_vec *path);

    Distance getDistance(WayID fromway);

    Coord getEndingCoord(Coord prevCoord, WayID way);

    pathEdge path_breadth_first_search(Coord fromcoord, Coord toCoord);

    pathEdge make_path(Coord fromcoord, Coord tocoord, travelHistory& coord_visited, bool with_cycle, Edge last_edge = Edge());

    return_tuple make_route_distance(pathEdge path_by_edges);

    bool isCrossRoad(Coord coordCheck, WayID wayCheck);
private:
    struct Way{
        //WayID id_;
        std::vector<Coord> coords_ = {};

        std::vector<Coord> crossRoads_vct = {};

        Coord dist_;
        //std::unordered_map<WayID,std::pair<Coord,Coord>> ways_ = {} ;
    };
    //Phase 1 operation
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

    // Phase 2 operation
    // Add stuff needed for your class implementation here

    // Error : call to implicitly-deleted default constructor of unordered_map<Coord,CoordMapWay> '
    /**/


    std::unordered_map<Coord,CoordMapWay,CoordHash> coords_map;
    //*/
    //std::unordered_map<Coord,Way> coords_map;



    std::vector<Coord> all_ways_coord_vct;
    std::unordered_map<WayID,Way> ways_map;


    //std::unordered_map<Coord,std::vector<Way*>> all_coord_way_map;

    bool existCoord(Coord id);
    bool existWay(WayID id);

};

#endif // DATASTRUCTURES_HH
