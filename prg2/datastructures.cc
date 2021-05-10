// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <QDebug>
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

Datastructures::Datastructures()
{
    // Replace this comment with your implementation
    //all_coord_way_map={};
    //coords_map = {};
    id_data={};
    all_areas_vct={};
    all_places_vct={};
    place_alpha={};
    place_coord={};
    all_ways_coord_vct = {};
    ways_map = {};


}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}
bool compare_place_name(const std::pair<PlaceID,Name> p1, const std::pair<PlaceID,Name> p2){
    return p1.second < p2.second;
}

long long int Datastructures::distance_square(const Coord& xy1,
    const Coord& xy2)
{
    long long int diff_x = (long long int) xy2.x - (long long int) xy1.x;
    long long int diff_y = (long long int) xy2.y - (long long int) xy1.y;

    return diff_x * diff_x + diff_y * diff_y;
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

        Name name = area_data.at(id)->name;
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
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
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
        }else{
            //Case isCoordSorted is true
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

    place_alpha.erase(std::remove(place_alpha.begin(),place_alpha.end(),id),place_alpha.end());
    place_coord.erase(std::remove(place_coord.begin(),place_coord.end(),id),place_coord.end());

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

// Phase 2 operations
bool Datastructures::existWay(WayID id){

    if (ways_map.find(id) == ways_map.end()){
        return  false;
    }else{
        return true;
    }
}

bool Datastructures::checkCoordExist(std::vector<Coord> coord_vct_, Coord coord_){
    if(coord_vct_.empty()){
        return  false;
    }

    for(unsigned int i = 0 ; i < coord_vct_.size(); i++){
        if(coord_vct_[i] == coord_){
            return  true;
        }
    }
    return false;
}


bool Datastructures::existCoord(Coord id){

    /*
    if(all_ways_coord_vct.empty()){
        return  false;
    }

    if(checkCoordExist(all_ways_coord_vct,id)){
        return true;
    }else{
        return false;
    }*/

    if(coords_map.find(id) != coords_map.end()){
        return  true;
    }else{
        return false;
    }

}

// A function that check the new vector of coordinate
// and update the all_ways_coord_vct
void Datastructures::updateAllWayCoord(std::vector<Coord> coord_vct_){
    for(auto it : coord_vct_){
        if(!checkCoordExist(all_ways_coord_vct,it)){
            //New coordinate for way
            all_ways_coord_vct.push_back(it);
        }
    }
}

std::vector<WayID> Datastructures::all_ways()
{
    if (ways_map.empty()){
        return  {};
    }
    ways_vec ways = {};
    for(auto &pair : ways_map ){
        ways.push_back(pair.first);
    }
    return ways;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{

    //---------------------------------------------Test new data structure
    if(existWay(id) || coords.size() <= 1){
        return false;
    }

    for(auto it = coords.begin(); it != coords.end(); it ++){
        /*
        if(!existCoord(*it)){
            return false;   //Coordinate does not exist ????
        }*/
        //std::pair<Coord,Coord> temp_pair;
        if(it == coords.begin()){
            coords_map[*it].ways_[id] = {NO_COORD,*(it+1)}; //Starting coord
            coords_map[*it].crossRoadWays_[id] = {NO_COORD,coords.back()};
            //qDebug() <<"On right"<< (it+1)->x << "  "<<(it+1)->y;
        }else{
            if(*it == coords.back()){
                coords_map[*it].ways_[id] = {*(it-1),NO_COORD}; // Ending coord
                coords_map[*it].crossRoadWays_[id] = {NO_COORD,coords.front()};
                //qDebug() <<"On left"<< (it-1)->x << "  "<<(it-1)->y;
            }else{
                //qDebug() << "Connecting";
                coords_map[*it].ways_[id] = {*(it-1),*(it+1)}; // Connecting coord
            }
        }
    }
    ways_map[id] = {coords};
    ways_map.at(id).crossRoads_vct.push_back(coords.front());
    ways_map.at(id).crossRoads_vct.push_back(coords.back());

    //qDebug() << ways_map.at(id).crossRoads_vct.front().x <<ways_map.at(id).crossRoads_vct.front().y << "     "
    //         <<ways_map.at(id).crossRoads_vct.back().x << ways_map.at(id).crossRoads_vct.back().y;
    return true;
    //---------------------------------------------Test new data structure
}

Coord Datastructures::getEndingCoord(Coord prevCoord, WayID way){
    auto new_pair = coords_map.at(prevCoord).ways_.at(way);
    if(new_pair.second == NO_COORD){
        return prevCoord;
    }else{
        return getEndingCoord(new_pair.second,way);
    }
}

pathEdge Datastructures::path_breadth_first_search(Coord fromcoord, Coord toCoord)
{
    if(coords_map.find(fromcoord) == coords_map.end() ||
            coords_map.find(toCoord) == coords_map.end()){
        return {};
    }
    qDebug()<<"In side path_breath";
    //Implement breadth first search by using queue
    std::queue<std::pair<Coord,Edge>> searching_coord;
    searching_coord.push(std::pair(fromcoord,Edge()));

    travelHistory coord_visited;

    while(searching_coord.size() != 0){
        std::pair<Coord,Edge> current_coord = searching_coord.front();
        coord_visited.insert({current_coord.first,current_coord.second});
        if(current_coord.first == toCoord){
            break;
        }else{
            //Loop through adjacent coord
            //for(auto it_edge = coords_map.at(current_coord.first).out_edges.begin();
            for(auto it_edge = coords_map[current_coord.first].out_edges.begin();
                it_edge != coords_map.at(current_coord.first).out_edges.end(); it_edge++){
                if(coord_visited.find(it_edge->second.tocoord) == coord_visited.end()){
                    searching_coord.push(std::make_pair(it_edge->second.tocoord,it_edge->second));
                }

            }
        }
    }

    if(searching_coord.size() == 0){
        return  {};

    }

    return make_path(fromcoord,toCoord,coord_visited,false);
}

pathEdge Datastructures::make_path(Coord fromcoord, Coord tocoord, travelHistory &coord_visited, bool with_cycle, Edge last_edge)
{
    pathEdge traceback_edge = {};
    Coord trace_coord = tocoord;

    bool path_has_cycle = false;

    if(with_cycle){
        traceback_edge.push_back(last_edge);
        trace_coord = last_edge.fromscoord;
    }

    do{
        Edge previous_edge = coord_visited.at(trace_coord);
        traceback_edge.push_back(previous_edge);

        trace_coord = previous_edge.fromscoord;

        if(trace_coord == tocoord){
            path_has_cycle = true;
        }
        if(trace_coord == tocoord){
            break;
        }
    }while (trace_coord != fromcoord || (path_has_cycle == false && with_cycle));

    std::reverse(traceback_edge.begin(),traceback_edge.end());
    return traceback_edge;
}

return_tuple Datastructures::make_route_distance(pathEdge path_by_edges)
{
    //Check if there is edge to make route
    if(path_by_edges.size() == 0){
        return {};
    }

    return_tuple route_dist;

    for(auto it_edge = path_by_edges.begin();it_edge != path_by_edges.end();it_edge++){
        //Added information
        Coord curr_coord = it_edge->fromscoord;
        WayID way_to_next_stop = it_edge->way_id;
        Distance dist_to_coord = 0;

        //Calculate distance up to current coord
        if(it_edge != path_by_edges.begin()){
            dist_to_coord = std::get<2>(route_dist.back())+ (--it_edge)->dist;
            ++it_edge;
        }

        route_dist.push_back(std::make_tuple(curr_coord,way_to_next_stop,dist_to_coord));
    }

    auto it_last_edge = path_by_edges.back();

    Distance dist_to_coord = std::get<2>(route_dist.back()) + it_last_edge.dist;
    route_dist.push_back(std::make_tuple(it_last_edge.tocoord,NO_WAY,dist_to_coord));

    return route_dist;

}

bool Datastructures::isCrossRoad(Coord coordCheck, WayID wayCheck)
{
    std::vector<Coord> tempCrossRoad = ways_map.at(wayCheck).crossRoads_vct;
    for(auto it : tempCrossRoad){
        if(it == coordCheck){
            return true;
        }
    }
    return  false;
}


std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{ 
    if(!existCoord(xy)){
        return {{NO_WAY,NO_COORD}};
    }

    connection ways_edge = coords_map[xy].ways_;
    qDebug() << "ways_edge.size()"<< ways_edge.size();

    if(ways_edge.empty()){
        return {}; // No way passing this coordinate
    }

    std::vector<std::pair<WayID, Coord>> way_return_vec = {};
    for(auto it : ways_edge){
        if(it.second.second == NO_COORD){
            //Add the ending coordinate
            //Coord crossroad_beg = ways_map[it.first].coords_.front();
            qDebug() << "Add ending crossroad";
            Coord crossroad_beg = ways_map.at(it.first).crossRoads_vct.front();
            way_return_vec.push_back({it.first,crossroad_beg});
            //continue;
        }else{
            qDebug() << "Add starting and mid crossroad";
            //Coord crossroad_end = ways_map[it.first].coords_.back();
            Coord crossroad_end = ways_map.at(it.first).crossRoads_vct.back();
            way_return_vec.push_back({it.first,crossroad_end});
            //way_return_vec.push_back({it.first,it.second.second});
        }
        //way_return_vec.push_back({it.first,it.second.second});
    }

    return way_return_vec;
}


std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    if(ways_map.find(id) == ways_map.end()){
        return {NO_COORD};
    }else{
        return  ways_map.at(id).coords_;
    }
}

void Datastructures::clear_ways()
{    
    if(!ways_map.empty()){
        ways_map.clear();
    }
}



void Datastructures::BFS(std::list<Coord> *queue, std::vector<Coord> *visited, std::unordered_map<Coord, std::pair<WayID, Coord>, CoordHash > *parent,
                         bool flow){
    //Flow -> flow search, not flow -> backward search
    //Retrive first item from queue
    qDebug() << "Call inside BFS";
    Coord current = queue->front();
    qDebug() << "Current from queue"<<current.x<<"   "<<current.y;


    queue->pop_front();
    auto adj_map = coords_map[current].crossRoadWays_;
    for(auto pair : adj_map){
        // If adjacent vertex is not visited earlier
        // mark it visited by assinging true value
        Coord adj_id;
        if(flow){
            adj_id = pair.second.second;
        }else{
            adj_id = pair.second.first;
        }


        //qDebug()<< "adj_id" << adj_id.x <<"   "<<adj_id.y;
        if(adj_id == NO_COORD){
            if(flow){
                adj_id = pair.second.first;
            }else{
                adj_id = pair.second.second;
            }
        }
        //qDebug()<< "adj_id after inverting" << adj_id.x <<"   "<<adj_id.y;
        if(std::find(visited->begin(),visited->end(),adj_id) == visited->end()){
            qDebug() << "Call unvisited";

            //Set current as parent of this vertex
            (*parent)[adj_id] = {pair.first,current};

            //Mark this vertex as visited
            (*visited).push_back(adj_id);
            qDebug() <<"Visited vec size"<<(*visited).size();
            //Push to the end of queue
            queue->push_back(adj_id);
        }

    }
}

bool Datastructures::DFS(std::vector<Coord> *visited, parent_map *parent, Coord cur_coord, std::tuple<WayID, Coord, Coord> *return_pair){
    bool found = false;
    visited->push_back(cur_coord);
    auto adj_map = coords_map[cur_coord].ways_;
    for(auto pair : adj_map){
        Coord adj_id = pair.second.second;
        if(adj_id == NO_COORD){
            //try
            adj_id = pair.second.first;
            //continue; // Terminus coord
        }

        //Found a repeated coordinate as it has 2 parent
        if(parent->find(adj_id) != parent->end() && (*parent)[adj_id].second != cur_coord){
            *return_pair = {pair.first,cur_coord,adj_id};
            return true;
        }

        //No visited
        if(std::find(visited->begin(),visited->end(),adj_id) == visited->end()){
            // set current as parent of this vertex
            (*parent)[adj_id] = {pair.first,cur_coord};

            //DFS recursion for all unvisited coordinate
            found = DFS(visited,parent,adj_id,return_pair);
            if(found){
                return true;
            }
        }
    }

    return false;
}

Coord Datastructures::isIntersecting(std::vector<Coord> *fw_visited, std::vector<Coord> *bw_visited){
    qDebug() << "Call intersecting";
    auto common_coord = std::find_first_of(fw_visited->begin(),fw_visited->end(),
                                           bw_visited->begin(),bw_visited->end());
    qDebug()<<"Common coord"<<common_coord->x<<"   "<<common_coord->y;


    if(*common_coord != *fw_visited->end()){
        qDebug() << " Find intersecting";
        return *common_coord;
    }else{
        qDebug() << "Find no intersecting";
        return NO_COORD;
    }
}

std::vector<std::pair<WayID, Coord> > Datastructures::bidirPath(parent_map *fw_parent, parent_map *bw_parent, Coord frommcoord, Coord tocoord,
                                                                Coord intersectNode)
{
    //qDebug() << "Call bi dir path";
    std::vector<std::pair<WayID,Coord>> path;
    Coord id = intersectNode;
    //Now we get hte path from forward search
    while(id != frommcoord){
        path.push_back({(*fw_parent)[id].first,(*fw_parent)[id].second});
        id = (*fw_parent)[id].second;
    }
    std::reverse(path.begin(),path.end()); // Reverse as we go backward
    id = intersectNode;
    //Add intersectNode manually
    path.push_back({(*bw_parent)[intersectNode].first,intersectNode});
    // Retrieve path from backward search

    // This time we are in the right track
    while(id != tocoord){
        //qDebug()<< "Call in right track";
        path.push_back({(*bw_parent)[(*bw_parent)[id].second].first,(*bw_parent)[id].second});
        id = (*bw_parent)[id].second;
    }
    //Last element need to be modify to match the format in description
    path.pop_back();
    path.push_back({NO_WAY,tocoord});
    //qDebug() << "Finish bidir path";
    return path;
}

return_tuple_without_dist Datastructures::getTupleWithoutDist(path_vec *path)
{
    return_tuple_without_dist return_vec = {};
    auto it = path->begin();

    while(true){
        return_vec.push_back({it->second,it->first});
        if(it == path->end()){
            break;
        }
        it++;
    }
    return return_vec;
}


return_tuple Datastructures::getTuple(std::vector<std::pair<WayID,Coord>>*path){
    //Get output format from given path
    qDebug() << "Start Get turple";
    Distance distance = 0;
    return_tuple return_vec = {};
    auto it = path->begin();

    while(true){
        return_vec.push_back({it->second,it->first,distance});
        if(*it == path->back()){
            break;
        }
        Distance d_prev = getDistance(it->first);
        distance += d_prev;
        it++;
    }

    return return_vec;

}

Distance Datastructures::getDistance(WayID fromway){

    std::vector<Coord> temp_way_coord_vct = ways_map.at(fromway).coords_;
    Distance dist = 0;
    for(unsigned long i =0; i<temp_way_coord_vct.size()-1;i++){
        int d_ = (temp_way_coord_vct.at(i).x-temp_way_coord_vct.at(i+1).x)*(temp_way_coord_vct.at(i).x-temp_way_coord_vct.at(i+1).x)
                + (temp_way_coord_vct.at(i).y-temp_way_coord_vct.at(i+1).y)*(temp_way_coord_vct.at(i).y-temp_way_coord_vct.at(i+1).y);
        dist += std::floor(sqrt(d_));

    }

    return dist;

}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return_tuple return_vec = route_least_crossroads(fromxy,toxy);
    return return_vec;
}

