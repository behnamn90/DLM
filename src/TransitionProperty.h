//
//  TransitionProperty.h
//  DLM
//
//  Created by Behnam Najafi on 22/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#ifndef TransitionProperty_h
#define TransitionProperty_h

#include "Common.h"


class TransitionProperty {
public:
	TransitionProperty();
	TransitionProperty(bool, short);
	TransitionProperty(bool, short, short);
    //TransitionProperty(State_t, State_t);
    //TransitionProperty(bool, char, char, char, char);

	virtual ~TransitionProperty(){}
	
    bool bind;  //bind or unbind?
    bool allowed; //Allowed currently or not?
    bool cross; //does it involve a crossover?
    
    //Normal
	short domain; //s_index of domain!
	short cross_dom; //the s_index of cross_dom!
};

typedef TransitionProperty TP;

#endif /* TransitionProperty_h */
