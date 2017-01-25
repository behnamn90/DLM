/*
 *
 *	Constants.h
 * 	Author: Behnam
 *
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "Headers.h"
#include "Common.h"

class Constants{
	public:
		Constants();
		Constants(double, double, double);
		Constants(double, double, double, int);
		
		void write_params(ofstream &); 

		void use_nanometer();
		void use_micrometer();
		void use_millimeter();
		void use_centimeter();
		void use_decimeter();
		void use_meter();

		void change_gamma(double);
		void change_n(double);
			
		double scale;


		double k_plus;

		double l_ds; // nm / bp
		double l_ss; // nm / nt
		double lambda_ss; // nm

		//int default_domain_size; 
		int default_crossover_size; 

		double gamma_parameter; 
		double C_parameter; 

		double n_parameter; 

		double conc_Mg; 
		double conc_Tris; 
		double salt_per_phosphate;
		double conc_staple; 

		double gas_constant; 

		//SantaLucia
		double dH_AA; 
		double dH_AT; 
		double dH_TA; 
		double dH_CA; 
		double dH_GT; 
		double dH_CT; 
		double dH_GA; 
		double dH_CG; 
		double dH_GC; 
		double dH_GG; 

		double dS_AA; 
		double dS_AT; 
		double dS_TA; 
		double dS_CA; 
		double dS_GT; 
		double dS_CT; 
		double dS_GA; 
		double dS_CG; 
		double dS_GC; 
		double dS_GG; 

		double dH_average; 
		double dS_average; 

		double dH_termAT; 
		double dS_termAT; 
		double dH_termCG; 
		double dS_termCG; 

		double dH_init; 
		double dS_init; 
};


#endif




// **** constants in dectameters
//const double l_ds = 0.34 * pow(10,-8); // nm / bp ---> dm / bp
//const double l_ss = 0.6 * pow(10,-8);// nm / nt ---> dm / nt
//const double lambda_ss = 1.8 * pow(10,-8); // nm ---> dm

//const double gamma_parameter = 2.5;
//const double C_parameter = 2.8 * pow(10,-16); //dm^2

/*
// **** constants in meters
const double l_ds = 0.34 * pow(10,-9); // nm / bp ---> m / bp
const double l_ss = 0.6 * pow(10,-9);// nm / nt ---> m / nt
const double lambda_ss = 1.8 * pow(10,-9); // nm ---> m

//const double gamma_parameter = 1.5;
//const double C_parameter = 6.7 * pow(10,-19); //m^2
const double gamma_parameter = 2.5;
const double C_parameter = 2.8 * pow(10,-18); //m^2


// **** constants in mol / m^3    [1M = 10^3 mol/m^3] ==> [1mM = 1mol/m^3]
const double k_plus = pow(10,6) * pow(10,-3); // /M /s -- > /mM/s
const double conc_Mg = 12.5; // mM 
const double conc_Tris = 40.; // mM
const double conc_staple = 20. * pow(10,-6); // nM ---> mM
*/
