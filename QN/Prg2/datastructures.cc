// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <stack>
#include <unordered_set>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end - start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range - 1)(rand_engine);

    return static_cast<Type>(start + num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)


Datastructures::Datastructures()
{
    // Initialize all map and unordered map is empty.
    // Initialize creation finished status of program.

    this->stop_list = std::unordered_map<StopID, Stop>();

    this->create_finished = false;

    this->stop_name_order = std::map<Name, std::vector<Stop*>>();
    this->stop_coord_order = std::map<Coord, std::vector<Stop*>>();

    this->region_list = std::unordered_map<RegionID, Region>();

    this->route_list = std::unordered_map<RouteID, Route>();
}

Datastructures::~Datastructures()
{
    // No dynamic allocation is used.
    // The destructtor of data member is called automatically.
}

int Datastructures::stop_count()
{
    return (int)stop_list.size();
}

void Datastructures::clear_all()
{
    // Clear all the map and unordered map.
    // Return the creation finished status to false.

    stop_list.clear();

    create_finished = false;

    stop_name_order.clear();
    stop_coord_order.clear();

    region_list.clear();

    route_list.clear();
}

// Create a vector contains of stop ID of all stops. The function return an
// empty vector when stop_list is empty.
std::vector<StopID> Datastructures::all_stops()
{
    if (stop_count() == 0)
    {
        return { };
    }

    std::vector<StopID> stopID_list;
    stopID_list.reserve(stop_count());

    for (auto stop : stop_list)
    {
        stopID_list.push_back(stop.first);
    }

    return stopID_list;
}

// Add stop to the stop lists. If creation_finished() had been called before
// function add the information of new stop to stop_name_order and
// stop_coord_order that keep tracks of the order of stop with corresponding
// to name and coordinates.
bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    if (stop_list.find(id) != stop_list.end())
    {
        return false;
    }

    stop_list.insert({ id, Stop{ id, name, xy, NO_REGION, { }, { } } });

    if (create_finished)
    {
        Stop* p_adding_stop = &stop_list.at(id);

        // Keep track of order of stops in name order.
        if (stop_name_order.find(name) == stop_name_order.end())
        {
            stop_name_order.insert({ name, { p_adding_stop } });
        }
        else
        {
            stop_name_order.at(name).push_back(p_adding_stop);
        }

        // Keep track of order of stops in coordinates order.
        if (stop_coord_order.find(xy) == stop_coord_order.end())
        {
            stop_coord_order.insert({ xy, { p_adding_stop } });
        }
        else
        {
            stop_coord_order.at(xy).push_back(p_adding_stop);
        }
    }

    return true;
}

// Function returns name of stop with given ID.
Name Datastructures::get_stop_name(StopID id)
{
    // Check if the ID is valid.
    if (stop_list.find(id) == stop_list.end())
    {
        return NO_NAME;
    }

    return stop_list.at(id).stop_name;
}

// Function returns the coordinates of stop with given ID.
Coord Datastructures::get_stop_coord(StopID id)
{
    // Check if the ID is valid.
    if (stop_list.find(id) == stop_list.end())
    {
        return NO_COORD;
    }

    return stop_list.at(id).location;
}

// Function returns a vector contains stops ID in alphabetical order of stop
// name. Stops with the same name can be in any order respect to each other.
std::vector<StopID> Datastructures::stops_alphabetically()
{
    if (stop_count() == 0)
    {
        return { };
    }

    std::vector<StopID> stopID_list;
    stopID_list.reserve(stop_count());

    if (create_finished == false)
    {
        for (auto stop : stop_list)
        {
            stopID_list.push_back(stop.first);
        }

        // Sort stops in alphabetical order of stop name.
        std::sort(stopID_list.begin(), stopID_list.end(),
            [this](StopID id1, StopID id2)
            { return stop_list.at(id1).stop_name <
            stop_list.at(id2).stop_name; });
    }
    else
    {
        // Get ID of stops.
        for (auto stop = stop_name_order.begin();
            stop != stop_name_order.end(); ++stop)
        {
            for (auto iter : stop->second)
            {
                stopID_list.push_back(iter->id);
            }
        }
    }

    return stopID_list;
}

// Function returns a vector of ID of the stop ID in coordinate order. Stops
// with the same location can be in any order with each others.
std::vector<StopID> Datastructures::stops_coord_order()
{
    if (stop_count() == 0)
    {
        return { };
    }

    std::vector<StopID> stopID_list;
    stopID_list.reserve(stop_count());

    if (create_finished == false)
    {
        for (auto stop : stop_list)
        {
            stopID_list.push_back(stop.first);
        }

        // Sort stops in coordinates order.
        std::sort(stopID_list.begin(), stopID_list.end(),
            [this](StopID id1, StopID id2)
            { return stop_list.at(id1).location <
            stop_list.at(id2).location; });
    }
    else
    {
        // Get ID of stops.
        for (auto stop = stop_coord_order.begin();
            stop != stop_coord_order.end(); ++stop)
        {
            for (auto iter : stop->second)
            {
                stopID_list.push_back(iter->id);
            }
        }
    }

    return stopID_list;
}

// Function returns the stop ID with smallest coordinates.
StopID Datastructures::min_coord()
{
    if (stop_count() == 0)
    {
        return NO_STOP;
    }

    StopID min_coord_id = stop_list.begin()->first;

    if (create_finished == false)
    {
        // Loop through all stops to find stop with smallest coordinates.
        for (auto stop : stop_list)
        {
            if (stop.second.location < stop_list.at(min_coord_id).location)
            {
                min_coord_id = stop.second.id;
            }
        }
    }
    else
    {
        min_coord_id = stop_coord_order.begin()->second.at(0)->id;
    }

    return min_coord_id;
}

// Function returns the stop ID with largest coordinates.
StopID Datastructures::max_coord()
{
    if (stop_count() == 0)
    {
        return NO_STOP;
    }

    StopID max_coord_id = stop_list.begin()->first;

    if (create_finished == false)
    {
        // Loop throught all stops to find stop with largest coordinates.
        for (auto stop : stop_list)
        {
            if (stop_list.at(max_coord_id).location < stop.second.location)
            {
                max_coord_id = stop.second.id;
            }
        }
    }
    else
    {
        max_coord_id = stop_coord_order.rbegin()->second.at(0)->id;
    }

    return max_coord_id;
}

// The function returns a vector of stop ID of all stops with the given name.
// If no stop is found function returns an empty vector.
std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    std::vector<StopID> stopID_list = { };

    if (create_finished == false)
    {
        // Search for stop with given name by loop through all keys of ID in
        // stop_list map.
        for (auto stop : stop_list)
        {
            if (stop.second.stop_name == name)
            {
                stopID_list.push_back(stop.first);
            }
        }
    }
    else
    {
        // Retrieve stop with given name from stop_name_order.
        if (stop_name_order.find(name) != stop_name_order.end())
        {
            stopID_list.reserve(stop_name_order.at(name).size());

            for (auto p : stop_name_order.at(name))
            {
                stopID_list.push_back(p->id);
            }
        }
    }

    if (stopID_list.size() == 0)
    {
        return { NO_STOP };
    }

    return stopID_list;
}

