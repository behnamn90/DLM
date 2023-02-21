#include "../src/Constants.h"
#include "../src/Design.h"
#include "../src/MyGraph.h"
#include "../src/TempRamp.h"
#include "../src/Simulation.h"


vector<vector<State_t>> cartesian( vector<vector<State_t> >& v ) {
    vector<vector<State_t>> result;
    auto product = []( long long a, vector<State_t>& b ) { return a*b.size(); };
    const long long N = accumulate( v.begin(), v.end(), 1LL, product );
    vector<State_t> u(v.size());
    for( long long n=0 ; n<N ; ++n ) {
        lldiv_t q { n, 0 };
        for( long long i=v.size()-1 ; 0<=i ; --i ) {
            q = div( q.quot, v[i].size() );
            u[i] = v[i][q.rem];
        }
        result.push_back(u);
    }
    return result;
}

void manual_apply_normal(TransitionManager *localManager, Uni& uni, MyGraph *G, TempRamp *ramp,
                         int pool_id, int domain_id, State_t initial_state, State_t final_state){
    int staple_id = localManager->design->staple_pools[pool_id].domains[domain_id].staple_id;
    string movie_file_name;
    localManager->fill_rates();
    localManager->select_transition(uni);
    cout << localManager->total_rate << "\n";
    localManager->manual_select_normal(pool_id, staple_id, domain_id, initial_state, final_state)->print();
    localManager->manual_select_normal(pool_id, staple_id, domain_id, initial_state, final_state)->apply(G);
    //if(!localManager->local) G->update_faces();
    localManager->step++;
    ramp->move_time(localManager->tau);
    localManager->reset_possibles();
    localManager->reset_recalculates();
    movie_file_name = "Output/Movie/";
    movie_file_name += to_string(localManager->step);
    movie_file_name += "_";
    movie_file_name += to_string_with_precision(centigrade(ramp->get_T()));
    G->write(movie_file_name);
}

void manual_apply_invasion(TransitionManager *localManager, Uni& uni, MyGraph *G, TempRamp *ramp,
                           int pool_id1, int domain_id1, State_t initial_state1, State_t final_state1,
                           int pool_id2, int domain_id2, State_t initial_state2, State_t final_state2){
    int staple_id1 = localManager->design->staple_pools[pool_id1].domains[domain_id1].staple_id;
    int staple_id2 = localManager->design->staple_pools[pool_id2].domains[domain_id2].staple_id;
    string movie_file_name;
    localManager->fill_rates();
    localManager->select_transition(uni);
    cout << localManager->total_rate << "\n";
    localManager->manual_select_invasion(pool_id1, staple_id1, domain_id1, initial_state1, final_state1,
                                         pool_id2, staple_id2, domain_id2, initial_state2, final_state2)->print();
    localManager->manual_select_invasion(pool_id1, staple_id1, domain_id1, initial_state1, final_state1,
                                         pool_id2, staple_id2, domain_id2, initial_state2, final_state2)->apply(G);
    //if(!localManager->local) G->update_faces();
    localManager->step++;
    ramp->move_time(localManager->tau);
    localManager->reset_possibles();
    localManager->reset_recalculates();
    movie_file_name = "Output/Movie/";
    movie_file_name += to_string(localManager->step);
    movie_file_name += "_";
    movie_file_name += to_string_with_precision(centigrade(ramp->get_T()));
    G->write(movie_file_name);
}

bool pairCompare(const std::pair<int, int*>& firstElem, const std::pair<int, int*>& secondElem) {
    return firstElem.first < secondElem.first;
}

