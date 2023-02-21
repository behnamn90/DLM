/*
 *
 *	TempRamp.h
 * 	Author: Behnam
 *
 */

#ifndef TEMPRAMP_H_
#define TEMPRAMP_H_

#include <ostream>
#include "Input.h"


class T_Window{
public:
    T_Window();
    T_Window(double);
    T_Window(double, pair<double,double>);
    static int count;
    int idx;
    double T;
    string str_T;
    int num_rejected = 0;
    vector<int> num_tr; //for normal and exchange.
    pair<double,double> t_limits;
};



class TempRamp{
private:
    double iso_T;   // Kelvin
    double T_high;  // Kelvin
    double T_low;   // Kelvin
    double dT;      // Kelvin
    double cool_rate; // Seconds/K


public:
    TempRamp();
    TempRamp(Inputs*);
    virtual ~TempRamp(){}
    
    Inputs * inputs;
    
    bool anneal;
    bool melt;
    bool isothermal;

    double dt; // Seconds

    double t_max; // seconds
    double current_t;   // Seconds
    double previous_t;

    //TempJump T_jump;
    bool T_was_changed;
    
    vector<T_Window> Temps;
    int prev_idx;
    int idx;

    friend ostream &operator<<(ostream &os, const TempRamp &ramp);

    void correct_overflow(double &tau);
    void initialise();
    void move_time(const double&);
    double get_T();
    void print();
};

/*
class TempRamp{
    
    int _current_t; // Seconds;
    vector<double> t2T; // time[s]->Temperature[K]

    // Note:
    // could use uniqueTs[t2index[t_]]
    // instead of t2T[t_]
 
    
    double iso_T;   // Kelvin
    double T_high;  // Kelvin
    double T_low;   // Kelvin
    
    double cool_rate; // Seconds/K
    int n_cycles;
    
    void _fill_ramp();
    void _fill_anneal();
    void _fill_melt();
    void _fill_iso();
    
public:
    TempRamp();
    TempRamp(Inputs*);
    virtual ~TempRamp(){}
    
    Inputs * inputs;
    
    bool anneal;
    bool melt;
	bool isothermal;
    
    double t_max; // seconds
    double current_t;   // Seconds
    double previous_t;
    double current_T;   // Kelvin
    double previous_T;  // Kelvin
    bool T_was_changed;
    
    vector<int> t2index; // time -> index of the temperature in unique_Ts
    // For use in StatManager
    vector<double> unique_Ts;   // Kelvin
    vector<string> str_unique_Ts;   // Centigrade
    string str_current_T;   //Centigrade
    string str_previous_T;  //Centigrade
    int current_index;
    int previous_index;
    vector<int> num_rejected;
    
    void print_ramp();
    void set_temp(double);

    void initialise_time();
    void move_time(double);
    double get_T();
};
*/

#endif
