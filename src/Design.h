class Domain {
	public:
		int source; int target;
		vector<int> cross_domains; //no need for this
		vector<int> crossover_ids;
		vector<int> stack_domains;
		int length;
		string seq;
};
class Crossover {
	public:
		int v_a; int v_b;
		int d_a; int d_b;
		int d_a1; int d_a2; int d_b1; int d_b2; //no need for this
		char type;  //i: inside, o: outside, m: seam
};
class Design {
	public:
		int n_vertices;
		int n_domains;
		vector<Domain> domains;
		vector<Crossover> crossovers;
		
		void fill_crossover_ids();
		Design();
		void print_domains();
		void print_crossovers();

		vector<int> domain_to_crossovers(int d);
		vector<int> vertex_to_crossovers(int v);
		int vpair_to_crossover(pair<int,int> v_pair);
		int dpair_to_crossover(pair<int,int> d_pair);
		bool check_single_domain_staple(int e);
		bool check_double_domain_staple(int e);
		bool check_triple_domain_staple(int e);

		string scaffold_sequence;
};
Design::Design () {
}
void Design::fill_crossover_ids(){
	for (vector<Domain>::iterator d = domains.begin(); d != domains.end(); ++d) {
		for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
			if ((*c).d_a == distance(domains.begin(),d) || (*c).d_b == distance(domains.begin(),d)){
				(*d).crossover_ids.push_back(distance(crossovers.begin(), c));
			}
		}
	}
}
void Design::print_domains () {
	for (vector<Domain>::iterator d = domains.begin(); d != domains.end(); ++d) {
		cout << distance(domains.begin(), d) << " (" << (*d).source << "," << (*d).target << ")\t" << (*d).length;
		cout << "\tcross_domains:\t";
		for (vector<int>::iterator bp = (*d).cross_domains.begin(); bp != (*d).cross_domains.end(); ++bp) {	
			cout << (*bp) << " (" << domains[*bp].source << "," << domains[*bp].target << ")\t";
		}
		cout << "\tstack_domains:\t";
		for (vector<int>::iterator sp = (*d).stack_domains.begin(); sp != (*d).stack_domains.end(); ++sp) {	
			cout << (*sp) << " (" << domains[*sp].source << "," << domains[*sp].target << ")\t";
		}
		cout << "\tcrossover_ids:\t";
		for (vector<int>::iterator sp = (*d).crossover_ids.begin(); sp != (*d).crossover_ids.end(); ++sp) {	
			cout << (*sp) << "\t";
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
vector<int> Design::domain_to_crossovers(int d){
	vector<int> result;
	for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
		if ((*c).d_a == d || (*c).d_b == d){
			result.push_back(distance(crossovers.begin(), c));
			break;
		}
	}
	return result;
}
vector<int> Design::vertex_to_crossovers(int v){
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
int Design::dpair_to_crossover(pair<int,int> dpair){
	int result;
	for (vector<Crossover>::iterator c = crossovers.begin(); c != crossovers.end(); ++c) {
		if ( ((*c).d_a == dpair.first && (*c).d_b == dpair.second) || ((*c).d_b == dpair.first && (*c).d_a == dpair.second) ){
			result = distance(crossovers.begin(), c);
			break;
		}
	}
	return result;
}
bool Design::check_single_domain_staple(int e){
	bool result;
	if (domains[e].crossover_ids.empty()){
		result = true;
	}
	else{
		result = false;
	}
	return result;
}
bool Design::check_double_domain_staple(int e){
	bool result;
	if (domains[e].crossover_ids.size()==1){
		result = true;
	}
	else{
		result = false;
	}
	return result;
}
bool Design::check_triple_domain_staple(int e){
	bool result;
	if (domains[e].crossover_ids.size()==2){
		result = true;
	}
	else{
		result = false;
	}
	return result;
}



class Frits: public Design{
	public:
		Frits();
		void fill_domains();
		void fill_crossovers();
};
Frits::Frits () {
	n_vertices = 166;
	n_domains = n_vertices;
	fill_domains();
	fill_crossovers();
	fill_crossover_ids();
}
void Frits::fill_domains(){
	Domain d; d.length = 16; 
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
	domains[d1].cross_domains.push_back(d2); domains[d2].cross_domains.push_back(d1);
	domains[d1].stack_domains.push_back(d1-1); domains[d1].stack_domains.push_back(d1+1);
	domains[d2].stack_domains.push_back(d2-1); domains[d2].stack_domains.push_back(d2+1);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 += 2;
			d2 -= 2;
			domains[d1].cross_domains.push_back(d2); domains[d2].cross_domains.push_back(d1);
			domains[d1].stack_domains.push_back(d1-1); domains[d1].stack_domains.push_back(d1+1);
			domains[d2].stack_domains.push_back(d2-1); domains[d2].stack_domains.push_back(d2+1);
		}
		if (L%2 != 0) {d1 += 2; d2 += 12;}
		else {d2 += 14;}	
	}
	d1 = 84;
	d2 = 93; 
	domains[d1].cross_domains.push_back(d2); domains[d2].cross_domains.push_back(d1);
	domains[d1].stack_domains.push_back(d1-1); domains[d1].stack_domains.push_back(d1+1);
	domains[d2].stack_domains.push_back(d2-1); domains[d2].stack_domains.push_back(d2+1);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if (L==0 && l==0){continue;}
			d1 += 2;
			d2 -= 2;
			domains[d1].cross_domains.push_back(d2); domains[d2].cross_domains.push_back(d1);
			domains[d1].stack_domains.push_back(d1-1); domains[d1].stack_domains.push_back(d1+1);
			domains[d2].stack_domains.push_back(d2-1); domains[d2].stack_domains.push_back(d2+1);
		}
		if (L%2 != 0) {d1 += 2; d2 += 12;}
		else {d2 += 14;}	
	}
	d1=12; d2=152;
	for (int i=0; i<5; i++){
		domains[d1].cross_domains.push_back(d2); domains[d2].cross_domains.push_back(d1);
		domains[d1].stack_domains.push_back(d1-1); domains[d1].stack_domains.push_back(d2);
		domains[d2].stack_domains.push_back(d2+1); domains[d2].stack_domains.push_back(d1);
		d1+=1; d2-=1;
		domains[d1].cross_domains.push_back(d2); domains[d2].cross_domains.push_back(d1);
		domains[d1].stack_domains.push_back(d1+1); domains[d1].stack_domains.push_back(d2);
		domains[d2].stack_domains.push_back(d2-1); domains[d2].stack_domains.push_back(d1);
		d1+=13; d2-=13;
	}
	domains[0].stack_domains.push_back(165); domains[0].stack_domains.push_back(1);
	domains[165].stack_domains.push_back(164); domains[165].stack_domains.push_back(0);
	int myarray[] = {2,4,77,79,81,82,83,85,87,160,162,164};
	int i;
	for (int j=0; j<sizeof(myarray)/sizeof(*myarray); j++){	
		i=myarray[j];
		domains[i].stack_domains.push_back(i-1); domains[i].stack_domains.push_back(i+1);
	}

	scaffold_sequence = "CTTCAAGAACTCTGTAGCACCGCCTACATACCTCGCTCTGCTAATCCTGTTACCAGTGGCTGCTGCCAGTGGCGATAAGTCGTGTCTTACCGGGTTGGACTCAAGACGATAGTTACCGGATAAGGCGCAGCGGTCGGGCTGAACGGGGGGTTCGTGCACACAGCCCAGCTTGGAGCGAACGACCTACACCGAACTGAGATACCTACAGCGTGAGCTATGAGAAAGCGCCACGCTTCCCGAAGGGAGAAAGGCGGACAGGTATCCGGTAAGCGGCAGGGTCGGAACAGGAGAGCGCACGAGGGAGCTTCCAGGGGGAAACGCCTGGTATCTTTATAGTCCTGTCGGGTTTCGCCACCTCTGACTTGAGCGTCGATTTTTGTGATGCTCGTCAGGGGGGCGGAGCCTATGGAAAAACGCCAGCAACGCGGCCTTTTTACGGTTCCTGGCCTTTTGCTGGCCTTTTGCTCACATGTTCTTTCCTGCGTTATCCCCTGATTCTGTGGATAACCGTATTACCGCCTTTGAGTGAGCTGATACCGCTCGCCGCAGCCGAACGACCGAGCGCAGCGAGTCAGTGAGCGAGGAAGCGGAAGAGCGCCCAATACGCAAACCGCCTCTCCCCGCGCGTTGGCCGATTCATTAATGCAGCTGGCACGACAGGTTTCCCGACTGGAAAGCGGGCAGTGAGCGCAACGCAATTAATGTGAGTTAGCTCACTCATTAGGCACCCCAGGCTTTACACTTTATGCTTCCGGCTCGTATGTTGTGTGGAATTGTGAGCGGATAACAATTTCACACAGGAAACAGCTATGACCATGATTACGCCAAGCTCCTCAGCAATTCACTGGCCGTCGTTTTACAACGTCGTGACTGGGAAAACCCTGGCGTTACCCAACTTAATCGCCTTGCAGCACATCCCCCTTTCGCCAGCTGGCGTAATAGCGAAGAGGCCCGCACCGATCGCCCTTCCCAACAGTTGCGCAGCCTGAATGGCGAATGGCGCCTGATGCGGTATTTTCTCCTTACGCATCTGTGCGGTATTTCACACCGCATATGGTGCACTCTCAGTACAATCTGCTCTGATGCCGCATAGTTAAGCCAGCCCCGACACCCGCCAACACCCGCTGACGCGCCCTGACGGGCTTGTCTGCTCCCGGCATCCGCTTACAGACAAGCTGTGACCGTCTCCGGGAGCTGCATGTGTCAGAGGTTTTCACCGTCATCACCGAAACGCGCGAGACGAAAGGGCCTCGTGATACGCCTATTTTTATAGGTTAATGTCATGATAATAATGGTTTCTTAGACGTCAGGTGGCACTTTTCGGGGAAATGTGCGCGGAACCCCTATTTGTTTATTTTTCTAAATACATTCAAATATGTATCCGCTCATGAGACAATAACCCTGATAAATGCTTCAATAATATTGAAAAAGGAAGAGTATGAGTATTCAACATTTCCGTGTCGCCCTTATTCCCTTTTTTGCGGCATTTTGCCTTCCTGTTTTTGCTCACCCAGAAACGCTGGTGAAAGTAAAAGATGCTGAAGATCAGTTGGGTGCACGAGTGGGTTACATCGAACTGGATCTCAACAGCGGTAAGATCCTTGAGAGTTTTCGCCCCGAAGAACGTTTTCCAATGATGAGCACTTTTAAAGTTCTGCTATGTGGCGCGGTATTATCCCGTATTGACGCCGGGCAAGAGCAACTCGGTCGCCGCATACACTATTCTCAGAATGACTTGGTTGAGTACTCACCAGTCACAGAAAAGCATCTTACGGATGGCATGACAGTAAGAGAATTATGCAGTGCTGCCATAACCATGAGTGATAACACTGCGGCCAACTTACTTCTGACAACGATCGGAGGACCGAAGGAGCTAACCGCTTTTTTGCACAACATGGGGGATCATGTAACTCGCCTTGATCGTTGGGAACCGGAGCTGAATGAAGCCATACCAAACGACGAGCGTGACACCACGATGCCTGTAGCAATGGCAACAACGTTGCGCAAACTATTAACTGGCGAACTACTTACTCTAGCTTCCCGGCAACAATTAATAGACTGGATGGAGGCGGATAAAGTTGCAGGACCACTTCTGCGCTCGGCCCTTCCGGCTGGCTGGTTTATTGCTGATAAATCTGGAGCCGGTGAGCGTGGGTCTCGCGGTATCATTGCAGCACTGGGGCCAGATGGTAAGCCCTCCCGTATCGTAGTTATCTACACGACGGGGAGTCAGGCAACTATGGATGAACGAAATAGACAGATCGCTGAGATAGGTGCCTCACTGATTAAGCATTGGTAACTGTCAGACCAAGTTTACTCATATATACTTTAGATTGATTTAAAACTTCATTTTTAATTTAAAAGGATCTAGGTGAAGATCCTTTTTGATAATCTCATGACCAAAATCCCTTAACGTGAGTTTTCGTTCCACTGAGCGTCAGACCCCGTAGAAAAGATCAAAGGATCTTCTTGAGATCCTTTTTTTCTGCGCGTAATCTGCTGCTTGCAAACAAAAAAACCACCGCTACCAGCGGTGGTTTGTTTGCCGGATCAAGAGCTACCAACTCTTTTTCCGAAGGTAACTGGCTTCAGCAGAGCGCAGATACCAAATACTGTTCTTCTAGTGTAGCCGTAGTTAGGCCACCA"; //5'-3'; start at domain 165, end at domain 0. 

	for (int i=1; i<82; i+=4){
		domains[i].length = 15;
	}
	for (int i=84; i<165; i+=4){
		domains[i].length = 15;
	}

	//string domain_sequence;
	int s = 0;
	for (unsigned i = domains.size(); i-- > 0; ) {
		//domain_sequence.clear();
		for (int j=0; j<domains[i].length; j++) {
			domains[i].seq += scaffold_sequence[s];
			//domain_sequence += scaffold_sequence[s];
			//(domains[i].seq).append(scaffold_sequence[s]);
			//cout << i << "\t" << j << "\t" << s << "\t" << scaffold_sequence[s] << endl;
	  		s++;
		}
		//domains[i].seq = domain_sequence;	
	}
}
void Frits::fill_crossovers(){
	int v1, v2, d1, d2;
   	Crossover c;
	v1=1; v2=11; d1=1; d2=10;
	c.v_a = v1; c.v_b = v2; c.d_a = d1; c.d_b = d2;	
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = 'i';
	crossovers.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; 
			if (L%2 == 0) {c.type = 'i';}
			else {c.type = 'o';}
			crossovers.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	v1 = 84; v2 = 94; d1 = 84; d2 = 93; 
	c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = 'i';
	crossovers.push_back(c);
	for (int L = 0; L < 11; L++){
		for (int l = 0; l < 3; l++){	
			if(L==0 && l==0){continue;}
			v1 = v1 + 2; v2 = v2 - 2; d1 = d1 + 2; d2 = d2 - 2;
			c.v_a = v1; c.v_b = v2;	c.d_a = d1; c.d_b = d2; 
			c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; 
			if (L%2 == 0) {c.type = 'i';}
			else {c.type = 'o';}
			crossovers.push_back(c);
		}
		v1 = v1 + 1; v2 = v2 + 13;	
		if (L%2 != 0) {d1 = d1 + 2; d2 = d2 + 12;}
		else {d2 = d2 + 14;}	
	}
	c.v_a = 12; c.v_b = 153; 	c.d_a = 12; c.d_b = 152;
	c.d_a1 = domains[12].source; c.d_a2 = domains[12].target; c.d_b1 = domains[152].source; c.d_b2 = domains[152].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 14; c.v_b = 151; 	c.d_a = 13; c.d_b = 151;
	c.d_a1 = domains[13].source; c.d_a2 = domains[13].target; c.d_b1 = domains[151].source; c.d_b2 = domains[151].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 26; c.v_b = 139; 	c.d_a = 26; c.d_b = 138;
	c.d_a1 = domains[26].source; c.d_a2 = domains[26].target; c.d_b1 = domains[138].source; c.d_b2 = domains[138].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 28; c.v_b = 137; 	c.d_a = 27; c.d_b = 137;
	c.d_a1 = domains[27].source; c.d_a2 = domains[27].target; c.d_b1 = domains[137].source; c.d_b2 = domains[137].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 40; c.v_b = 125; 	c.d_a = 40; c.d_b = 124;
	c.d_a1 = domains[40].source; c.d_a2 = domains[40].target; c.d_b1 = domains[124].source; c.d_b2 = domains[124].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 42; c.v_b = 123; 	c.d_a = 41; c.d_b = 123;
	c.d_a1 = domains[41].source; c.d_a2 = domains[41].target; c.d_b1 = domains[123].source; c.d_b2 = domains[123].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 54; c.v_b = 111; 	c.d_a = 54; c.d_b = 110;
	c.d_a1 = domains[54].source; c.d_a2 = domains[54].target; c.d_b1 = domains[110].source; c.d_b2 = domains[110].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 56; c.v_b = 109; 	c.d_a = 55; c.d_b = 109;
	c.d_a1 = domains[55].source; c.d_a2 = domains[55].target; c.d_b1 = domains[109].source; c.d_b2 = domains[109].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 68; c.v_b = 97; 	c.d_a = 68; c.d_b = 96;
	c.d_a1 = domains[68].source; c.d_a2 = domains[68].target; c.d_b1 = domains[96].source; c.d_b2 = domains[96].target; c.type = 'm';
	crossovers.push_back(c);
	c.v_a = 70; c.v_b = 95; 	c.d_a = 69; c.d_b = 95;
	c.d_a1 = domains[69].source; c.d_a2 = domains[69].target; c.d_b1 = domains[95].source; c.d_b2 = domains[95].target; c.type = 'm';
	crossovers.push_back(c);
}

