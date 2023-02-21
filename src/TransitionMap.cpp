//
//  TransitionMap.cpp
//  DLM
//
//  Created by Behnam Najafi on 22/05/2017.
//  Copyright © 2017 Behnam Najafi. All rights reserved.
//

#include "TransitionMap.h"

TransitionMap::TransitionMap(){

	for (int i=0; i<State_MAX; i++){
		for (int j=0; j<State_MAX; j++){
			FullMap[make_pair(State_t(i),State_t(j))] = TP();
		}
	}

	FullMap[make_pair(s0,s1)] = TP(true,0);
	
	FullMap[make_pair(s00,s10)] = TP(true,0);     FullMap[make_pair(s00,s01)] = TP(true,1);
	FullMap[make_pair(s10,s11)] = TP(true,1,0);   FullMap[make_pair(s10,s12)] = TP(true,1);
	FullMap[make_pair(s01,s11)] = TP(true,0,1);   FullMap[make_pair(s01,s12)] = TP(true,0);
	
	FullMap[make_pair(s000,s001)] = TP(true,2);   FullMap[make_pair(s000,s010)] = TP(true,1);   FullMap[make_pair(s000,s100)] = TP(true,0);
	FullMap[make_pair(s001,s011)] = TP(true,1,2); FullMap[make_pair(s001,s101)] = TP(true,0,2); FullMap[make_pair(s001,s012)] = TP(true,1);   FullMap[make_pair(s001,s102)] = TP(true,0);
	FullMap[make_pair(s010,s011)] = TP(true,2,1); FullMap[make_pair(s010,s110)] = TP(true,0,1); FullMap[make_pair(s010,s012)] = TP(true,2);   FullMap[make_pair(s010,s120)] = TP(true,0);
	FullMap[make_pair(s100,s110)] = TP(true,1,0); FullMap[make_pair(s100,s101)] = TP(true,2,0); FullMap[make_pair(s100,s120)] = TP(true,1);   FullMap[make_pair(s100,s102)] = TP(true,2);
	
	FullMap[make_pair(s011,s211)] = TP(true,0); FullMap[make_pair(s011,s111)] = TP(true,0,1);
	FullMap[make_pair(s110,s112)] = TP(true,2); FullMap[make_pair(s110,s111)] = TP(true,2,1);
	FullMap[make_pair(s101,s121)] = TP(true,1); //FullMap[make_pair(s101,s111)] = TP(true,0,1);
	
	FullMap[make_pair(s012,s112)] = TP(true,0,1); FullMap[make_pair(s012,s121)] = TP(true,0,2); FullMap[make_pair(s012,s123)] = TP(true,0);
	FullMap[make_pair(s120,s211)] = TP(true,2,1); FullMap[make_pair(s120,s121)] = TP(true,2,0); FullMap[make_pair(s120,s123)] = TP(true,2);
	FullMap[make_pair(s102,s112)] = TP(true,1,0); FullMap[make_pair(s102,s211)] = TP(true,1,2); FullMap[make_pair(s102,s123)] = TP(true,1);
    
	for (int i = 0; i<State_MAX; i++){
		for (int j = 0; j<State_MAX; j++){
			if (j>i){
				FullMap[make_pair(State_t(j),State_t(i))] = FullMap[make_pair(State_t(i),State_t(j))];
				FullMap[make_pair(State_t(j),State_t(i))].bind = false;
			}
		}
	}

    //FullMap[make_pair(s111,s101)] = TP(false,0,2);
}

void TransitionMap::print(){
	cout << "\t";
	for (int j = s0; j<State_MAX; j++){
		cout << state_names.array(j) << "\t";
	}
	cout << "\n";
	for (int i = 0; i<State_MAX; i++){
		cout << state_names.array(i) << "\t";
		for (int j = s0; j<State_MAX; j++){
			cout << FullMap[make_pair(State_t(i),State_t(j))].allowed << "\t";
		}
		cout << endl;
	}
}
