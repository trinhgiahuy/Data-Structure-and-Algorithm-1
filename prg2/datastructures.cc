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
    /*
    //--------------------------------------------My working data structure
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
    return false;
    //--------------------------------------------My working data structure
    */


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
    }
    return false;
    */

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
        qDebug()<<"5";
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
        qDebug()<<"6";
        if(current_coord.first == toCoord){
            qDebug()<<"7";
            break;
        }else{
            //Loop through adjacent coord
            //for(auto it_edge = coords_map.at(current_coord.first).out_edges.begin();
            qDebug()<<"8";
            for(auto it_edge = coords_map[current_coord.first].out_edges.begin();
                it_edge != coords_map.at(current_coord.first).out_edges.end(); it_edge++){
                qDebug()<<"9";
                if(coord_visited.find(it_edge->second.tocoord) == coord_visited.end()){
                    qDebug()<<"10";
                    searching_coord.push(std::make_pair(it_edge->second.tocoord,it_edge->second));
                }

            }
        }
    }

    qDebug()<<"11";
    if(searching_coord.size() == 0){
        return  {};

    }

    return make_path(fromcoord,toCoord,coord_visited,false);
    qDebug()<<"12 After return BFS";
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
    /*
    if(!checkCoordExist(all_ways_coord_vct,xy) || ways_map.empty()){
        return {{NO_WAY, NO_COORD}};
    }*/
    if(!existCoord(xy)){
        return {{NO_WAY,NO_COORD}};
    }

    auto ways = coords_map[xy].ways_;


    if(ways.empty()){
        return {}; // No way passing this coordinate
    }

    std::vector<std::pair<WayID, Coord>> way_return_vec = {};
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


    for(auto it : ways){
        if(it.second.second == NO_COORD){
            //Add the ending coordinate
            //Coord crossroad_beg = ways_map[it.first].coords_.front();
            Coord crossroad_beg = ways_map.at(it.first).crossRoads_vct.front();
            way_return_vec.push_back({it.first,crossroad_beg});
            //continue;
        }else{
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
    //std::vector<Coord> temp_coord_vct_;
    /*
    if(ways_map.empty()){
        return {NO_COORD};
    }*/

    if(ways_map.find(id) == ways_map.end()){
        return {NO_COORD};
    }else{
        /*
        for(auto it : ways_map.at(id).coords_){
            temp_coord_vct_.push_back(it);
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


        qDebug()<< "adj_id" << adj_id.x <<"   "<<adj_id.y;
        if(adj_id == NO_COORD){
            if(flow){
                adj_id = pair.second.first;
            }else{
                adj_id = pair.second.second;
            }
        }
        qDebug()<< "adj_id after inverting" << adj_id.x <<"   "<<adj_id.y;
        //if(isCrossRoad(adj_id,pair.first)){
            //?? Revert the searching
            /*
            if(flow){
                adj_id = pair.second.first;
            }else{
                adj_id = pair.second.second;
            }*/
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
            //continue;
        //}
        //qDebug()<< "adj_id after invert" << adj_id.x <<"   "<<adj_id.y;
        //Not visited

    }
}

bool Datastructures::DFS(std::vector<Coord> *visited, parent_map *parent, Coord cur_coord, std::tuple<WayID, Coord, Coord> *return_pair){
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
    qDebug() << "Call bi dir path";
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
       qDebug()<< "Call in right track";
       path.push_back({(*bw_parent)[(*bw_parent)[id].second].first,(*bw_parent)[id].second});
       id = (*bw_parent)[id].second;
   }
   //Last element need to be modify to match the format in description
   path.pop_back();
   path.push_back({NO_WAY,tocoord});
   qDebug() << "Finish bidir path";
   return path;
}


return_tuple Datastructures::getTuple(std::vector<std::pair<WayID,Coord>>*path){
    //Get output format from given path
    qDebug() << "Start Get turple";
    Distance distance = 0;
    return_tuple return_vec = {};
    auto it = path->begin();
    while(true){
        return_vec.push_back({it->second,it->first,distance});
        it++;
        if(it == path->end()){
            break;
        }
        distance += getDistance(it->second,(it-1)->second);
    }
    return return_vec;
    qDebug() << "End Get turple";
}

Distance Datastructures::getDistance(Coord frompcoord, Coord tocoord){
    int d_2 = (frompcoord.x - tocoord.x)*(frompcoord.x - tocoord.x) + (frompcoord.y - tocoord.y)*(frompcoord.y - tocoord.y);
    return std::floor(sqrt(d_2));
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
    return false;
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

        qDebug()<<"fw_queue size"<<fw_queue.size();
        qDebug()<<"fw_visited size"<<fw_visited.size();
        qDebug()<<"bw_queue size"<<bw_queue.size();
        qDebug()<<"bw_visied size"<<bw_visited.size();

        // check for intersecting vertex
        intersectNode = isIntersecting(&fw_visited,&bw_visited);

        //If intersecting vertex is found that mean there exist a path
        if(intersectNode != NO_COORD){
            //Get the path and return
            qDebug()<<"fw parent size"<<fw_parent.size();
            qDebug()<<"fw parent size"<<bw_parent.size();
            auto path = bidirPath(&fw_parent,&bw_parent,fromxy,toxy,intersectNode);
            return getTuple(&path);
        }
    }

    return {{NO_COORD,NO_WAY,NO_DISTANCE}};// No route has been found
    /*-----------------------*/


    /*-----------------
    if(coords_map.find(fromxy) == coords_map.end() ||
            coords_map.find(toxy) == coords_map.end()){
        qDebug()<<"1";
        return {{NO_COORD,NO_WAY,NO_DISTANCE}};
    }
    if(fromxy == toxy){
        qDebug()<<"2";
        return  {};
    }

    qDebug()<<"3";
    pathEdge path_bfs = path_breadth_first_search(fromxy,toxy);
    qDebug()<<"4";
    return make_route_distance(path_bfs);
    ----------------------*/

}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    //return {{NO_COORD, NO_WAY, NO_DISTANCE}};

    return route_least_crossroads(fromxy,toxy);
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}
