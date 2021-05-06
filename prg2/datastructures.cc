// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

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

int Datastructures::place_count()
{
    // Replace this comment with your implementation
    return 0;
}

void Datastructures::clear_all()
{
    // Replace this comment with your implementation
}

std::vector<PlaceID> Datastructures::all_places()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    // Replace this comment with your implementation
    return false;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    // Replace this comment with your implementation
    return {NO_NAME, PlaceType::NO_TYPE};
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    // Replace this comment with your implementation
    return NO_COORD;
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    // Replace this comment with your implementation
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    // Replace this comment with your implementation
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    // Replace this comment with your implementation
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}

// Phase 2 operations
bool Datastructures::existWay(WayID id){

    if(ways_map.empty()){
        return false;
    }

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

/*
bool Datastructures::existCoord(Coord id){

    if(all_ways_coord_vct.empty()){
        return  false;
    }

    if(checkCoordExist(all_ways_coord_vct,id)){
        return true;
    }else{
        return false;
    }
}*/

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
        return  {NO_WAY};
    }
    ways_vec ways = {};
    for(auto &pair : ways_map ){
        ways.push_back(pair.first);
    }
    return ways;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{
    //Invalid coordinate for way
    if( coords.size() <= 1){
        return  false;
    }


    if(ways_map.empty()){
        Way new_way = Way{coords};
        ways_map.insert({id,new_way});
        updateAllWayCoord(coords);
        return true;
    }else{
        if(ways_map.find(id) == ways_map.end()){
            Way new_way = Way{coords};
            ways_map.insert({id,new_way});
            updateAllWayCoord(coords);
            return true;
        }else{
            //Way already exist
            return false;
        }
    }
    /*
    if(ways_map.empty()){
        std::vector<std::pair<Coord,Coord>> temp_vector;
        for(auto it = coords.begin(); it!= coords.end(); it++){
            if(it == coords.begin()){
                std::pair<Coord,Coord> temp_pair =std::make_pair(NO_COORD,coords.front());
                temp_vector.push_back(temp_pair);
                Way new_way = Way{temp_vector};
                ways_map.insert({id,new_way});
                updateAllWayCoord(coords);
                return  true;
            }else if ( it == coords.end()){
                std::pair<Coord,Coord> temp_pair =std::make_pair(coords.back(),NO_COORD);
                temp_vector.push_back(temp_pair);
                Way new_way = Way{temp_vector};
                ways_map.insert({id,new_way});
                updateAllWayCoord(coords);
                return  true;
            }else{
                std::pair<Coord,Coord> temp_pair =std::make_pair(*it,*it);
                temp_vector.push_back(temp_pair);
                Way new_way = Way{temp_vector};
                ways_map.insert({id,new_way});
                updateAllWayCoord(coords);
                return  true;
            }
        }
    }else{
        //Way already exist
        if(existWay(id)){
            return false;
        }else{
            //Case way map contain some way.
            std::vector<std::pair<Coord,Coord>> temp_vector;
            for(auto it = coords.begin(); it!= coords.end(); it++){
                if(it == coords.begin()){
                    std::pair<Coord,Coord> temp_pair =std::make_pair(NO_COORD,coords.front());
                    temp_vector.push_back(temp_pair);
                    Way new_way = Way{temp_vector};
                    ways_map.insert({id,new_way});
                    updateAllWayCoord(coords);
                    return  true;
                }else if ( it == coords.end()){
                    std::pair<Coord,Coord> temp_pair =std::make_pair(coords.back(),NO_COORD);
                    temp_vector.push_back(temp_pair);
                    Way new_way = Way{temp_vector};
                    ways_map.insert({id,new_way});
                    updateAllWayCoord(coords);
                    return  true;
                }else{
                    std::pair<Coord,Coord> temp_pair =std::make_pair(*it,*it);
                    temp_vector.push_back(temp_pair);
                    Way new_way = Way{temp_vector};
                    ways_map.insert({id,new_way});
                    updateAllWayCoord(coords);
                    return  true;
                }
            }
        }
    }*/
    return false;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{ 

    if(!checkCoordExist(all_ways_coord_vct,xy) || ways_map.empty()){
        return {{NO_WAY, NO_COORD}};
    }


    std::vector<std::pair<WayID, Coord>> temp_coord_way;
    /*
    for(auto it : ways_map){
        if(it.second.coords_.empty()){
            //No crossroad case, return empty vector
            return {};
        }else{
            //Starting or ending crosssroad of way
            if( (it.second.coords_.front().first == NO_COORD && it.second.coords_.front().second == xy) ||
                (it.second.coords_.back().second == NO_COORD && it.second.coords_.back().first == xy) ){
                temp_coord_way.push_back(std::make_pair(it.first,xy));
                continue;
            }else{
                if(it.second.coords_.front().first == xy){
                    temp_coord_way.push_back(std::make_pair(it.first,xy));
                }
            }
        }
    }*/

    for(auto it : ways_map){
        if(it.second.coords_.empty()){
            return {};
        }else{
            if(it.second.coords_.front() == xy){
                temp_coord_way.push_back(std::make_pair(it.first,it.second.coords_.back()));
            }else if(it.second.coords_.back() == xy){
                temp_coord_way.push_back(std::make_pair(it.first,it.second.coords_.front()));
            }
        }
    }

    return temp_coord_way;
}


std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    //std::vector<Coord> temp_coord_vct_;

    if(ways_map.empty()){
        return {NO_COORD};
    }

    if(ways_map.find(id) == ways_map.end()){
        return {};
    }else{
        /*
        for(auto it : ways_map.at(id).coords_){
            if(it.first == NO_COORD){
                temp_coord_vct_.push_back(it.second);
            }else{
                temp_coord_vct_.push_back(it.first);
            }
        }*/
        return  ways_map.at(id).coords_;
    }
    //return temp_coord_vct_;


}

