// The main program (provided by the course), TIE-20100/TIE-20106
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do so
//  (for debugging, for example), DO NOT commit any changes to git, but revert all
//  changes later. Otherwise you won't be able to get any updates/fixes to this
//  file from git!)

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;
using std::noskipws;

#include <iomanip>
using std::setfill;
using std::setw;

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;
using std::ostringstream;
using std::stringstream;

#include <iomanip>
using std::setw;

#include <tuple>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;

#include <regex>
using std::regex_match;
using std::regex_search;
using std::smatch;
using std::regex;
using std::sregex_token_iterator;

#include <algorithm>
using std::find_if;
using std::find;
using std::binary_search;
using std::max_element;
using std::max;
using std::min;
using std::shuffle;
using std::sort;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

#include <chrono>

#include <functional>
using std::function;
using std::equal_to;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <array>
using std::array;

#include <bitset>
using std::bitset;

#include <iterator>
using std::next;

#include <ctime>
using std::time;

#include <memory>
using std::move;

#include <utility>
using std::pair;

#include <cmath>
using std::abs;

#include <cstdlib>
using std::div;

#include <algorithm>
using std::transform;

#include <iterator>
using std::back_inserter;

#include <cstddef>
#include <cassert>


#include "mainprogram.hh"

#include "datastructures.hh"

#ifdef GRAPHICAL_GUI
#include "mainwindow.hh"
#endif

string const MainProgram::PROMPT = "> ";

MainProgram::CmdResult MainProgram::cmd_add_place(std::ostream& output, MatchIter begin, MatchIter end)
{
    string idstr = *begin++;
    string name = *begin++;
    string typestr = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    PlaceID id = convert_string_to<PlaceID>(idstr);
    PlaceType type = convert_string_to_placetype(typestr);
    if (type == PlaceType::NO_TYPE)
    {
        output << "Impossible place type: " << typestr << endl;
        return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, {NO_PLACE}}};
    }
    Coord xy = {convert_string_to<int>(xstr), convert_string_to<int>(ystr)};

    bool success = ds_.add_place(id, name, type, xy);
    if (!success) { id = NO_PLACE; }

    view_dirty = true;
    return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, {id}}};
}

MainProgram::CmdResult MainProgram::cmd_place_name_type(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string placeidstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    PlaceID placeid = convert_string_to<PlaceID>(placeidstr);

    auto [name, type] = ds_.get_place_name_type(placeid);
    if (name == NO_NAME)
    {
        return {ResultType::PLACEIDLIST, MainProgram::CmdResultPlaceIDs{NO_AREA, {NO_PLACE}}};
    }
    else
    {
        output << "Place ID " << placeid << " has name '" << name << "' and type '" << convert_placetype_to_string(type) << "'" << endl;
        return {ResultType::PLACEIDLIST, MainProgram::CmdResultPlaceIDs{NO_AREA, {placeid}}};
    }
}

