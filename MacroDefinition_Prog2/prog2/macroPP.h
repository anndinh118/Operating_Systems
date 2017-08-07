/*  Vivien Bautista, Redgy Canos, Ann Dinh, Danelle Paul
	cssc0601
	team missouri
	macroPP.h
	cs530, Fall 2016
*/
#ifndef MACROPP_H
#define MACROPP_H

#include "file_parser.h"
#include "file_parse_exception.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

class macroPP {
	public:
        void print_macros();	// Will print macros
        
    private:
		struct entry {					// Struct to hold the individual tokens
			string label;
			string opcode;
			string operand;
			string comment;
		};
		entry tmp;						// Temp struct
        string fname;					// Filename 
		map<string,vector<entry> > DEFTAB;		// Key: name, Value: definition
		map<string,string> PARAMTAB;			// Key: \#, Value: parameter
		vector<entry> macro_vector;				// Vector of macros
		// Methods
		void store_macro(file_parser, int);				// Grab the macro
		
};

#endif
