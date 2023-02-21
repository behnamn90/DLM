/*
 *
 *	Design.cpp
 * 	Author: Behnam
 *
 */

#include "Design.h"

bool sort_domains(Domain d1,Domain d2) {
    bool answer = d1.nucs.first < d2.nucs.first;
    return (answer);
}
bool sort_nodes(Node n1, Node n2) {
    bool answer;
    answer = min(n1.nucs.first,n1.nucs.second) < min(n2.nucs.first,n2.nucs.second);
    return answer;
}

//Constructors
Design::Design(){}
Design::Design(Inputs* inputs_) : inputs(inputs_){
    if (inputs->domain_energies == "seq"){
        this->scaffold = Scaffold(inputs->scaffold_file_name);
        for (int i = 0; i< inputs->scaff_rot; i++){
            std::rotate(scaffold.seq.begin(), scaffold.seq.begin()+1, scaffold.seq.end());
        }
    }
    else{
        this->scaffold = Scaffold();
    }
    target_reached = false;
    target_reached_at = make_pair(-1,-1);
    add_pools();
    refill_pool_pointers();
    add_nodes();
    add_subdoms();
    calc_subdom_energies();
    add_parents();
    add_pointers();
    if (inputs->miss_staples) add_missing();
    add_OPs();
    add_possible_states();
    initialise_obj_states();
    initialise_OP_states();
    add_OPs_to_objects();
}