// The funtion changes name of the stop with given ID.
bool Datastructures::change_stop_name(StopID id, const Name& newname)
{
    if (stop_list.find(id) == stop_list.end())
    {
        return false;
    }

    // Update key and value in stop_name_order.
    std::string old_name = stop_list.at(id).stop_name;
    auto change_name_stop = stop_name_order.find(old_name);

    if (create_finished)
    {
        if (change_name_stop != stop_name_order.end())
        {
            // Remove pointer to stop with has name that will be changed.
            if (change_name_stop->second.size() == 1)
            {
                stop_name_order.erase(change_name_stop);
            }
            else
            {
                // Find pointer to stop with given name.
                for (auto iter = change_name_stop->second.begin();
                    iter != change_name_stop->second.end(); ++iter)
                {
                    if ((*iter)->id == id)
                    {
                        change_name_stop->second.erase(iter);
                        break;
                    }
                }
            }

            // Adding new name of stop and pointer to the stop which name
            // changed.
            Stop* p_adding_stop = &stop_list.at(id);
            if (stop_name_order.find(newname) == stop_name_order.end())
            {
                stop_name_order.insert({ newname, { p_adding_stop } });
            }
            else
            {
                stop_name_order.at(newname).push_back(p_adding_stop);
            }
        }
    }

    // Update new name for the stop with given ID.
    stop_list.at(id).stop_name = newname;

    return true;
}

// The funtion changes coordinates of the stop with given ID.
bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    if (stop_list.find(id) == stop_list.end())
    {
        return false;
    }

    stop_list.at(id).location = newcoord;

    // Update key and value in stop_coord_order.
    Coord old_coord = stop_list.at(id).location;

    // Update key and value in stop_coord_order.

    auto change_coord_stop = stop_coord_order.find(old_coord);

    if (create_finished)
    {
        if (change_coord_stop != stop_coord_order.end())
        {
            if (change_coord_stop->second.size() == 1)
            {
                stop_coord_order.erase(change_coord_stop);
            }
            else
            {
                // Find pointer to stop with given coordinates.
                for (auto iter = change_coord_stop->second.begin();
                    iter != change_coord_stop->second.end(); ++iter)
                {
                    if ((*iter)->id == id)
                    {
                        change_coord_stop->second.erase(iter);
                        break;
                    }
                }
            }

            // Adding new coordinates of stop and pointer to the stop which
            // coordinates changed.
            Stop* p_adding_stop = &stop_list.at(id);
            if (stop_coord_order.find(newcoord) == stop_coord_order.end())
            {
                stop_coord_order.insert({ newcoord, { p_adding_stop } });
            }
            else
            {
                stop_coord_order.at(newcoord).push_back(p_adding_stop);
            }
        }
    }

    stop_list.at(id).location = newcoord;

    return true;
}

// Function add region to the data structure with given unique ID and name.
bool Datastructures::add_region(RegionID id, const Name& name)
{
    if (region_list.find(id) != region_list.end())
    {
        return false;
    }

    Region temp_region = { id, name, NULL, { }, { } };
    region_list.insert({ id, temp_region });

    return true;
}

// Function returns the name of the region with given ID or NO_NAME if the ID is
// not valid.
Name Datastructures::get_region_name(RegionID id)
{
    // Check if the region ID is valid.
    if (region_list.find(id) == region_list.end())
    {
        return NO_NAME;
    }

    return region_list.at(id).region_name;
}

// Function returns a vector of the regions in arbitrary order. The function
// returns an empty vector when there is no region in the data structure.
std::vector<RegionID> Datastructures::all_regions()
{
    if (region_list.size() == 0)
    {
        return { };
    }

    std::vector<RegionID> regionID_list;
    regionID_list.reserve(region_list.size());

    for (auto region : region_list)
    {
        regionID_list.push_back(region.first);
    }

    return regionID_list;
}

// Functions adds a ponier of given stop to the given region and adds the ID of
// the region to the stop then returns true. If adding not success function
// returns fasle.
bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{
    // Check the validity of ID of stop and ID of region.
    if (stop_list.find(id) == stop_list.end() ||
        region_list.find(parentid) == region_list.end())
    {
        return false;
    }

    // Check if the stop already belongs to a region.
    if (stop_list.find(id) != stop_list.end())
    {
        if (stop_list.at(id).regionID != NO_REGION)
        {
            return false;
        }
    }

    // Add region ID to stop.
    stop_list.at(id).regionID = parentid;

    // Add stop to region.
    Stop* p_stop = &stop_list.at(id);
    region_list.at(parentid).stop_in_region.push_back(p_stop);

    return true;
}

// Function adds a given as a subregion to another region.
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    if (region_list.find(id) == region_list.end() ||
        region_list.find(parentid) == region_list.end())
    {
        return false;
    }

    if (region_list.at(id).parent != NULL)
    {
        return false;
    }

    // Add pointer to parent region.
    Region* p_parent = &region_list.at(parentid);
    region_list.at(id).parent = p_parent;

    // Add pointer to its children.
    Region* p_children = &region_list.at(id);
    region_list.at(parentid).children.push_back(p_children);

    return true;
}

// Function returns a vector of regions ID to which the given stop belongs
// either directly or indirectly.
std::vector<RegionID> Datastructures::stop_regions(StopID id)
{
    // Check if the stop ID is valid.
    if (stop_list.find(id) == stop_list.end())
    {
        return { NO_REGION };
    }

    // Check if the stop belongs to any region.
    if (stop_list.at(id).regionID == NO_REGION)
    {
        return { NO_REGION };
    }

    std::vector<RegionID> regionID_list;

    // Find all the regions that the stop belongs directly or indirectly.
    RegionID parentid = stop_list.at(id).regionID;
    Region* p_parent = &region_list.at(parentid);
    while (p_parent != NULL)
    {
        regionID_list.push_back(p_parent->id);
        p_parent = p_parent->parent;
    }

    return regionID_list;
}
// The function is used for changing the status of the program by changing the
// value of boolean variable create_finished. The status is the used by other
// functions to choosing method of implementation.
void Datastructures::creation_finished()
{
    // Creating map with key is stop name and stop coordinates.
    for (auto iter = stop_list.begin(); iter != stop_list.end(); ++iter)
    {
        // Get information of the stop going to add.
        Stop* p_adding_stop = &(iter->second);

        Name name_stop = p_adding_stop->stop_name;
        Coord stop_coord = p_adding_stop->location;

        // Create map elemennt with key is the stop name in order to have the
        // stops in alphabetical order of their name.
        if (stop_name_order.find(name_stop) == stop_name_order.end())
        {
            stop_name_order.insert({ name_stop, { p_adding_stop } });
        }
        else
        {
            stop_name_order.at(name_stop).push_back(p_adding_stop);
        }

        // Create map element with key is the stop location in order to have
        // the stops in coordinates order.
        if (stop_coord_order.find(stop_coord) == stop_coord_order.end())
        {
            stop_coord_order.insert({ stop_coord, { p_adding_stop } });
        }
        else
        {
            stop_coord_order.at(stop_coord).push_back(p_adding_stop);
        }
    }

    // Use for changing algorithms.
    create_finished = true;
}

