/*  Vivien Bautista, Redgy Canos, Ann Dinh, Danelle Paul
	cssc0601
	team missouri
	file_parser.cc
	cs530, Fall 2016
*/
#include "file_parser.h"
#include "file_parse_exception.h"
using namespace std;

	file_parser::file_parser(string s) {
		fname = s;			// Filename
		numlines = 0;		// Number of lines in file
		is_macro = 0;
	} // End of constructor

	file_parser::~file_parser() {} // End of destructor
	
	int file_parser::is_label(string s) {
		if(s.at(s.length() - 1) == ':' || s.at(0) == '#') 
			return 1;
		return 0;
	} // End is_label
	
	
	int file_parser::is_comment(string s) {
		if(s.at(0) == '*' || s.at(0) == ';')
			return 1;
		return 0;
	} // End is_comment
	
	
	int file_parser::has_quote(string s) {
		if(s.find("'") != string::npos)
			return 1;
		return 0;
	} // End has_quote
	
	
	// Throws an exception with passed in string
	void file_parser::throw_exception(string s) {
		ostr << fname + " <<ERROR>> Line " << numlines << ": " << s;
		throw file_parse_exception(ostr.str());
	} // End throw_exception
	
	
	// Validates the label
	void file_parser::valid_label(string lbl, string oc) {
		if((oc.compare("macro") == 0))
			is_macro = 1;
		if(is_macro == 1)
			return;		
		
		if(is_label(lbl) == 0) 
			throw_exception("token in column[0] is not a label");

		if(isalpha(lbl.at(0)) == 0) {
			if(lbl.compare("#minclude") != 0) 
				throw_exception("labels may only start with letters (or #minclude)");
		}
		
		// Check each individual character is valid
		for(unsigned int i = 1; i < lbl.length() - 1; i++) {
			if(isalnum(lbl.at(i)) == 0)
				throw_exception("labels may only consist of A-Z, a-z, 0-9");
		} 
		
		if(oc.compare("endm") == 0)
			is_macro = 0;
	} // End valid_label
	
	
	// Checks if vector is valid
	int file_parser::validate() {
		if(line_vector.size() > 4) 
			throw_exception("has more than 4 tokens");
		
		vector<string>::iterator it = line_vector.begin();
		string s = *it;
		// Check first column is a label
/*		if(is_comment(s) == 0 && isspace(s.at(0)) == 0) 	// Only checks if there is a label (could be a space)
			valid_label(s, *(it + 1));
		
		else if(line_vector.size() == 4) {
*/
		if(line_vector.size() == 4) {
			it += 3; 	// Last column
			s = *it;
			if(is_comment(s) == 0)
				throw_exception("last column should be a comment");
		}
		return 0;
	} // End validate
	
	
	// Creates an empty struct
	void file_parser::default_struct() {
		tmp_entry.label = "";
		tmp_entry.opcode = "";
		tmp_entry.operand = "";
		tmp_entry.comment = "";
	} // End default_struct

	
	/********* The individual creation of structs with different sizes ***********/
	void file_parser::struct1(string s) {
		if(is_comment(s) != 0) {	// 1st line comment
			tmp_entry.comment = s;
			tmp_entry.label = "";	// Clear label since it is comment
		}
	} // End struct of size 1
	
	void file_parser::struct2(string s) {
		if(is_comment(s) != 0) 
			tmp_entry.comment = s;
		else
			tmp_entry.opcode = s;
	} // End struct of size 2
	
	void file_parser::struct3(vector<string>::iterator it) {
		string s= *++it;
		tmp_entry.opcode = s;		// Second item is always an opcode
		s = *++it;
		if(is_comment(s) != 0)		// Either a comment or operand
			tmp_entry.comment = s;
		else
			tmp_entry.operand = s;
	} // End struct of size 3
	
	void file_parser::struct4(vector<string>::iterator it) {
		tmp_entry.opcode = *++it;
		tmp_entry.operand = *++it;
		tmp_entry.comment = *++it;	
	} // End struct of size 4
	
	// Creates a struct after validating
	void file_parser::create_struct() {
		vector<string>::iterator it = line_vector.begin();
		string s = *it;
		tmp_entry.label = s;	// Label is either a label or space
		
		switch(line_vector.size()) {	
			case 1:
				struct1(s);
				break;
				
			case 2:
				s = *++it;
				struct2(s);
				break;
			
			case 3:
				struct3(it);
				break;
			
			default:	// 4 elements
				struct4(it);	
				break;
		} // End switch
	} // End create_struct

	
	// Resets the fields when tokenizing each line
	void file_parser::reset_fields() {
		line_vector.clear();
		default_struct();
	} // End reset_fields
	
	string file_parser::get_quotes(string s) {
		string::size_type quote_index = s.find_first_of("'", 0);
		string::size_type tab_index = s.find_first_of(" \t" , 0);
		string quote = "";
		while(quote_index != string::npos) {
			if(quote_index > tab_index) // At the end of token that contains quotes
				break;
			quote_index = s.find_first_of("'", quote_index + 1);
			if(quote_index == string::npos) // Could not find matching quote
				throw_exception("invalid token, missing a quote (')");
			quote = s.substr(0, quote_index + 1);	// Store quote
			tab_index = s.find_first_of(" \t", quote_index + 1);
			quote_index = s.find_first_of("'", quote_index + 1);
		}
		if(tab_index == string::npos)	// At the end of the line
			return s;
		quote = s.substr(0, tab_index + 1);
		return quote;
	} // End get_quotes
	
	
	string file_parser::remove_cr(string s) {
		if(s.find_first_of("\r", 0) != string::npos)
			s = s.erase(s.length() - 1);
		return s;
	} // End remove_cr
	
	
	// Tokenizes each individual line
	void file_parser::tokenize(int i) {
		reset_fields();
		string s = file_contents[i];
		const string delim = " \t";
		string::size_type pos, last_pos, before_last;
		last_pos = s.find_first_not_of(delim, 0);
		pos = s.find_first_of(delim, last_pos);
		before_last = s.find_first_of(delim, 0);	// Points to beginning looking for space

			
		// Check if there's a space or tab in the beginning then push onto vector
		if(before_last == 0) 
			line_vector.push_back(" \t");
		
		// Grabs the rest of the line
		for(int i = 1; string::npos != pos || string::npos != last_pos; i++) {
			string str = s.substr(last_pos, pos-last_pos);	// Grab stringj
			if(is_comment(str) != 0) {		// Grabs entire string
				str = s.substr(last_pos);
				pos = string::npos;		// Updates position to end of line
			}
			else if(has_quote(str) != 0) {
				str = get_quotes(s.substr(last_pos));
				pos = last_pos + str.length();
			}
			str = remove_cr(str);
			if(str.length() != 0)	// Avoid pushing empty line onto vector
				line_vector.push_back(str); // Pushing the string onto line_vector

			// Update positions
			last_pos = s.find_first_not_of(delim, pos);
			pos = s.find_first_of(delim, last_pos);
		} // End for loop	
	} // End tokenize
	
	
	// Fills the tokens into the token vector
	void file_parser::fill_tokens() {
		// Empty line
		if(line_vector.size() == 0) 
			token_vector.push_back(tmp_entry);

		// Validate the line
		else if(validate() == 0) {
			create_struct();
			token_vector.push_back(tmp_entry);
		}	
	} // End fill_tokens
	
	
	// Get token at specified row and column
	string file_parser::get_token(unsigned int row, unsigned int col) { 
		if(row > numlines - 1) {
			ostr << "<<ERROR>> get_token: Invalid row input. (line: " << row << ") is out of range";
			throw file_parse_exception(ostr.str());
		}
		if(col > 3) {
			ostr << "<<ERROR>> get_token: " << col << " was inputted. Column must be between 0-3";
			throw file_parse_exception(ostr.str());
		}
		switch (col) {
			case 0:
				return token_vector[row].label;
			case 1:
				return token_vector[row].opcode;
			case 2:
				return token_vector[row].operand;
			default:
				return token_vector[row].comment;
		}
	} // End get token
    
	
	// Reads file that was passed in
	void file_parser::read_file() {
		ifstream in;
		string line;
		in.open(fname.c_str(), ios::in);
		if(!in) throw file_parse_exception("<<ERROR>> Could not open the file {" + fname + "}");
		while(!in.eof()) {
			getline(in, line);
			file_contents.push_back(line);
			tokenize(numlines++);
			fill_tokens();
		}
		in.close(); 
	} // End read_file
	
    
	// Prints file
	void file_parser::print_file() {
		for(unsigned int i = 0; i < token_vector.size(); i++) {
			cout << left << setfill(' ') << setw(10) << get_token(i, 0) << "\t" 
				 <<	left << setfill(' ') << setw(10) << get_token(i, 1) << "\t" 
				 <<	left << setfill(' ') << setw(10) << get_token(i, 2) << "\t" 
				 << left << setfill(' ') << setw(10) << get_token(i, 3) << "\t" << endl;
		}
	} // End print file
	
	
	// Returns the number of lines in the source code file
	int file_parser::size() {	
		return numlines;
	}