void test_loops(Constants* constants, MyGraph *G,  Design *design){
    //cout << constants->C_parameter << "\t" << log(constants->C_parameter) << endl;
    design->staple_pools[0].print_crossovers();

    double Wshape = G->faces_weight();
    double Gshape = ( (G->numFaces + G->numLong) * log(constants->C_parameter) ) + Wshape;
    /*
    for (auto loop = G->Faces.faces.begin(); loop != G->Faces.faces.end(); ++loop){
        loop->calculate_weight(G->g);
        W+= loop->weight;
        Gshape+= log(constants->C_parameter / loop->weight);
    }*/
    //G->Faces.print();
    cout << "Total W = " << Wshape << endl;
    cout << "Gshape = " << Gshape << endl;

    double Wlast, Glast, Wlocal;
    for (auto cross = design->staple_pools[0].crossovers.begin();
         cross != design->staple_pools[0].crossovers.end(); ++cross){
        std::cout << "--------------------------------------------------------------- adding cr ";
        std::cout << cross->id << std::endl;
        Glast = Gshape;
        G->bind_domain(cross->domains.first);
        G->bind_domain(cross->domains.second);
        G->add_crossover(cross);
        Wshape = G->faces_weight();
        Gshape = ( (G->numFaces + G->numLong) * log(constants->C_parameter) ) + Wshape;
        Wlocal = G->shortest_path(cross);
        cout << "Gshape = " << Gshape << endl;
        cout << "Delta Gshape = " << Gshape - Glast << endl;
        cout << "Delta Glocal = " << log(constants->C_parameter / Wlocal) << endl;
    }

    for (auto cross = design->staple_pools[0].crossovers.begin();
         cross != design->staple_pools[0].crossovers.end(); ++cross){
        std::cout << "--------------------------------------------------------------- removing cr ";
        std::cout << cross->id << std::endl;
        Wlast = Wshape;
        Glast = Gshape;
        G->unbind_domain(cross->domains.first);
        G->unbind_domain(cross->domains.second);
        G->remove_crossover(cross);
        Wshape = G->faces_weight();
        Gshape = ( (G->numFaces + G->numLong) * log(constants->C_parameter) ) + Wshape;
        Wlocal = G->shortest_path(cross);
        cout << "Gshape = " << Gshape << endl;
        cout << "Delta Gshape = " << Gshape - Glast << endl;
        cout << "Delta Glocal = " << log(constants->C_parameter / Wlocal) << endl;
    }
}