// Function return the region bounding box of that contains all stop of the
// stops in it and all the stops of all of the region's subregion.
std::pair<Coord, Coord> Datastructures::region_bounding_box(RegionID id)
{
    if (region_list.find(id) == region_list.end())
    {
        return { NO_COORD, NO_COORD };
    }

    Coord lower_left = NO_COORD;
    Coord upper_right = NO_COORD;

    region_traversal(region_list.at(id), lower_left, upper_right);

    return { lower_left, upper_right };
}

// Function returns five stop closest to the given stop in increasing order.
// If there are less than six stops function return less stop.
std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    // Check if the stop ID is valid.
    if (stop_list.find(id) == stop_list.end())
    {
        return { NO_STOP };
    }

    std::vector < std::pair < StopID, long long int >> stop_closest;
    stop_closest.reserve(5);

    Coord center_coord = stop_list.at(id).location;
    size_t index_upper = 0;

    // Search for five closest stops to the given stop.
    for (auto it = stop_list.begin(); it != stop_list.end(); ++it)
    {
        if (it->second.id != id)
        {
            Coord iter_coord = stop_list.at(it->second.id).location;

            if (stop_closest.size() < 5)
            {
                // Push new element in the stop closest list.
                stop_closest.push_back(std::make_pair(it->second.id,
                    distance_square(iter_coord, center_coord)));

                // Update index to position of upper bound element.
                if (stop_closest[index_upper].second <
                    stop_closest.back().second)
                {
                    index_upper = stop_closest.size() - 1;
                }
            }
            else
            {
                // Update list of stops closest.
                if (stop_closest[index_upper].second >
                    distance_square(iter_coord, center_coord))
                {
                    // Insert new element
                    stop_closest[index_upper] = std::make_pair(it->second.id,
                        distance_square(iter_coord, center_coord));

                    // Update index to element that has maximum distance.
                    for (unsigned int i = 0; i < stop_closest.size(); ++i)
                    {
                        if (stop_closest[i].second >
                            stop_closest[index_upper].second)
                        {
                            index_upper = i;
                        }
                    }
                }
            }
        }
    }

    // Sort the stop ID in ascending order of the distance to given stop.
    std::sort(stop_closest.begin(), stop_closest.end(),
        [](std::pair<StopID, int> s1, std::pair<StopID, int> s2)
        { return (s1.second < s2.second); });

    // Make the stop ID vector.
    std::vector<StopID> stopID_list;
    for (unsigned int i = 0; i < stop_closest.size(); ++i)
    {
        stopID_list.push_back(stop_closest[i].first);
    }

    return stopID_list;
}

// Function removes the stop with given ID. If the stop is found function
// function removes stop from everywhere contains information about it.
bool Datastructures::remove_stop(StopID id)
{
    // Check if the ID of stop is valid.
    if (stop_list.find(id) == stop_list.end())
    {
        return false;
    }

    // Remove pointer to stop in stop_name_order and stop_coord_order before
    // remove the stop in the stop list for avoiding invalidate of poiter
    // to the stop.
    if (create_finished)
    {
        // Remove pointer to the stop in stop_name_order map.
        Name name_stop = stop_list.at(id).stop_name;

        if (stop_name_order.find(name_stop) != stop_name_order.end())
        {
            // Remove unordered map element of stop_name_order when there is
            // only one stop with given name.
            if (stop_name_order.at(name_stop).size() <= 1)
            {
                stop_name_order.erase(name_stop);
            }
            else
            {
                // Find the pointer to the stop with given ID in all stops with
                // same name.
                auto it_name = std::find_if(
                    stop_name_order.at(name_stop).begin(),
                    stop_name_order.at(name_stop).end(),
                    [id](Stop* p) { return (p->id == id); });

                // Remove the pointer to stop.
                if (it_name != stop_name_order.at(name_stop).end())
                {
                    stop_name_order.at(name_stop).erase(it_name);
                }
            }
        }

        // Remove pointer to the stop in stop_coord_order map.
        Coord location = stop_list.at(id).location;

        if (stop_coord_order.find(location) != stop_coord_order.end())
        {
            // Remove unordered map element of stop_coord_order when there is
            // only one stop with given coordinates.
            if (stop_coord_order.at(location).size() <= 1)
            {
                stop_coord_order.erase(location);
            }
            else
            {
                // Find the pointer to the stop with given ID in all stops with
                // the same coordinates.
                auto it_location = std::find_if(
                    stop_coord_order.at(location).begin(),
                    stop_coord_order.at(location).end(),
                    [id](Stop* p) { return (p->id == id); });

                // Remove the pointer to stop.
                if (it_location != stop_coord_order.at(location).end())
                {
                    stop_coord_order.at(location).erase(it_location);
                }
            }
        }
    }

    // Get the region that contains the stop.
    RegionID region_id = stop_list.at(id).regionID;

    // Find the pointer to stop in the list of stops contained by the region.
    auto it = std::find_if(region_list.at(region_id).stop_in_region.begin(),
        region_list.at(region_id).stop_in_region.end(),
        [id](Stop* p) { return (p->id == id); });

    // Remove stop from the region.
    if (it != region_list.at(region_id).stop_in_region.end())
    {
        region_list.at(region_id).stop_in_region.erase(it);
    }

    // Remove stop from stop_list.
    stop_list.erase(id);

    return true;
}

// Function return the "lowest" common region in the subregion hierachy for the
// stops.
RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{
    if (stop_list.find(id1) == stop_list.end() ||
        stop_list.find(id2) == stop_list.end())
    {
        return NO_REGION;
    }

    RegionID parentid1 = stop_list.at(id1).regionID;
    RegionID parentid2 = stop_list.at(id2).regionID;

    // Initalize the path from the region that contains the stop to all of
    // their direct or indirect parents.
    std::vector<RegionID> path1 = { parentid1 };
    std::vector<RegionID> path2 = { parentid2 };

    // Adding the parents region contains directly or indirectly the stops.
    while (region_list.at(parentid1).parent != NULL)
    {
        parentid1 = region_list.at(parentid1).parent->id;
        path1.push_back(parentid1);
    }

    while (region_list.at(parentid2).parent != NULL)
    {
        parentid2 = region_list.at(parentid2).parent->id;
        path2.push_back(parentid2);
    }

    auto iter1 = path1.rbegin();
    auto iter2 = path2.rbegin();

    // Find the common region that contains both stop.
    while (iter1 != path1.rend() && iter2 != path2.rend())
    {
        if (*iter1 == *iter2)
        {
            ++iter1;
            ++iter2;
        }
        else
        {
            break;
        }
    }

    if (iter1 == path1.rbegin())
    {
        return NO_REGION;
    }
    else
    {
        --iter1;
        return *iter1;
    }
}