void MainProgram::test_place_name_type()
{
    if (random_places_added_ > 0) // Don't do anything if there's no places
    {
        PlaceID id = n_to_placeid(random<decltype(random_places_added_)>(0, random_places_added_));
        ds_.get_place_name_type(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_place_coord(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string placeidstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    PlaceID placeid = convert_string_to<PlaceID>(placeidstr);

    auto coord = ds_.get_place_coord(placeid);
    if (coord == NO_COORD)
    {
        return {ResultType::PLACEIDLIST, MainProgram::CmdResultPlaceIDs{NO_AREA, {NO_PLACE}}};
    }
    else
    {
        output << "Place ID " << placeid << " is in position ";
        print_coord(coord, output);
        return {ResultType::PLACEIDLIST, MainProgram::CmdResultPlaceIDs{NO_AREA, {placeid}}};
    }
}

void MainProgram::test_place_coord()
{
    if (random_places_added_ > 0) // Don't do anything if there's no places
    {
        PlaceID id = n_to_placeid(random<decltype(random_places_added_)>(0, random_places_added_));
        ds_.get_place_coord(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_add_area(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    string name = *begin++;
    string coordsstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID id = convert_string_to<AreaID>(idstr);

    vector<Coord> coords;
    smatch coord;
    auto sbeg = coordsstr.cbegin();
    auto send = coordsstr.cend();
    for ( ; regex_search(sbeg, send, coord, coords_regex_); sbeg = coord.suffix().first)
    {
        coords.push_back({convert_string_to<int>(coord[1]),convert_string_to<int>(coord[2])});
    }

    if (coords.size() < 3)
    {
        output << "An area must have at least 3 coords, only " << coords.size() << " coords given!" << endl;
        return {};
    }

    bool success = ds_.add_area(id, name, coords);

    if (success)
    {
        view_dirty = true;
        return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{id, {}}};
    }
    else
    {
        output << "Adding area failed." << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_area_name(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID id = convert_string_to<AreaID>(idstr);

    auto result = ds_.get_area_name(id);
    if (result == NO_NAME)
    {
        return {ResultType::AREAIDLIST, MainProgram::CmdResultAreaIDs{NO_AREA}};
    }
    else
    {
        output << "Area ID " << id << " has name '" << result << "'" << endl;
        return {ResultType::AREAIDLIST, MainProgram::CmdResultAreaIDs{id}};
    }
}

void MainProgram::test_area_name()
{
    if (random_areas_added_ > 0)
    {
        auto id = n_to_areaid(random<decltype(random_areas_added_)>(0, random_areas_added_));
        ds_.get_area_name(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_area_coords(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID id = convert_string_to<AreaID>(idstr);

    auto coords = ds_.get_area_coords(id);

    if (coords.empty())
    {
        output << "No coords returned!" << endl;
        return {};
    }

    if (coords.size() == 1 && coords.front() == NO_COORD)
    {
        return {ResultType::AREAIDLIST, MainProgram::CmdResultAreaIDs{NO_AREA}};
    }

    output << "Area "; print_area(id,output,false); output << " has coords:" << endl;
    std::for_each(coords.begin(), coords.end(), [&output,this](auto const& coord){ print_coord(coord,output); });
    output << endl;

    return {ResultType::AREAIDLIST, MainProgram::CmdResultAreaIDs{id}};
}

MainProgram::CmdResult MainProgram::cmd_creation_finished(std::ostream& output, MainProgram::MatchIter /*begin*/, MainProgram::MatchIter /*end*/)
{
    ds_.creation_finished();
    output << "Creation finished.";

    return {};
}

MainProgram::CmdResult MainProgram::cmd_change_place_name(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    string newname = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    PlaceID id = convert_string_to<PlaceID>(idstr);

    bool success = ds_.change_place_name(id, newname);
    if (!success) { id = NO_PLACE; }

    view_dirty = true;
    return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, {id}}};
}

void MainProgram::test_change_place_name()
{
  if (random_places_added_ > 0) // Don't do anything if there's no places
  {
      PlaceID id = n_to_placeid(random<decltype(random_places_added_)>(0, random_places_added_));
      auto newname = n_to_name(random<decltype(random_places_added_)>(0, random_places_added_));
      ds_.change_place_name(id, newname);
  }
}

MainProgram::CmdResult MainProgram::cmd_change_place_coord(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    string xstr = *begin++;
    string ystr = *begin++;

    assert( begin == end && "Impossible number of parameters!");

    PlaceID id = convert_string_to<PlaceID>(idstr);
    int x = convert_string_to<int>(xstr);
    int y = convert_string_to<int>(ystr);

    bool success = ds_.change_place_coord(id, {x, y});
    if (!success) { id = NO_PLACE; }

    view_dirty = true;
    return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, {id}}};
}

void MainProgram::test_change_place_coord()
{
    if (random_places_added_ > 0) // Don't do anything if there's no places
    {
        PlaceID id = n_to_placeid(random<decltype(random_places_added_)>(0, random_places_added_));
        auto x = random(0, 1000);
        auto y = random(0, 1000);
        ds_.change_place_coord(id, {x, y});
    }
}

MainProgram::CmdResult MainProgram::cmd_add_subarea_to_area(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string sourceidstr = *begin++;
    string targetidstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID sourceid = convert_string_to<AreaID>(sourceidstr);
    AreaID targetid = convert_string_to<AreaID>(targetidstr);

    view_dirty = true;

    bool ok = ds_.add_subarea_to_area(sourceid, targetid);
    if (ok)
    {
        auto sourcename = ds_.get_area_name(sourceid);
        auto targetname = ds_.get_area_name(targetid);
        output << "Added subarea " << sourcename << " to area " << targetname << endl;
    }
    else
    {
        output << "Adding subarea to area failed!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_add_way(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    string coordsstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    WayID id = idstr;

    vector<Coord> coords;
    smatch coord;
    auto sbeg = coordsstr.cbegin();
    auto send = coordsstr.cend();
    for ( ; regex_search(sbeg, send, coord, coords_regex_); sbeg = coord.suffix().first)
    {
        coords.push_back({convert_string_to<int>(coord[1]),convert_string_to<int>(coord[2])});
    }

    if (coords.size() < 2)
    {
        output << "A way must have at least 2 points, only " << coords.size() << " points given!" << endl;
        return {};
    }

    bool ok = ds_.add_way(id, coords);
    if (ok)
    {
        output << "Added way " << id << " with coords:";
        std::for_each(coords.begin(), coords.end(), [&output,this](auto const& coord){ output << ' '; print_coord(coord,output,false); });
        output << endl;

        std::vector<std::tuple<Coord, Coord, WayID, Distance>> result;
        result.emplace_back(coords.front(), coords.back(), id, NO_DISTANCE);
        result.emplace_back(coords.back(), NO_COORD, NO_WAY, NO_DISTANCE);
        return {ResultType::ROUTE, CmdResultRoute{result}};
    }
    else
    {
        output << "Adding way failed!" << endl;
        return {};
    }
}

MainProgram::CmdResult MainProgram::cmd_subarea_in_areas(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID id = convert_string_to<AreaID>(idstr);

    output << "Area hierarchy for area ";
    print_area(id, output);

    auto result = ds_.subarea_in_areas(id);
    if (result.empty()) { output << "Area is not a subarea of any area." << endl; }
    return {ResultType::AREAIDLIST, result};
}

void MainProgram::test_subarea_in_areas()
{
    if (random_areas_added_ > 0) // Don't do anything if there's no places
    {
        auto id = n_to_areaid(random<decltype(random_areas_added_)>(0, random_areas_added_));
        ds_.subarea_in_areas(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_all_subareas_in_area(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID id = convert_string_to<AreaID>(idstr);

    output << "All subareas of ";
    print_area(id, output);

    auto result = ds_.all_subareas_in_area(id);
    sort(result.begin(), result.end());
    if (result.empty()) { output << "No subareas found." << endl; }
    return {ResultType::AREAIDLIST, result};
}

void MainProgram::test_all_subareas_in_area()
{
    if (random_areas_added_ > 0) // Don't do anything if there's no places
    {
        auto id = n_to_areaid(random<decltype(random_areas_added_)>(0, random_areas_added_));
        ds_.all_subareas_in_area(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_places_closest_to(std::ostream& /*output*/, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
  string xstr = *begin++;
  string ystr = *begin++;
  string typestr = *begin++;
  assert( begin == end && "Impossible number of parameters!");

  Coord coord = {convert_string_to<int>(xstr),convert_string_to<int>(ystr)};
  PlaceType type = PlaceType::NO_TYPE;
  if (!typestr.empty())
  {
      type = convert_string_to_placetype(typestr);
  }

  auto result = ds_.places_closest_to(coord, type);
  return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, result}};
}

void MainProgram::test_places_closest_to()
{
    if (random_places_added_ > 0) // Don't do anything if there's no places
    {
        auto x = random<int>(0, 1000);
        auto y = random<int>(0, 1000);
        PlaceType type{random(0, static_cast<int>(PlaceType::NO_TYPE))};
        ds_.places_closest_to({x,y}, type);
    }
}

MainProgram::CmdResult MainProgram::cmd_common_area_of_subareas(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string id1str = *begin++;
    string id2str = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    AreaID id1 = convert_string_to<PlaceID>(id1str);
    AreaID id2 = convert_string_to<PlaceID>(id2str);

    auto result = ds_.common_area_of_subareas(id1, id2);
    if (result == NO_AREA)
    {
        output << "No common area found!" << endl;
        return {};
    }

    output << "Common area of areas "; print_area(id1, output, false); output << " and "; print_area(id2, output, false); output << " is:" << endl;
    return {ResultType::AREAIDLIST, CmdResultAreaIDs{{result}}};
}

void MainProgram::test_common_area_of_subareas()
{
    if (random_areas_added_ > 0) // Don't do anything if there's no places
    {
        auto id1 = n_to_areaid(random<decltype(random_areas_added_)>(0, random_areas_added_));
        auto id2 = n_to_areaid(random<decltype(random_areas_added_)>(0, random_areas_added_));
        ds_.common_area_of_subareas(id1, id2);
    }
}

MainProgram::CmdResult MainProgram::cmd_all_ways(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto wayids = ds_.all_ways();
    if (wayids.empty())
    {
        output << "No ways!" << endl;
    }

    sort(wayids.begin(), wayids.end());

    unsigned int i = 1;
    for (auto const& wayid : wayids)
    {
        output << i <<". " << wayid << endl;
        ++i;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_ways_from(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string xstr = *begin++;
    string ystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord coord = {convert_string_to<int>(xstr),convert_string_to<int>(ystr)};

    auto ways = ds_.ways_from(coord);
    if (ways.empty())
    {
        output << "No ways from coord ";
        print_coord(coord, output);
    }

    sort(ways.begin(), ways.end());
    vector<tuple<Coord, Coord, WayID, Distance>> result;
    transform(ways.begin(), ways.end(), back_inserter(result),
              [coord](auto way)mutable{ return make_tuple(coord, way.second, way.first, NO_DISTANCE); });

    return {ResultType::WAYS, CmdResultRoute{result}};
}

void MainProgram::test_ways_from()
{
 if (random_ways_added_ > 0) // Don't do anything if there's no ways
 {
     auto coord = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));
     ds_.ways_from(coord);
 }
}

MainProgram::CmdResult MainProgram::cmd_way_coords(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    WayID id = idstr;

    auto coords = ds_.get_way_coords(id);

    if (coords.empty())
    {
        output << "No coords returned!" << endl;
        return {};
    }

    output << "Way "; print_way(id, output, false); output << " has coords:" << endl;
    std::for_each(coords.begin(), coords.end(), [&output,this](auto const& coord){ print_coord(coord,output); });
    output << endl;

    return {};
}


void MainProgram::test_way_coords()
{
    if (random_ways_added_ > 0)
    {
        WayID id = n_to_wayid(random<decltype(random_ways_added_)>(0, random_ways_added_));
        ds_.get_way_coords(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_remove_place(std::ostream& output, MatchIter begin, MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    PlaceID id = convert_string_to<PlaceID>(idstr);
    auto [name,type] = ds_.get_place_name_type(id);
    bool success = ds_.remove_place(id);
    if (success)
    {
        output << "Place " << name << "(" << convert_placetype_to_string(type) << ") removed." << endl;
        view_dirty = true;
        return {};
    }
    else
    {
        return {ResultType::PLACEIDLIST, MainProgram::CmdResultPlaceIDs{NO_AREA, {NO_PLACE}}};
    }
}

void MainProgram::test_remove_place()
{
    // Choose random number to remove
    if (random_places_added_ > 0) // Don't remove if there's nothing to remove
    {
        auto id = n_to_placeid(random<decltype(random_places_added_)>(0, random_places_added_));
        ds_.remove_place(id);
    }
}

void MainProgram::add_random_places_areas(unsigned int size, Coord min, Coord max)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        auto name = n_to_name(random_places_added_);
        PlaceID id = n_to_placeid(random_places_added_);
        PlaceType type{random(0, static_cast<int>(PlaceType::NO_TYPE))};

        int x = random<int>(min.x, max.x);
        int y = random<int>(min.y, max.y);

        ds_.add_place(id, name, type, {x, y});

        // Add a new area for every 10 places
        if (random_places_added_ % 10 == 0)
        {
            auto areaid = n_to_areaid(random_areas_added_);
            vector<Coord> coords;
            for (int j=0; j<3; ++j)
            {
                coords.push_back({random<int>(min.x, max.x),random<int>(min.y, max.y)});
            }
            ds_.add_area(areaid, convert_to_string(areaid), std::move(coords));
            // Add area as subarea so that we get a binary tree
            if (random_areas_added_ > 0)
            {
//                auto parentid = random<decltype(random_areas_added_)>(0, random_areas_added_);
                auto parentid = n_to_areaid(random_areas_added_ / 2);
                ds_.add_subarea_to_area(areaid, parentid);
            }
            ++random_areas_added_;
        }

        ++random_places_added_;
    }
}

MainProgram::CmdResult MainProgram::cmd_random_add(std::ostream& output, MatchIter begin, MatchIter end)
{
    string sizestr = *begin++;
    string minxstr = *begin++;
    string minystr = *begin++;
    string maxxstr = *begin++;
    string maxystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    Coord def_min{1, 1};
    Coord def_max{100, 100};
    Coord min = def_min;
    Coord max = def_max;
    if (!minxstr.empty() && !minystr.empty() && !maxxstr.empty() && !maxystr.empty())
    {
        min.x = convert_string_to<unsigned int>(minxstr);
        min.y = convert_string_to<unsigned int>(minystr);
        max.x = convert_string_to<unsigned int>(maxxstr);
        max.y = convert_string_to<unsigned int>(maxystr);
    }
    else
    {
        auto places = ds_.all_places();
        if (!places.empty())
        {
            // Find out bounding box
            min = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
            max = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
            for (auto  place : places)
            {
                auto [x,y] = ds_.get_place_coord(place);
                if (x < min.x) { min.x = x; }
                if (y < min.y) { min.y = y; }
                if (x > max.x) { max.x = x; }
                if (y > max.y) { max.y = y; }
            }
        }
    }

    if (min == max)
    {
        min = def_min;
        max = def_max;
    }

    add_random_places_areas(size, min, max);

    output << "Added: " << size << " places." << endl;

    view_dirty = true;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_random_ways(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string sizestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    unsigned int size = convert_string_to<unsigned int>(sizestr);

    add_random_ways(size);

    output << "Added: " << size << " ways." << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_random_add()
{
    add_random_places_areas(1);
}

void MainProgram::test_random_ways()
{
    add_random_ways(1);
}

MainProgram::CmdResult MainProgram::cmd_randseed(std::ostream& output, MatchIter begin, MatchIter end)
{
    string seedstr = *begin++;
    assert(begin == end && "Invalid number of parameters");

    unsigned long int seed = convert_string_to<unsigned long int>(seedstr);

    rand_engine_.seed(seed);
    init_primes();

    output << "Random seed set to " << seed << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_read(std::ostream& output, MatchIter begin, MatchIter end)
{
    string filename = *begin++;
    string silentstr =  *begin++;
    assert( begin == end && "Impossible number of parameters!");

    bool silent = !silentstr.empty();
    ostream* new_output = &output;

    ostringstream dummystr; // Given as output if "silent" is specified, the output is discarded
    if (silent)
    {
        new_output = &dummystr;
    }

    ifstream input(filename);
    if (input)
    {
        output << "** Commands from '" << filename << "'" << endl;
        command_parser(input, *new_output, PromptStyle::NORMAL);
        if (silent) { output << "...(output discarded in silent mode)..." << endl; }
        output << "** End of commands from '" << filename << "'" << endl;
    }
    else
    {
        output << "Cannot open file '" << filename << "'!" << endl;
    }

    return {};
}


MainProgram::CmdResult MainProgram::cmd_testread(std::ostream& output, MatchIter begin, MatchIter end)
{
    string infilename = *begin++;
    string outfilename = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    ifstream input(infilename);
    if (input)
    {
        ifstream expected_output(outfilename);
        if (output)
        {
            stringstream actual_output;
            command_parser(input, actual_output, PromptStyle::NO_NESTING);

            vector<string> actual_lines;
            while (actual_output)
            {
                string line;
                getline(actual_output, line);
                if (!actual_output) { break; }
                actual_lines.push_back(line);
            }

            vector<string> expected_lines;
            while (expected_output)
            {
                string line;
                getline(expected_output, line);
                if (!expected_output) { break; }
                expected_lines.push_back(line);
            }

            string heading_actual = "Actual output";
            unsigned int actual_max_length = heading_actual.length();
            auto actual_max_iter = max_element(actual_lines.cbegin(), actual_lines.cend(),
                                               [](string s1, string s2){ return s1.length() < s2.length(); });
            if (actual_max_iter != actual_lines.cend())
            {
                actual_max_length = actual_max_iter->length();
            }

            string heading_expected = "Expected output";
            unsigned int expected_max_length = heading_expected.length();
            auto expected_max_iter = std::max_element(expected_lines.cbegin(), expected_lines.cend(),
                                                    [](string s1, string s2){ return s1.length() < s2.length(); });
            if (expected_max_iter != expected_lines.cend())
            {
                expected_max_length = expected_max_iter->length();
            }

            auto pos_actual = actual_lines.cbegin();
            auto pos_expected = expected_lines.cbegin();
            output << "  " << heading_actual << string(actual_max_length - heading_actual.length(), ' ') << " | " << heading_expected << endl;
            output << "--" << string(actual_max_length, '-') << "-|-" << string(expected_max_length, '-') << endl;

            bool lines_ok = true;
            while (pos_expected != expected_lines.cend() || pos_actual != actual_lines.cend())
            {
                if (pos_expected != expected_lines.cend())
                {
                    if (pos_actual != actual_lines.cend())
                    {
                        bool ok = (*pos_expected == *pos_actual);
                        output << (ok ? ' ' : '?') << ' ' << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = lines_ok && ok;
                        ++pos_actual;
                    }
                    else
                    { // Actual output was too short
                        output << "? " << string(actual_max_length, ' ')
                               << " | " << *pos_expected << endl;
                        lines_ok = false;
                    }
                    ++pos_expected;
                }
                else
                { // Actual output was too long
                    output << "? " << *pos_actual << string(actual_max_length - pos_actual->length(), ' ')
                           << " | " << endl;
                    lines_ok = false;
                    ++pos_actual;
                }
            }
            if (lines_ok)
            {
                output << "**No differences in output.**" << endl;
                if (test_status_ == TestStatus::NOT_RUN)
                {
                    test_status_ = TestStatus::NO_DIFFS;
                }
            }
            else
            {
                output << "**Differences found! (Lines beginning with '?')**" << endl;
                test_status_ = TestStatus::DIFFS_FOUND;
            }

        }
        else
        {
            output << "Cannot open file '" << outfilename << "'!" << endl;
        }
    }
    else
    {
        output << "Cannot open file '" << infilename << "'!" << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_place_count(std::ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    output << "Number of places: " << ds_.place_count() << endl;

    return {};
}

MainProgram::CmdResult MainProgram::cmd_all_places(std::ostream& output, MatchIter begin, MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto places = ds_.all_places();
    if (places.empty())
    {
        output << "No places!" << endl;
    }

    sort(places.begin(), places.end());
    return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, places}};
}

MainProgram::CmdResult MainProgram::cmd_all_areas(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto areas = ds_.all_areas();
    if (areas.empty())
    {
        output << "No areas!" << endl;
    }

    sort(areas.begin(), areas.end());
    return {ResultType::AREAIDLIST, areas};
}

void MainProgram::add_random_ways(unsigned int n)
{
    for (unsigned int i=0; i<n; ++i)
    {
        ++random_ways_added_;

        WayID id = n_to_wayid(random_ways_added_);
        Coord c1 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));
        Coord c2 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));
        if (c1.x != c2.x || c1.y != c2.y)
        {
            ds_.add_way(id, {c1,c2});
        }
    }
}

MainProgram::CmdResult MainProgram::cmd_stopwatch(std::ostream& output, MatchIter begin, MatchIter end)
{
    string on = *begin++;
    string off = *begin++;
    string next = *begin++;
    assert(begin == end && "Invalid number of parameters");

    if (!on.empty())
    {
        stopwatch_mode = StopwatchMode::ON;
        output << "Stopwatch: on" << endl;
    }
    else if (!off.empty())
    {
        stopwatch_mode = StopwatchMode::OFF;
        output << "Stopwatch: off" << endl;
    }
    else if (!next.empty())
    {
        stopwatch_mode = StopwatchMode::NEXT;
        output << "Stopwatch: on for the next command" << endl;
    }
    else
    {
        assert(!"Impossible stopwatch mode!");
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_clear_all(std::ostream& output, MatchIter begin, MatchIter end)
{
    assert(begin == end && "Invalid number of parameters");

    ds_.clear_all();
    init_primes();

    output << "Cleared everything." << endl;

    view_dirty = true;

    return {};
}

std::string MainProgram::print_place(PlaceID id, ostream& output, bool nl)
{
    if (id != NO_PLACE)
    {
        auto [name,type] = ds_.get_place_name_type(id);
        auto xy = ds_.get_place_coord(id);
        if (!name.empty())
        {
            output << name << " (" << convert_placetype_to_string(type) << ")";
        }
        else
        {
            output << "*";
        }

         output << ": pos=";
         print_coord(xy, output, false);
         output << ", id=" << id;
         if (nl) { output << endl; }

        ostringstream retstream;
        retstream << id;
        return retstream.str();
    }
    else
    {
        output << "--NO_PLACE--";
        if (nl) { output << endl; }
        return "";
    }
}

std::string MainProgram::print_place_name(PlaceID id, std::ostream &output, bool nl)
{
    if (id != NO_PLACE)
    {
        auto [name,type] = ds_.get_place_name_type(id);
        if (!name.empty())
        {
            output << name;
        }
        else
        {
            output << "*";
        }

        ostringstream retstream;
        retstream << name;
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "--NO_PLACE--";
        if (nl) { output << endl; }
        return "";
    }
}

std::string MainProgram::print_area(AreaID id, std::ostream& output, bool nl)
{
    if (id != NO_AREA)
    {
        auto name = ds_.get_area_name(id);
        if (!name.empty())
        {
            output << name << ": id=" << id;
        }
        else
        {
            output << "*" << ": id=" << id;
        }
        ostringstream retstream;
        retstream << id;
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "--NO_AREA--";
        if (nl) { output << endl; }
        return "";
    }
}

std::string MainProgram::print_way(WayID id, std::ostream &output, bool nl)
{
    if (id != NO_WAY)
    {
        output << "Way id " << id;
        ostringstream retstream;
        retstream << id;
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "--NO_WAY--";
        if (nl) { output << endl; }
        return "";
    }
}

std::string MainProgram::print_coord(Coord coord, std::ostream& output, bool nl)
{
    if (coord != NO_COORD)
    {
        output << "(" << coord.x << "," << coord.y << ")";
        ostringstream retstream;
        retstream << "(" << coord.x << "," << coord.y << ")";
        if (nl) { output << endl; }
        return retstream.str();
    }
    else
    {
        output << "(--NO_COORD--)";
        if (nl) { output << endl; }
        return "";
    }
}

MainProgram::CmdResult MainProgram::cmd_find_places_name(std::ostream& output, MatchIter begin, MatchIter end)
{
    string name = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.find_places_name(name);
    if (result.empty())
    {
        output << "No Places!" << std::endl;
    }

    sort(result.begin(), result.end());
    return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, result}};
}

void MainProgram::test_find_places_name()
{
    // Choose random number to remove
    if (random_places_added_ > 0) // Don't find if there's nothing to find
    {
        auto name = n_to_name(random<decltype(random_places_added_)>(0, random_places_added_));
        ds_.find_places_name(name);
    }
}

MainProgram::CmdResult MainProgram::cmd_find_places_type(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string typestr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    PlaceType type = convert_string_to_placetype(typestr);
    if (type == PlaceType::NO_TYPE)
    {
        output << "Impossible place type: " << typestr << endl;
        return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, {NO_PLACE}}};
    }

    auto result = ds_.find_places_type(type);
    if (result.empty())
    {
        output << "No Places!" << std::endl;
    }

    sort(result.begin(), result.end());
    return {ResultType::PLACEIDLIST, CmdResultPlaceIDs{NO_AREA, result}};
}

void MainProgram::test_find_places_type()
{
    // Choose random number to remove
    if (random_places_added_ > 0) // Don't find if there's nothing to find
    {
        PlaceType type{random<int>(0, static_cast<int>(PlaceType::NO_TYPE))};
        ds_.find_places_type(type);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_any(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    string toxstr = *begin++;
    string toystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord fromxy = {convert_string_to<int>(fromxstr),convert_string_to<int>(fromystr)};
    Coord toxy = {convert_string_to<int>(toxstr),convert_string_to<int>(toystr)};

    auto steps = ds_.route_any(fromxy, toxy);

    vector<tuple<Coord, Coord, WayID, Distance>> result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_tuple(NO_COORD, NO_WAY, NO_DISTANCE))
    {
        output << "Starting or destination coord has no ways!" << endl;
    }
    else
    {
        auto [coord, wayid, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [ncoord, nwayid, ndist] = *iter;
            result.emplace_back(coord, ncoord, wayid, dist);
            coord = ncoord; wayid = nwayid; dist = ndist;
        }
        result.emplace_back(coord, NO_COORD, NO_WAY, dist);
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_any()
{
    // Choose two random places
    Coord coord1 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));
    Coord coord2 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));

    ds_.route_any(coord1, coord2);
}

MainProgram::CmdResult MainProgram::cmd_remove_way(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string idstr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    WayID id = idstr;

    bool ok = ds_.remove_way(id);
    if (ok)
    {
        output << "Removed way " << id << endl;
    }
    else
    {
        output << "Removing way failed!" << endl;
    }

    return {};
}

void MainProgram::test_remove_way()
{
    if (random_ways_added_ > 0)
    {
        WayID id = n_to_wayid(random<decltype(random_ways_added_)>(0, random_ways_added_));
        ds_.remove_way(id);
    }
}

MainProgram::CmdResult MainProgram::cmd_route_shortest_distance(std::ostream& output, MatchIter begin, MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    string toxstr = *begin++;
    string toystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord fromxy = {convert_string_to<int>(fromxstr),convert_string_to<int>(fromystr)};
    Coord toxy = {convert_string_to<int>(toxstr),convert_string_to<int>(toystr)};

    auto steps = ds_.route_shortest_distance(fromxy, toxy);

    vector<tuple<Coord, Coord, WayID, Distance>> result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_tuple(NO_COORD, NO_WAY, NO_DISTANCE))
    {
        output << "Starting or destination coord has no ways!" << endl;
    }
    else
    {
        auto [coord, wayid, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [ncoord, nwayid, ndist] = *iter;
            result.emplace_back(coord, ncoord, wayid, dist);
            coord = ncoord; wayid = nwayid; dist = ndist;
        }
        result.emplace_back(coord, NO_COORD, NO_WAY, dist);
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_shortest_distance()
{
    // Choose two random places
    Coord coord1 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));
    Coord coord2 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));

    ds_.route_shortest_distance(coord1, coord2);
}

MainProgram::CmdResult MainProgram::cmd_route_least_crossroads(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    string toxstr = *begin++;
    string toystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord fromxy = {convert_string_to<int>(fromxstr),convert_string_to<int>(fromystr)};
    Coord toxy = {convert_string_to<int>(toxstr),convert_string_to<int>(toystr)};

    auto steps = ds_.route_least_crossroads(fromxy, toxy);

    vector<tuple<Coord, Coord, WayID, Distance>> result;

    if (steps.empty())
    {
        output << "No route found!" << endl;
    }
    else if (steps.front() == make_tuple(NO_COORD, NO_WAY, NO_DISTANCE))
    {
        output << "Starting or destination coord has no ways!" << endl;
    }
    else
    {
        auto [coord, wayid, dist] = steps.front();
        for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
        {
            auto& [ncoord, nwayid, ndist] = *iter;
            result.emplace_back(coord, ncoord, wayid, dist);
            coord = ncoord; wayid = nwayid; dist = ndist;
        }
        result.emplace_back(coord, NO_COORD, NO_WAY, dist);
    }

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_least_crossroads()
{
    // Choose two random places
    Coord coord1 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));
    Coord coord2 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));

    ds_.route_least_crossroads(coord1, coord2);
}

MainProgram::CmdResult MainProgram::cmd_route_with_cycle(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    string fromxstr = *begin++;
    string fromystr = *begin++;
    assert( begin == end && "Impossible number of parameters!");

    Coord fromxy = {convert_string_to<int>(fromxstr),convert_string_to<int>(fromystr)};

    auto steps = ds_.route_with_cycle(fromxy);

    if (steps.empty())
    {
        output << "No route found!" << endl;
        return {};
    }

    if (steps.front() == make_tuple(NO_COORD, NO_WAY/*, NO_DISTANCE*/))
    {
        output << "Starting coord has no ways!" << endl;
        return {};
    }

    if (steps.size() < 2)
    {
        output << "Too short route (" << steps.size() << ") to contain cycles!" << endl;
        return {};
    }

    auto lastcoord = std::get<0>(steps.back());
    auto cycbeg = std::find_if(steps.begin(), steps.end()-1, [lastcoord](auto const& e){ return std::get<0>(e) == lastcoord; });
    if (cycbeg == steps.end())
    {
        output << "No cycle found in returned route!";
        return {};
    }

    // If necessary, swap cycle so that it starts with smaller wayid
    if ((cycbeg+1) < (steps.end()-2))
    {
        auto wayfirst = std::get<1>(*cycbeg);
        auto waylast = std::get<1>(*(steps.end()-2));
        if (waylast < wayfirst)
        {
           std::reverse(cycbeg+1, steps.end()-1);
           // Rotate the wayids to fix the reversed order
           auto firstid = std::get<1>(*cycbeg);
           for (auto i = cycbeg; i != steps.end()-2; ++i)
           {
               std::get<1>(*i) = std::get<1>(*(i+1));
           }
           std::get<1>(*(steps.end()-2)) = firstid;
        }
    }

    vector<tuple<Coord, Coord, WayID, Distance>> result;
    // Don't put returned way ids or distances in the result
    auto [coord, wayid/*, dist*/] = steps.front();
    for (auto iter = steps.begin()+1; iter != steps.end(); ++iter)
    {
        auto& [ncoord, nwayid/*, ndist*/] = *iter;
        result.emplace_back(coord, ncoord, wayid, NO_DISTANCE);
        coord = ncoord; wayid = nwayid;/* dist = ndist;*/
    }
    result.emplace_back(coord, NO_COORD, NO_WAY, NO_DISTANCE);

    return {ResultType::ROUTE, result};
}

void MainProgram::test_route_with_cycle()
{
    // Choose two random places
    Coord coord1 = n_to_coord(random(decltype(random_ways_added_)(0),random_ways_added_));

    ds_.route_with_cycle(coord1);
}

MainProgram::CmdResult MainProgram::cmd_trim_ways(std::ostream &output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    auto result = ds_.trim_ways();

    output << "The remaining ways have a total length of " << result << endl;

    view_dirty = true;

    return {};
}

void MainProgram::test_trim_ways()
{
    ds_.trim_ways();
}

MainProgram::CmdResult MainProgram::cmd_clear_ways(std::ostream& output, MainProgram::MatchIter begin, MainProgram::MatchIter end)
{
    assert( begin == end && "Impossible number of parameters!");

    ds_.clear_ways();
    output << "All routes removed." << std::endl;

    return {};
}

string const plcidx = "([0-9]+)";
string const areaidx = "([0-9]+)";
string const wayidx = "([a-zA-Z0-9]+)";
string const namex = "'([a-zA-Z0-9 -]+)'";
string const typex = "([a-zA-Z0-9]+)";
string const numx = "([0-9]+)";
string const optcoordx = "\\([[:space:]]*[0-9]+[[:space:]]*,[[:space:]]*[0-9]+[[:space:]]*\\)";
string const coordx = "\\([[:space:]]*([0-9]+)[[:space:]]*,[[:space:]]*([0-9]+)[[:space:]]*\\)";
string const wsx = "[[:space:]]+";

vector<MainProgram::CmdInfo> MainProgram::cmds_ =
{
    {"add_place", "ID 'Name' Type (x,y)", plcidx+wsx+namex+wsx+typex+wsx+coordx, &MainProgram::cmd_add_place, nullptr },
    {"random_add", "number_of_places_to_add  [(minx,miny) (maxx,maxy)] (coordinates optional)", numx+"(?:"+wsx+coordx+wsx+coordx+")?",
     &MainProgram::cmd_random_add, &MainProgram::test_random_add },
    {"all_places", "", "", &MainProgram::cmd_all_places, nullptr },
    {"place_name_type", "ID", plcidx, &MainProgram::cmd_place_name_type, &MainProgram::test_place_name_type },
    {"place_coord", "ID", plcidx, &MainProgram::cmd_place_coord, &MainProgram::test_place_coord },
    {"add_area", "ID Name (x,y) (x,y)...", areaidx+wsx+namex+"((?:"+wsx+optcoordx+")+)", &MainProgram::cmd_add_area, nullptr },
    {"all_areas", "", "", &MainProgram::cmd_all_areas, nullptr },
    {"area_name", "AreaID", areaidx, &MainProgram::cmd_area_name, &MainProgram::test_area_name },
    {"area_coords", "AreaID", areaidx, &MainProgram::cmd_area_coords, nullptr },
    {"creation_finished", "", "", &MainProgram::cmd_creation_finished, nullptr },
    {"place_count", "", "", &MainProgram::cmd_place_count, nullptr },
    {"clear_all", "", "", &MainProgram::cmd_clear_all, nullptr },
    {"places_alphabetically", "", "", &MainProgram::NoParPlaceListCmd<&Datastructures::places_alphabetically>, &MainProgram::NoParPlaceListTestCmd<&Datastructures::places_alphabetically> },
    {"places_coord_order", "", "", &MainProgram::NoParPlaceListCmd<&Datastructures::places_coord_order>, &MainProgram::NoParPlaceListTestCmd<&Datastructures::places_coord_order> },
    {"places_closest_to", "Coord [type] (type optional)", coordx+"(?:"+wsx+typex+")?", &MainProgram::cmd_places_closest_to, &MainProgram::test_places_closest_to },
    {"common_area_of_subareas", "ID1 ID2", plcidx+wsx+plcidx, &MainProgram::cmd_common_area_of_subareas, &MainProgram::test_common_area_of_subareas },
    {"remove_place", "ID", plcidx, &MainProgram::cmd_remove_place, &MainProgram::test_remove_place },
    {"find_places_name", "'Name'", namex, &MainProgram::cmd_find_places_name, &MainProgram::test_find_places_name },
    {"find_places_type", "type", typex, &MainProgram::cmd_find_places_type, &MainProgram::test_find_places_type },
    {"change_place_name", "ID 'Newname'", plcidx+wsx+namex, &MainProgram::cmd_change_place_name, &MainProgram::test_change_place_name },
    {"change_place_coord", "ID (x,y)", plcidx+wsx+coordx, &MainProgram::cmd_change_place_coord, &MainProgram::test_change_place_coord },
    {"add_subarea_to_area", "SubareaID AreaID", areaidx+wsx+areaidx, &MainProgram::cmd_add_subarea_to_area, nullptr },
    {"all_ways", "", "", &MainProgram::cmd_all_ways, nullptr },
    {"add_way", "WayID (x,y) (x,y)...", wayidx+"((?:"+wsx+optcoordx+")+)", &MainProgram::cmd_add_way, nullptr },
    {"random_ways", "number_of_ways_to_add", numx,
     &MainProgram::cmd_random_ways, &MainProgram::test_random_ways },
    {"way_coords", "WayID", wayidx, &MainProgram::cmd_way_coords, &MainProgram::test_way_coords },
    {"ways_from", "Coord", coordx, &MainProgram::cmd_ways_from, &MainProgram::test_ways_from },
    {"clear_ways", "", "", &MainProgram::cmd_clear_ways, nullptr },
    {"remove_way", "WayID", wayidx, &MainProgram::cmd_remove_way, &MainProgram::test_remove_way },
    {"subarea_in_areas", "AreaID", areaidx, &MainProgram::cmd_subarea_in_areas, &MainProgram::test_subarea_in_areas },
    {"all_subareas_in_area", "AreaID", areaidx, &MainProgram::cmd_all_subareas_in_area, &MainProgram::test_all_subareas_in_area },
    {"route_any", "CoordFrom CoordTo", coordx+wsx+coordx, &MainProgram::cmd_route_any, &MainProgram::test_route_any },
    {"route_least_crossroads", "CoordFrom CoordTo", coordx+wsx+coordx, &MainProgram::cmd_route_least_crossroads, &MainProgram::test_route_least_crossroads },
    {"route_shortest_distance", "CoordFrom CoordTo", coordx+wsx+coordx, &MainProgram::cmd_route_shortest_distance, &MainProgram::test_route_shortest_distance },
    {"route_with_cycle", "Coordfrom", coordx, &MainProgram::cmd_route_with_cycle, &MainProgram::test_route_with_cycle },
    {"trim_ways", "", "", &MainProgram::cmd_trim_ways, &MainProgram::test_trim_ways },
    {"quit", "", "", nullptr, nullptr },
    {"help", "", "", &MainProgram::help_command, nullptr },
    {"read", "\"in-filename\" [silent]", "\"([-a-zA-Z0-9 ./:_]+)\"(?:"+wsx+"(silent))?", &MainProgram::cmd_read, nullptr },
    {"testread", "\"in-filename\" \"out-filename\"", "\"([-a-zA-Z0-9 ./:_]+)\""+wsx+"\"([-a-zA-Z0-9 ./:_]+)\"", &MainProgram::cmd_testread, nullptr },
    {"perftest", "cmd1|all|compulsory[;cmd2...] timeout repeat_count n1[;n2...] (parts in [] are optional, alternatives separated by |)",
     "([0-9a-zA-Z_]+(?:;[0-9a-zA-Z_]+)*)"+wsx+numx+wsx+numx+wsx+"([0-9]+(?:;[0-9]+)*)", &MainProgram::cmd_perftest, nullptr },
    {"stopwatch", "on|off|next (alternatives separated by |)", "(?:(on)|(off)|(next))", &MainProgram::cmd_stopwatch, nullptr },
    {"random_seed", "new-random-seed-integer", numx, &MainProgram::cmd_randseed, nullptr },
    {"#", "comment text", ".*", &MainProgram::cmd_comment, nullptr },
};

MainProgram::CmdResult MainProgram::help_command(std::ostream& output, MatchIter /*begin*/, MatchIter /*end*/)
{
    output << "Commands:" << endl;
    for (auto& i : cmds_)
    {
        output << "  " << i.cmd << " " << i.info << endl;
    }

    return {};
}

MainProgram::CmdResult MainProgram::cmd_perftest(std::ostream& output, MatchIter begin, MatchIter end)
{
#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    vector<string> optional_cmds({"places_closest_to", "places_common_area", "route_least_crossroads", "route_with_cycle", "route_shortest_distance",
                                  "add_walking_connections"});
    vector<string> nondefault_cmds({"remove_place", "find_places", "way_coords"});

    string commandstr = *begin++;
    unsigned int timeout = convert_string_to<unsigned int>(*begin++);
    unsigned int repeat_count = convert_string_to<unsigned int>(*begin++);
//    unsigned int friend_count = convert_string_to<unsigned int>(*begin++);
    string sizes = *begin++;
    assert(begin == end && "Invalid number of parameters");

    vector<string> testcmds;
    bool additional_get_cmds = true;
    if (commandstr != "all" && commandstr != "compulsory")
    {
        additional_get_cmds = false;
        smatch scmd;
        auto cbeg = commandstr.cbegin();
        auto cend = commandstr.cend();
        for ( ; regex_search(cbeg, cend, scmd, commands_regex_); cbeg = scmd.suffix().first)
        {
            testcmds.push_back(scmd[1]);
        }
    }

    vector<unsigned int> init_ns;
    smatch size;
    auto sbeg = sizes.cbegin();
    auto send = sizes.cend();
    for ( ; regex_search(sbeg, send, size, sizes_regex_); sbeg = size.suffix().first)
    {
        init_ns.push_back(convert_string_to<unsigned int>(size[1]));
    }

    output << "Timeout for each N is " << timeout << " sec. " << endl;
//    output << "Add 0.." << friend_count << " friends for every employee." << endl;
    output << "For each N perform " << repeat_count << " random command(s) from:" << endl;

    // Initialize test functions
    vector<void(MainProgram::*)()> testfuncs;
    if (testcmds.empty())
    { // Add all commands
        for (auto& i : cmds_)
        {
            if (i.testfunc)
            {
                if (find(nondefault_cmds.begin(), nondefault_cmds.end(), i.cmd) == nondefault_cmds.end() &&
                    (commandstr == "all" || find(optional_cmds.begin(), optional_cmds.end(), i.cmd) == optional_cmds.end()))
                {
                    output << i.cmd << " ";
                    testfuncs.push_back(i.testfunc);
                }
            }
        }
    }
    else
    {
        for (auto& i : testcmds)
        {
            auto pos = find_if(cmds_.begin(), cmds_.end(), [&i](auto const& cmd){ return cmd.cmd == i; });
            if (pos != cmds_.end() && pos->testfunc)
            {
                output << i << " ";
                testfuncs.push_back(pos->testfunc);
            }
            else
            {
                output << "(cannot test " << i << ") ";
            }
        }
    }
    output << endl << endl;

    if (testfuncs.empty())
    {
        output << "No commands to test!" << endl;
        return {};
    }

#ifdef USE_PERF_EVENT
    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "add (count)" << " , " << setw(12) << "cmds (sec)" << " , "
           << setw(12) << "cmds (count)"  << " , " << setw(12) << "total (sec)" << " , " << setw(12) << "total (count)" << endl;
#else
    output << setw(7) << "N" << " , " << setw(12) << "add (sec)" << " , " << setw(12) << "cmds (sec)" << " , "
           << setw(12) << "total (sec)" << endl;
#endif
    flush_output(output);

    auto stop = false;
    for (unsigned int n : init_ns)
    {
        if (stop) { break; }

        output << setw(7) << n << " , " << flush;

        ds_.clear_all();
        ds_.clear_ways();
        init_primes();

        Stopwatch stopwatch(true); // Use also instruction counting, if enabled

        // Add random places
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            stopwatch.start();
            add_random_places_areas(1000);
            stopwatch.stop();

            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
        }
        if (stop) { break; }

        if (n % 1000 != 0)
        {
            stopwatch.start();
            add_random_places_areas(n % 1000);
            stopwatch.stop();
        }

        // Add random ways
        for (unsigned int i = 0; i < n / 1000; ++i)
        {
            stopwatch.start();
            add_random_ways(1000);
            stopwatch.stop();

            if (stopwatch.elapsed() >= timeout)
            {
                output << "Timeout!" << endl;
                stop = true;
                break;
            }
            if (check_stop())
            {
                output << "Stopped!" << endl;
                stop = true;
                break;
            }
        }
        if (stop) { break; }

        if (n % 1000 != 0)
        {
            stopwatch.start();
            add_random_ways(n % 1000);
            stopwatch.stop();
        }

#ifdef USE_PERF_EVENT
        auto addcount = stopwatch.count();
#endif
        auto addsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
        output << setw(12) << addsec << " , " << setw(12) << addcount << " , " << flush;
#else
        output << setw(12) << addsec << " , " << flush;
#endif

        if (addsec >= timeout)
        {
            output << "Timeout!" << endl;
            stop = true;
            break;
        }

        stopwatch.start();
        ds_.creation_finished();
        for (unsigned int repeat = 0; repeat < repeat_count; ++repeat)
        {
            auto cmdpos = random(testfuncs.begin(), testfuncs.end());

            (this->**cmdpos)();
            if (additional_get_cmds)
            {
                if (random_places_added_ > 0) // Don't do anything if there's no places
                {
                    PlaceID id = random<decltype(random_places_added_)>(0, random_places_added_);
                    ds_.get_place_name_type(id);
                    ds_.get_place_coord(id);
                }
                if (random_areas_added_ > 0)
                {
                    auto areaid = n_to_areaid(random<decltype(random_areas_added_)>(0, random_areas_added_));
                    ds_.get_area_name(areaid);
                }
            }

            if (repeat % 10 == 0)
            {
                stopwatch.stop();
                if (stopwatch.elapsed() >= timeout)
                {
                    output << "Timeout!" << endl;
                    stop = true;
                    break;
                }
                if (check_stop())
                {
                    output << "Stopped!" << endl;
                    stop = true;
                    break;
                }
                stopwatch.start();
            }
        }
        stopwatch.stop();
        if (stop) { break; }

#ifdef USE_PERF_EVENT
        auto totalcount = stopwatch.count();
#endif
        auto totalsec = stopwatch.elapsed();

#ifdef USE_PERF_EVENT
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalcount-addcount << " , " << setw(12) << totalsec << " , " << setw(12) << totalcount;
#else
        output << setw(12) << totalsec-addsec << " , " << setw(12) << totalsec;
#endif

//        unsigned long int maxmem;
//        string unit;
//        tie(maxmem, unit) = mempeak();
//        maxmem -=  startmem;
//        if (maxmem != 0)
//        {
//            output << ", memory " << maxmem << " " << unit;
//        }
        output << endl;
        flush_output(output);
    }

    ds_.clear_all();
    ds_.clear_ways();
    init_primes();

