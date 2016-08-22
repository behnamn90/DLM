const double k_plus = pow(10,6); // /M /s

const double l_ds = 0.34; // nm / bp
const double l_ss = 0.6; // nm / nt
const double lambda_ss = 1.8; // nm

const int default_domain_size = 3; //should be 16
const int default_crossover_size = 1; //should be 0
const double default_temperature = 360.; // K

//const double gamma_parameter = 1.5;
//const double C_parameter = 6.7 * pow(10,-19) * pow(10,18); //m^2 ---> nm^2
const double gamma_parameter = 2.5;
const double C_parameter = 2.8 * pow(10,-18) * pow(10,18); //m^2 ---> nm^2
//const double gamma_parameter = 3.5;
//const double C_parameter = 5.2 * pow(10,-18) * pow(10,18); //m^2 ---> nm^2

const double n_parameter = 2.0;

const double conc_Mg = 12.5 * 0.001; // mM ---> M
const double conc_Tris = 40. * 0.001; // mM ---> M
const double conc_staple = 20. * pow(10,-9); // nM ---> M

const double gas_constant = 0.00198720358509; // kcal / mol

const double T_max = 100. + 273.15; // K
const double T_min = 40. + 273.15; // K
const double cool_rate = 60.; // seconds/K   ***should be 60.
const double t_max = (T_max-T_min) * cool_rate; // seconds

//SantaLucia
const double dH_AA = -7.6;	// kcal / mol
const double dH_AT = -7.2;
const double dH_TA = -7.2;
const double dH_CA = -8.5;
const double dH_GT = -8.4;
const double dH_CT = -7.8;
const double dH_GA = -8.2;
const double dH_CG = -10.6; 
const double dH_GC = -9.8;
const double dH_GG = -8.0;

const double dS_AA = -21.3 * 0.001; // (cal/K) / mol  ---> (kcal/K) / mol
const double dS_AT = -20.4 * 0.001;
const double dS_TA = -21.3 * 0.001;
const double dS_CA = -22.7 * 0.001;
const double dS_GT = -22.4 * 0.001;
const double dS_CT = -21.0 * 0.001;
const double dS_GA = -22.2 * 0.001;
const double dS_CG = -27.2 * 0.001;
const double dS_GC = -24.2 * 0.001;
const double dS_GG = -19.9 * 0.001;

const double dH_average = ( 2.*(dH_AA + dH_CA + dH_GT + dH_CT + dH_GA + dH_GG) + (dH_AT + dH_TA + dH_CG + dH_GC) ) / 16.;
const double dS_average = ( 2.*(dS_AA + dS_CA + dS_GT + dS_CT + dS_GA + dS_GG) + (dS_AT + dS_TA + dS_CG + dS_GC) ) / 16.;

const double dH_termAT = 2.2;
const double dS_termAT = 6.9 * 0.001;
const double dH_termCG = 0.0;
const double dS_termCG = 0.0;

const double dH_init = 0.2;
const double dS_init = -5.7 * 0.001;


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