void test_full(Simulation* sim){
    auto trManager = sim->trManager;
    MyGraph *G = trManager->G;

    print_embedding_storage(sim->G->g, sim->G->embedding_storage);

    Design* design = trManager->design;
    StaplePool* pool = &design->staple_pools[0];
    vector<Transition*> path, reverse_path;
    for (auto &st: pool->staples){
        for (auto &trans : st.transitions){
            if (st.num_domains == 1 && trans->initial_state == State_t(s0) && trans->final_state == State_t(s1))
                path.push_back(trans);
            if (st.num_domains == 2 && trans->initial_state == State_t(s00) && trans->final_state == State_t(s01))
                path.push_back(trans);
            if (st.num_domains == 3 && trans->initial_state == State_t(s000) && trans->final_state == State_t(s001))
                path.push_back(trans);
        }
    }

    for (auto &st: pool->staples){
        for (auto &trans : st.transitions){
            if (st.num_domains == 2 && trans->initial_state == State_t(s01) && trans->final_state == State_t(s11))
                path.push_back(trans);
            if (st.num_domains == 3 && st.id <= 0 && trans->initial_state == State_t(s001) && trans->final_state == State_t(s101))
                path.push_back(trans);
            if (st.num_domains == 3 && st.id > 0 && trans->initial_state == State_t(s001) && trans->final_state == State_t(s011))
                path.push_back(trans);
        }
    }
    for (auto &st: pool->staples){
        for (auto &trans : st.transitions){
            if (st.num_domains == 3 && st.id > 0 && trans->initial_state == State_t(s011) && trans->final_state == State_t(s111))
                path.push_back(trans);
        }
    }
    for (auto tr = path.rbegin(); tr!=path.rend(); ++tr){
        reverse_path.push_back(&(*((*tr)->reverse)));
    }

    std::cout << "--------------------------------------------------------------- Binding";
    if (!trManager->local){
        double Wshape, Gshape;
        Wshape = G->faces_weight();
        Gshape = ( (G->numFaces + G->numLong) * log(trManager->constants->C_parameter) ) + Wshape;
        std::cout << Wshape << "\t" << Gshape;
    }
    std::cout << std::endl;

    double k_bi = trManager->inputs->k_bi;
    double k_uni = trManager->inputs->k_uni;
    double RT = gas_constant * trManager->ramp->get_T();

    for (auto &tr : path){
        trManager->reset_possibles();
        trManager->next = trManager->transitions.begin() + tr->id;

        trManager->set_dG_duplex(trManager->next);
        trManager->set_dG_stack(trManager->next);
        trManager->set_dG_shape(trManager->next);
        //trManager->next->dG_shape = 0;

        if (tr->uni){
            if (tr->forward){
                //trManager->set_dG_shape(trManager->next);
                tr->dG = tr->dG_shape;
                tr->rate = k_uni * exp( -tr->dG / RT );
            }
            else{
                tr->dG = tr->dG_duplex+tr->dG_stack;
                tr->rate = k_uni * exp( tr->dG / RT );
            }
        }
        else{
            if (tr->forward){
                tr->rate = k_bi * tr->staple->concentration;
            }
            else{
                //if (!trManager->local){trManager->set_dG_shape(trManager->next);}
                tr->dG = tr->dG_duplex+tr->dG_stack-tr->dG_shape;
                tr->rate = k_uni * exp( tr->dG / RT );
            }
        }

        trManager->total_rate = 100;
        trManager->tau = 1/trManager->total_rate;
        trManager->next->tau = trManager->tau;

        std::cout << *trManager->next << "\t";
        std::cout << trManager->next->dG_duplex << "\t";
        std::cout << trManager->next->dG_stack << "\t";
        std::cout << trManager->next->dG_shape << "\t";
        if (trManager->next->properties.cross)
            std::cout << trManager->next->crossover.first->type;
        std::cout << std::endl;
        trManager->apply_next();
    }

    std::cout << "--------------------------------------------------------------- Unbinding";
    if (!trManager->local) {
        double Wshape, Gshape;
        Wshape = G->faces_weight();
        Gshape = ( (G->numFaces + G->numLong) * log(trManager->constants->C_parameter) ) + Wshape;
        std::cout << Wshape << "\t" << Gshape;
    }
    std::cout << std::endl;



    for (auto &tr : reverse_path){
        trManager->reset_possibles();
        trManager->next = trManager->transitions.begin() + tr->id;

        trManager->set_dG_duplex(trManager->next);
        trManager->set_dG_stack(trManager->next);
        trManager->set_dG_shape(trManager->next);
        //trManager->next->dG_shape = 0;

        if (tr->uni){
            if (tr->forward){
                //trManager->set_dG_shape(trManager->next);
                tr->dG = tr->dG_shape;
                tr->rate = k_uni * exp( -tr->dG / RT );
            }
            else{
                tr->dG = tr->dG_duplex+tr->dG_stack;
                tr->rate = k_uni * exp( tr->dG / RT );
            }
        }
        else{
            if (tr->forward){
                tr->rate = k_bi * tr->staple->concentration;
            }
            else{
                //if (!trManager->local){trManager->set_dG_shape(trManager->next);}
                tr->dG = tr->dG_duplex+tr->dG_stack-tr->dG_shape;
                tr->rate = k_uni * exp( tr->dG / RT );
            }
        }

        trManager->total_rate = 100;
        trManager->tau = 1/trManager->total_rate;
        trManager->next->tau = trManager->tau;

        std::cout << *trManager->next << "\t";
        std::cout << trManager->next->dG_duplex << "\t";
        std::cout << trManager->next->dG_stack << "\t";
        std::cout << trManager->next->dG_shape << "\t";
        if (trManager->next->properties.cross)
            std::cout << trManager->next->crossover.first->type;
        std::cout << std::endl;
        trManager->apply_next();
    }

    std::cout << "--------------------------------------------------------------- ";
    if (!trManager->local) {
        double Wshape, Gshape;
        Wshape = G->faces_weight();
        Gshape = ( (G->numFaces + G->numLong) * log(trManager->constants->C_parameter) ) + Wshape;
        std::cout << Wshape << "\t" << Gshape;
    }
    std::cout << std::endl;
}