#ifdef _GLIBCXX_DEBUG
    output << "WARNING: Debug STL enabled, performance will be worse than expected (maybe also asymptotically)!" << endl;
#endif // _GLIBCXX_DEBUG

    return {};
}

MainProgram::CmdResult MainProgram::cmd_comment(std::ostream& /*output*/, MatchIter /*begin*/, MatchIter /*end*/)
{
    return {};
}

bool MainProgram::command_parse_line(string inputline, ostream& output)
{
//    static unsigned int nesting_level = 0; // UGLY! Remember nesting level to print correct amount of >:s.
//    if (promptstyle != PromptStyle::NO_NESTING) { ++nesting_level; }

    if (inputline.empty()) { return true; }

    smatch match;
    bool matched = regex_match(inputline, match, cmds_regex_);
    if (matched)
    {
        assert(match.size() == 3);
        string cmd = match[1];
        string params = match[2];

        auto pos = find_if(cmds_.begin(), cmds_.end(), [cmd](CmdInfo const& ci) { return ci.cmd == cmd; });
        assert(pos != cmds_.end());

        smatch match2;
        bool matched2 = regex_match(params, match2, pos->param_regex);
        if (matched2)
        {
            if (pos->func)
            {
                assert(!match2.empty());

                Stopwatch stopwatch;
                bool use_stopwatch = (stopwatch_mode != StopwatchMode::OFF);
                // Reset stopwatch mode if only for the next command
                if (stopwatch_mode == StopwatchMode::NEXT) { stopwatch_mode = StopwatchMode::OFF; }

               TestStatus initial_status = test_status_;
               test_status_ = TestStatus::NOT_RUN;

                if (use_stopwatch)
                {
                    stopwatch.start();
                }

                CmdResult result;
                try
                {
                    result = (this->*(pos->func))(output, ++(match2.begin()), match2.end());
                }
                catch (std::exception const& e)
                {
                    output << "Error: " << e.what() << endl;
                }

                if (use_stopwatch)
                {
                    stopwatch.stop();
                }

                switch (result.first)
                {
                    case ResultType::NOTHING:
                    {
                        break;
                    }
                    case ResultType::PLACEIDLIST:
                    {
                        auto& [area, places] = std::get<CmdResultPlaceIDs>(result.second);
                        if (area != NO_AREA)
                        {
                            output << "Area: ";
                            print_area(area, output);
                        }
                        if (!places.empty())
                        {
                            if (places.size() == 1 && places.front() == NO_PLACE)
                            {
                                output << "Failed (NO_... returned)!!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (PlaceID id : places)
                                {
                                    ++num;
                                    if (places.size() > 1) { output << num << ". "; }
                                    print_place(id, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::AREAIDLIST:
                    {
                        auto& areas = std::get<CmdResultAreaIDs>(result.second);
                        if (!areas.empty())
                        {
                            if (areas.size() == 1 && areas.front() == NO_AREA)
                            {
                                output << "Failed (NO_... returned)!!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 0;
                                for (auto area : areas)
                                {
                                    ++num;
                                    if (areas.size() > 1) { output << num << ". "; }
                                    print_area(area, output);
                                }
                            }
                        }
                        break;
                    }
                    case ResultType::ROUTE:
                    {
                        auto& route = std::get<CmdResultRoute>(result.second);
                        if (!route.empty())
                        {
                            if (route.size() == 1 && get<0>(route.front()) == NO_COORD)
                            {
                                output << "Failed (NO_... returned)!!" << std::endl;
                            }
                            else
                            {
                                unsigned int num = 1;
                                for (auto& [coord, nextcoord, wayid, distance] : route)
                                {
                                    output << num << ". ";
                                    ++num;
                                    print_coord(coord, output, false);
                                    if (wayid != NO_WAY) { output << " way " << wayid; }
                                    if (distance != NO_DISTANCE) { output << " distance " << distance; }
                                    output << endl;
                                }
                            }
                        }
                        break;
                    }
                case ResultType::WAYS:
                {
                    auto& ways = std::get<CmdResultRoute>(result.second);
                    if (!ways.empty())
                    {
                        if (ways.size() == 1 && get<0>(ways.front()) == NO_COORD)
                        {
                            output << "Failed (NO_... returned)!!" << std::endl;
                        }
                        else
                        {
                            unsigned int num = 1;
                            for (auto& [fromcoord, tocoord, wayid, distance] : ways)
                            {
                                output << num << ". ";
                                ++num;
                                print_coord(tocoord, output, false);
                                if (wayid != NO_WAY) { output << " way " << wayid << " "; }
                                if (distance != NO_DISTANCE) { output << "distance " << distance; }
                                output << endl;
                            }
                        }
                    }
                    break;
                }
                    default:
                    {
                        assert(false && "Unsupported result type!");
                    }
                }

                if (result != prev_result)
                {
                    prev_result = move(result);
                    view_dirty = true;
                }

                if (use_stopwatch)
                {
                    output << "Command '" << cmd << "': " << stopwatch.elapsed() << " sec" << endl;
                }

                if (test_status_ != TestStatus::NOT_RUN)
                {
                    output << "Testread-tests have been run, " << ((test_status_ == TestStatus::DIFFS_FOUND) ? "differences found!" : "no differences found.") << endl;
                }
                if (test_status_ == TestStatus::NOT_RUN || (test_status_ == TestStatus::NO_DIFFS && initial_status == TestStatus::DIFFS_FOUND))
                {
                    test_status_ = initial_status;
                }
            }
            else
            { // No function to run = quit command
                return false;
            }
        }
        else
        {
            output << "Invalid parameters for command '" << cmd << "'!" << endl;
        }
    }
    else
    {
        output << "Unknown command!" << endl;
    }

    return true; // Signal continuing
}

void MainProgram::command_parser(istream& input, ostream& output, PromptStyle promptstyle)
{
    string line;
    do
    {
//        output << string(nesting_level, '>') << " ";
        output << PROMPT;
        getline(input, line, '\n');

        if (promptstyle != PromptStyle::NO_ECHO)
        {
            output << line << endl;
        }

        if (!input) { break; }

        bool cont = command_parse_line(line, output);
        view_dirty = false; // No need to keep track of individual result changes
        if (!cont) { break; }
    }
    while (input);
    //    if (promptstyle != PromptStyle::NO_NESTING) { --nesting_level; }

    view_dirty = true; // To be safe, assume that results have been changed
}

void MainProgram::setui(MainWindow* ui)
{
    ui_ = ui;
}

#ifdef GRAPHICAL_GUI
void MainProgram::flush_output(std::ostream& output)
{
    if (ui_)
    {
        if (auto soutput = dynamic_cast<ostringstream*>(&output))
        {
            ui_->output_text(*soutput);
        }
    }
}
#else
void MainProgram::flush_output(std::ostream& /*output*/)
{
}
#endif

bool MainProgram::check_stop() const
{
#ifdef GRAPHICAL_GUI
    if (ui_)
    {
        return ui_->check_stop_pressed();
    }
#endif
    return false;
}

std::array<unsigned long int, 20> const MainProgram::primes1{4943,   4951,   4957,   4967,   4969,   4973,   4987,   4993,   4999,   5003,
                                                             5009,   5011,   5021,   5023,   5039,   5051,   5059,   5077,   5081,   5087};
std::array<unsigned long int, 20> const MainProgram::primes2{81031,  81041,  81043,  81047,  81049,  81071,  81077,  81083,  81097,  81101,
                                                             81119,  81131,  81157,  81163,  81173,  81181,  81197,  81199,  81203,  81223};

MainProgram::MainProgram()
{
    rand_engine_.seed(time(nullptr));

    //    startmem = get<0>(mempeak());

    init_primes();
    init_regexs();
}

int MainProgram::mainprogram(int argc, char* argv[])
{
    vector<string> args(argv, argv+argc);

    if (args.size() > 2)
    {
        cerr << "Usage: " + ((args.size() > 0) ? args[0] : "<program name>") + " [<command file>]" << endl;
        return EXIT_FAILURE;
    }

    MainProgram mainprg;

    if (args.size() == 2 && args[1] != "--console")
    {
        string filename = args[1];
        ifstream input(filename);
        if (input)
        {
            mainprg.command_parser(input, cout, MainProgram::PromptStyle::NORMAL);
        }
        else
        {
            cout << "Cannot open file '" << filename << "'!" << endl;
        }
    }
    else
    {
        mainprg.command_parser(cin, cout, MainProgram::PromptStyle::NO_ECHO);
    }

    cerr << "Program ended normally." << endl;
    if (mainprg.test_status_ == TestStatus::DIFFS_FOUND)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

PlaceType MainProgram::convert_string_to_placetype(std::string from)
{
    std::istringstream istr(from);
    std::string typestr;
    istr >> std::noskipws >> typestr;
    if (istr.fail() || !istr.eof())
    {
        throw std::invalid_argument("Cannot convert string to place type");
    }

    PlaceType result = PlaceType::NO_TYPE;
    if (typestr == "firepit") { result = PlaceType::FIREPIT; }
    else if (typestr == "shelter") { result = PlaceType::SHELTER; }
    else if (typestr == "parking") { result = PlaceType::PARKING; }
    else if (typestr == "peak") { result = PlaceType::PEAK; }
    else if (typestr == "bay") { result = PlaceType::BAY; }
    else if (typestr == "area") { result = PlaceType::AREA; }
    else if (typestr == "other") { result = PlaceType::OTHER; }
    else
    {
        throw std::invalid_argument("Cannot convert string to place type");
    }

    return result;
}

std::string MainProgram::convert_placetype_to_string(PlaceType type)
{
    switch (type)
    {
    case PlaceType::FIREPIT:
        return "firepit";
    case PlaceType::SHELTER:
        return "shelter";
    case PlaceType::PARKING:
        return "parking";
    case PlaceType::PEAK:
        return "peak";
    case PlaceType::BAY:
        return "bay";
    case PlaceType::AREA:
        return "area";
    case PlaceType::OTHER:
        return "other";
    default:
        return "!!no type!!";
    }
}


void MainProgram::init_primes()
{
    // Initialize id generator
    prime1_ = primes1[random<int>(0, primes1.size())];
    prime2_ = primes2[random<int>(0, primes2.size())];
    random_places_added_ = 0;
    random_areas_added_ = 0;
    random_ways_added_ = 0;
}

Name MainProgram::n_to_name(unsigned long n)
{
    unsigned long int hash = prime1_*n + prime2_;
    string name;

    while (hash > 0)
    {
        auto hexnum = hash % 26;
        hash /= 26;
        name.push_back('a'+hexnum);
    }

    return name;
}

AreaID MainProgram::n_to_areaid(unsigned long n)
{
    return n_to_placeid(n);
}

WayID MainProgram::n_to_wayid(unsigned long n)
{
 std::ostringstream ostr;
 ostr << "R" << n;
 return ostr.str();
}

PlaceID MainProgram::n_to_placeid(unsigned long int n)
{
    unsigned long int hash = prime2_*n + prime1_;

    return hash % static_cast<unsigned long int>(std::numeric_limits<PlaceID>::max());
}

Coord MainProgram::n_to_coord(unsigned long n)
{
    unsigned long int hash = prime1_ * n + prime2_;
    hash = hash ^ (hash + 0x9e3779b9 + (hash << 6) + (hash >> 2)); // :-P

    return {static_cast<int>(hash % 1000), static_cast<int>((hash/1000) % 1000)};
}

void MainProgram::init_regexs()
{
    // Create regex <whitespace>(cmd1|cmd2|...)<whitespace>(.*)
    string cmds_regex_str = "[[:space:]]*(";
    bool first = true;
    for (auto& cmd : cmds_)
    {
        cmds_regex_str += (first ? "" : "|") + cmd.cmd;
        first = false;

        cmd.param_regex = regex(cmd.param_regex_str+"[[:space:]]*", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    }
    cmds_regex_str += ")(?:[[:space:]]*$|"+wsx+"(.*))";
    cmds_regex_ = regex(cmds_regex_str, std::regex_constants::ECMAScript | std::regex_constants::optimize);
    coords_regex_ = regex(coordx+"[[:space:]]?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    times_regex_ = regex(wsx+"([0-9][0-9]):([0-9][0-9]):([0-9][0-9])", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    commands_regex_ = regex("([0-9a-zA-Z_]+);?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
    sizes_regex_ = regex(numx+";?", std::regex_constants::ECMAScript | std::regex_constants::optimize);
}
