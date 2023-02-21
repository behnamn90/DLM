//
//  FileIO.hpp
//  DLM
//
//  Created by Behnam Najafi on 19/03/2019.
//  Copyright © 2019 Behnam Najafi. All rights reserved.
//

#ifndef FILEIO_h
#define FILEIO_h

#include "Design.h"

void open_trunc(ofstream&, const string&, const string& precursor= "");
void retruncate(ofstream&, const string&, const string& precursor= "");

class FileIO{
public:
    FileIO(){}
    FileIO(Design*);
    virtual ~FileIO(){}
    
    Design* design;
    Inputs* inputs;
    
    void create_dirs();
    
    ofstream transition_file;
    
    ofstream trajectory_file;
    ofstream last_conf_file;
    ofstream simple_traj_file;
    
    ofstream stats_file;
    ofstream sdom_stats_file;
    
    vector<ofstream> pdom_stats_files;
    vector<ofstream> cr_stats_files;
    vector<ofstream> crp_stats_files;
    vector<ofstream> st_stats_files;
    vector<ofstream> hlx_stats_files;
    
    ofstream hist_file;
    ofstream burn_file;
    vector<ofstream> hist_files_2D;
    ofstream traj_hist_file;
    ofstream sdom_hist_file;

    vector<ofstream> pdom_hist_files;
    vector<ofstream> cr_hist_files;
    vector<ofstream> crp_hist_files;
    vector<ofstream> st_hist_files;
    vector<ofstream> hlx_hist_files;
    
    ofstream times_file;
    
    ofstream info_file;
    
    string str;
    
    void open_files();
    void retrunc_hist_files();
    
    //void open_2D_file(const string&);
    //void retrunc_2D_file(const string&);
    
    void close_files();
};





#endif
