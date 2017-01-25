/*
 *
 *	TempRamp.cpp
 * 	Author: Behnam
 *
 */

#include "TempRamp.h"


//General TempRamp Methods
TempRamp::TempRamp(){}
void TempRamp::_fill_anneal(){
	double Temp;
	for (Temp = T_high; Temp > T_low; Temp -=1){
		for (int i=0; i< cool_rate; i=i+1){
			_ramp.push_back(Temp);
		}
	}
}
void TempRamp::_fill_melt(){
	double Temp;
	for (Temp = T_low; Temp < T_high; Temp +=1){
		for (int i=0; i< cool_rate; i=i+1){
			_ramp.push_back(Temp);
		}
	}
}
void TempRamp::set_time(double current_t_){
	current_t = current_t_;
	if (current_t < t_max){ 
		_current_t = current_t;
		current_T = _ramp[_current_t];
	}
	else {
		cout << "Simulation time exceeded so cannot update the temperature in TempRamp.\n";
	} 
}
void TempRamp::move_time(double delta_t){
	current_t = current_t + delta_t;
	if (current_t < t_max){ 
		_current_t = current_t;
		current_T = _ramp[_current_t];
	}
	else {
		cout << "Simulation time exceeded so cannot update the temperature in TempRamp.\n";
	} 
}
void TempRamp::print_ramp(){
	print_vec(_ramp);
}
double TempRamp::get_T(){
	return current_T;
}
void TempRamp::set_temp(double current_T_){
	current_T = kelvin(current_T_);
}


//AnnealMelt Methods
AnnealMelt::AnnealMelt(double T_start_, double T_end_, double cool_rate_, int n_cycles_){
	current_t = 0.;
	_current_t = current_t;
	T_start = kelvin(T_start_);
	T_end = kelvin(T_end_);
	cool_rate = cool_rate_;
	n_cycles = n_cycles_;
	if (T_start < T_end) {anneal_first = false; T_high = T_end; T_low = T_start;}
	else {anneal_first = true; T_high = T_start; T_low = T_end;}
	t_max_in_each = abs(T_start-T_end) * cool_rate;
	t_max = 2. * n_cycles * t_max_in_each;
	_fill_ramp();
	current_T = _ramp[_current_t];
}
void AnnealMelt::_fill_ramp(){
	if (anneal_first){
		for (int i=0; i<n_cycles; i++){
			_fill_anneal();
			_fill_melt();	
		}
	}
	else {
		for (int i=0; i<n_cycles; i++){
			_fill_melt();	
			_fill_anneal();
		}
	}	
}


//Anneal Methods
Anneal::Anneal(double T_start_, double T_end_, double cool_rate_, int n_cycles_){
	current_t = 0.;
	_current_t = current_t;
	T_start = kelvin(T_start_);
	T_end = kelvin(T_end_);
	cool_rate = cool_rate_;
	n_cycles = n_cycles_;
	if (T_start < T_end) {anneal_first = false; T_high = T_end; T_low = T_start;}
	else {anneal_first = true; T_high = T_start; T_low = T_end;}
	t_max_in_each = abs(T_start-T_end) * cool_rate;
	t_max = n_cycles * t_max_in_each;
	_fill_ramp();
	current_T = _ramp[_current_t];
}
Anneal::Anneal(double T_start_, double T_end_, double cool_rate_){
	int n_cycles_ = 1;
	current_t = 0.;
	_current_t = current_t;
	T_start = kelvin(T_start_);
	T_end = kelvin(T_end_);
	cool_rate = cool_rate_;
	n_cycles = n_cycles_;
	if (T_start < T_end) {anneal_first = false; T_high = T_end; T_low = T_start;}
	else {anneal_first = true; T_high = T_start; T_low = T_end;}
	t_max_in_each = abs(T_start-T_end) * cool_rate;
	t_max = n_cycles * t_max_in_each;
	_fill_ramp();
	current_T = _ramp[_current_t];
}
void Anneal::_fill_ramp(){
	if (anneal_first){
		_fill_anneal();
	}
	else {
		cout << "Anneal ramp selected with wrong temperature gradient!" << endl;
	}	
}


//Melt Methods
Melt::Melt(double T_start_, double T_end_, double cool_rate_, int n_cycles_){
	current_t = 0.;
	_current_t = current_t;
	T_start = kelvin(T_start_);
	T_end = kelvin(T_end_);
	cool_rate = cool_rate_;
	n_cycles = n_cycles_;
	if (T_start < T_end) {anneal_first = false; T_high = T_end; T_low = T_start;}
	else {anneal_first = true; T_high = T_start; T_low = T_end;}
	t_max_in_each = abs(T_start-T_end) * cool_rate;
	t_max = n_cycles * t_max_in_each;
	_fill_ramp();
	current_T = _ramp[_current_t];
}
Melt::Melt(double T_start_, double T_end_, double cool_rate_){
	int n_cycles_ = 1;
	current_t = 0.;
	_current_t = current_t;
	T_start = kelvin(T_start_);
	T_end = kelvin(T_end_);
	cool_rate = cool_rate_;
	n_cycles = n_cycles_;
	if (T_start < T_end) {anneal_first = false; T_high = T_end; T_low = T_start;}
	else {anneal_first = true; T_high = T_start; T_low = T_end;}
	t_max_in_each = abs(T_start-T_end) * cool_rate;
	t_max = n_cycles * t_max_in_each;
	_fill_ramp();
	current_T = _ramp[_current_t];
}
void Melt::_fill_ramp(){
	if (anneal_first){
		cout << "Melt ramp selected with wrong temperature gradient!" << endl;
	}
	else {
		_fill_melt();
	}	
}