void test_random(){
    int myidx;

    int inputrandom = 12341;
    base_generator_type generator(12341);
    typedef boost::variate_generator<base_generator_type&, uniform_int<> > UniformInt_gen;

    std::vector<int> myvec = {3,5,647,4352,7,4,2};
    uniform_int<> uniformInt(0, myvec.size()-1);
    UniformInt_gen uni(generator,uniformInt);

    for (int i = 0; i < 100; i++){
        myidx = uni();
        std::cout << i << "\t" << myidx << "\t" << myvec[myidx] << std::endl;
    }

    std::vector<int> myvec2 = {3,5,647,4352,7,4,2,6,12,65,2,1};
    uniform_int<> uniformInt2(0, myvec2.size()-1);
    UniformInt_gen uni2(generator,uniformInt2);

    for (int i = 0; i < 100; i++){
        myidx = uni2();
        std::cout << i << "\t" << myidx << "\t" << myvec2[myidx] << std::endl;
    }
}

void exact(Simulation* sim){
    ofstream outfile;
    open_trunc(outfile,"Exact.csv","");
    outfile << "step" << ",";
    outfile << "state" << ",";
    outfile << "numBoundDomains" << ",";
    outfile << "numStapleCopies" << ",";
    outfile << "numStack" << ",";
    outfile << "logsumC" << ",";
    outfile << "G_duplex" << ",";
    outfile << "G_stack" << ",";
    outfile << "G_shape";
    outfile << "\n";

    //Constants *constants = sim->constants;
    //TempRamp *ramp = sim->ramp;
    double T = sim->ramp->get_T();
    double gamma = sim->constants->gamma_parameter;
    double cParameter = sim->constants->C_parameter;
    double nParam = sim->constants->n_parameter;
    std::cout << gamma << "\t" << nParam << std::endl;
    Design *design = sim->design;
    StaplePool *pool = &(design->staple_pools[0]);

    vector<vector<State_t>> to_prod;
    for (auto &staple : pool->staples){
        to_prod.push_back(staple.possible_states);
    }
    vector<vector<State_t>> all_states = cartesian(to_prod);

    int i = 0;
    double G_duplex, G_stack, G_shape;
    double logsumC, Wshape;
    int numBoundDomains, numStack, numStapleCopies;
    for (auto &pstate : all_states){
        if (i%10000==0) {
            std::cout << i << std::endl;
            outfile << std::flush;
        }

        int stIdx = 0;
        numStapleCopies = 0;
        for (auto &state : pstate){
            if (state == s0 || state == s00){}
            else if (state == s1 || state == s01 || state == s10 || state == s11){numStapleCopies++;}
            else if (state == s12){numStapleCopies+=2;}
            else{std::cout << "state not recognised!" << std::endl;}
            ST staple = pool->staples.begin()+stIdx;
            staple->initialise_state(state);
            //design->change_state(pool->staples.begin()+stIdx,state);
            stIdx++;
        }
        MyGraph mygraph(design);
        //logsumC = mygraph.faces_weight();
        Wshape = mygraph.faces_weight();
        logsumC = ( (mygraph.numFaces + mygraph.numLong) * log(cParameter) ) + Wshape;
        G_shape = -(gas_constant * T * gamma ) * logsumC;
        G_duplex = 0;

        numBoundDomains = 0;
        numStack = 0;
        for (auto &domain : pool->domains){
            if (domain.state){
                G_duplex += domain.dH - T * domain.dS;
                numBoundDomains++;
                for (auto &stack : domain.stack_domains){
                    if (stack->state)
                        numStack++;
                }
            }
        }
        numStack /= 2;
        G_stack = numStack * nParam * (dH_average - T * dS_average);
        outfile << i << ",";
        outfile << pstate << ",";
        outfile << numBoundDomains << ",";
        outfile << numStapleCopies << ",";
        outfile << numStack << ",";
        outfile << logsumC << ",";
        outfile << G_duplex << ",";
        outfile << G_stack << ",";
        outfile << G_shape;
        outfile << "\n";
        i++;
    }
    std::cout << all_states.size() << std::endl;
    outfile.close();
}

