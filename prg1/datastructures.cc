// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <map>
std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)
/*
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}
*/


// Students implementation
// Function to sort placs'ID-name pair by name
bool compare_place_name(const std::pair<PlaceID,Name> p1, const std::pair<PlaceID,Name> p2){
    return p1.second < p2.second;
}

bool compare_place_coord(const std::map<Coord,PlaceID> p1, const std::map<Coord,PlaceID> p2){

    for(auto it_1 = p1.begin(),it_2 = p2.begin(); it_1 != p1.end() && it_2 != p2.end(); ++it_1,++it_2 ){
        float dist1 = sqrt(it_1->first.x*it_1->first.x+it_1->first.y*it_1->first.y);
        float dist2 = sqrt(it_2->first.x*it_2->first.x+it_2->first.y*it_2->first.y);
        if (dist1 == dist2){
            //operator<(p1.second.y,p2.second.y);
            operator<(it_1->first,it_2->first);
        }else{
            return dist1 <dist2;
        }
    }
}

long long int Datastructures::distance_square(const Coord& xy1,
    const Coord& xy2)
{
    long long int diff_x = (long long int) xy2.x - (long long int) xy1.x;
    long long int diff_y = (long long int) xy2.y - (long long int) xy1.y;

    return diff_x * diff_x + diff_y * diff_y;
}
Datastructures::Datastructures()
{
    id_data = {};
    area_data = {};
}

Datastructures::~Datastructures()
{
    clear_all();
}

int Datastructures::place_count()
{
    std::size_t size = id_data.size();
    return size;
}

void Datastructures::clear_all()
{
    id_data.clear();
    area_data.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    // Replace this comment with your implementation

    std::vector<PlaceID> all_places_vct;
    for (auto place : id_data){
        all_places_vct.push_back(place.first);
    }

    return all_places_vct;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    if (id_data.find(id) == id_data.end()){
        auto new_place = std::make_shared<Place>(Place{id,name,type,xy});
        id_data.insert({id,new_place});

        isNameSorted = false;
        isCoordSorted = false;
        return true;
    }else{
        return false;
    }

}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{

    if (id_data.find(id) == id_data.end()){
        return {NO_NAME, PlaceType::NO_TYPE};
    }else{
        std::pair<Name,PlaceType> pair;
        Name name = id_data.at(id)->name;
        PlaceType type = id_data.at(id)->type;
        pair.first = name;
        pair.second = type;
        return pair;
    }
}

Coord Datastructures::get_place_coord(PlaceID id)
{

    if (id_data.find(id) == id_data.end()){
        return NO_COORD;
    }else{
        return id_data.at(id)->coord;
    }

}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    // Replace this comment with your implementation
    if (area_data.empty()){
        auto new_area = std::make_shared<Area>(Area{id,name,coords, NO_AREA,{}});
        area_data.insert({id,new_area});
        return true;
    }else{
        if(area_data.find(id) == area_data.end()){
            auto new_area = std::make_shared<Area>(Area{id,name,coords, NO_AREA,{}});
            area_data.insert({id,new_area});
            return true;
        }else{
            return false;
        }
    }
}