bool Datastructures::remove_way(WayID id)
{
    // Replace this comment with your implementation
    if(!existWay(id)){
        return false;
    }
    /*
    Coord cross_beg = ways_map.at(id).crossRoads_vct.front();
    Coord cross_end = ways_map.at(id).crossRoads_vct.back();
    if(ways_from(cross_beg).size() < 2){
        //Case start point of of deleted way has no other leading way
        ways_map.at(id).crossRoads_vct.erase(ways_map.at(id).crossRoads_vct.begin());
    }else
        if(ways_from(cross_end).size() < 2){
            //Case the end crossroad of deleted way has no other leading way
            ways_map.at(id).crossRoads_vct.erase(ways_map.at(id).crossRoads_vct.end());
        }
        */
    ways_map.erase(id);
    qDebug() <<"coord map size"<<coords_map.size();
    for(auto i = coords_map.begin(); i!= coords_map.end(); i++){
        qDebug()<<"Start crooss"<<i->first.x<<i->first.y;
        connection ways_edge = coords_map.at(i->first).ways_;
        auto it = ways_edge.find(id);
        //Case that the starting and ending point of deleteed way
        if(it != ways_edge.end()){
            qDebug()<<"Single "<<it->second.first.x << it->second.first.y <<"@"<<it->second.second.x<<it->second.second.y;
            qDebug()<<ways_edge.size();
            //coords_map.erase(i);
            ways_edge.erase(it);
            qDebug()<<"After del size"<<ways_edge.size();
        }
        /*
            //Case the crossroad has multiple way lead to it (included the deleted way)
            if(ways_edge.size()>1 && it != ways_edge.end()){
                qDebug()<<"Multiple "<<it->second.first.x << it->second.first.y <<"@"<<it->second.second.x<<it->second.second.y;
                ways_edge.erase(it);
            }*/
        /*
        if(ways_edge.size() == 0){
            coords_map.erase(i);
        }*/

    }

    //Clean cross road that no way longer lead to
    for(auto it = coords_map.begin(); it != coords_map.end(); it++){
        connection ways_edge = coords_map.at(it->first).ways_;
        if(ways_edge.size() == 0){
            coords_map.erase(it);
        }
    }
    //qDebug()<<"4";
    return true;

}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    /*---------*/
    // Replace this comment with your implementation
    if(!existCoord(fromxy) || !existCoord(toxy) || fromxy == toxy){
        return {{NO_COORD, NO_WAY, NO_DISTANCE}};
    }
    // In this function I use bidirectional BFS search for fromcoord and tocoord

    //Visited coord of forward and backward searches
    std::vector<Coord> fw_visited, bw_visited = {};

    //Save previous Coordinate and WayID from that Crossroad
    parent_map fw_parent = {};
    parent_map bw_parent = {};

    // queue for front and backward search
    std::list<Coord> fw_queue, bw_queue;

    Coord intersectNode = NO_COORD;

    fw_queue.push_back(fromxy);
    fw_visited.push_back(fromxy);


    bw_queue.push_back(toxy);
    bw_visited.push_back(toxy);

    while(!fw_queue.empty() && !bw_queue.empty()){
        // Do BFS from source and target vertices.
        // True indicate forward search and false show backward search
        BFS(&fw_queue,&fw_visited,&fw_parent,true);
        BFS(&bw_queue,&bw_visited,&bw_parent,false);

        /*
        qDebug()<<"fw_queue size"<<fw_queue.size();
        qDebug()<<"fw_visited size"<<fw_visited.size();
        qDebug()<<"bw_queue size"<<bw_queue.size();
        qDebug()<<"bw_visied size"<<bw_visited.size();
        */

        // check for intersecting vertex
        intersectNode = isIntersecting(&fw_visited,&bw_visited);

        //If intersecting vertex is found that mean there exist a path
        if(intersectNode != NO_COORD){
            //Get the path and return

            //qDebug()<<"fw parent size"<<fw_parent.size();
            //qDebug()<<"fw parent size"<<bw_parent.size();
            auto path = bidirPath(&fw_parent,&bw_parent,fromxy,toxy,intersectNode);
            return getTuple(&path);
        }
    }

    return {{NO_COORD,NO_WAY,NO_DISTANCE}};// No route has been found
    /*-----------------------*/
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    if(!existCoord(fromxy)){
        return {{NO_COORD, NO_WAY}};
    }

    // In this function, I use recursive DFS.

     std::vector<Coord> visited = {}; // Vector storing visited stops.
     parent_map parent = {}; // Parent of each visited stops.

     std::tuple<WayID,Coord,Coord> cycle_point = {NO_WAY,NO_COORD,NO_COORD};
    bool found = DFS(&visited, &parent, fromxy, &cycle_point); // Recursive DFS.
    qDebug() << "After call DFS";
    if(!found){
        qDebug() << "Not found";
        return {};
    }

    Coord cur_coord = std::get<1>(cycle_point);
    path_vec path = {};
    // Now we get the path from the starting stop to the second last stop
    // by tracing back the parent of each stop.
    path.push_back({std::get<0>(cycle_point), cur_coord}); // Second last stop
    while (cur_coord != fromxy) {
        qDebug() << "Backward traversal";
        path.push_back({parent[cur_coord].first, parent[cur_coord].second});
        cur_coord = parent[cur_coord].second;
    }
    // As we go backward, we have to reverse the path found.
    std::reverse(path.begin(), path.end());
    qDebug() <<"After reverse";
    path.push_back({NO_WAY, std::get<2>(cycle_point)}); // Repeating stop, end point.
    return getTupleWithoutDist(&path);

}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    //return {{NO_COORD, NO_WAY, NO_DISTANCE}};

    //return route_least_crossroads(fromxy,toxy);
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}
