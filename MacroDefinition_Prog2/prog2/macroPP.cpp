/*  Vivien Bautista, Redgy Canos, Ann Dinh, Danelle Paul
	cssc0601
	team missouri
	macroPP.cpp
	cs530, Fall 2016
*/    
#include "macroPP.h"

using namespace std;
	void store_macro(file_parser parser, int row) {
		
	}
	/*
	void store_macro(file_parser parser, int row) {
		while(parser.get_token(row, 1).compare("endm") != 0) {
			tmp.label = parser.get_token(row, 0);
			tmp.opcode = parser.get_token(row, 1);
			tmp.operand = parser.get_token(row, 2);
			macro_vector.push_back(tmp);
		}
	}
	*/
	struct entry {					// Struct to hold the individual tokens
		string label;
		string opcode;
		string operand;
	};
	entry tmp;						// Temp struct
	map<string,vector<entry> > DEFTAB;		// Key: name, Value: definition
	map<string,string> PARAMTAB;
	vector<entry> macro_vector;				// Holds macro defs



	int main(int argc, char ** argv) {
		if (argc != 2) {	
			cout << "usage: " << argv[0] << " <filename>" << endl;
			return 1;
		}
		try {
			file_parser parser(argv[1]); // Calls file_parser constructor
			parser.read_file();
			//parser.print_file();
			for(int i = 0; i < parser.size(); i++) {
				// Grabs the macro definition 
				if(parser.get_token(i, 1).compare("macro") == 0) {
					while(parser.get_token(i, 1).compare("endm") != 0) {
				// Create struct and store into vector
						tmp.label = parser.get_token(i, 0);
						tmp.opcode = parser.get_token(i, 1);
						tmp.operand = parser.get_token(i, 2);
						macro_vector.push_back(tmp);
						i++;
					} // End while not endm
				
				// Store the macro vector into our macro map
					DEFTAB.insert(pair<string,vector<entry> >(macro_vector[0].label, macro_vector));
				} // End if macro
			} // End for loop
			
			
		//	Prints the macro definition vector
			for(unsigned int i = 0; i < macro_vector.size(); i++) {
				cout << "Label: " << macro_vector[i].label << endl;
				cout << "Opcode: " <<  macro_vector[i].opcode << endl;
				cout << "Operand: " <<  macro_vector[i].operand << endl;
				cout << "\n\n" << endl;
			} 
		} catch(file_parse_exception& e) {
			cout << e.get_message() << endl;
		} // End catch
	   
		return 0;
	} // End main class
