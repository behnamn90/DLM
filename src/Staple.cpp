/*
 *
 *	Staple.cpp
 * 	Author: Behnam
 *
 */

#include "Staple.h"

//Staple Methods
Staple::Staple () {
}
Staple::Staple (const string &string_) {
	set_length();
	set_reverse_comp();
}
void Staple::print(){
    cout << this->id << " ";
	cout << num_domains << " ";
	cout << this->length << "\t";
    //cout << this->concentration << "\t";
	cout << this->reverse_comp << "\t";
	cout << "s?" << this->is_seam << "\t";
	//cout << reverse_comp << "\t";
	cout << "Domains:\t";
	for (auto dom = domains.begin(); dom != domains.end(); ++dom){
		cout << "(" << (**dom).id << ") ";
		//cout << "(" << (**dom).node_idps.first << "->" << (**dom).node_idps.second << ") ";
		//cout << "(" << (**dom).nucs.first << "->" << (**dom).nucs.second << ") ";
		//cout << "(" << (**dom).seq.length() << ")\t";
		//cout << "(" << seq_domains[d] << ")\t";
	}
	//cout << "Crossovers:\t";
	//for (auto cross = crossovers.begin(); cross != crossovers.end(); ++cross){
		//cout << "(" << (**cross).id << ") ";
		//cout << "(" << (**cross).node_idps.first << "->" << (**cross).node_idps.second << ") ";
		//cout << "(" << (**cross).nucs.first << "->" << (**cross).nucs.second << ") ";
	//}
	cout << endl;
}
pair<CR,bool> Staple::dp2cross(const PDOM d1, const PDOM d2){
	pair<CR,bool> result;
	result.second = false;
	for (auto c1 = d1->crossovers.begin(); c1!= d1->crossovers.end(); ++c1){
		for (auto c2 = d2->crossovers.begin(); c2!= d2->crossovers.end(); ++c2){
			if((*c1)->id == (*c2)->id){
				result.second = true;
				result.first = *(c1);
				break;
			}
		}
	}
	return result;
}
void Staple::make_empty(){
    initialise_state(possible_states[0]);
}
void Staple::make_full(){
    initialise_state(possible_states[possible_states.size()-1]);
}
void Staple::change_state(State_t to){
    prev_state = state;
	if (to == s0){
		if (domains[0]->state) domains[0]->unbind();
	}
	else if (to == s1){
		if (!domains[0]->state) domains[0]->bind();
	}
    else if (to == s00){
        if (domains[0]->state) domains[0]->unbind();
        if (domains[1]->state) domains[1]->unbind();
    }
    else if (to == s10){
        if (!domains[0]->state) domains[0]->bind();
        if (domains[1]->state) domains[1]->unbind();
        if (crossovers[0]->state) crossovers[0]->unbind();
    }
    else if (to == s01){
        if (domains[0]->state) domains[0]->unbind();
        if (!domains[1]->state) domains[1]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
    }
    else if (to == s11){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (!crossovers[0]->state) crossovers[0]->bind();
    }
    else if (to == s12){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
    }
	else if (to == s000){
        if (domains[0]->state) domains[0]->unbind();
        if (domains[1]->state) domains[1]->unbind();
        if (domains[2]->state) domains[2]->unbind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s001){
        if (domains[0]->state) domains[0]->unbind();
        if (domains[1]->state) domains[1]->unbind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s010){
        if (domains[0]->state) domains[0]->unbind();
        if (!domains[1]->state) domains[1]->bind();
        if (domains[2]->state) domains[2]->unbind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s100){
        if (!domains[0]->state) domains[0]->bind();
        if (domains[1]->state) domains[1]->unbind();
        if (domains[2]->state) domains[2]->unbind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s011){
        if (domains[0]->state) domains[0]->unbind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (!crossovers[1]->state) crossovers[1]->bind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s110){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (domains[2]->state) domains[2]->unbind();
        if (!crossovers[0]->state) crossovers[0]->bind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s101){
        if (!domains[0]->state) domains[0]->bind();
        if (domains[1]->state) domains[1]->unbind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (!crossovers[2]->state) crossovers[2]->bind();
	}
	else if (to == s012){
        if (domains[0]->state) domains[0]->unbind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s120){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (domains[2]->state) domains[2]->unbind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s102){
        if (!domains[0]->state) domains[0]->bind();
        if (domains[1]->state) domains[1]->unbind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s112){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (!crossovers[0]->state) crossovers[0]->bind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s211){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (!crossovers[1]->state) crossovers[1]->bind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s121){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (!crossovers[2]->state) crossovers[2]->bind();
	}
	else if (to == s123){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (crossovers[0]->state) crossovers[0]->unbind();
        if (crossovers[1]->state) crossovers[1]->unbind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else if (to == s111){
        if (!domains[0]->state) domains[0]->bind();
        if (!domains[1]->state) domains[1]->bind();
        if (!domains[2]->state) domains[2]->bind();
        if (!crossovers[0]->state) crossovers[0]->bind();
        if (!crossovers[1]->state) crossovers[1]->bind();
        if (crossovers[2]->state) crossovers[2]->unbind();
	}
	else{
		cout << "Staple with " << state_names.array(to) << " state is not supported!\n";
	}
    this->state = to;
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        if((*op)->type == OP_t(BOUND_STAPLES)){
            if(state == *(possible_states.end()-1))
                (*op)->state++;
            if(prev_state == *(possible_states.end()-1))
                (*op)->state--;
        }
        if((*op)->type == OP_t(MISBOUND_STAPLES)){
            if(state == s12)
                (*op)->state++;
            if(prev_state == s12)
                (*op)->state--;
        }
         
    }
}
void Staple::initialise_state(State_t to){
    if (to == s0){
        domains[0]->unbind();
    }
    else if (to == s1){
        domains[0]->bind();
    }
    else if (to == s00){
        domains[0]->unbind();
        domains[1]->unbind();
        crossovers[0]->unbind();
    }
    else if (to == s10){
        domains[0]->bind();
        domains[1]->unbind();
        crossovers[0]->unbind();
    }
    else if (to == s01){
        domains[1]->bind();
        domains[0]->unbind();
        crossovers[0]->unbind();
    }
    else if (to == s11){
        domains[0]->bind();
        domains[1]->bind();
        crossovers[0]->bind();
    }
    else if (to == s12){
        domains[0]->bind();
        domains[1]->bind();
        crossovers[0]->unbind();
    }
    else if (to == s000){
        domains[0]->unbind();
        domains[1]->unbind();
        domains[2]->unbind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s001){
        domains[0]->unbind();
        domains[1]->unbind();
        domains[2]->bind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s010){
        domains[0]->unbind();
        domains[1]->bind();
        domains[2]->unbind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s100){
        domains[0]->bind();
        domains[1]->unbind();
        domains[2]->unbind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s011){
        domains[0]->unbind();
        domains[1]->bind();
        domains[2]->bind();
        (dp2cross(domains[0],domains[1]).first)->unbind();
        (dp2cross(domains[1],domains[2]).first)->bind();
        (dp2cross(domains[0],domains[2]).first)->unbind();
    }
    else if (to == s110){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->unbind();
        (dp2cross(domains[0],domains[1]).first)->bind();
        (dp2cross(domains[1],domains[2]).first)->unbind();
        (dp2cross(domains[0],domains[2]).first)->unbind();
    }
    else if (to == s101){
        domains[0]->bind();
        domains[1]->unbind();
        domains[2]->bind();
        (dp2cross(domains[0],domains[1]).first)->unbind();
        (dp2cross(domains[1],domains[2]).first)->unbind();
        (dp2cross(domains[0],domains[2]).first)->bind();
    }
    else if (to == s012){
        domains[0]->unbind();
        domains[1]->bind();
        domains[2]->bind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s120){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->unbind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s102){
        domains[0]->bind();
        domains[1]->unbind();
        domains[2]->bind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s112){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->bind();
        (dp2cross(domains[0],domains[1]).first)->bind();
        (dp2cross(domains[1],domains[2]).first)->unbind();
        (dp2cross(domains[0],domains[2]).first)->unbind();
    }
    else if (to == s211){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->bind();
        (dp2cross(domains[0],domains[1]).first)->unbind();
        (dp2cross(domains[1],domains[2]).first)->bind();
        (dp2cross(domains[0],domains[2]).first)->unbind();
    }
    else if (to == s121){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->bind();
        (dp2cross(domains[0],domains[1]).first)->unbind();
        (dp2cross(domains[1],domains[2]).first)->unbind();
        (dp2cross(domains[0],domains[2]).first)->bind();
    }
    else if (to == s123){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->bind();
        crossovers[0]->unbind();
        crossovers[1]->unbind();
        crossovers[2]->unbind();
    }
    else if (to == s111){
        domains[0]->bind();
        domains[1]->bind();
        domains[2]->bind();
        (dp2cross(domains[0],domains[1]).first)->bind();
        (dp2cross(domains[1],domains[2]).first)->bind();
        (dp2cross(domains[0],domains[2]).first)->unbind();
    }
    else{
        cout << "Staple with " << state_names.array(to) << " state is not supported!\n";
    }
    this->state = to;
}
