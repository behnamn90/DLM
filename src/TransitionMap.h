//
//  TransitionMap.hpp
//  DLM
//
//  Created by Behnam Najafi on 22/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef TransitionMap_hpp
#define TransitionMap_hpp

#include "TransitionProperty.h"
#include "Staple.h"

class TransitionMap{
public:
    State_names state_names;
	std::map< pair<State_t,State_t>, TP > FullMap;
    
    std::map< pair<State_t,State_t>, TP > CompMap;
    std::map< pair<State_t,State_t>, TP > EdgeMap;
    std::map< pair<State_t,State_t>, TP > NearEdgeMap;
    
    TransitionMap();
	virtual ~TransitionMap(){}
	
	void print();
	void print_allowed();
};


#endif /* TransitionMap_hpp */
