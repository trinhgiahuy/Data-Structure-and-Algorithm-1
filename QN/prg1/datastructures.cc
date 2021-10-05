// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>

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
}

Datastructures::~Datastructures()
{
    // No dynamic allocation is used.
    // The destructor of data member is called automatically.
}

int Datastructures::stop_count()
{
    return (int) stop_list.size();
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

    stop_list.insert({ id, Stop{ id, name, xy, NO_REGION } });

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

// Function returns a vector of stop ID in coordinates order. Stops with the
// same location can be in any order with each other.
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
        // Consider checking the implementation of stop_name_order.
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
        // Consider checking the implementation of stop_coord_order.
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

    // Update new coordinates for the stop with given ID.
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

    // Consider a variable instead of 5.
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