class Simple: public Design{
	public:
		Simple();
		void fill_domains();
		void fill_crossovers();
};
Simple::Simple () {
	n_vertices = 10;
	n_domains = n_vertices;
	fill_domains();
	fill_crossovers();
}
void Simple::fill_domains(){
	Domain d; d.length = 16; 
	for(int i=0; i<n_vertices-1; i++){
		d.source = i; d.target = i+1;
		domains.push_back(d);
	}
	d.source = n_vertices-1; d.target = 0;
	domains.push_back(d);
	domains[0].stack_domains.push_back(9); domains[0].stack_domains.push_back(1);
	for (int i=1; i<n_vertices-1; i++){
		domains[i].stack_domains.push_back(i-1); domains[i].stack_domains.push_back(i+1);
	}
	domains[9].stack_domains.push_back(8); domains[9].stack_domains.push_back(0);
	
	domains[1].cross_domains.push_back(8);
	domains[8].cross_domains.push_back(1);
	
	domains[3].cross_domains.push_back(6);
	domains[6].cross_domains.push_back(3);
}
void Simple::fill_crossovers(){
	int v1, v2, d1, d2;
   	Crossover c;
	v1=2; v2=8; d1=1; d2=8;
	c.v_a = v1; c.v_b = v2; c.d_a = d1; c.d_b = d2;	
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = 'i';
	crossovers.push_back(c);
	v1=4; v2=6; d1=3; d2=6;
	c.v_a = v1; c.v_b = v2; c.d_a = d1; c.d_b = d2;	
	c.d_a1 = domains[d1].source; c.d_a2 = domains[d1].target; c.d_b1 = domains[d2].source; c.d_b2 = domains[d2].target; c.type = 'i';
	crossovers.push_back(c);
}















































