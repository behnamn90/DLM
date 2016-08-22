#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>


vector< vector<double> > Gillespie(const int n_reactions, const int A0[], const int A0_size, const double k[], const int k_size, int seed_iter){
	typedef boost::minstd_rand base_generator_type;
	base_generator_type generator(seed_iter);
	boost::uniform_real<> uni_dist(0,1);
	boost::variate_generator<base_generator_type&, boost::uniform_real<> > uni(generator, uni_dist);

	const double t0 = 0.;
	double t = t0;
	int A[A0_size]; copy_vec(A,A0,A0_size);  
    double r1, r2;
	vector< vector<double> > result;
	double p[k_size];
	double p_tot;
	double tau;
	
	for (int n=0; n<100; n++){
		vector<double> row(1+A0_size);
		row[0] = t; for (int i=1; i<row.size(); i++) {row[i]=A[i-1];}
		result.push_back(row);
		r1 = uni();
		r2 = uni();
		p[0] = A[0]*(A[0]-1)*k[0]; p[1] = A[0]*A[1]*k[1]; p[2] = k[2]; p[3] = k[3]; p_tot = p[0]+p[1]+p[2]+p[3];
		tau = (1./(p_tot))*log(1./r1);
		t = t + tau;
		if ( r2 >= 0 && r2 < p[0]/p_tot){A[0]=A[0]-2;A[1]=A[1];}
		else if ( r2 >= p[0]/p_tot && r2 < (p[0]+p[1])/p_tot){A[0]=A[0]-1;A[1]=A[1]-1;}
		else if ( r2 >= (p[0]+p[1])/p_tot && r2 < (p[0]+p[1]+p[2])/p_tot){A[0]=A[0]+1;A[1]=A[1];}
		else if ( r2 >= (p[0]+p[1]+p[2])/p_tot && r2 < (p[0]+p[1]+p[2]+p[3])/p_tot){A[0]=A[0]; A[1]=A[1]+1;}
		else cout << "Behnam error\n";
	}
	return result;
}
