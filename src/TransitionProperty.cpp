//
//  TransitionProperty.cpp
//  DLM
//
//  Created by Behnam Najafi on 22/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#include "TransitionProperty.h"


TransitionProperty::TransitionProperty(){
	allowed = false;
	domain = -1;
	bind = false;
	cross = false;
	cross_dom = -1;
}
TransitionProperty::TransitionProperty(bool bind_, short domain_)
    : bind(bind_) , domain(domain_) {
	allowed = true;
	cross = false;
	cross_dom = -1;
}
TransitionProperty::TransitionProperty(bool bind_, short domain_, short cross_dom_) :
    bind(bind_) , domain(domain_) , cross_dom(cross_dom_){
	allowed = true;
	cross = true;
}
