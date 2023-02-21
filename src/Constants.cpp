/*
 *
 *	Constants.cpp
 * 	Author: Behnam
 *
 */

#include "Constants.h"


void Constants::change_gamma(double gamma_){
	if (abs(gamma_-1.5) < EPSILON){
		gamma_parameter = 1.5;
        C_parameter = 6.7 * pow(10,-19);
        //C_parameter = 7.013391991073558 * pow(10,-20);
	}
	else if (abs(gamma_-2.5) < EPSILON){
		gamma_parameter = 2.5;
        C_parameter = 2.8 * pow(10,-18);
        //C_parameter = 6.7 * pow(10,-19);
        //C_parameter = 7.013391991073558 * pow(10,-20);
	}
	else if (abs(gamma_-3.5) < EPSILON){
		gamma_parameter = 3.5;
		C_parameter = 5.2 * pow(10,-18);
	}
	else {printf ("Error! Value of gamma not supported.\n"); exit (EXIT_FAILURE);}
}

void Constants::write_params(ofstream &file_) {
	file_ << "y= " << gamma_parameter << "\n= " << n_parameter << endl;
}

Constants::Constants () { //conc_ in nM
	change_gamma(2.5);
	n_parameter = 2.;
}

Constants::Constants (Inputs* inputs_) : inputs(inputs_) { //conc_ in nM
    gamma_parameter = inputs->gamma;
    n_parameter = inputs->n_param;
    change_gamma(gamma_parameter);
}

Constants::~Constants(){}