// Function returns the square of Euclidean distance between two stops.
long long int Datastructures::distance_square(const Coord& xy1,
    const Coord& xy2)
{
    long long int diff_x = (long long int) xy2.x - (long long int) xy1.x;
    long long int diff_y = (long long int) xy2.y - (long long int) xy1.y;

    return diff_x * diff_x + diff_y * diff_y;
}

// Function compares the current bounding box and the considered stop
// coordinates.
void Datastructures::update_bounding_box(const Coord& stop_location, Coord& lower_left,
    Coord& upper_right)
{
    // Update lower left corner.
    if (lower_left == NO_COORD)
    {
        lower_left = stop_location;
    }
    else
    {
        if (stop_location.x < lower_left.x)
        {
            lower_left.x = stop_location.x;
        }

        if (stop_location.y < lower_left.y)
        {
            lower_left.y = stop_location.y;
        }
    }

    // Update upper right corner.
    if (upper_right == NO_COORD)
    {
        upper_right = stop_location;
    }
    else
    {
        if (stop_location.x > upper_right.x)
        {
            upper_right.x = stop_location.x;
        }

        if (stop_location.y > upper_right.y)
        {
            upper_right.y = stop_location.y;
        }
    }
}

// Function traversal all the regions and update bounding box of the given
// region.
void Datastructures::region_traversal(Region& root, Coord& lower_left,
    Coord& upper_right)
{
    // Update the bounding box of region by comparing to the stops contained in
    // the region.
    if (root.stop_in_region.size() > 0)
    {
        for (auto p_stop : root.stop_in_region)
        {
            update_bounding_box(p_stop->location, lower_left, upper_right);
        }
    }

    // Update the bounding box of region by comparing to the stops contained in
    // the subregion of the region.
    if (root.children.size() > 0)
    {
        for (unsigned int i = 0; i < root.children.size(); ++i)
        {
            region_traversal(*root.children.at(i), lower_left, upper_right);
        }
    }
}

// ------------------------------------------------------------------------------------------------------Phase 2.

// Function returns a list of routes in arbitrary order.
std::vector<RouteID> Datastructures::all_routes()
{
    if (route_list.size() == 0)
    {
        return { };
    }

    std::vector<RouteID> routeID_list;
    routeID_list.reserve(route_list.size());

    for (auto route : route_list)
    {
        routeID_list.push_back(route.first);
    }

    return routeID_list;
}

// Function adds a new route running through the given stops.
bool Datastructures::add_route(RouteID id, std::vector<StopID> stops)
{
    // Check if route ID is valid and there are stops to add.
    if ((route_list.find(id) != route_list.end()) ||
        stops.size() <= 1)
    {
        return false;
    }

    Route add_route;
    add_route.id = id;
    add_route.stops_on_route.reserve(stops.size());

    for (auto it_stop = stops.begin(); it_stop != stops.end(); ++it_stop)
    {
        if (stop_list.find(*it_stop) == stop_list.end())
        {
            return false;
        }

        StopID curr_stop = *it_stop;
        add_route.stops_on_route.push_back(curr_stop);

        // Add incoming edge to a stop.
        if (*it_stop != *stops.begin())
        {
            StopID prev_stop = *(--it_stop);
            ++it_stop;

            Edge add_in_edge = make_edge_no_trip(prev_stop, curr_stop, id);

            stop_list.at(curr_stop).in_edges.insert({ id, add_in_edge });
        }

        // Add outcoming edge to a stop.
        if (*it_stop != *stops.rbegin())
        {
            StopID next_stop = *(++it_stop);
            --it_stop;

            Edge add_out_edge = make_edge_no_trip(curr_stop, next_stop, id);

            stop_list.at(curr_stop).out_edges.insert({ id, add_out_edge });
        }
    }

    route_list.insert({ id, add_route });
    return true;
}

// Function returns a list of routes from the given stops and what stops is
// next on each route.
std::vector<std::pair<RouteID, StopID>> Datastructures::routes_from(StopID stopid)
{
    // Check if stop ID is valid.
    if (stop_list.find(stopid) == stop_list.end())
    {
        return { {NO_ROUTE, NO_STOP} };
    }

    std::vector<std::pair<RouteID, StopID>> routes_out;

    for (auto it = stop_list.at(stopid).out_edges.begin();
        it != stop_list.at(stopid).out_edges.end(); ++it)
    {
        routes_out.push_back(std::make_pair(it->first, it->second.to_stop_ID));
    }

    return routes_out;
}

// Function returns list of route's stops in the driving order.
std::vector<StopID> Datastructures::route_stops(RouteID id)
{
    // Check if route ID is valid.
    if (route_list.find(id) == route_list.end())
    {
        return { NO_STOP };
    }

    return route_list.at(id).stops_on_route;
}

// Function clears out routes and trips
void Datastructures::clear_routes()
{
    for (auto it_stop = stop_list.begin(); it_stop != stop_list.end();
        ++it_stop)
    {
        // Remove all route information between stops.
        it_stop->second.in_edges.clear();
        it_stop->second.out_edges.clear();

        // Keep walking connections.
    }

    route_list.clear();
}

// Function returns any journey between the given stops. In this implementation
// function calls journey_least_stop().
JourneyDistance Datastructures::journey_any(StopID fromstop, StopID tostop)
{
    return journey_least_stops(fromstop, tostop);
}

// Function returns a journey between the given stios with the minimum number
// of stops.
JourneyDistance Datastructures::journey_least_stops(StopID fromstop, StopID tostop)
{
    // Check if stop IDs are valid.
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return { {NO_STOP, NO_ROUTE, NO_DISTANCE} };
    }

    if (fromstop == tostop)
    {
        return { };
    }

    // Find path with least stops by using breadth first search.
    PathEdge path_bfs = path_breadth_first_search(fromstop, tostop);

    return make_journey_distance(path_bfs);
}

// Function returns a journey between the given stops that has a cycle.
JourneyDistance Datastructures::journey_with_cycle(StopID fromstop)
{
    // Check if stop IDs are valid.
    if (stop_list.find(fromstop) == stop_list.end())
    {
        return { {NO_STOP, NO_ROUTE, NO_DISTANCE} };
    }

    // Find path that contain cycle by depth first search with condition stops
    // searching when found cycle.
    PathEdge path_dfs = path_depth_first_search(fromstop);

    return make_journey_distance(path_dfs);
}

