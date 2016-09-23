class Constants{
	public:
		Constants();
		Constants(double y_, double n_, double conc_);
		Constants(double y_, double n_, double conc_, int scale_);
		
		void write_params(ofstream &file_); 

		void use_nanometer();
		void use_micrometer();
		void use_millimeter();
		void use_centimeter();
		void use_decimeter();
		void use_meter();

		void change_gamma(double gamma_);
		void change_n(double n_);
			
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

void Constants::use_nanometer(){
	scale = 1000000000.;
}
void Constants::use_micrometer(){
	scale = 1000000.;
}
void Constants::use_millimeter(){
	scale = 1000.;
}
void Constants::use_centimeter(){
	scale = 100.;
}
void Constants::use_decimeter(){
	scale = 10.;
}
void Constants::use_meter(){
	scale = 1.;
}

void Constants::change_gamma(double gamma_){
	if (abs(gamma_-1.5) < 0.000000000000001){
		gamma_parameter = 1.5;
		C_parameter = 6.7 * pow(10,-19) * scale*scale;
	}
	else if (abs(gamma_-2.5) < 0.000000000000001){
		gamma_parameter = 2.5;
		C_parameter = 2.8 * pow(10,-18) * scale*scale;
	}
	else if (abs(gamma_-3.5) < 0.00000000000001){
		gamma_parameter = 3.5;
		C_parameter = 5.2 * pow(10,-18) * scale*scale;
	}
	else {printf ("Error! Value of gamma not supported.\n"); exit (EXIT_FAILURE);}
}
void Constants::change_n(double n_){
	n_parameter = n_;
}

void Constants::write_params(ofstream &file_) {
	file_ << "y= " << gamma_parameter << "\nn= " << n_parameter << "\nscale= " << scale << "\nconc= " << conc_staple << "\nsalt= " << salt_per_phosphate << endl;
}

Constants::Constants () {
	use_decimeter();
	//meter
	//10^6 M-1 s-1 = 10^6 (mol/L)-1 s-1 = 10^6 (mol/0.001m3)-1 s-1 = 10^6 0.001 m3 mol-1 s-1
	//10^6 M-1 s-1 = 10^6 (1000 mM)-1 s-1 = 10^6 0.001 mM-1 s-1
	k_plus = pow(10,3) * scale*scale*scale; // mol-1  m3  s-1

	// 0.34nm , 0.6nm, 1.8nm
	l_ds = 0.34 * pow(10,-9) * scale; // m  bp-1
	l_ss = 0.6 * pow(10,-9) * scale; // m  nt-1
	lambda_ss = 1.8 * pow(10,-9) * scale; //  m

	gamma_parameter = 1.5;
	C_parameter = 6.7 * pow(10,-19) * scale*scale;

	salt_per_phosphate = -0.944;
	//12.5 mM = 12.5 (10-3) mol L-1 = 12.5 (10-3) mol (0.001m3)-1 = 12.5 mol m-3
	conc_Mg = 12.5 * (1./(scale*scale*scale)); // mol  m-3
	//40 mM = 40 mol m-3
	conc_Tris = 40. * (1./(scale*scale*scale)); // mol  m-3
	//20 nM = 20 * 10^6 mM = 20 * 10^6 mol m-3
	conc_staple = 20. * pow(10,-9) * pow(10,3) * (1./(scale*scale*scale)); // mol m-3
	
	n_parameter = 2.0;

	gas_constant = 0.00198720358509; // kcal / mol
	default_crossover_size = 1; //should be 0

	//SantaLucia
	dH_AA = -7.6;	// kcal / mol
	dH_AT = -7.2;
	dH_TA = -7.2;
	dH_CA = -8.5;
	dH_GT = -8.4;
	dH_CT = -7.8;
	dH_GA = -8.2;
	dH_CG = -10.6; 
	dH_GC = -9.8;
	dH_GG = -8.0;

	dS_AA = -21.3 * 0.001; // (cal/K) / mol  ---> (kcal/K) / mol
	dS_AT = -20.4 * 0.001;
	dS_TA = -21.3 * 0.001;
	dS_CA = -22.7 * 0.001;
	dS_GT = -22.4 * 0.001;
	dS_CT = -21.0 * 0.001;
	dS_GA = -22.2 * 0.001;
	dS_CG = -27.2 * 0.001;
	dS_GC = -24.2 * 0.001;
	dS_GG = -19.9 * 0.001;

	dH_average = ( 2.*(dH_AA + dH_CA + dH_GT + dH_CT + dH_GA + dH_GG) + (dH_AT + dH_TA + dH_CG + dH_GC) ) / 16.;
	dS_average = ( 2.*(dS_AA + dS_CA + dS_GT + dS_CT + dS_GA + dS_GG) + (dS_AT + dS_TA + dS_CG + dS_GC) ) / 16.;

	dH_termAT = 2.2;
	dS_termAT = 6.9 * 0.001;
	dH_termCG = 0.0;
	dS_termCG = 0.0 * 0.001;

	dH_init = 0.2;
	dS_init = -5.7 * 0.001;
}

Constants::Constants (double y_, double n_, double conc_) { //conc_ in nM
	use_millimeter();
	//meter
	//10^6 M-1 s-1 = 10^6 (mol/L)-1 s-1 = 10^6 (mol/0.001m3)-1 s-1 = 10^6 0.001 m3 mol-1 s-1
	//10^6 M-1 s-1 = 10^6 (1000 mM)-1 s-1 = 10^6 0.001 mM-1 s-1
	k_plus = pow(10,3) * scale*scale*scale; // mol-1  m3  s-1

	// 0.34nm , 0.6nm, 1.8nm
	l_ds = 0.34 * pow(10,-9) * scale; // m  bp-1
	l_ss = 0.6 * pow(10,-9) * scale; // m  nt-1
	lambda_ss = 1.8 * pow(10,-9) * scale; //  m

	change_gamma(y_);

	salt_per_phosphate = -0.944;
	//12.5 mM = 12.5 (10-3) mol L-1 = 12.5 (10-3) mol (0.001m3)-1 = 12.5 mol m-3
	conc_Mg = 12.5 * (1./(scale*scale*scale)); // mol  m-3
	//40 mM = 40 mol m-3
	conc_Tris = 40. * (1./(scale*scale*scale)); // mol  m-3
	//20 nM = 20 * 10^6 mM = 20 * 10^6 mol m-3
	conc_staple = conc_ * pow(10,-9) * pow(10,3) * (1./(scale*scale*scale)); // mol m-3
	
	n_parameter = n_;

	gas_constant = 0.00198720358509; // kcal / mol
	default_crossover_size = 1; //should be 0

	//SantaLucia
	dH_AA = -7.6;	// kcal / mol
	dH_AT = -7.2;
	dH_TA = -7.2;
	dH_CA = -8.5;
	dH_GT = -8.4;
	dH_CT = -7.8;
	dH_GA = -8.2;
	dH_CG = -10.6; 
	dH_GC = -9.8;
	dH_GG = -8.0;

	dS_AA = -21.3 * 0.001; // (cal/K) / mol  ---> (kcal/K) / mol
	dS_AT = -20.4 * 0.001;
	dS_TA = -21.3 * 0.001;
	dS_CA = -22.7 * 0.001;
	dS_GT = -22.4 * 0.001;
	dS_CT = -21.0 * 0.001;
	dS_GA = -22.2 * 0.001;
	dS_CG = -27.2 * 0.001;
	dS_GC = -24.2 * 0.001;
	dS_GG = -19.9 * 0.001;

	dH_average = ( 2.*(dH_AA + dH_CA + dH_GT + dH_CT + dH_GA + dH_GG) + (dH_AT + dH_TA + dH_CG + dH_GC) ) / 16.;
	dS_average = ( 2.*(dS_AA + dS_CA + dS_GT + dS_CT + dS_GA + dS_GG) + (dS_AT + dS_TA + dS_CG + dS_GC) ) / 16.;

	dH_termAT = 2.2;
	dS_termAT = 6.9 * 0.001;
	dH_termCG = 0.0;
	dS_termCG = 0.0 * 0.001;

	dH_init = 0.2;
	dS_init = -5.7 * 0.001;
}

Constants::Constants (double y_, double n_, double conc_, int scale_) { //conc_ in nM
	scale = pow(10,scale_);
	//meter
	//10^6 M-1 s-1 = 10^6 (mol/L)-1 s-1 = 10^6 (mol/0.001m3)-1 s-1 = 10^6 0.001 m3 mol-1 s-1
	//10^6 M-1 s-1 = 10^6 (1000 mM)-1 s-1 = 10^6 0.001 mM-1 s-1
	k_plus = pow(10,3) * scale*scale*scale; // mol-1  m3  s-1

	// 0.34nm , 0.6nm, 1.8nm
	l_ds = 0.34 * pow(10,-9) * scale; // m  bp-1
	l_ss = 0.6 * pow(10,-9) * scale; // m  nt-1
	lambda_ss = 1.8 * pow(10,-9) * scale; //  m

	change_gamma(y_);

	//salt_per_phosphate = -0.944;
	salt_per_phosphate = 0.;
	//12.5 mM = 12.5 (10-3) mol L-1 = 12.5 (10-3) mol (0.001m3)-1 = 12.5 mol m-3
	conc_Mg = 12.5 * (1./(scale*scale*scale)); // mol  m-3
	//40 mM = 40 mol m-3
	conc_Tris = 40. * (1./(scale*scale*scale)); // mol  m-3
	//20 nM = 20 * 10^6 mM = 20 * 10^6 mol m-3
	conc_staple = conc_ * pow(10,-9) * pow(10,3) * (1./(scale*scale*scale)); // mol m-3
	
	n_parameter = n_;

	gas_constant = 0.00198720358509; // kcal / mol
	default_crossover_size = 1; //should be 0

	//SantaLucia
	dH_AA = -7.6;	// kcal / mol
	dH_AT = -7.2;
	dH_TA = -7.2;
	dH_CA = -8.5;
	dH_GT = -8.4;
	dH_CT = -7.8;
	dH_GA = -8.2;
	dH_CG = -10.6; 
	dH_GC = -9.8;
	dH_GG = -8.0;

	dS_AA = -21.3 * 0.001; // (cal/K) / mol  ---> (kcal/K) / mol
	dS_AT = -20.4 * 0.001;
	dS_TA = -21.3 * 0.001;
	dS_CA = -22.7 * 0.001;
	dS_GT = -22.4 * 0.001;
	dS_CT = -21.0 * 0.001;
	dS_GA = -22.2 * 0.001;
	dS_CG = -27.2 * 0.001;
	dS_GC = -24.2 * 0.001;
	dS_GG = -19.9 * 0.001;

	dH_average = ( 2.*(dH_AA + dH_CA + dH_GT + dH_CT + dH_GA + dH_GG) + (dH_AT + dH_TA + dH_CG + dH_GC) ) / 16.;
	dS_average = ( 2.*(dS_AA + dS_CA + dS_GT + dS_CT + dS_GA + dS_GG) + (dS_AT + dS_TA + dS_CG + dS_GC) ) / 16.;

	dH_termAT = 2.2;
	dS_termAT = 6.9 * 0.001;
	dH_termCG = 0.0;
	dS_termCG = 0.0 * 0.001;

	dH_init = 0.2;
	dS_init = -5.7 * 0.001;
}
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