void Design::add_pools(){
    ifstream myfile;
    myfile.open(inputs->topology_file_name);
    
    std::string all;
    myfile.seekg(0, std::ios::end);
    all.reserve(myfile.tellg());
    myfile.seekg(0, std::ios::beg);
    all.assign((std::istreambuf_iterator<char>(myfile)),std::istreambuf_iterator<char>());
    
    vector<std::string> StaplePools_i;
    boost::split(StaplePools_i, all, boost::is_any_of("}"));
    num_staple_pools = StaplePools_i.size() - 1;
    
    //StaplePool staple_pool;
    for (auto pool = StaplePools_i.begin(); pool != StaplePools_i.end()-1; ++pool){
        //staple_pool = StaplePool(inputs, *pool, pool-StaplePools_i.begin(), &scaffold);
        //staple_pools.push_back(staple_pool);
        staple_pools.push_back(StaplePool(inputs, *pool, std::distance(StaplePools_i.begin(),pool), &scaffold));
    }
    myfile.close();
    
    this->num_nucs = staple_pools[0].num_nucs;
    this->num_domains = 0;
    this->num_crossovers = 0;
    this->num_crosspairs = 0;
    this->num_staple = 0;
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        this->num_domains = this->num_domains + pool->num_domains;
        this->num_crossovers = this->num_crossovers + pool->num_crossovers;
        this->num_crosspairs = this->num_crosspairs + pool->num_crosspairs;
        this->num_staple = this->num_staple + pool->num_staples;
    }
}
void Design::refill_pool_pointers(){
    for (auto pool = this->staple_pools.begin(); pool!= this->staple_pools.end(); ++pool){
        for (auto dom = pool->domains.begin(); dom!= pool->domains.end(); ++dom){
            dom->helix = pool->helices.begin()+dom->helix_id;
            dom->staple = pool->staples.begin()+dom->staple_id;
            dom->crossovers.clear();
            for (auto cross = dom->crossover_ids.begin(); cross!= dom->crossover_ids.end(); ++cross){
                dom->crossovers.push_back(pool->crossovers.begin()+(*cross));
            }
            dom->cross_domains.clear();
            for (auto cross = dom->cross_dom_ids.begin(); cross!= dom->cross_dom_ids.end(); ++cross){
                dom->cross_domains.push_back(pool->domains.begin()+(*cross));
            }
            dom->stack_domains.clear();
            for (auto stack = dom->stack_ids.begin(); stack!= dom->stack_ids.end(); ++stack){
                dom->stack_domains.push_back(pool->domains.begin()+(*stack));
            }
        }
        for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
            st->domains.clear();
            for (auto dom = st->domain_ids.begin(); dom!=st->domain_ids.end(); ++dom){
                st->domains.push_back(pool->domains.begin()+(*dom));
            }
            st->crossovers.clear();
            for (auto cross = st->crossover_ids.begin(); cross!= st->crossover_ids.end(); ++cross){
                st->crossovers.push_back(pool->crossovers.begin()+(*cross));
            }
            st->helices.clear();
            for (auto helix = st->helix_ids.begin(); helix!=st->helix_ids.end(); ++helix){
                st->helices.push_back(pool->helices.begin()+(*helix));
            }
        }
        for (auto cross = pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
            cross->staple = pool->staples.begin()+cross->staple_id;
            cross->domains.first = pool->domains.begin()+(cross->domain_ids).first;
            cross->domains.second = pool->domains.begin()+(cross->domain_ids).second;
            cross->helices.first = pool->helices.begin()+(cross->helix_ids).first;
            cross->helices.second = pool->helices.begin()+(cross->helix_ids).second;
            if (cross->is_crosspair) cross->crosspair = pool->crosspairs.begin()+cross->crosspair_id;
        }
        for (auto cpair = pool->crosspairs.begin(); cpair!= pool->crosspairs.end(); ++cpair){
            cpair->crossovers.first = pool->crossovers.begin()+cpair->crossover_ids.first;
            cpair->crossovers.second = pool->crossovers.begin()+cpair->crossover_ids.second;
        }
        for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
            helix->pool = &(*pool);
            helix->domains.clear();
            for (auto d = helix->domain_ids.begin(); d!= helix->domain_ids.end(); ++d){
                helix->domains.push_back(pool->domains.begin()+*d);
            }
            for (auto part = helix->parts.begin(); part!= helix->parts.end(); ++part){
                part->domains.clear();
                for (auto d = part->domain_ids.begin(); d!= part->domain_ids.end(); ++d){
                    part->domains.push_back(pool->domains.begin()+*d);
                }
            }
        }
    }
}
void Design::add_nodes(){
    Node node;
    int next;
    for (int pid = 0; pid < num_staple_pools; pid++){
        for (int did = 0; did < staple_pools[pid].num_domains; did++){
            next = circ_add(did, 1, staple_pools[pid].num_domains);
            node.pool_id = pid;
            node.pooldomain_ids.first = did;
            node.pooldomain_ids.second = next;
            node.nucs.first = staple_pools[pid].domains[did].nucs.second;
            node.nucs.second = staple_pools[pid].domains[next].nucs.first;
            this->nodes.push_back(node);
        }
    }
    std::sort (this->nodes.begin(), this->nodes.end(), sort_nodes);
    int i = 0;
    for (auto node = nodes.begin(); node!= nodes.end(); ++node){
        node->id = i;
        i++;
    }
    int id_in_pool;
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        id_in_pool = 0;
        for (auto node = nodes.begin(); node!= nodes.end(); ++node){
            if (node->pool_id == pool->id){
                node->idp = id_in_pool;
                id_in_pool++;
            }
        }
    }
    for (auto node = nodes.begin(); node!= nodes.end(); ++node){
        staple_pools[node->pool_id].domains[(node->pooldomain_ids).first].node_ids.second = node->id;
        staple_pools[node->pool_id].domains[(node->pooldomain_ids).second].node_ids.first = node->id;
    }
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto cross= pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
            cross->node_ids.first = cross->domains.first->node_ids.second;
            cross->node_ids.second = cross->domains.second->node_ids.first;
        }
    }
}
void Design::add_subdoms(){
    SubDomain domain;
    int n2;
    for (int n1 = 0; n1<this->num_domains; n1++){
        n2 = circ_add(n1,1,num_domains);
        domain.id = nodes[n1].id;
        domain.node_ids = make_pair(n1,n2);
        domain.nucs = make_pair(nodes[n1].nucs.second, nodes[n2].nucs.first);
        domain.length = circ_dist(domain.nucs.second, domain.nucs.first, num_nucs);
        if (inputs->domain_energies == "seq"){
            if (domain.nucs.second > domain.nucs.first){
                domain.seq = this->scaffold.seq.substr(domain.nucs.first,domain.length);
            }
            else{
                domain.seq = this->scaffold.seq.substr(domain.nucs.first,scaffold.length-domain.nucs.first);
                domain.seq += this->scaffold.seq.substr(0,domain.nucs.second+1);
            }
        }
        nodes[n1].subdomain_ids.second = domain.id;
        nodes[n2].subdomain_ids.first = domain.id;
        domain.ss_weight = domain.length * ss_hack;
        domain.ds_weight = domain.length * domain.length * ds_hack;
        domains.push_back(domain);
    }
}
void Design::calc_subdom_energies(){
    double dH,dS;
    if (inputs->domain_energies == "average"){
        for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
            dH = 0.;
            dS = 0.;
            dH += (dom->length - 1) * dH_average;
            dS += (dom->length - 1) * dS_average;
            dH += dH_init;
            dS += dS_init;
            dH += dH_termAT;
            dS += dS_termAT;
            dS += (dom->length - 1) * salt_per_phosphate_hack;
            dom->dH = dH;
            dom->dS = dS;
        }
    }
    else if (inputs->domain_energies == "seq"){
        for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
            dH = 0.;
            dS = 0.;
            string sub;
            for (int i=0; i<dom->length-1; i++){
                sub.clear();
                sub += dom->seq[i];
                sub += dom->seq[i+1];
                if (sub == "AA" || sub == "TT") { dH+=dH_AA; dS+=dS_AA; }
                else if (sub == "CA" || sub == "TG") { dH+=dH_CA; dS+=dS_CA; }
                else if (sub == "GT" || sub == "AC") { dH+=dH_GT; dS+=dS_GT; }
                else if (sub == "CT" || sub == "AG") { dH+=dH_CT; dS+=dS_CT; }
                else if (sub == "GA" || sub == "TC") { dH+=dH_GA; dS+=dS_GA; }
                else if (sub == "GG" || sub == "CC") { dH+=dH_GG; dS+=dS_GG; }
                else if (sub == "AT") { dH+=dH_AT; dS+=dS_AT; }
                else if (sub == "TA") { dH+=dH_TA; dS+=dS_TA; }
                else if (sub == "CG") { dH+=dH_CG; dS+=dS_CG; }
                else if (sub == "GC") { dH+=dH_GC; dS+=dS_GC; }
                else {
                    cout << "Error! dom->seq not in nn database. \n" << dom->seq << "\n";
                    exit (EXIT_FAILURE);
                }
            }
            dH+= dH_init;
            dS+= dS_init;
            if (dom->seq[0] == 'A' || dom->seq[0] == 'T'){
                dH += dH_termAT;
                dS += dS_termAT;
            }
            if (dom->seq[dom->length-1] == 'A' || dom->seq[dom->length-1] == 'T'){
                dH += dH_termAT;
                dS += dS_termAT;
            }
            dS+= (dom->length - 1) * salt_per_phosphate_hack;
            dom->dH = dH;
            dom->dS = dS;
        }
    }
    else if (inputs->domain_energies == "custom"){
        if (this->num_staple_pools == 1){
            string line_in;
            vector<std::string> line; //To split line_in
            ifstream myfile (inputs->energy_file_name);
            if (myfile.is_open()) {
                while(getline(myfile, line_in)) {
                    boost::split(line, line_in, boost::is_any_of("\t"));
                    this->domains[stoi(line[0])].dH = stod(line[1]);
                    this->domains[stoi(line[0])].dS = stod(line[2]);
                }
                myfile.close();
            }
            else {printf ("Error:\t Could not read energy file\n"); exit (EXIT_FAILURE);}
        }
        else{
            printf ("Error:\t Cannot have custom energies with more than 1 staple pool.\n"); exit (EXIT_FAILURE);
            
        }
    }
    else{
        printf ("Error:\t Specify either average, seq or custom energies.\n"); exit (EXIT_FAILURE);
    }
}
void Design::add_parents(){
    int n1, n2;
    for (auto sdom = domains.begin(); sdom!= domains.end(); ++sdom){
        n1 = sdom->node_ids.first;
        n2 = sdom->node_ids.second;
        sdom->parent_ids[nodes[n1].pool_id] = nodes[n1].pooldomain_ids.second;
        sdom->parent_ids[nodes[n2].pool_id] = nodes[n2].pooldomain_ids.first;
        for (auto par = sdom->parent_ids.begin(); par!= sdom->parent_ids.end(); ++par){
            staple_pools[par->first].domains[par->second].children_ids.push_back(sdom->id);
            sdom->staple_ids[par->first] = staple_pools[par->first].domains[par->second].staple_id;
        }
    }
    for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
        dom->bind_pair_ids.clear();
        for(auto parent = dom->parent_ids.begin(); parent!= dom->parent_ids.end(); ++parent){
            for (auto child = (staple_pools[parent->first]).domains[parent->second].children_ids.begin();
                 child != (staple_pools[parent->first]).domains[parent->second].children_ids.end(); ++child){
                if (*child != dom->id){
                    dom->bind_pair_ids[parent->first] = *child;
                }
            }
        }
    }
    //Some adjustments
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for(auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
            if (pdom->children_ids[0] == 0 && pdom->children_ids[1] == num_domains-1){
                pdom->children_ids.clear();
                pdom->children_ids.push_back(num_domains-1);
                pdom->children_ids.push_back(0);
            }
        }
    }
}
void Design::add_pointers(){
    //Filling staple pointers for subdomains
    for (auto sdom = domains.begin(); sdom!=domains.end(); ++sdom){
        sdom->staples.clear();
        for (auto st = sdom->staple_ids.begin(); st!= sdom->staple_ids.end(); ++st){
            sdom->staples[st->first] = staple_pools[st->first].staples.begin()+st->second;
        }
    }
    //Filling parent pointers for subdomains
    for (auto sdom = domains.begin(); sdom!=domains.end(); ++sdom){
        for (auto par = sdom->parent_ids.begin(); par!= sdom->parent_ids.end(); ++par){
            sdom->parents[par->first] = staple_pools[par->first].domains.begin()+par->second;
        }
    }
    //Filling children pointers for pooldomains
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
            pdom->children.clear();
            for (auto child = pdom->children_ids.begin(); child != pdom->children_ids.end(); ++child){
                pdom->children.push_back(this->domains.begin()+*child);
            }
        }
    }
    //Filling bind_pair pointers for subdomains
    for (auto dom = domains.begin(); dom!=domains.end(); ++dom){
        for (auto it= dom->bind_pair_ids.begin(); it!= dom->bind_pair_ids.end(); ++it){
            dom->bind_pairs[it->first] = domains.begin()+it->second;
        }
    }
    //Filling Node pointers
    for (auto node = this->nodes.begin(); node!= this->nodes.end(); ++node){
        node->subdomains.first = this->domains.begin()+node->subdomain_ids.first;
        node->subdomains.second = this->domains.begin()+node->subdomain_ids.second;
        auto pool = staple_pools.begin()+node->pool_id;
        node->pool = &(*pool);
        node->pooldomains.first = pool->domains.begin()+node->pooldomain_ids.first;
        node->pooldomains.second = pool->domains.begin()+node->pooldomain_ids.second;
    }
    for (auto sdom = this->domains.begin(); sdom!= this->domains.end(); ++sdom){
        sdom->nodes.first = this->nodes.begin()+sdom->node_ids.first;
        sdom->nodes.second = this->nodes.begin()+sdom->node_ids.second;
        sdom->node_idps = make_pair(sdom->nodes.first->idp,sdom->nodes.second->idp);
    }
    for (auto pool = this->staple_pools.begin(); pool!= this->staple_pools.end(); ++pool){
        for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
            pdom->nodes.first = this->nodes.begin()+pdom->node_ids.first;
            pdom->nodes.second = this->nodes.begin()+pdom->node_ids.second;
        }
    }
    for (auto pool = this->staple_pools.begin(); pool!= this->staple_pools.end(); ++pool){
        for (auto cross = pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
            cross->nodes.first = this->nodes.begin()+cross->node_ids.first;
            cross->nodes.second = this->nodes.begin()+cross->node_ids.second;
        }
    }
}
void Design::add_missing(){
    ifstream myfile;
    myfile.open(inputs->missing_staple_file_name);
    std::string all;
    myfile.seekg(0, std::ios::end);
    all.reserve(myfile.tellg());
    myfile.seekg(0, std::ios::beg);
    all.assign((std::istreambuf_iterator<char>(myfile)),std::istreambuf_iterator<char>());
    
    vector<std::string> Lines;
    boost::split(Lines, all, boost::is_any_of("\n"));
    vector<std::string> Line;
    for (auto line = Lines.begin(); line!= Lines.end(); ++line){
        boost::split(Line, *line, boost::is_any_of(" "));
        if (Line.size() > 1) {
            staple_pools[stoi(Line[0])].staples[stoi(Line[1])].is_missing = true;
            staple_pools[stoi(Line[0])].staples[stoi(Line[1])].concentration = 0;
        }
    }
}
void Design::add_OPs(){
    OrderParameter OP;
    string name;
    
    name = "BPs";
    OP = OrderParameter(-1,BOUND_BPS,name);
    for (auto sdom = domains.begin(); sdom!= domains.end(); ++sdom){
        OP.group_ids.insert(sdom->id);
    }
    this->OPs.push_back(OP);
    name = "Domains";
    OP = OrderParameter(-1,BOUND_DOMAINS,name);
    for (auto sdom = domains.begin(); sdom!= domains.end(); ++sdom){
        OP.group_ids.insert(sdom->id);
    }
    this->OPs.push_back(OP);
    
    for(auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        name = "BPs_"; name+= pool->name;
        OP = OrderParameter(pool->id,BOUND_BPS,name);
        for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
            OP.group_ids.insert(pdom->id);
        }
        pool->OPs.push_back(OP);
        name = "Domains_"; name+= pool->name;
        OP = OrderParameter(pool->id,BOUND_DOMAINS,name);
        for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
            OP.group_ids.insert(pdom->id);
        }
        pool->OPs.push_back(OP);
    }
    if (inputs->extra_ops){
        this->read_OPs();
    }
}
void Design::read_OPs(){
    OrderParameter OP;
    string name;
    OP_t type = OP_MAX;
    int pool_id = -10;
    std::set<int> group_ids;
    
    ifstream myfile;
    myfile.open(inputs->op_file_name);
    
    std::string all;
    myfile.seekg(0, std::ios::end);
    all.reserve(myfile.tellg());
    myfile.seekg(0, std::ios::beg);
    all.assign((std::istreambuf_iterator<char>(myfile)),std::istreambuf_iterator<char>());
    
    vector<std::string> OPs_i;
    boost::split(OPs_i, all, boost::is_any_of("}"));
    OPs_i.erase(OPs_i.end()-1);
    //cout << OPs_i.size() << endl;
    
    vector<string> Lines;
    vector<string> vline;
    vector<string> vset;
    for (auto OP_i = OPs_i.begin(); OP_i!= OPs_i.end(); ++OP_i){
        boost::split(Lines, *OP_i, boost::is_any_of("\n"));
        Lines.erase(Lines.begin());
        Lines.erase(Lines.end()-1);
        if (OP_i != OPs_i.begin())
            Lines.erase(Lines.begin());
        for (auto line = Lines.begin(); line!= Lines.end(); ++line){
            line->erase(std::remove(line->begin(), line->end(), '\t'), line->end());
            boost::split(vline, *line, boost::is_any_of("="));
            vline[1].erase(std::remove(vline[1].begin(), vline[1].end(), ']'), vline[1].end());
            vline[1].erase(std::remove(vline[1].begin(), vline[1].end(), '['), vline[1].end());
            boost::algorithm::trim(vline[0]);
            boost::algorithm::trim(vline[1]);
            if (vline[0] == "OP_t"){
                type = OP_t(stoi(vline[1]));
            }
            if (vline[0] == "Name"){
                name = vline[1];
            }
            if (vline[0] == "Pool"){
                pool_id = stoi(vline[1]);
            }
            if (vline[0] == "Set"){
                boost::split(vset, vline[1], boost::is_any_of(" "));
                group_ids.clear();
                for (auto id = vset.begin(); id!= vset.end(); ++id){
                    group_ids.insert(stoi(*id));
                }
            }
        }
        name+= "_";
        if (pool_id != -10 && type != OP_MAX){
            name+= staple_pools[pool_id].name;
            OP = OrderParameter(pool_id,type,name);
            OP.group_ids = group_ids;
            staple_pools[OP.pool_id].OPs.push_back(OP);
        }
        else{
            printf ("Design::read_OPs(): pool_id or op_type not read!\n");
            exit (EXIT_FAILURE);
        }
    }
}
void Design::add_possible_states(){
    for (auto dom = domains.begin(); dom!= domains.end(); ++dom){
        dom->possible_states.push_back(-1);
        dom->possible_state_names.push_back("N");
        for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
            dom->possible_states.push_back(pool->id);
            dom->possible_state_names.push_back(pool->name);
        }
    }
    sdom_poss_states.push_back(-1);
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        sdom_poss_states.push_back(pool->id);
    }
    pdom_poss_states.push_back(false);
    pdom_poss_states.push_back(true);
    cross_poss_states.push_back(false);
    cross_poss_states.push_back(true);
    crosspair_poss_states.push_back(false);
    crosspair_poss_states.push_back(true);
    helix_poss_states.push_back('N');
    helix_poss_states.push_back('L');
    helix_poss_states.push_back('R');
    helix_poss_states.push_back('F');
    set<int> possible_num_domains;
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
            possible_num_domains.insert(st->num_domains);
        }
    }
    if (possible_num_domains.find(1) != possible_num_domains.end()){
        for (int i = 0; i < s00; i++){
            staple_poss_states.push_back(State_t(i));
        }
    }
    if (possible_num_domains.find(2) != possible_num_domains.end()){
        for (int i = s00; i < s000; i++){
            staple_poss_states.push_back(State_t(i));
        }
    }
    if (possible_num_domains.find(3) != possible_num_domains.end()){
        for (int i = s000; i < State_MAX; i++){
            staple_poss_states.push_back(State_t(i));
        }
    }
    sdom_poss_state_names.push_back("N");
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        sdom_poss_state_names.push_back(pool->name);
    }
    pdom_poss_state_names.push_back("N");
    pdom_poss_state_names.push_back("B");
    cross_poss_state_names.push_back("N");
    cross_poss_state_names.push_back("B");
    crosspair_poss_state_names.push_back("N");
    crosspair_poss_state_names.push_back("B");
    helix_poss_state_names.push_back("N");
    helix_poss_state_names.push_back("L");
    helix_poss_state_names.push_back("R");
    helix_poss_state_names.push_back("F");
    for (auto it = staple_poss_states.begin(); it!= staple_poss_states.end(); ++it){
        staple_poss_state_names.push_back(state_names.array(*it));
    }
}
void Design::initialise_obj_states(){
    if (inputs->initialise_as == "empty" || inputs->initialise_as == "random"){
        for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
            for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                st->make_empty();
            }
        }
    }
    else if (inputs->initialise_as == "coated"){
        for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
            if (pool->id != inputs->coat_with){
                for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                    st->make_empty();
                }
            }
        }
        auto pool = staple_pools.begin() + inputs->coat_with;
        for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
            st->make_full();
        }
    }
    else if (inputs->initialise_as == "conf_file"){
        ifstream myfile;
        myfile.open(inputs->configuration_file_name);
        
        std::string all;
        myfile.seekg(0, std::ios::end);
        all.reserve(myfile.tellg());
        myfile.seekg(0, std::ios::beg);
        all.assign((std::istreambuf_iterator<char>(myfile)),std::istreambuf_iterator<char>());
        
        vector<std::string> all_i;
        boost::split(all_i, all, boost::is_any_of("{"));
        
        vector<std::string> A_i;
        boost::split(A_i, all_i[0], boost::is_any_of("\n"));
        string subdomains = A_i[2];
        vector<std::string> subdomains_i;
        boost::split(subdomains_i, subdomains, boost::is_any_of(" "));
        
        vector<std::string> P_i;
        vector<std::string> st_i;
        vector<std::string> dom_i;
        vector<std::string> cr_i;
        for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
            boost::split(P_i, all_i[pool->id+1], boost::is_any_of("\n"));
            boost::split(st_i, P_i[1], boost::is_any_of(" "));
            boost::split(dom_i, P_i[2], boost::is_any_of(" "));
            boost::split(cr_i, P_i[3], boost::is_any_of(" "));
            for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                st->state = State_t(stoi(st_i[st->id]));
            }
            for (auto dom = pool->domains.begin(); dom!= pool->domains.end(); ++dom){
                dom->state = false;
                if (stoi(dom_i[dom->id]) == 1) dom->state = true;
            }
            for (auto cr = pool->crossovers.begin(); cr!= pool->crossovers.end(); ++cr){
                cr->state = false;
                if (stoi(cr_i[cr->id]) == 1) cr->state = true;
            }
        }
        for (auto dom = domains.begin(); dom!= domains.end(); ++dom){
            dom->state = stoi(subdomains_i[dom->id]);
        }
        myfile.close();
    }
    else{
        printf ("Error! initialisation not specified.\n"); exit (EXIT_FAILURE);
    }
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
            if (st->is_missing) st->make_empty();
        }
    }
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
            helix->update_state();
        }
    }
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto cpair = pool->crosspairs.begin(); cpair!= pool->crosspairs.end(); ++cpair){
            cpair->update_state();
        }
    }
}
void Design::initialise_OP_states(){
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        op->state = 0;
        if (op->type == OP_t(BOUND_BPS)){
            for (auto sdom = this->domains.begin(); sdom!= this->domains.end(); ++sdom){
                if (sdom->state>-1 && op->group_ids.find(sdom->id) != op->group_ids.end())
                    op->state += sdom->length;
            }
        }
        if (op->type == OP_t(BOUND_DOMAINS)){
            for (auto sdom = this->domains.begin(); sdom!= this->domains.end(); ++sdom){
                if (sdom->state>-1 && op->group_ids.find(sdom->id) != op->group_ids.end())
                    op->state++;
            }
        }
    }
    for(auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
            op->state = 0;
            if (op->type == OP_t(BOUND_BPS)){
                for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
                    if(pdom->state && op->group_ids.find(pdom->id) != op->group_ids.end())
                        op->state += pdom->length;
                }
            }
            if (op->type == OP_t(BOUND_DOMAINS)){
                for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
                    if(pdom->state && op->group_ids.find(pdom->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(BOUND_CROSSOVERS)){
                for (auto cross = pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
                    if(cross->state && op->group_ids.find(cross->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(BOUND_CROSSPAIRS)){
                for (auto cpair = pool->crosspairs.begin(); cpair!= pool->crosspairs.end(); ++cpair){
                    if(cpair->state && op->group_ids.find(cpair->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(BOUND_STAPLES)){
                for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                    if(st->state == *(st->possible_states.end()-1) && op->group_ids.find(st->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(MISBOUND_STAPLES)){
                for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                    if(st->state == s12 && op->group_ids.find(st->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(BOUND_HELICES)){
                for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
                    if(helix->state == *(helix->possible_states.end()-1) && op->group_ids.find(helix->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(LBOUND_HELICES)){
                for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
                    if(helix->state == 'L' && op->group_ids.find(helix->id) != op->group_ids.end())
                        op->state++;
                }
            }
            if (op->type == OP_t(RBOUND_HELICES)){
                for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
                    if(helix->state == 'R' && op->group_ids.find(helix->id) != op->group_ids.end())
                        op->state++;
                }
            }
        }
    }
}
void Design::add_OPs_to_objects(){
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        if (op->type == OP_t(BOUND_BPS) ||
            op->type == OP_t(BOUND_DOMAINS)){
            for (auto sdom = this->domains.begin(); sdom!= this->domains.end(); ++sdom){
                if (op->group_ids.find(sdom->id) != op->group_ids.end())
                    sdom->OPs.push_back(&(*op));
            }
        }
    }
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
            if (op->type == OP_t(BOUND_BPS) ||
                op->type == OP_t(BOUND_DOMAINS)){
                for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
                    if(op->group_ids.find(pdom->id) != op->group_ids.end())
                        pdom->OPs.push_back(&(*op));
                }
            }
            if (op->type == OP_t(BOUND_CROSSOVERS)){
                for (auto cross = pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
                    if(op->group_ids.find(cross->id) != op->group_ids.end())
                        cross->OPs.push_back(&(*op));
                }
            }
            if (op->type == OP_t(BOUND_CROSSPAIRS)){
                for (auto cpair = pool->crosspairs.begin(); cpair!= pool->crosspairs.end(); ++cpair){
                    if(op->group_ids.find(cpair->id) != op->group_ids.end())
                        cpair->OPs.push_back(&(*op));
                }
            }
            if (op->type == OP_t(BOUND_STAPLES)){
                for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                    if(op->group_ids.find(st->id) != op->group_ids.end())
                        st->OPs.push_back(&(*op));
                }
            }
            if (op->type == OP_t(MISBOUND_STAPLES)){
                for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
                    if(op->group_ids.find(st->id) != op->group_ids.end())
                        st->OPs.push_back(&(*op));
                }
            }
            if (op->type == OP_t(BOUND_HELICES) ||
                op->type == OP_t(LBOUND_HELICES) ||
                op->type == OP_t(RBOUND_HELICES)){
                for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
                    if(op->group_ids.find(helix->id) != op->group_ids.end())
                        helix->OPs.push_back(&(*op));
                }
            }
        }
    }
}
void Design::change_state(const ST& staple, const State_t final_state){
    for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
        op->prev_prev_state = op->prev_state;
        op->prev_state = op->state;
    }
    for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
        for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
            op->prev_prev_state = op->prev_state;
            op->prev_state = op->state;
        }
    }
    staple->change_state(final_state);
    for (auto& pool : staple_pools){
        for (auto& cpair : pool.crosspairs){
            cpair.update_state();
        }
        for (auto& helix : pool.helices){
            helix.update_state();
        }
    }
}