void Datastructures::clear_ways()
{    
    if(!ways_map.empty()){
        ways_map.clear();
    }
}

void Datastructures::BFS(std::list<Coord> *queue, std::vector<Coord> *visited, std::unordered_map<Coord, std::pair<WayID, Coord> > *parent, bool flow){
    //Flow -> flow search, not flow -> backward search
    //Retrive first item from queue
    Coord current = queue->front();
    queue->pop_front();
    auto adj_map = coords_map[current].ways_;
    for(auto pair : adj_map){
        // If adjacent vertex is not visited earlier
        // mark it visited by assinging true value
        Coord adj_id;
        if(flow){
            adj_id = pair.second.second;
        }else{
            adj_id = pair.second.first;
        }

        if(adj_id == NO_COORD){
            continue; //Terminus coord
        }

        //Not visited
        if(std::find(visited->begin(),visited->end(),adj_id) == visited->end()){
            //Set current as parent of this vertex
            (*parent)[adj_id] = {pair.first,current};

            //Mark this vertex as visited
            (*visited).push_back(adj_id);
        }
    }
}

bool Datastructures::DFS(std::vector<Coord> *visited, std::unordered_map<Coord, std::pair<WayID, Coord> > *parent, Coord cur_coord, std::tuple<WayID, Coord, Coord> *return_pair){
    bool found = false;
    visited->push_back(cur_coord);
    auto adj_map = coords_map[cur_coord].ways_;
    for(auto pair : adj_map){
        Coord adj_id = pair.second.second;
        if(adj_id == NO_COORD){
            continue; // Terminus coord
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

    auto common_coord = std::find_first_of(fw_visited->begin(),fw_visited->end(),
                                           bw_visited->begin(),bw_visited->end());
    if(*common_coord != *fw_visited->end()){
        return *common_coord;
    }else{
        return NO_COORD;
    }
}

std::vector<std::pair<WayID, Coord> > Datastructures::bidirPath(parent_map *fw_parent, parent_map *bw_parent, Coord frommcoord, Coord tocoord, Coord intersectNode)
{
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
       path.push_back({(*bw_parent)[(*bw_parent)[id].second].first,(*bw_parent)[id].second});
       id = (*bw_parent)[id].second;
   }
   //Last element need to be modify to match the format in description
   path.pop_back();
   path.push_back({NO_WAY,tocoord});
   return path;
}


return_tuple Datastructures::getTuple(std::vector<std::pair<WayID,Coord>>*path){
    //Get output format from given path
    Distance distance = 0;
    return_tuple return_vec = {};
    auto it = path->begin();
    while(true){
        /*
        std::tuple<Coord,WayID,Distance> temp_tuple;
        temp_tuple = {it->second,it->first,distance};*/
        return_vec.push_back({it->second,it->first,distance});
        it++;
        if(it == path->end()){
            break;
        }
        distance += getDistance(it->second,(it-1)->second);
    }
    return return_vec;
}

Distance Datastructures::getDistance(Coord frompcoord, Coord tocoord){
    int d_2 = (frompcoord.x - tocoord.x)*(frompcoord.x - tocoord.x) + (frompcoord.y - tocoord.y)*(frompcoord.y - tocoord.y);
    return int(sqrt(d_2));
}
std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

bool Datastructures::remove_way(WayID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}
