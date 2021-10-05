1. CHOOSING DATA STRUCTURE FOR STOP AND REGION.

1.1. STRUCT OF PLACE
    
	+ "id" : PlaceID type. It represents the unique ID of each place
	
	+ name" : Name type. It represents the name of each place, that is not unique.
	
	+ "type" :PlaceType. There are several specific type that a place can have
	
	+ "coord" : Coord type. It represents the coordinate of the string
	
1.2. STRUCT OF AREA

	+ "id" : AreaID type. It represents the unique ID of each Area
	
	+ "name" : Name type. It represents the name of each area, that is not unique
	
	+ "bounds_" : vector of Coord type that define the boundary of specific place
	
	+ " parent_area_id" : that point to its direct parent area. One area that will have one direct 
				parent only.
				
	+ "souces_nodes_" : is a vector of AreaID. souces_nodes_ contains all its directly and indirectly child sub_area.
	
	
2. CHOOSING IMPLEMENTATION OF CLASS "Datastructures"

2.1. DATA MEMBER OF CLASS "Datastructures"

Datastructures class has private date members:

				   
	+ "id_data" : is an unordered map that store a PlaceID as a key and a shared pointer to the struct Place of that
				given PlaceID. The reason for choosing unordered map is that in some operation such as remove element
				or search element, its time complexcity in average case is lower compared to map (constant). In addition it can be used in some 
				function such as all_places, all_areas that return element in arbitary order. There shared pointer is chosen
				here for various reason such as since it is a one type of smart pointer, it will automatically deallocating of
				memory when nothing points to it anymore
				
	+ "area_data" : is an unordered map that store a AreaID as a key and a shared pointer to the struct Place of that
				given AreaID. The reason for choosing unordered map is that in some operation such as remove element
				or search element, its time complexcity in average case is lower compared to map (constant). In addition it can be used in some 
				function such as all_places, all_areas that return element in arbitary order. There shared pointer is chosen
				here for various reason such as since it is a one type of smart pointer, it will automatically deallocating of
				memory when nothing points to it anymore	
				
	+ "place_alpha" : is a vector contain PlaceID, will be used in function places_alphabetically
	
	+ "place_coord" : is a vector contain PlaceID, will be used in function places_coord_order.
	
	+ "isCoordSorted" : a bool value that will be used to check the place_coord vector if is already sorted by coordinate or not.
	
	+ "isNameSorted" : a bool value that will be used to check the place_alpha vector if is already sorted by name or not.	

	+ "all_places_vct" : is a vector contatain all places ID. It will used in function all_places.
	
	+ "all_areas_vct" : is a vector contatain all area ID. It will used in function all_areas.
	
	
2.2 CHOOSING IMPLEMENTING OF PUBLIC MEMBER FUNCTION

	+ "distace_square": 	a function that calculate the distance between 2 coordinate based on Euclid equation.
	
	+"compare_place_name":	a function that will compare a pair of PlaceID and Name and will return the bool value in acending order of name
							used in std::sort.
	
	
	
