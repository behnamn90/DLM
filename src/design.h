void fill_vertices(vector<int>& v_i, int n_vertices){
	for(int i=0; i<n_vertices; i++)
		v_i.push_back(i);
}
void fill_domains(vector< pair<int,int> >& d_i, int n_domains){
	for(int i=0; i<n_domains; i++)
		d_i.push_back(make_pair(i,i+1));
}
void fill_vertex_pairs(vector< pair<int,int> >& v_pairs){
	pair<int,int> dummy_pair;
	int v1, v2;
	v1 = 1;
	v2 = 11; 
	dummy_pair = make_pair(v1,v2);
	v_pairs.push_back(dummy_pair);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2;
			v2 = v2 - 2;
			dummy_pair = make_pair(v1,v2);
			v_pairs.push_back(dummy_pair);
		}
		v1 = v1 + 1;
		v2 = v2 + 13;	
	}
	v1 = 84;
	v2 = 94; 
	dummy_pair = make_pair(v1,v2);
	v_pairs.push_back(dummy_pair);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2;
			v2 = v2 - 2;
			dummy_pair = make_pair(v1,v2);
			v_pairs.push_back(dummy_pair);
		}
		v1 = v1 + 1;
		v2 = v2 + 13;	
	}
	v_pairs.push_back(make_pair(12,153));
	v_pairs.push_back(make_pair(14,151));
	v_pairs.push_back(make_pair(26,139));
	v_pairs.push_back(make_pair(28,137));
	v_pairs.push_back(make_pair(40,125));
	v_pairs.push_back(make_pair(42,123));
	v_pairs.push_back(make_pair(54,111));
	v_pairs.push_back(make_pair(56,109));
	v_pairs.push_back(make_pair(68,97));
	v_pairs.push_back(make_pair(70,95));
}
void fill_domain_pairs(vector< pair< pair<int,int>, pair<int,int> > >& d_pairs, vector< pair<int,int> >& d_i){
	int d1, d2;
	d1 = 1;
	d2 = 10; 
	d_pairs.push_back(make_pair(d_i[d1],d_i[d2]));
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 = d1 + 2;
			d2 = d2 - 2;
			d_pairs.push_back(make_pair(d_i[d1],d_i[d2]));
		}
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	d1 = 84;
	d2 = 93; 
	d_pairs.push_back(make_pair(d_i[d1],d_i[d2]));
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 = d1 + 2;
			d2 = d2 - 2;
			d_pairs.push_back(make_pair(d_i[d1],d_i[d2]));
		}
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	d_pairs.push_back(make_pair(d_i[12],d_i[152]));
	d_pairs.push_back(make_pair(d_i[13],d_i[151]));
	d_pairs.push_back(make_pair(d_i[26],d_i[138]));
	d_pairs.push_back(make_pair(d_i[27],d_i[137]));
	d_pairs.push_back(make_pair(d_i[40],d_i[124]));
	d_pairs.push_back(make_pair(d_i[41],d_i[123]));
	d_pairs.push_back(make_pair(d_i[54],d_i[110]));
	d_pairs.push_back(make_pair(d_i[55],d_i[109]));
	d_pairs.push_back(make_pair(d_i[68],d_i[96]));
	d_pairs.push_back(make_pair(d_i[69],d_i[95]));
}
void fill_seam_only(vector< pair<int,int> >& v_pairs){
	v_pairs.push_back(make_pair(12,153));
	v_pairs.push_back(make_pair(14,151));
	v_pairs.push_back(make_pair(26,139));
	v_pairs.push_back(make_pair(28,137));
	v_pairs.push_back(make_pair(40,125));
	v_pairs.push_back(make_pair(42,123));
	v_pairs.push_back(make_pair(54,111));
	v_pairs.push_back(make_pair(56,109));
	v_pairs.push_back(make_pair(68,97));
	v_pairs.push_back(make_pair(70,95));
}

