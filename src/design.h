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
void fill_domains(vector<Domain>& domains, int& n_vertices){
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
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 = d1 + 2;
			d2 = d2 - 2;
			domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
		}
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	d1 = 84;
	d2 = 93; 
	domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 = d1 + 2;
			d2 = d2 - 2;
			domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
		}
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	d1=12; d2=152;
	for (int i=0; i<5; i++){
		domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
		d1+=1; d2-=1;
		domains[d1].cross_pairs.push_back(d2); domains[d2].cross_pairs.push_back(d1);
		d1+=13; d2-=13;
	}
}
void fill_crossovers(vector<Crossover>& c_vec, vector<Domain>& domains){
	int v1, v2, d1, d2;
   	Crossover c;
	v1=1; v2=11; d1=1; d2=10;
	c.v_a = v1; c.v_b = v2; c.d_a = d1; c.d_b = d2;	
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = "i";
	c_vec.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; 
			if (L%2 == 0) {c.type = "i";}
			else {c.type = "o";}
			c_vec.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	v1 = 84; v2 = 94; d1 = 84; d2 = 93; 
	c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = "i";
	c_vec.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; 
			if (L%2 == 0) {c.type = "i";}
			else {c.type = "o";}
			c_vec.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	c.v_a = 12; c.v_b = 153; 	c.d_a = 12; c.d_b = 152;
	c.d_a1 = domains[12].source; c.d_a2 = domains[12].target; c.d_b1 = domains[152].source; c.d_b2 = domains[152].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 14; c.v_b = 151; 	c.d_a = 13; c.d_b = 151;
	c.d_a1 = domains[13].source; c.d_a2 = domains[13].target; c.d_b1 = domains[151].source; c.d_b2 = domains[151].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 26; c.v_b = 139; 	c.d_a = 26; c.d_b = 138;
	c.d_a1 = domains[26].source; c.d_a2 = domains[26].target; c.d_b1 = domains[138].source; c.d_b2 = domains[138].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 28; c.v_b = 137; 	c.d_a = 27; c.d_b = 137;
	c.d_a1 = domains[27].source; c.d_a2 = domains[27].target; c.d_b1 = domains[137].source; c.d_b2 = domains[137].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 40; c.v_b = 125; 	c.d_a = 40; c.d_b = 124;
	c.d_a1 = domains[40].source; c.d_a2 = domains[40].target; c.d_b1 = domains[124].source; c.d_b2 = domains[124].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 42; c.v_b = 123; 	c.d_a = 41; c.d_b = 123;
	c.d_a1 = domains[41].source; c.d_a2 = domains[41].target; c.d_b1 = domains[123].source; c.d_b2 = domains[123].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 54; c.v_b = 111; 	c.d_a = 54; c.d_b = 110;
	c.d_a1 = domains[54].source; c.d_a2 = domains[54].target; c.d_b1 = domains[110].source; c.d_b2 = domains[110].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 56; c.v_b = 109; 	c.d_a = 55; c.d_b = 109;
	c.d_a1 = domains[55].source; c.d_a2 = domains[55].target; c.d_b1 = domains[109].source; c.d_b2 = domains[109].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 68; c.v_b = 97; 	c.d_a = 68; c.d_b = 96;
	c.d_a1 = domains[68].source; c.d_a2 = domains[68].target; c.d_b1 = domains[96].source; c.d_b2 = domains[96].target; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 70; c.v_b = 95; 	c.d_a = 69; c.d_b = 95;
	c.d_a1 = domains[69].source; c.d_a2 = domains[69].target; c.d_b1 = domains[95].source; c.d_b2 = domains[95].target; c.type = "m";
	c_vec.push_back(c);
}

class Tile {
	public:
		int n_vertices;
		int n_domains;
		vector<Domain> domains;
		vector<Crossover> crossovers;
		
		Tile();
		void print_domains();
		//void print_crossovers();	
};

void Tile::print_domains () {
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

Tile::Tile () {
	n_vertices = 166;
	n_domains = n_vertices;
	fill_domains(domains, n_vertices);
	fill_crossovers(crossovers, domains);
}

































































