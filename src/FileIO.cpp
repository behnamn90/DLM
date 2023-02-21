//
//  FileIO.cpp
//  DLM
//
//  Created by Behnam Najafi on 19/03/2019.
//  Copyright © 2019 Behnam Najafi. All rights reserved.
//

#include "FileIO.h"


void open_trunc(ofstream& file, const string& name, const string& precursor){
    string str = "Output/"; str+=precursor; str+= name;
    file.open(str, std::ofstream::out | std::ofstream::trunc);
}
void retruncate(ofstream& file, const string& name, const string& precursor){
    file.close();
    string str = "Output/"; str+=precursor; str+= name;
    file.open(str, std::ofstream::out | std::ofstream::trunc);
}

FileIO::FileIO(Design* design_) : design(design_){
    this->inputs = design->inputs;
    if (!inputs->test){
        this->create_dirs();
        if (!inputs->exact)
            this->open_files();
    }
}
void FileIO::create_dirs(){
    struct stat st;
    string idummy, dummy;
    string command = "mv Output ";
    if(stat("Output",&st) == 0){
        for (int i = 1; i<100; i++){
            idummy = to_string(i);
            dummy = "Output" + idummy;
            if(!(stat(dummy.c_str(),&st) == 0)){
                break;
            }
        }
        command += dummy;
        std::system(command.c_str());
        cout << "Renamed old Output to " << dummy << "\n";
    }
    std::system("mkdir Output");
    if (inputs->make_movie) {std::system("mkdir Output/Movie");}
    std::system("cp input.txt Output/");
}


void FileIO::retrunc_hist_files(){
    retruncate(hist_file, inputs->hist_file_name);
    //retruncate(hist_file_2D, inputs->hist_file_name, "2D");
    //retruncate(traj_hist_file, inputs->traj_hist_file_name);
    retruncate(sdom_hist_file, inputs->domain_hist_file_name);
    for (const auto& pool : design->staple_pools){
        retruncate(pdom_hist_files[pool.id], inputs->domain_hist_file_name, pool.name+"_");
        retruncate(cr_hist_files[pool.id], inputs->cross_hist_file_name, pool.name+"_");
        retruncate(crp_hist_files[pool.id], inputs->crosspair_hist_file_name, pool.name+"_");
        retruncate(st_hist_files[pool.id], inputs->staple_hist_file_name, pool.name+"_");
        retruncate(hlx_hist_files[pool.id], inputs->helix_hist_file_name, pool.name+"_");
    }
}

void FileIO::open_files(){
    open_trunc(transition_file, inputs->transition_file_name);
    
    open_trunc(trajectory_file, inputs->trajectory_file_name);
    open_trunc(last_conf_file, inputs->last_conf_file_name);
    open_trunc(simple_traj_file, inputs->simple_trajectory_file_name);
    
    open_trunc(stats_file, inputs->statistics_file_name);
    
    if (!inputs->isothermal){
        open_trunc(sdom_stats_file, inputs->domain_stats_file_name);
        ofstream a;
        for (const auto& pool : design->staple_pools){
            pdom_stats_files.push_back(std::move(a));
            cr_stats_files.push_back(std::move(a));
            crp_stats_files.push_back(std::move(a));
            st_stats_files.push_back(std::move(a));
            hlx_stats_files.push_back(std::move(a));
            open_trunc(pdom_stats_files[pool.id], inputs->domain_stats_file_name, pool.name+"_");
            open_trunc(cr_stats_files[pool.id], inputs->cross_stats_file_name, pool.name+"_");
            open_trunc(crp_stats_files[pool.id], inputs->crosspair_stats_file_name, pool.name+"_");
            open_trunc(st_stats_files[pool.id], inputs->staple_stats_file_name, pool.name+"_");
            open_trunc(hlx_stats_files[pool.id], inputs->helix_stats_file_name, pool.name+"_");
        }
    }
    if (inputs->isothermal){
        open_trunc(hist_file, inputs->hist_file_name);
        open_trunc(burn_file, inputs->burn_file_name);
        //open_trunc(hist_file_2D, inputs->hist_file_name, "2D");
        open_trunc(traj_hist_file, inputs->traj_hist_file_name);
        open_trunc(sdom_hist_file, inputs->domain_hist_file_name);
        ofstream a;
        for (const auto& pool : design->staple_pools){
            pdom_hist_files.push_back(std::move(a));
            cr_hist_files.push_back(std::move(a));
            crp_hist_files.push_back(std::move(a));
            st_hist_files.push_back(std::move(a));
            hlx_hist_files.push_back(std::move(a));
            open_trunc(pdom_hist_files[pool.id], inputs->domain_hist_file_name, pool.name+"_");
            open_trunc(cr_hist_files[pool.id], inputs->cross_hist_file_name, pool.name+"_");
            open_trunc(crp_hist_files[pool.id], inputs->crosspair_hist_file_name, pool.name+"_");
            open_trunc(st_hist_files[pool.id], inputs->staple_hist_file_name, pool.name+"_");
            open_trunc(hlx_hist_files[pool.id], inputs->helix_hist_file_name, pool.name+"_");
        }
    }
    
    open_trunc(times_file, "In_times.dat");
    
    open_trunc(info_file, inputs->info_file_name);
}


void FileIO::close_files(){
    transition_file.close();
    
    trajectory_file.close();
    last_conf_file.close();
    simple_traj_file.close();
    
    stats_file.close();
    sdom_stats_file.close();
    if (!inputs->isothermal){
        for (auto& file : pdom_stats_files) {file.close();}
        for (auto& file : cr_stats_files) {file.close();}
        for (auto& file : crp_stats_files) {file.close();}
        for (auto& file : st_stats_files) {file.close();}
        for (auto& file : hlx_stats_files) {file.close();}
    }
    if (inputs->isothermal){
        hist_file.close();
        burn_file.close();
        traj_hist_file.close();
        //for (auto& file : hist_files_2D) {file.close();}
        for (auto& file : pdom_hist_files) {file.close();}
        for (auto& file : cr_hist_files) {file.close();}
        for (auto& file : crp_hist_files) {file.close();}
        for (auto& file : st_hist_files) {file.close();}
        for (auto& file : hlx_hist_files) {file.close();}
    }
    
    times_file.close();
    info_file.close();
}

/*
void FileIO::open_2D_file(const string& name){
    ofstream a;
    hist_files_2D.push_back(a);
    open_trunc(*(hist_files_2D.end()-1), name, "2D_");
}
*/
