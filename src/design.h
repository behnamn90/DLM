class Domain {
	public:
		int source; int target;
		vector<int> cross_pairs;
		vector<int> stack_pairs;
		int length;
		string seq;
};
class Crossover {
	public:
		int v_a; int v_b;
		int d_a; int d_b;
		int d_a1; int d_a2; int d_b1; int d_b2;
		string type;  //i: inside, o: outside, m: seam
};

class Design {
	public:
		int n_vertices;
		int n_domains;
		vector<Domain> domains;
		vector<Crossover> crossovers;
		
		void fill_domains_Frits();
		void fill_crossovers_Frits();	
		Design();
		void print_domains();
		void print_crossovers();

		int domain_to_crossover(int d);
		vector<int> vertex_to_crossover(int v);
		int vpair_to_crossover(pair<int,int> v_pair);
};

void Design::fill_domains_Frits(){
	Domain d; d.length = default_domain_size; 
	for(int i=0; i<n_vertices-1; i++){
		d.source = i; d.target = i+1;
		domains.push_back(d);
	}
	d.source = n_vertices-1; d.target = 0;
	domains.push_back(d);
	domains[82].length *= 2; 
	domains[165].length *= 2; 
	int d1, d2;
	d1 = 1;
	d2 = 10;
	domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
	domains[d1].stack_pairs.push_back(d1-1); domains[d1].stack_pairs.push_back(d1+1);
	domains[d2].stack_pairs.push_back(d2-1); domains[d2].stack_pairs.push_back(d2+1);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 += 2;
			d2 -= 2;
			domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
			domains[d1].stack_pairs.push_back(d1-1); domains[d1].stack_pairs.push_back(d1+1);
			domains[d2].stack_pairs.push_back(d2-1); domains[d2].stack_pairs.push_back(d2+1);
		}
		if (L%2 != 0) {d1 += 2; d2 += 12;}
		else {d2 += 14;}	
	}
	d1 = 84;
	d2 = 93; 
	domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
	domains[d1].stack_pairs.push_back(d1-1); domains[d1].stack_pairs.push_back(d1+1);
	domains[d2].stack_pairs.push_back(d2-1); domains[d2].stack_pairs.push_back(d2+1);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 += 2;
			d2 -= 2;
			domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
			domains[d1].stack_pairs.push_back(d1-1); domains[d1].stack_pairs.push_back(d1+1);
			domains[d2].stack_pairs.push_back(d2-1); domains[d2].stack_pairs.push_back(d2+1);
		}
		if (L%2 != 0) {d1 += 2; d2 += 12;}
		else {d2 += 14;}	
	}
	d1=12; d2=152;
	for (int i=0; i<5; i++){
		domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
		domains[d1].stack_pairs.push_back(d1-1); domains[d1].stack_pairs.push_back(d2);
		domains[d2].stack_pairs.push_back(d2+1); domains[d2].stack_pairs.push_back(d1);
		d1+=1; d2-=1;
		domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
		domains[d1].stack_pairs.push_back(d1+1); domains[d1].stack_pairs.push_back(d2);
		domains[d2].stack_pairs.push_back(d2-1); domains[d2].stack_pairs.push_back(d1);
		d1+=13; d2-=13;
	}
	domains[0].stack_pairs.push_back(165); domains[0].stack_pairs.push_back(1);
	domains[165].stack_pairs.push_back(164); domains[165].stack_pairs.push_back(0);
	int myarray[] = {2,4,77,79,81,82,83,85,87,160,162,164};
	int i;
	for (int j=0; j<sizeof(myarray)/sizeof(*myarray); j++){	
		i=myarray[j];
		domains[i].stack_pairs.push_back(i-1); domains[i].stack_pairs.push_back(i+1);
	}
}
void Design::fill_crossovers_Frits(){
	int v1, v2, d1, d2;
   	Crossover c;
	v1=1; v2=11; d1=1; d2=10;
	c.v_a = v1; c.v_b = v2; c.d_a = d1; c.d_b = d2;	
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = "i";
	crossovers.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; 
			if (L%2 == 0) {c.type = "i";}
			else {c.type = "o";}
			crossovers.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	v1 = 84; v2 = 94; d1 = 84; d2 = 93; 
	c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = "i";
	crossovers.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; 
			if (L%2 == 0) {c.type = "i";}
			else {c.type = "o";}
			crossovers.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	c.v_a = 12; c.v_b = 153; 	c.d_a = 12; c.d_b = 152;
	c.d_a1 = domains[12].source; c.d_a2 = domains[12].target; c.d_b1 = domains[152].source; c.d_b2 = domains[152].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 14; c.v_b = 151; 	c.d_a = 13; c.d_b = 151;
	c.d_a1 = domains[13].source; c.d_a2 = domains[13].target; c.d_b1 = domains[151].source; c.d_b2 = domains[151].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 26; c.v_b = 139; 	c.d_a = 26; c.d_b = 138;
	c.d_a1 = domains[26].source; c.d_a2 = domains[26].target; c.d_b1 = domains[138].source; c.d_b2 = domains[138].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 28; c.v_b = 137; 	c.d_a = 27; c.d_b = 137;
	c.d_a1 = domains[27].source; c.d_a2 = domains[27].target; c.d_b1 = domains[137].source; c.d_b2 = domains[137].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 40; c.v_b = 125; 	c.d_a = 40; c.d_b = 124;
	c.d_a1 = domains[40].source; c.d_a2 = domains[40].target; c.d_b1 = domains[124].source; c.d_b2 = domains[124].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 42; c.v_b = 123; 	c.d_a = 41; c.d_b = 123;
	c.d_a1 = domains[41].source; c.d_a2 = domains[41].target; c.d_b1 = domains[123].source; c.d_b2 = domains[123].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 54; c.v_b = 111; 	c.d_a = 54; c.d_b = 110;
	c.d_a1 = domains[54].source; c.d_a2 = domains[54].target; c.d_b1 = domains[110].source; c.d_b2 = domains[110].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 56; c.v_b = 109; 	c.d_a = 55; c.d_b = 109;
	c.d_a1 = domains[55].source; c.d_a2 = domains[55].target; c.d_b1 = domains[109].source; c.d_b2 = domains[109].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 68; c.v_b = 97; 	c.d_a = 68; c.d_b = 96;
	c.d_a1 = domains[68].source; c.d_a2 = domains[68].target; c.d_b1 = domains[96].source; c.d_b2 = domains[96].target; c.type = "m";
	crossovers.push_back(c);
	c.v_a = 70; c.v_b = 95; 	c.d_a = 69; c.d_b = 95;
	c.d_a1 = domains[69].source; c.d_a2 = domains[69].target; c.d_b1 = domains[95].source; c.d_b2 = domains[95].target; c.type = "m";
	crossovers.push_back(c);
}
Design::Design () {
	n_vertices = 166;
	n_domains = n_vertices;
	fill_domains_Frits();
	fill_crossovers_Frits();
}
void Design::print_domains () {
	for (vector<Domain>::iterator d = domains.begin(); d != domains.end(); ++d) {
		cout << distance(domains.begin(), d) << " (" << (*d).source << "," << (*d).target << ")\t" << (*d).length;
		cout << "\tcross_pairs:\t";
		for (vector<int>::iterator bp = (*d).cross_pairs.begin(); bp != (*d).cross_pairs.end(); ++bp) {	
			cout << (*bp) << " (" << domains[*bp].source << "," << domains[*bp].target << ")\t";
		}
		cout << "\tstack_pairs:\t";
		for (vector<int>::iterator sp = (*d).stack_pairs.begin(); sp != (*d).stack_pairs.end(); ++sp) {	
			cout << (*sp) << " (" << domains[*sp].source << "," << domains[*sp].target << ")\t";
		}
		cout << "\n";
	}
	cout << endl;
}
void Design::print_crossovers () {
	for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
		cout << distance(crossovers.begin(), c) << "\t";
		cout << "Vertices: " << (*c).v_a << "->" << (*c).v_b << "\t";
	   	cout << "Domains: " << (*c).d_a << "->" << (*c).d_b << "\t";
	   	cout << "Full: (" << (*c).d_a1 << "," << (*c).d_a2 << ")->(" << (*c).d_b1 << "," << (*c).d_b2 << ")" << "\t";
		cout << "Type: " << (*c).type << endl;
	}
	cout << endl;
}

int Design::domain_to_crossover(int d){
	int result;
	for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
		if ((*c).d_a == d || (*c).d_b == d){
			result = distance(crossovers.begin(), c);
			break;
		}
	}
	return result;
}
vector<int> Design::vertex_to_crossover(int v){
	vector<int> result;
	for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
		if ((*c).v_a == v || (*c).v_b == v){
			result.push_back(distance(crossovers.begin(), c));
		}
	}
	return result;
}
int Design::vpair_to_crossover(pair<int,int> vpair){
	int result;
	for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
		if ( ((*c).v_a == vpair.first && (*c).v_b == vpair.second) || ((*c).v_b == vpair.first && (*c).v_a == vpair.second) ){
			result = distance(crossovers.begin(), c);
			break;
		}
	}
	return result;
}
	



























