// Function returns a journey between the given stops with the minimum total
// distance.
JourneyDistance Datastructures::journey_shortest_distance(StopID fromstop, StopID tostop)
{
    // Check if stop IDs are valid.
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return { {NO_STOP, NO_ROUTE, NO_DISTANCE} };
    }

    if (fromstop == tostop)
    {
        return { };
    }

    // Find journey with shortest distance travel by implementing Dijkstra
    // algorithms with label on each stop is total distance needed to travel to
    // reach that stop starting from source stop.
    PathEdge path_shortest = shortest_path_distance(fromstop, tostop);

    return make_journey_distance(path_shortest);
}

// Function adds trip to a given route.
bool Datastructures::add_trip(RouteID routeid, std::vector<Time> const& stop_times)
{
    // Check if route ID is valid.
    if (route_list.find(routeid) == route_list.end())
    {
        return false;
    }

    Trip add_trip;

    // Trip ID contain the route ID of trip and the number denote the order
    // the trip is added in the route.
    TripID tripid = { routeid, (int) route_list.at(routeid).trip_list.size() };

    for (unsigned int i = 0; i < stop_times.size(); ++i)
    {
        StopID curr_stop = route_list.at(routeid).stops_on_route.at(i);
        Time depart_curr_stop = stop_times.at(i);

        add_trip.timetable.insert({ curr_stop, depart_curr_stop });

        // Adding time information between adjacent stops on a route.

        // Adding edge go in a stop.
        if (i > 0)
        {
            Time depart_prev_stop = stop_times.at(i - 1);

            // Depart time to current stop is the same as arrival time
            // to current stop.
            TimeTravel time_info = std::make_pair(depart_prev_stop, depart_curr_stop);
            std::pair<TripID, TimeTravel> bus_on_edge = std::make_pair(tripid, time_info);

            stop_list.at(curr_stop).in_edges.at(routeid).bus_travel.push_back(bus_on_edge);
        }

        // Adding egde go out a stop.
        if (i < stop_times.size() - 1)
        {
            Time depart_next_stop = stop_times.at(i + 1);

            // Depart time to next stop is the same as arrival time to next stop.
            TimeTravel time_info = std::make_pair(depart_curr_stop, depart_next_stop);
            std::pair<TripID, TimeTravel> bus_on_edge = std::make_pair(tripid, time_info);

            stop_list.at(curr_stop).out_edges.at(routeid).bus_travel.push_back(bus_on_edge);
        }
    }

    // Adding trip to route list.
    route_list.at(routeid).trip_list.push_back(add_trip);

    return true;
}

// Functions returns info when buses on a given stop depart from the given
// stop.
std::vector<std::pair<Time, Duration>> Datastructures::route_times_from(RouteID routeid, StopID stopid)
{
    // Check if route ID and stop ID are valid.
    if (route_list.find(routeid) == route_list.end() ||
        stop_list.find(stopid) == stop_list.end())
    {
        return { {NO_TIME, NO_DURATION} };
    }

    bool route_has_stop = false;
    Stop start_stop = stop_list.at(stopid);

    // Check that the route has passed over the stop.
    // This will not check that the stop is the first stop of the route so we
    // meed the checking below.
    for (auto it = start_stop.in_edges.begin(); it != start_stop.in_edges.end(); ++it)
    {
        if (it->first == routeid)
        {
            route_has_stop = true;
            break;
        }
    }

    // Check that the route has passed over the stop.
    // This will not check that the stop is the last stop of the route so we
    // need the checking above.
    for (auto it = start_stop.out_edges.begin(); it != start_stop.out_edges.end(); ++it)
    {
        if (it->first == routeid)
        {
            route_has_stop = true;
            break;
        }
    }

    if (route_has_stop == false)
    {
        return { {NO_TIME, NO_DURATION} };
    }

    Route temp_route = route_list.at(routeid);

    // Find next stop.
    StopID next_stop = NO_STOP;
    for (auto it = temp_route.stops_on_route.begin();
        it != temp_route.stops_on_route.end(); ++it)
    {
        if (*it == stopid)
        {
            if (*it == temp_route.stops_on_route.back())
            {
                next_stop = NO_STOP;
            }
            else
            {
                next_stop = *(++it);
                break;
            }
        }
    }

    if (next_stop == NO_STOP)
    {
        return { };
    }

    // Construct buses information depart from stop.
    std::vector<std::pair<Time, Duration>> route_times;

    for (unsigned int i = 0; i < temp_route.trip_list.size(); ++i)
    {
        Time start_time = temp_route.trip_list.at(i).timetable.at(stopid);
        Time end_time = temp_route.trip_list.at(i).timetable.at(next_stop);

        route_times.push_back(std::make_pair(start_time, end_time - start_time));
    }

    return route_times;
}

// Function returns a journey between the given stops with the earliest
// possible arrival time.
JourneyTime Datastructures::journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime)
{
    // Check if stop IDs are valid.
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return { {NO_STOP, NO_ROUTE, NO_TIME} };
    }

    if (fromstop == tostop)
    {
        return { };
    }

    // Find journey with earliest arrival by implementing Dijkstra algorithms
    // with label used for each stop is the time arrival to that stop.
    PathEdge path_soonest_arrival = shortest_path_duration(fromstop, tostop, starttime);

    return make_journey_time(path_soonest_arrival, starttime);
}

// Adding walking connection from each stop to its five closest stops.
void Datastructures::add_walking_connections()
{
    // Loop through all stops.
    for (auto it_stop = stop_list.begin(); it_stop != stop_list.end(); ++it_stop)
    {
        StopID fromstop = it_stop->first;

        // Finding 5 stops closest to a stop.
        std::vector<StopID> stops_closest = stops_closest_to(it_stop->first);

        // Adding connection to five nearest stop.
        for (auto it_near = stops_closest.begin(); it_near != stops_closest.end();
            ++it_near)
        {
            StopID tostop = *it_near;
            // Route ID for walking path is NO_ROUTE.
            Edge walking_path = make_edge_no_trip(fromstop, tostop, NO_ROUTE);

            // Calculate walking time.
            walking_path.walking_time = (int)(walking_path.dist / walking_speed);

            stop_list.at(fromstop).walking_to.push_back(walking_path);
        }
    }
}

// Finding path path from source stop to target stop by implementing breadth
// first search.
PathEdge Datastructures::path_breadth_first_search(StopID fromstop, StopID tostop)
{
    // Check if stop IDs are valid.
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return { };
    }

    // Implementing breadth first search by using queue.
    std::queue<std::pair<StopID, Edge>> searching_stop;
    searching_stop.push(std::pair(fromstop, Edge()));

    TravelHistory stop_visited;

    while (searching_stop.size() != 0)
    {
        std::pair<StopID, Edge> current_stop = searching_stop.front();
        stop_visited.insert({ current_stop.first, current_stop.second });

        if (current_stop.first == tostop)
        {
            break;
        }
        else
        {
            searching_stop.pop();

            // Loop through adjacent stop.
            for (auto it_edge = stop_list.at(current_stop.first).out_edges.begin();
                it_edge != stop_list.at(current_stop.first).out_edges.end(); ++it_edge)
            {
                // If not visit the stop.
                if (stop_visited.find(it_edge->second.to_stop_ID) == stop_visited.end())
                {
                    searching_stop.push(std::make_pair(,it_edge->second.to_stop_ID, it_edge->second));
                }
            }
        }
    }

    // There is no journey that connect source stop to target stop.
    if (searching_stop.size() == 0)
    {
        return { };
    }

    return make_path(fromstop, tostop, stop_visited, false);
}