Name Datastructures::get_area_name(AreaID id)
{

    if (area_data.find(id) == area_data.end()){
        return NO_NAME;
    }else{

        Name name = id_data.at(id)->name;
        return name;
    }
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{

    if (area_data.find(id) == area_data.end()){
        return {NO_COORD};
    }else{
        return area_data.at(id)->bounds_;
    }
}

void Datastructures::creation_finished()
{

}


std::vector<PlaceID> Datastructures::places_alphabetically()
{

    if (id_data.size() != 0){
        if(isNameSorted == false){
            place_alpha.clear();
            std::vector<std::pair<PlaceID,Name>> id_names;
            for (auto place : id_data){
                PlaceID id = place.first;
                Name name = place.second->name;
                std::pair<PlaceID,Name> id_name (id,name);
                id_names.push_back(id_name);
            }
            std::sort(id_names.begin(),id_names.end(), &compare_place_name);

            for (auto pair : id_names){
                place_alpha.push_back(pair.first);
            }
            isNameSorted = true;
            return place_alpha;
        }else{
            return place_alpha;
        }
    }else{
        return {};
    }
}

std::vector<PlaceID> Datastructures::places_coord_order()
{

    if (id_data.size() != 0){
        if(isCoordSorted == false){
            std::vector<PlaceID> id_coords;
            id_coords.reserve(place_count());
            for (auto place : id_data){
                PlaceID id = place.first;
                //Coord coord = place.second->coord;
                id_coords.push_back(id);
            }

            std::sort(id_coords.begin(),id_coords.end(),
                      [this](PlaceID id1, PlaceID id2)
                      {return id_data.at(id1)->coord < id_data.at(id2)->coord;});
            isCoordSorted = true;
                //id_coords.push_back(id_coord);
            //std::sort(id_coords.begin(),id_coords.end(),&compare_place_coord);
            return id_coords;
        }
    }else{
        return {};
    }
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{

    std::vector<PlaceID> name_places = {};
    for (auto place : id_data){
        if (place.second->name == name){
            name_places.push_back(place.first);
        }
    }
    std::sort(name_places.begin(),name_places.end());
    return name_places;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{

    std::vector<PlaceID> name_types = {};
    for (auto place : id_data){
        if(place.second->type == type){
            name_types.push_back(place.first);
        }
    }
    std::sort(name_types.begin(),name_types.end());
    return name_types;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    // Replace this comment with your implementation
    if (id_data.find(id) != id_data.end()){
        id_data.at(id)->name = newname;
        isNameSorted = false;
        return true;
    }else{
        return false;
    }
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // Replace this comment with your implementation
    if (id_data.find(id) != id_data.end()){
        id_data.at(id)->coord = newcoord;
        isCoordSorted = false;
        return true;
    }else{

        return false;
    }
}

std::vector<AreaID> Datastructures::all_areas()
{
    std::vector<AreaID>all_areas_vct;
    for (auto area : area_data){
        all_areas_vct.push_back(area.first);
    }
    return all_areas_vct;
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    // Replace this comment with your implementation
    if (area_data.find(id) == area_data.end() || area_data.find(parentid) == area_data.end()){
        return false;
    }

    if (area_data.at(id)->parent_area_id != NO_AREA){
        return false;
    }

    std::shared_ptr<Area> child = area_data.at(id);
    std::shared_ptr<Area> parent = area_data.at(parentid);
    if(child->parent_area_id == NO_AREA){
        child->parent_area_id = parent->id;
        parent->sources_nodes_.push_back(child->id);
        return true;
    }


    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    std::vector<AreaID> temp_area_vct;
    if (area_data.find(id) == area_data.end()){
        return {NO_AREA};
    }else{
        AreaID parent_id = area_data.at(id)->parent_area_id;

        std::vector<AreaID> temp_parent_area;
        if (parent_id != NO_AREA){
            temp_area_vct.push_back(parent_id);

            //Call the grandparent of child using recursion
            temp_parent_area = subarea_in_areas(parent_id);
            for (auto area_: temp_parent_area){
                temp_area_vct.push_back(area_);
            }
        }
    }
    return temp_area_vct;
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::vector<std::pair<PlaceID,long long int>> place_closest;
    place_closest.reserve(3);

    Coord center_coord = xy;
    size_t index_upper = 0;
    std::vector<PlaceID> temp_place_vct;
    if(type !=  PlaceType::NO_TYPE){
        temp_place_vct = find_places_type(type);
    }else{
        temp_place_vct = all_places();
    }

    if(temp_place_vct.empty()){
        return {};
    }

    for (auto it = temp_place_vct.begin(); it != temp_place_vct.end(); it++){
         Coord iter_coord = id_data.at(*it)->coord;
        if(place_closest.size() < 3){


            //Push a new element into place closest list
            place_closest.push_back(std::make_pair(*it,distance_square(iter_coord,center_coord)));

            //Update index to position of upper bound element
            if(place_closest[index_upper].second < place_closest.back().second){
                index_upper = place_closest.size()-1;
            }
        }else{
            //Update list of place closest
            if(place_closest[index_upper].second > distance_square(iter_coord,center_coord)){

                //Insert a new elemnent
                place_closest[index_upper]=std::make_pair(*it,distance_square(iter_coord,center_coord));
            }

            //Update index to element that has maximum distance
            for(unsigned int i = 0; i < place_closest.size();++i){
                if(place_closest[i].second>place_closest[index_upper].second){
                    index_upper = i;
                }
            }
        }
    }

    //Sort the place ID in ascending order of distance to given coordinate
    std::sort(place_closest.begin(),place_closest.end(),
              [](std::pair<PlaceID,int>s1,std::pair<PlaceID,int>s2)
              {return (s1.second < s2.second); });

    //Make the place ID vector
    std::vector<PlaceID> placeID_list;
    for (unsigned int i =0; i < place_closest.size();++i){
        placeID_list.push_back(place_closest[i].first);
    }
    return placeID_list;
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    if(id_data.find(id) == id_data.end()){
        return false;
    }

    //std::shared_ptr<Place> this_place = id_data.at(id);

    id_data.erase(id);
    return true;

}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{

    std::vector<AreaID> all_subareas_vct = {};
    std::vector<AreaID> subareas_child_vct_tmp = {};


    if(area_data.find(id) == area_data.end()){
        return {NO_AREA};
    }else{
        std::vector<AreaID> area_temp_vct = area_data.at(id)->sources_nodes_;
        std::vector<AreaID> subarea_child;
        if (area_temp_vct.size() > 1 || (area_temp_vct.size()==1 && area_temp_vct.at(0) != NO_AREA)){
            for (auto it : area_temp_vct){
                all_subareas_vct.push_back(it);
                subarea_child = all_subareas_in_area(it);
                for (auto iter : subarea_child){
                        all_subareas_vct.push_back(iter);
                }
                //Here it++
            }
        }else{
            //Case subarea has no sub-area.
            //all_subareas_vct.push_back(id);
        }
    }
    return all_subareas_vct;
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    if(area_data.find(id1) == area_data.end()
            || area_data.find(id2) == area_data.end()){
        return NO_AREA;
    }

    std::vector<AreaID> all_parent_area_1 = subarea_in_areas(id1);
    std::vector<AreaID> all_parent_area_2 = subarea_in_areas(id2);

    auto it_1 = all_parent_area_1.rbegin();
    auto it_2 = all_parent_area_2.rbegin();

    while(it_1 != all_parent_area_1.rend() && it_2 != all_parent_area_2.rend()){
        if (*it_1 == *it_2){
            ++it_1;
            ++it_2;
        }else{
            break;
        }
    }

    if(it_1 == all_parent_area_1.rbegin()){
        return NO_AREA;
    }else{
        --it_1;
        return *it_1;
    }
}