void Er2_legacy(Simulation* sim){
    ofstream outfile;
    open_trunc(outfile,"Er2.csv","");

    Design *design = sim->design;
    StaplePool *pool = &(design->staple_pools[0]);

    for (auto &staple : pool->staples){
        outfile << "state" << staple.id << ",";
    }
    outfile << "Er2" << "," << "numfaces" << "," << "numbounddxpairs" << std::endl;


    vector<vector<State_t>> to_prod;
    for (auto &staple : pool->staples){
        if (staple.num_domains==1) {
            std::vector<State_t> possible_states = {s0,s1};
            to_prod.push_back(possible_states);
        }
        else if (staple.num_domains==2) {
            std::vector<State_t> possible_states = {s00,s10,s01,s11,s12};
            to_prod.push_back(possible_states);
        }
        else{
            std::cout << "Staple with more than 2 domains not supported.";
        }
    }
    vector<vector<State_t>> all_states = cartesian(to_prod);
    std::cout << all_states.size() << std::endl;


    int i = 0;
    double Wshape;
    int numBoundDomains, numStack, numStapleCopies;
    for (auto &pstate : all_states){
        if (i%10000==0) { std::cout << i << std::endl; }
        int stIdx = 0;

        for (auto &state : pstate){
            if (state == s0 || state == s1) {outfile << state << ",";}
            else if (state == s00) {outfile << "\"(0, 0)\"" << ",";}
            else if (state == s10) {outfile << "\"(1, 0)\"" << ",";}
            else if (state == s01) {outfile << "\"(0, 1)\"" << ",";}
            else if (state == s11) {outfile << "\"(1, 1)\"" << ",";}
            else if (state == s12) {outfile << "\"(1, 2)\"" << ",";}
            else{std::cout << "state not recognised!" << std::endl;}

            ST staple = pool->staples.begin()+stIdx;
            staple->initialise_state(state);
            //design->change_state(pool->staples.begin()+stIdx,state);
            stIdx++;
        }
        MyGraph mygraph(design);
        //logsumC = mygraph.faces_weight();
        Wshape = mygraph.faces_weight();
        outfile << Wshape << "," << mygraph.numFaces << "," << 0 << std::endl;
        if (i<100) std::cout << pstate << "\t" << mygraph.numFaces << "\t" << mygraph.embedding_storage << std::endl;

        i++;
    }
    outfile.close();
}

