class Energy {
	protected:
		double dH;
		double dS;
		double initial;
		double terminal;
		double salt;

		void set_dH (double& H){
			dH = H;
		}
		void set_dS (double& S){
			dS = S;
		}

	public:
		double dG;
		double get_dG (double& T){
			return dH - T*dS + init + terminal + salt;
		}
};

class G_duplex: public Energy {
	public:
		
