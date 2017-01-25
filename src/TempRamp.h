/*
 *
 *	TempRamp.h
 * 	Author: Behnam
 *
 */

#ifndef TEMPRAMP_H_
#define TEMPRAMP_H_

#include "Headers.h"
#include "Common.h"

class TempRamp{
	public:
		int _current_t;
		double T_high;
		double T_low;
		vector<double> _ramp;
		void _fill_anneal();
		void _fill_melt();
		TempRamp();
		//TempRamp(double T_start_, double T_end_, double cool_rate_, int n_cycles_);
		
		bool anneal_first;
		double T_start;
		double T_end;
		double cool_rate; // seconds/K   ***should be 60.
		int n_cycles;
		double t_max; // seconds
		double t_max_in_each;

		double current_t;
		double current_T;
		
		void print_ramp();
		void set_temp(double);

		void set_time(double);
		void move_time(double);
		double get_T();
};

class AnnealMelt: public TempRamp{
	public:
		AnnealMelt(double, double, double, int);
		void _fill_ramp();
};

class Anneal: public TempRamp{
	public:
		Anneal(double, double, double);
		Anneal(double, double, double, int);
		void _fill_ramp();
};

class Melt: public TempRamp{
	public:
		Melt(double, double, double);
		Melt(double, double, double, int);
		void _fill_ramp();
};


#endif












