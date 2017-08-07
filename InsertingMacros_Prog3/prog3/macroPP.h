/*  Vivien Bautista, Redgy Canos, Ann Dinh, Danelle Paul
	cssc0601
	team missouri
	macroPP.h
	cs530, Fall 2016
*/
#ifndef MACROPP_H
#define MACROPP_H

#include "file_parser.h"
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
		// Methods
		macroPP(string);		// Constructor
		~macroPP();				// Destructor
        void print_macros();	
		void print_params();
		void process(file_parser*);			// Grabs macros, stores macros, writes to disk
		void second_pass();					// Invocation of macros, expands, substitutes
		void close_file();	
		
		// Variables
		file_parser *parser;				// Pointer to a parser	
        file_parser *mparser;				// Parser for minclude
		string fname;						// Filename
		string tmp_file;					// temp.txt
		string se_file;						// .se filename
		
    private:
		// Methods
		int create_def_struct(file_parser*, int);		// Creates macro label, opcode, operand
		void create_info_struct();						// Creates struct that holds info of macro
		void create_file(string);						// Creates a new file
		string create_line(file_parser*, int);			// Concatenates entire line to one string
		int store_macro(file_parser*, int);				// Stores macro to macro_vector	
		void write_to_disk(file_parser*, int);			// Writes line to disk
		void space_to_disk();							// Writes a \n to disk				
		void process_minclude(string);					// Will take care of minclude
		void throw_exception(int, string);				// Throws exception
		void clear_vectors();							// Clears vector for each new line
		string check_colon(string);						// Truncates colon for macro def names
		void invocation();								// Second pass' invocation of macro
		void update_param(string);						// Within macrotab the parameter vector is updated
		void fill_param(string);						// Fills parameter vector
		
		// Variables
		struct definition {					// Struct to hold the individual tokens
			string label;
			string opcode;
			string operand;
		};
		
		struct info {
			vector<definition> def_vec;		// Definition of macro
			vector<string> param_vec; 		// Key: \#, Value: parameter
			unsigned int counter;				// Invocation counter
			string size;							// b, w, or l
		};
		
		definition tmpdef;						// Line of definition
		info tmpinfo;							// Struct of tmp info
		ostringstream ostr;						// Holds error messages
		ofstream new_file;						// Disk file
		map<string,info> MACROTAB;				// Key: name Value: info about macro
		vector<string> tmp_param_vector;		// Holds individual parameters
		vector<definition> tmp_def_vector;		// Holds individual macro defs
};

#endif