// Finding path with contains circle by implementing depth first search search.
PathEdge Datastructures::path_depth_first_search(StopID fromstop)
{
    // Check if stop ID is valid.
    if (stop_list.find(fromstop) == stop_list.end())
    {
        return { };
    }

    bool found_cycle = false;
    StopID tostop;
    // Last edge used for making path.
    Edge last_edge;

    // Implementing depth first search by using a stack.
    std::stack<std::pair<StopID, Edge>> searching_stop;
    searching_stop.push(std::make_pair(fromstop, Edge()));

    // Storing stop with edges travel to that stop used later for making path.
    TravelHistory visited_stop;

    while (searching_stop.size() != 0 && !found_cycle)
    {
        std::pair<StopID, Edge> curr_stop = searching_stop.top();
        visited_stop.insert({ curr_stop.first, curr_stop.second });

        searching_stop.pop();

        bool can_continue = false;
        std::unordered_set<StopID> already_travel;

        // Traversal all stop that can reach from current stop.
        for (auto it_edge = stop_list.at(curr_stop.first).out_edges.begin();
            it_edge != stop_list.at(curr_stop.first).out_edges.end(); ++it_edge)

        {
            // If the stop has not been added in travel history.
            if (visited_stop.find(it_edge->second.to_stop_ID) ==
                    visited_stop.end())
            {
                // And not visited by any other path.
                if (already_travel.find(it_edge->second.to_stop_ID) ==
                        already_travel.end())
                {
                    can_continue = true;
                    already_travel.insert(it_edge->second.to_stop_ID);
                    searching_stop.push(std::make_pair(it_edge->second.to_stop_ID,
                                                       it_edge->second));
                }
            }
            else
            {
                // If the stop has been visited.
                if (visited_stop.size() > 1)
                {
                    can_continue = true;
                    tostop = it_edge->second.to_stop_ID;
                    last_edge = it_edge->second;
                    found_cycle = true;
                    break;
                }
            }
        }

        // Delete search information of edge travel to stop that does not lead
        // to a cycle.
        if (!can_continue)
        {
            visited_stop.erase(curr_stop.first);
        }
    }

    if (!found_cycle)
    {
        return { };
    }

    return make_path(fromstop, tostop, visited_stop, true, last_edge);
}

// Function makes a journey with distance from startting stop up to a stop and
// route to take to the next stop from current stop. Except for the last stop
// with route ID is NO_ROUTE since the journey doesn't continue.
JourneyDistance Datastructures::make_journey_distance(PathEdge path_by_edges)
{
    // Check if there is edge to make journey.
    if (path_by_edges.size() == 0)
    {
        return { };
    }

    JourneyDistance jour_dist;

    for (auto it_edge = path_by_edges.begin(); it_edge != path_by_edges.end();
        ++it_edge)
    {
        // Added information.
        StopID curr_stop = it_edge->from_stop_ID;
        RouteID route_to_next_stop = it_edge->route_ID;
        Distance dist_to_stop = 0;

        // Calculation disatnce up to current stop.
        if (it_edge != path_by_edges.begin())
        {
            dist_to_stop = std::get<2>(jour_dist.back()) + (--it_edge)->dist;
            ++it_edge;
        }

        jour_dist.push_back(std::make_tuple(curr_stop, route_to_next_stop,
            dist_to_stop));
    }

    auto it_last_edge = path_by_edges.back();

    // Add information for the last stop.
    Distance dist_to_stop = std::get<2>(jour_dist.back()) + it_last_edge.dist;
    jour_dist.push_back(std::make_tuple(it_last_edge.to_stop_ID, NO_ROUTE,
        dist_to_stop));

    return jour_dist;
}

// Function makes a journey with departure time from each stop and route to
// take to the next stop. Except for the last stop with time information is the
// arrival time and route ID is NO_ROUTE since the journey doesn't continue.
JourneyTime Datastructures::make_journey_time(PathEdge path_by_edges, Time starttime)
{
    // Check if there is edge to make journey.
    if (path_by_edges.size() == 0)
    {
        return { };
    }

    // Adding time when have walking connection.

    JourneyDistance jour_depart;

    for (auto it_edge = path_by_edges.begin(); it_edge != path_by_edges.end();
        ++it_edge)
    {
        StopID curr_stop = it_edge->from_stop_ID;
        RouteID route_to_next_stop = it_edge->route_ID;
        Time depart_from_stop;

        // If this edge is going by walking.
        if (route_to_next_stop == NO_ROUTE)
        {
            if (it_edge == path_by_edges.begin())
            {
                depart_from_stop = starttime;
            }
            else
            {
                // Walking to next stop immediatly arriving current stop.
                // Calculate departure time that is the same as the departure
                // time of previous stop and time walking from previous stop to
                // current stop.

                --it_edge;

                if (it_edge->route_ID == NO_ROUTE)
                {
                    // Arrival time from walking.
                    depart_from_stop = std::get<2>(jour_depart.back()) +
                        it_edge->walking_time;
                }
                else
                {
                    // Arrival time of the bus.
                    depart_from_stop = it_edge->bus_travel.front().second.second;
                }

                ++it_edge;
            }
        }
        // If this edge is going by bus.
        else
        {
            depart_from_stop = it_edge->bus_travel.front().second.first;
        }

        jour_depart.push_back(std::make_tuple(curr_stop, route_to_next_stop,
            depart_from_stop));
    }

    // Add information for the last stop.
    Edge last_edge = path_by_edges.back();
    Time arrival_last_stop;

    // If last edge is going by walking.
    if (last_edge.route_ID == NO_ROUTE)
    {
        // Walking to next stop immediatly arriving current stop.
        arrival_last_stop = std::get<2>(jour_depart.back()) + last_edge.walking_time;
    }
    else
    {
        // If this edge is going by bus.
        // For illustration:
        //                |Last edge|List of trip|Only | Time | Arrival|
        //                |         | pass edge  |trip |travel| time   |
        arrival_last_stop = last_edge.bus_travel.front().second.second;
    }


    jour_depart.push_back(std::make_tuple(last_edge.to_stop_ID, NO_ROUTE,
        arrival_last_stop));

    return jour_depart;
}