//Printers
void Design::print_domains(){
    cout << "\n------- Printing Domains -------\n";
    for (auto dom = domains.begin(); dom != domains.end(); ++dom){
        cout << dom->id << "\t";

        cout << dom->state << "\t";
        cout << "len: " << dom->length << "\t";
        cout << "nucs: (" << dom->nucs.first << "->" << dom->nucs.second << ")\t";
        cout << "nodes: (" << dom->nodes.first->id << "->" << dom->nodes.second->id << ")\t";
        /*
        cout << "s? " << dom->is_seam << "\t";
        cout << "e? " << dom->is_edge << "\t";
        cout << "dG " << dom->dH << " " << dom->dS << "\t";
        cout << "helix " << dom->helix_id << "\t";
        cout << "staples:\t";
        for(auto st = dom->staples.begin(); st!= dom->staples.end(); ++st){
            cout << "(" << staple_pools[st->first].name << "," << st->second->id << ") ";
        }   cout << "\t";
        cout << "pars:\t";
        for (auto d=dom->parents.begin(); d!= dom->parents.end(); ++d){
            cout << "(" << staple_pools[d->first].name << "," << d->second->id << ") ";
        }   cout << "\t";
        cout << "bps: ";
        for (auto d=dom->bind_pairs.begin(); d!=dom->bind_pairs.end(); ++d){
            cout << "(" << staple_pools[d->first].name << "," << d->second->id << ") ";
        }   cout << "\t";
        cout << dom->dH << " , ";
        cout << dom->dS << "\t";
        cout << dom->seq << "\t";
        for (auto op = dom->OPs.begin(); op!= dom->OPs.end(); ++op){
            cout << (*op)->pool_id << "(" << (*op)->name << ")" << "\t";
        }
        */
        cout << endl;
    }
}
void Design::print_nodes(){
    cout << "\n------- Printing Nodes -------\n";
    for (auto node = nodes.begin(); node != nodes.end(); ++node){
        cout << node->id << " (";
        cout << node->subdomain_ids.first << "," << node->subdomain_ids.second << ")\t";
        cout << " (" << node->nucs.first << "," << node->nucs.second << ")\t";
        cout << node->pool_id << " - ";
        cout << node->idp << " (";
        cout << node->pooldomain_ids.first << "," << node->pooldomain_ids.second << ")\t";
        cout << endl;
    }
}
void Design::print_OPs(){
    OP_names names;
    cout << "\n---------------- Printing OPs ----------------\n\n";
    for (auto op = OPs.begin(); op != OPs.end(); ++op){
        cout << op->name << "\t";
        cout << names.array(op->type) << "\t";
        //cout << staple_pools[op->pool_id].name << "\t";
        cout << op->group_ids.size() << "\t";
        cout << "\n";
    }
}
void Design::print(){
    cout << "\n---------------- Printing Design ----------------\n\n";
    print_nodes();
    print_domains();
}