struct crossover{
	int v_a; int v_b;
	int d_a; int d_b;
	int d_a1; int d_a2; int d_b1; int d_b2;
	string type;
};
void fill_tile_crossovers(vector< crossover >& c_vec, vector< pair<int,int> >& d_i){
	int v1, v2, d1, d2;
   	crossover c;
	v1=1; v2=11; d1=1; d2=10;
	c.v_a = v1; c.v_b = v2; c.d_a = d1; c.d_b = d2;	
	c.d_a1 = d_i[d1].first; c.d_a2 = d_i[d1].second; c.d_b1 = d_i[d2].first; c.d_b2 = d_i[d2].second; c.type = "i";
	c_vec.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = d_i[d1].first; c.d_a2 = d_i[d1].second; c.d_b1 = d_i[d2].first; c.d_b2 = d_i[d2].second; 
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
	c.d_a1 = d_i[d1].first; c.d_a2 = d_i[d1].second; c.d_b1 = d_i[d2].first; c.d_b2 = d_i[d2].second; c.type = "i";
	c_vec.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = d_i[d1].first; c.d_a2 = d_i[d1].second; c.d_b1 = d_i[d2].first; c.d_b2 = d_i[d2].second; 
			if (L%2 == 0) {c.type = "i";}
			else {c.type = "o";}
			c_vec.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	c.v_a = 12; c.v_b = 153; 	c.d_a = 12; c.d_b = 152;
	c.d_a1 = d_i[12].first; c.d_a2 = d_i[12].second; c.d_b1 = d_i[152].first; c.d_b2 = d_i[152].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 14; c.v_b = 151; 	c.d_a = 13; c.d_b = 151;
	c.d_a1 = d_i[13].first; c.d_a2 = d_i[13].second; c.d_b1 = d_i[151].first; c.d_b2 = d_i[151].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 26; c.v_b = 139; 	c.d_a = 26; c.d_b = 138;
	c.d_a1 = d_i[26].first; c.d_a2 = d_i[26].second; c.d_b1 = d_i[138].first; c.d_b2 = d_i[138].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 28; c.v_b = 137; 	c.d_a = 27; c.d_b = 137;
	c.d_a1 = d_i[27].first; c.d_a2 = d_i[27].second; c.d_b1 = d_i[137].first; c.d_b2 = d_i[137].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 40; c.v_b = 125; 	c.d_a = 40; c.d_b = 124;
	c.d_a1 = d_i[40].first; c.d_a2 = d_i[40].second; c.d_b1 = d_i[124].first; c.d_b2 = d_i[124].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 42; c.v_b = 123; 	c.d_a = 41; c.d_b = 123;
	c.d_a1 = d_i[41].first; c.d_a2 = d_i[41].second; c.d_b1 = d_i[123].first; c.d_b2 = d_i[123].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 54; c.v_b = 111; 	c.d_a = 54; c.d_b = 110;
	c.d_a1 = d_i[54].first; c.d_a2 = d_i[54].second; c.d_b1 = d_i[110].first; c.d_b2 = d_i[110].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 56; c.v_b = 109; 	c.d_a = 55; c.d_b = 109;
	c.d_a1 = d_i[55].first; c.d_a2 = d_i[55].second; c.d_b1 = d_i[109].first; c.d_b2 = d_i[109].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 68; c.v_b = 97; 	c.d_a = 68; c.d_b = 96;
	c.d_a1 = d_i[68].first; c.d_a2 = d_i[68].second; c.d_b1 = d_i[96].first; c.d_b2 = d_i[96].second; c.type = "m";
	c_vec.push_back(c);
	c.v_a = 70; c.v_b = 95; 	c.d_a = 69; c.d_b = 95;
	c.d_a1 = d_i[69].first; c.d_a2 = d_i[69].second; c.d_b1 = d_i[95].first; c.d_b2 = d_i[95].second; c.type = "m";
	c_vec.push_back(c);
}

class Tile {
	public:
		int n_vertices;
		int n_domains;
		vector<int> v_i;
		vector< pair<int,int> > d_i;
		vector< pair<int,int> > v_pairs;
		vector< pair< pair<int,int>, pair<int,int> > > d_pairs;

		vector<crossover> crossovers;

		vector< pair<int,int> > seam_only;
		
		Tile();
		void print_vertex_pairs();	
		void print_domain_pairs();
};

Tile::Tile () {
	n_vertices = 166;
	n_domains = n_vertices;
	fill_vertices(v_i, n_vertices);
	fill_domains(d_i, n_domains);
	fill_vertex_pairs(v_pairs);
	fill_domain_pairs(d_pairs,d_i);
	fill_seam_only(seam_only);
	fill_tile_crossovers(crossovers,d_i);
}

void Tile::print_vertex_pairs () {
	for (vector< pair<int,int> >::iterator it = v_pairs.begin(); it != v_pairs.end(); ++it)
		cout << "(" << (*it).first << "," << (*it).second << ")" << '\n';
	cout << endl;
}

void Tile::print_domain_pairs () {
	for (vector< pair < pair<int,int>, pair<int,int> > >::iterator it = d_pairs.begin(); it != d_pairs.end(); ++it)
		cout << "(" << (*it).first.first << "," << (*it).first.second << ")" << " -- " << "(" << (*it).second.first << "," << (*it).second.second << ")" << '\n';
	cout << endl;
}













































