void set_shape_values(const vector<State_t> &pstate, const double &gamma, const double &cParameter,
                      Design *design, double &S_shape, int &numStapleCopies){
    StaplePool *pool = &(design->staple_pools[0]);
    int stIdx = 0;
    numStapleCopies = 0;
    vector <State_t> noCopies = {s0, s00, s000};
    vector <State_t> oneCopy = {s1, s10, s01, s11, s001, s010, s100, s011, s110, s101, s111};
    vector <State_t> twoCopies = {s12, s012, s120, s102, s112, s211, s121};
    vector <State_t> threeCopies = {s123};
    std::map <State_t,int> state2copies;
    for (auto state : noCopies) state2copies[state] = 0;
    for (auto state : oneCopy) state2copies[state] = 1;
    for (auto state : twoCopies) state2copies[state] = 2;
    for (auto state : threeCopies) state2copies[state] = 3;

    for (auto &state : pstate){
        numStapleCopies+=state2copies[state];
        design->change_state(pool->staples.begin()+stIdx,state);
        stIdx++;
    }
    MyGraph mygraph(design);

    double Wshape = mygraph.faces_weight();
    double logsumC = ( (mygraph.numFaces + mygraph.numLong) * log(cParameter) ) + Wshape;

    S_shape = gas_constant * gamma * logsumC;
}
void set_duplex_values(const vector<State_t> &pstate, const StaplePool* pool,
                       const double &nParam,
                       double &H_duplex, double &H_stack,
                       double &S_duplex, double &S_stack){
    H_duplex = H_stack = S_duplex = S_stack = 0;
    int numBoundDomains = 0;
    int numStack = 0;
    for (auto &domain : pool->domains){
        if (domain.state){
            H_duplex += domain.dH;
            S_duplex += domain.dS;
            numBoundDomains++;
            for (auto &stack : domain.stack_domains){
                if (stack->state)
                    numStack++;
            }
        }
    }
    numStack /= 2; //correct for double count
    H_stack = numStack * nParam * dH_average;
    S_stack = numStack * nParam * dS_average;
}
double calculate_Tm(Simulation* sim){
    //ofstream outfile;
    //open_trunc(outfile,"Exact.csv","");

    double gamma = sim->constants->gamma_parameter;
    double cParameter = sim->constants->C_parameter;
    double nParam = sim->constants->n_parameter;
    Design *design = sim->design;
    StaplePool *pool = &design->staple_pools[0];

    vector<State_t> empty;//(pool->num_staples);
    vector<State_t> target;//(pool->num_staples);
    for (auto &staple : pool->staples){
        empty.push_back(staple.possible_states[0]);
        size_t lastIdx = staple.possible_states.size()-1;
        target.push_back(staple.possible_states[lastIdx]);
    }

    std::cout << empty << std::endl;
    std::cout << target << std::endl;

    double H0_duplex, H0_stack, S0_duplex, S0_stack, S0_shape;
    double Hf_duplex, Hf_stack, Sf_duplex, Sf_stack, Sf_shape;
    int numStapleCopies0, numStapleCopiesf;

    set_shape_values(empty,gamma,cParameter,design,S0_shape,numStapleCopies0);
    set_duplex_values(empty,pool,nParam,H0_duplex,H0_stack,S0_duplex,S0_stack);
    set_shape_values(target,gamma,cParameter,design,Sf_shape,numStapleCopiesf);
    set_duplex_values(target,pool,nParam,Hf_duplex,Hf_stack,Sf_duplex,Sf_stack);

    double conc = 100;
    double mu0 = gas_constant * numStapleCopies0 * (log(conc) - 9*log(10));
    double muf = gas_constant * numStapleCopiesf * (log(conc) - 9*log(10));

    double H0 = H0_duplex + H0_stack;
    double S0 = S0_duplex + S0_stack + S0_shape + mu0;
    double Hf = Hf_duplex + Hf_stack;
    double Sf = Sf_duplex + Sf_stack + Sf_shape + muf;

    double Tm = (Hf-H0)/(Sf-S0);

    std::cout << H0 << ":( ";
    std::cout << H0_duplex << ", ";
    std::cout << H0_stack << " )\t";
    std::cout << S0 << ":( ";
    std::cout << S0_duplex << ", ";
    std::cout << S0_stack << ", ";
    std::cout << S0_shape << " )";
    std::cout << std::endl;

    std::cout << Hf << ":( ";
    std::cout << Hf_duplex << ", ";
    std::cout << Hf_stack << " )\t";
    std::cout << Sf << ":( ";
    std::cout << Sf_duplex << ", ";
    std::cout << Sf_stack << ", ";
    std::cout << Sf_shape << " )";
    std::cout << std::endl;

    std::cout << centigrade(Tm) << std::endl;

    return Tm;
}


int main(int argc, char * argv[]) {
    Inputs *inputs = new Inputs(argc,argv);
    Constants *constants = new Constants(inputs);
    Design *design = new Design(inputs);
    MyGraph *G = new MyGraph(design);
    TempRamp *ramp = new TempRamp(inputs);
    FileIO *ofiles = new FileIO(design);
    TransitionManager *trManager = new TransitionManager(constants, G, ramp, ofiles);
    StatManager *statManager = new StatManager(trManager);
    OPManager *opManager = new OPManager(statManager);
    Simulation *sim = new Simulation(opManager);
    if (inputs->test){
    }
    else if (inputs->exact){
        //double Tm = calculate_Tm(sim);
        exact(sim);
        //Er2_legacy(sim);
        sim->ofiles->close_files();
    }
    else if (inputs->config_generator){
        sim->run();
    }
    else if (inputs->anneal || inputs->melt || inputs->isothermal){
        sim->run();
    }
    else{
        printf ("Please select sim type: test, anneal, melt, isothermal, config_generator.\n");
        exit (EXIT_FAILURE);
    }
    delete inputs;
    delete ofiles;
    delete constants;
    delete design;
    delete G;
    delete ramp;
    delete trManager;
    delete sim;
    return 0;
}