const map<pair<int, int>, pair<bool, int>> &Design::getVmap() const {
    return _vmap;
}

void Design::setVmap(const map<pair<int, int>, pair<bool, int>> &vmap) {
    _vmap = vmap;
}


/*
 void Design::change_state(const ST& staple, const State_t final_state){
 staple->change_state(final_state);
 for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
 for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
 helix->update_state();
 }
 }
 
 for (auto op = this->OPs.begin(); op!= this->OPs.end(); ++op){
 op->prev_prev_state = op->prev_state;
 op->prev_state = op->state;
 op->state = 0;
 if (op->type == OP_t(BOUND_BPS)){
 for (auto sdom = this->domains.begin(); sdom!= this->domains.end(); ++sdom){
 if (sdom->state>-1 && op->group_ids.find(sdom->id) != op->group_ids.end())
 op->state += sdom->length;
 }
 }
 if (op->type == OP_t(BOUND_DOMAINS)){
 for (auto sdom = this->domains.begin(); sdom!= this->domains.end(); ++sdom){
 if (sdom->state>-1 && op->group_ids.find(sdom->id) != op->group_ids.end())
 op->state++;
 }
 }
 }
 for (auto pool = staple_pools.begin(); pool!= staple_pools.end(); ++pool){
 for (auto op = pool->OPs.begin(); op!= pool->OPs.end(); ++op){
 op->prev_prev_state = op->prev_state;
 op->prev_state = op->state;
 op->state = 0;
 if (op->type == OP_t(BOUND_BPS)){
 for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
 if(pdom->state && op->group_ids.find(pdom->id) != op->group_ids.end())
 op->state += pdom->length;
 }
 }
 if (op->type == OP_t(BOUND_DOMAINS)){
 for (auto pdom = pool->domains.begin(); pdom!= pool->domains.end(); ++pdom){
 if(pdom->state && op->group_ids.find(pdom->id) != op->group_ids.end())
 op->state++;
 }
 }
 if (op->type == OP_t(BOUND_CROSSOVERS)){
 for (auto cross = pool->crossovers.begin(); cross!= pool->crossovers.end(); ++cross){
 if(cross->state && op->group_ids.find(cross->id) != op->group_ids.end())
 op->state++;
 }
 }
 if (op->type == OP_t(BOUND_STAPLES)){
 for (auto st = pool->staples.begin(); st!= pool->staples.end(); ++st){
 if(st->state == *(st->possible_states.end()-1) && op->group_ids.find(st->id) != op->group_ids.end())
 op->state++;
 }
 }
 if (op->type == OP_t(BOUND_HELICES)){
 for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
 if(helix->state == *(helix->possible_states.end()-1) && op->group_ids.find(helix->id) != op->group_ids.end())
 op->state++;
 }
 }
 if (op->type == OP_t(LBOUND_HELICES)){
 for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
 if(helix->state == 'L' && op->group_ids.find(helix->id) != op->group_ids.end())
 op->state++;
 }
 }
 if (op->type == OP_t(RBOUND_HELICES)){
 for (auto helix = pool->helices.begin(); helix!= pool->helices.end(); ++helix){
 if(helix->state == 'R' && op->group_ids.find(helix->id) != op->group_ids.end())
 op->state++;
 }
 }
 }
 }
 }
 */
