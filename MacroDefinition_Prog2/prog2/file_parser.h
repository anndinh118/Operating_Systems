/*  Vivien Bautista, Redgy Canos, Ann Dinh, Danelle Paul
	cssc0601
	team missouri
	file_parser.h
	cs530, Fall 2016
*/
#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

class file_parser {
	public:
        // ctor, filename is the parameter.  A driver program will read
        // the filename from the command line, and pass the filename to
        // the file_parser constructor.  Filenames must not be hard-coded.
        file_parser(string); 
        
        // dtor
        ~file_parser();
        
        // reads the source file, storing the information is some
        // auxiliary data structure you define in the private section. 
        // Throws a file_parse_exception if an error occurs.
        // if the source code file fails to conform to the above
        // specification, this is an error condition.     
        void read_file();    
        
        // returns the token found at (row, column).  Rows and columns
        // are zero based.  Returns the empty string "" if there is no 
        // token at that location. column refers to the four fields
        // identified above.
        string get_token(unsigned int, unsigned int);
        
        // prints the source code file to stdout.  Should not repeat 
        // the exact formatting of the original, but uses tabs to align
        // similar tokens in a column. The fields should match the 
        // order of token fields given above (label/opcode/operands/comments)
        void print_file();
        
        // returns the number of lines in the source code file
        int size();
        
    private:
		struct entry {					// Struct to hold the individual tokens
			string label;
			string opcode;
			string operand;
			string comment;
		};
        string fname;					// Filename 
		unsigned int numlines;			// Number of lines of file
		int is_macro;					// Flag that tells us if in macro definition
		
		ostringstream ostr;				// Will hold error messages
		vector<string> file_contents;	// Holds contents of the file
		vector<string> line_vector;		// Holds tokens of each line
		vector<entry> token_vector;		// Vector full of structs that hold tokens
		vector<string> tmp;
		entry tmp_entry;				// Temporary entry 
		
		// Methods
		void tokenize(int);				// Tokenizes each line
		void fill_tokens();				// Fills token vector
		void reset_fields();			// Resets fields needed for tokenizing
		
		void default_struct();			// Creates a struct with empty strings
		void create_struct();			// Creates struct with data from vector
		void struct1(string);			// Struct size 1
		void struct2(string);			// Struct size 2
		void struct3(vector<string>::iterator);	// Struct size 3
		void struct4(vector<string>::iterator);	// Struct size 4
		
		int validate();					// Validates tokenized vector
		int is_label(string);			// Checks token is a label with # or :
		int is_comment(string);			// Checks token is a comment with * or ;
		int has_quote(string);			// Checks token starts with a quote '
		void valid_label(string);		// Validates label (alphanumeric)
		void throw_exception(string);	// Throws exceptions with desired message
		string get_quotes(string);		// Grabs entire quote plus string until delimiter
		string remove_space(string);	// Removes space at the end of the string
};

#endif