// Function implements Dijkstra algorithm for finding path with shortest
// distance from source stop to target stop.
PathEdge Datastructures::shortest_path_distance(StopID fromstop, StopID tostop)
{
    // Check if stop IDs are valid.
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return { };
    }

    if (fromstop == tostop)
    {
        return { };
    }

    // For storing shortest distance found.
    Label stop_label;
    stop_label.insert({ fromstop, 0 });

    // For later traceback to construct journey information.
    TravelHistory stop_visited;
    stop_visited.insert({ fromstop, Edge() });

    // For implementing priority - queue in Dijkstra algorithm with priority
    // is the total distance travel up to current stop.
    MinWeightHeap min_heap;
    min_heap.push(std::make_pair(0, fromstop));

    while (!min_heap.empty())
    {
        // Access min element.
        StopID curr_stop = min_heap.top().second;

        // Stop when found shortest path to target stop.
        if (curr_stop == tostop)
        {
            break;
        }

        // Remove min element from priority queue.
        min_heap.pop();

        // Loop for all bus connection.
        for (auto it_edge = stop_list.at(curr_stop).out_edges.begin();
            it_edge != stop_list.at(curr_stop).out_edges.end(); ++it_edge)
        {
            Edge travel_edge = it_edge->second;
            StopID next_stop = travel_edge.to_stop_ID;

            // If stop was not found.
            if (stop_visited.find(next_stop) == stop_visited.end())
            {
                // Adding new discover stop.
                stop_label.insert({ next_stop, INF });

                // Add edge the lead to this stop.
                stop_visited.insert({ next_stop, travel_edge });

                // Add stop to the priority queue.
                min_heap.push(std::make_pair(INF, next_stop));
            }

            // Update label and the priority of stops in priority queue.
            relax_distance(travel_edge, stop_label, stop_visited, min_heap);
        }

        // Loop for all walking connection.
        for (auto it_edge = stop_list.at(curr_stop).walking_to.begin();
            it_edge != stop_list.at(curr_stop).walking_to.end(); ++it_edge)
        {
            Edge travel_edge = *it_edge;
            StopID next_stop = travel_edge.to_stop_ID;

            // If stop was not found.
            if (stop_visited.find(next_stop) == stop_visited.end())
            {
                // Adding new discover stop.
                stop_label.insert({ next_stop, INF });

                // Add edge the lead to this stop.
                stop_visited.insert({ next_stop, travel_edge });
            }

            // Update label and the priority of stops in priority queue.
            relax_distance(travel_edge, stop_label, stop_visited, min_heap);
        }

    }

    // If there was no path between source stop and target stop.
    if (min_heap.empty())
    {
        return { };
    }

    // Make path by edge from source stop to target stop with minimum total
    // weight.
    return make_path(fromstop, tostop, stop_visited, false);
}

// Function implements Dijkstra algorithms for finding path with shortest
// duration since the startting time is fixed. The function used modify the
// algorithm by using the label of stop is the arrival time to that stop and
// some modification in relaxtion rule.
PathEdge Datastructures::shortest_path_duration(StopID fromstop, StopID tostop,
    Time starttime)
{
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return { };
    }

    if (fromstop == tostop)
    {
        return { };
    }

    // For storing soonest arrival found.
    Label stop_label;
    stop_label.insert({ fromstop, starttime });

    // For later traceback to construct journey information.
    TravelHistory stop_visited;
    stop_visited.insert({ fromstop, Edge() });

    // For implementing priority - queue in Dijkstra algorithm with priority is
    // the arrival time to stop
    MinWeightHeap min_heap;
    min_heap.push(std::make_pair(starttime, fromstop));

    while (!min_heap.empty())
    {
        // Access min element.
        StopID curr_stop = min_heap.top().second;
        Time arrival_time = min_heap.top().first;

        if (curr_stop == tostop)
        {
            break;
        }

        // Remove min element from priority queue.
        min_heap.pop();

        // Loop through all bus travel through the stop.
        for (auto it_edge = stop_list.at(curr_stop).out_edges.begin();
            it_edge != stop_list.at(curr_stop).out_edges.end(); ++it_edge)
        {
            // Loop for all possible bus.
            for (auto it_bus = it_edge->second.bus_travel.begin();
                it_bus != it_edge->second.bus_travel.end(); ++it_bus)
            {
                // If can catch up the bus.
                if (arrival_time <= it_bus->second.first)
                {
                    // Add edge with only one time choice for later traceback.
                    Edge travel_edge = it_edge->second;
                    travel_edge.bus_travel = { *it_bus };

                    StopID next_stop = it_edge->second.to_stop_ID;

                    // If stop was not found.
                    if (stop_visited.find(next_stop) == stop_visited.end())
                    {
                        // Adding new discover stop.
                        stop_label.insert({ next_stop, INF });

                        // Add stop with edge that lead to this stop.
                        // stop_visited.insert({ next_stop, travel_edge });
                        stop_visited.insert({ next_stop, travel_edge });
                    }

                    // Update label and the priority of stops in priority queue.
                    relax_time(travel_edge, stop_label, stop_visited, min_heap);
                }
            }
        }

        // Loop through walking connection.
        for (auto it_edge = stop_list.at(curr_stop).walking_to.begin();
            it_edge != stop_list.at(curr_stop).walking_to.end(); ++it_edge)
        {
            Edge travel_edge = *it_edge;
            StopID next_stop = travel_edge.to_stop_ID;

            // If stop was not found.
            if (stop_visited.find(next_stop) == stop_visited.end())
            {
                // Adding new discover stop.
                stop_label.insert({ next_stop, INF });

                // Add stop with edge that lead to this stop.
                stop_visited.insert({ next_stop, travel_edge });
            }

            // Update label and the priority of stops in priority queue.
            relax_time(travel_edge, stop_label, stop_visited, min_heap);
        }
    }

    // If there was no path between source stop and target stop.
    if (stop_visited.find(tostop) == stop_visited.end())
    {
        return { };
    }

    // Make path by edges from source stop to target stop with minimum total
    // weight.
    return find_path(fromstop, tostop, starttime, stop_label.at(tostop));
}

// Function make a path with edge traveled from source stop to target stop.
// Parameter with_cycle and last_edge is used when with_cycle has value true
// then function makes path that ends immediately when it contains circle with
// the ending edge of the path is last_edge.
PathEdge Datastructures::make_path(StopID fromstop, StopID tostop,
    TravelHistory& stop_visited, bool with_cycle, Edge last_edge)
{
    PathEdge traceback_edges = { };
    StopID trace_stop = tostop;

    // Used by detect the cycle has been added into the created edge.
    bool path_has_cycle = false;

    if (with_cycle)
    {
        traceback_edges.push_back(last_edge);
        trace_stop = last_edge.from_stop_ID;
    }

    do
    {
        Edge previous_edge = stop_visited.at(trace_stop);
        traceback_edges.push_back(previous_edge);

        trace_stop = previous_edge.from_stop_ID;

        if (trace_stop == tostop)
        {
            path_has_cycle = true;
        }

        if (trace_stop == fromstop)
        {
            break;
        }

    } while (trace_stop != fromstop || (path_has_cycle == false && with_cycle));

    std::reverse(traceback_edges.begin(), traceback_edges.end());
    return traceback_edges;
}

// Function updates distance to the stop.
void Datastructures::relax_distance(Edge connected_edge, Label& stop_label,
    TravelHistory& stop_visited, MinWeightHeap& min_heap)
{
    // Get information from edge.
    StopID fromstop = connected_edge.from_stop_ID;
    StopID tostop = connected_edge.to_stop_ID;
    Distance dist = connected_edge.dist;

    if (stop_label.at(tostop) >
        stop_label.at(fromstop) + dist)
    {
        stop_label.at(tostop) = stop_label.at(fromstop) + dist;

        stop_visited.at(tostop) = connected_edge;

        // Update priority of elements.
        min_heap.push(std::make_pair(stop_label.at(tostop), tostop));
    }
}

// Function updates the arrival time to the stop.
void Datastructures::relax_time(Edge connected_edge, Label& stop_label,
    TravelHistory& stop_visited, MinWeightHeap& min_heap)
{
    // Get information from edge.
    StopID fromstop = connected_edge.from_stop_ID;
    StopID tostop = connected_edge.to_stop_ID;

    Time depart_curr_stop;
    Time arrive_next_stop;

    // If go by bus.
    if (connected_edge.route_ID != NO_ROUTE)
    {
        depart_curr_stop = connected_edge.bus_travel.front().second.first;
        arrive_next_stop = connected_edge.bus_travel.front().second.second;
    }
    else
    {
        // If go by walking.
        depart_curr_stop = stop_label.at(fromstop);
        arrive_next_stop = depart_curr_stop + connected_edge.walking_time;
    }

    // If after arriving source stop we can catch up the next bus.
    if (stop_label.at(fromstop) <= depart_curr_stop)
    {
        // If going from stop 1 to stop 2 by connected edge make
        // arriving sooner.
        if (stop_label.at(tostop) > arrive_next_stop)
        {
            // Update current found soonest arrival time.
            stop_label.at(tostop) = arrive_next_stop;

            stop_visited.at(tostop) = connected_edge;

            // Update priority of new arrival time to stop.
            min_heap.push(std::make_pair(stop_label.at(tostop), tostop));
        }
    }
}

// Function makes and returns an directed edge from fromstop totstop with
// information about route that connect two stops and distance between two
// stops.
Edge Datastructures::make_edge_no_trip(StopID fromstop, StopID tostop, RouteID id)
{
    if (stop_list.find(fromstop) == stop_list.end() ||
        stop_list.find(tostop) == stop_list.end())
    {
        return Edge();
    }

    Coord location_fromstop = stop_list.at(fromstop).location;
    Coord location_tostop = stop_list.at(tostop).location;

    // Calculate distance between stops and round down to integer.
    Distance dist = (int)std::sqrt(distance_square(location_fromstop,
                                                   location_tostop));

    Edge new_edge = { fromstop, tostop, id, dist, { } };

    return new_edge;
}

bool Datastructures::change_bus(Edge next_edge, Edge prev_edge)
{
    // If previous travel and next travel are walking or go by bus on the same
    // route.
    if (prev_edge.route_ID == next_edge.route_ID)
    {
        // If not travel by walking or current stop is not the startting stop.
        if (prev_edge.route_ID != NO_ROUTE)
        {
            TripID prev_index = prev_edge.bus_travel.front().first;
            TripID next_index = next_edge.bus_travel.front().first;

            // Not going on the same bus.
            if (prev_index.trip_index != next_index.trip_index)
            {
                return true;;
            }
        }
    }
    else
    {
        // Change from walk to bus or from bus to walk or change from one bus
        // to another bus.
        return true;
    }

    return false;
}

PathEdge Datastructures::find_path(StopID fromstop, StopID tostop, Time starttime,
                                   Time soonest_arrival_time)
{
    // Check if stop ID is valid.
    if (stop_list.find(fromstop) == stop_list.end())
    {
        return { };
    }

    // Implementing depth first search by using a stack.
    std::stack<std::pair<StopID, Edge>> searching_stop;
    searching_stop.push(std::make_pair(fromstop, Edge()));

    // Storing stop with edges travel to that stop used later for making path.
    TravelHistory visited_stop;

    Label stop_change_bus;
    stop_change_bus.insert({ fromstop, 0});

    int current_change_bus = INF;

    TravelHistory temp_stop_visited;

    while (searching_stop.size())
    {
        std::pair<StopID, Edge> curr_stop = searching_stop.top();


        int change_times = 0;
        if (curr_stop.first != fromstop)
        {
             StopID prev_stop = curr_stop.second.from_stop_ID;
             change_times = stop_change_bus.at(prev_stop);
             if (change_bus(visited_stop.at(curr_stop.second.from_stop_ID), curr_stop.second))
             {
                 change_times++;
             }
        }

        searching_stop.pop();

        if (change_times <= current_change_bus)
        {
            stop_change_bus.insert({curr_stop.first, change_times});
            visited_stop.insert({ curr_stop.first, curr_stop.second });
        }
        else
        {
            continue;
        }

        if (curr_stop.first == tostop)
        {
            temp_stop_visited = visited_stop;
            current_change_bus = stop_change_bus.at(curr_stop.first);
            continue;
        }

        bool can_continue = false;

        // Traversal all stop that can reach from current stop.
        for (auto it_edge = stop_list.at(curr_stop.first).out_edges.begin();
            it_edge != stop_list.at(curr_stop.first).out_edges.end(); ++it_edge)

        {
            Time arrive_time_current_stop;
            if (curr_stop.first == fromstop)
            {
                arrive_time_current_stop = starttime;
            }
            else
            {
                arrive_time_current_stop = visited_stop.at(curr_stop.first).bus_travel.front().second.second;
            }


            for (auto it_bus = it_edge->second.bus_travel.begin();
                 it_bus != it_edge->second.bus_travel.end(); ++it_bus)
            {

                if (it_bus->second.first >= arrive_time_current_stop &&
                    it_bus->second.second <= soonest_arrival_time)
                {

                    Edge temp_edge = it_edge->second;
                    temp_edge.bus_travel = { *it_bus };
                    // If the stop has not been added in travel history.

                    searching_stop.push(std::make_pair(temp_edge.to_stop_ID,
                                                           temp_edge));
                    can_continue = true;
                }
            }
        }

        if (!can_continue)
        {
            visited_stop.erase(curr_stop.first);
            stop_change_bus.erase(curr_stop.first);
        }
    }

    if (temp_stop_visited.find(tostop) == temp_stop_visited.end())
    {
        return { };
    }

    return make_path(fromstop, tostop, temp_stop_visited, false);
}
