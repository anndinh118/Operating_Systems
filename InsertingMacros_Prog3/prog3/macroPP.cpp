/*  Vivien Bautista, Redgy Canos, Ann Dinh, Danelle Paul
	cssc0601
	team missouri
	macroPP.cpp
	cs530, Fall 2016
*/   
#include "macroPP.h"
#include "macroPP_error_exception.h"
#include "file_parse_exception.h"

using namespace std;
	macroPP::macroPP(string s) {
		try {
			parser = new file_parser(s);
			parser->read_file();
		} catch (file_parse_exception& e) {
			throw macroPP_error_exception(e.get_message());
		}
		se_file = s + "e";
		expansion_file = se_file + "e";
	} // End constructor
	
	macroPP::~macroPP() {
		delete(parser);
	} // End destructor
	
	
	// Throws an exception with passed in string
	void macroPP::throw_exception(int n, string s) {
		ostr << "<<ERROR>> Line " << n << ": " << s;
		if(n == -1)	// Dummy index for clean_file method
			ostr << "<<ERROR>>: " << s;
		throw macroPP_error_exception(ostr.str());
	} // End throw_exception
	

	int macroPP::create_def_struct(file_parser *p, int row) {
		definition tmpdef;
		string macro_name = check_colon(p->get_token(row,0));
		int macro_row = row + 1;

		// Keep going until we reach the end of macro definition
		try {
			for(int i = 1; i != -1; row++) { // Update row number
				tmpdef.label = check_colon(p->get_token(row, 0));
				tmpdef.opcode = p->get_token(row, 1);
				tmpdef.operand = p->get_token(row, 2);
				tmp_def_vector.push_back(tmpdef);
				if((tmpdef.opcode.compare("endm") == 0) || (i == 0) )
					i--; // Stopping condition is the line after endm
			} // End for loop
		} catch(file_parse_exception& e) {	// endm not found
			throw_exception(macro_row, "endm is missing for {" + macro_name + "}");
		}
		return ++row;
	} // End create_macro_struct
	
	
	void macroPP::create_info_struct() {
		tmpinfo.def_vec = tmp_def_vector;	// Holds definition
		tmpinfo.counter = 0;
	} // End create_info_struct
	
	
	string macroPP::check_colon(string s) {
		if(s.length() == 0)	// Empty string
			return s;
		if(s.at(s.length() - 1) == ':')
			s.erase(s.length() - 1);
		return s;
	} // End colon
	
	
	void macroPP::fill_param(string s) {
		const string delim = ",";	// Our delimiter is a comma
		string::size_type pos, last_pos, before_last;
		last_pos = s.find_first_not_of(delim, 0);
		pos = s.find_first_of(delim, last_pos);
		
		while(pos != string::npos || last_pos != string::npos) {
			param_vec.push_back(s.substr(last_pos, pos-last_pos)); // Push parameter onto vector
			last_pos = s.find_first_not_of(delim, pos);	// Update positions
			pos = s.find_first_of(delim, last_pos);
		} // End while
	} // End fill_param
	
	
	int macroPP::store_macro(file_parser *p, int row) {
		string key = check_colon(p->get_token(row, 0));	// Label is our key
		if(MACROTAB.count(key) != 0)	// Ensure key is not within map 
			throw_exception(row + 1, ("duplicate macro definition {" + key + "}"));
		row = create_def_struct(p, row);		// Create structs and will return current line number
		
		// Store the macro vector into our macro map
		create_info_struct();		// Info about macro
		MACROTAB.insert(pair<string,info>(key, tmpinfo)); // Insert key, value pair to table
		tmp_def_vector.clear(); // Clear for new macro definition
		return row;	
	} // End store_macro
	
	
	void macroPP::write_to_disk(string fname, file_parser *p, int row) {	
		new_file.open(fname.c_str(), ios::app);	// Opens file and appends to end of file
		new_file << left << setfill(' ') << setw(12) << p->get_token(row, 0) << "\t" 
				 << left << setfill(' ') << setw(12) << p->get_token(row, 1) << "\t"
				 << left << setfill(' ') << setw(12) << p->get_token(row, 2) << "\t"
				 << left << setfill(' ') << setw(12) << p->get_token(row, 3) << "\n";
		new_file.close();
	} // End write_to_disk
	
	
	void macroPP::process_minclude(string s) {
		try {
			mparser = new file_parser(s);
			mparser->read_file();	// minclude's parser
		} catch(file_parse_exception& e) {
			throw macroPP_error_exception(e.get_message());
		}
		process(mparser);	// Process with minclude's fileparser
		delete(mparser);	// Avoid memory leaks
		
	} // End process_minclude
	
	
	void macroPP::process(file_parser *p) {
		new_file.open((se_file).c_str());	// Create a new files with e attached
		for(int i = 0; i < p->size(); i++) { // Go through each line
			int m_processed = 0;	// minclude was not processed
			int is_macro = 0;
			string label = p->get_token(i, 0);
			string opcode = p->get_token(i, 1);
			if(label.compare("#minclude") == 0) {
				process_minclude(opcode);	// Pass in file name
				m_processed = 1;
			}
			if(opcode.compare("macro") == 0) {	// Grabs macro defintion
				if(label.find_first_not_of(" \t") == string::npos) // Label is an empty string
					throw_exception(i + 1, "macro definition is missing a label");
				i = store_macro(p, i) - 1;	// Don't skip the line after endm
				is_macro = 1;
			}
			if(m_processed == 0 && is_macro == 0)	// Writes to disk if minclude was not processed
				write_to_disk(se_file, p, i);
		} // End for loop
		new_file.close();	// Close se_file
		
	} // End process
	
	void macroPP::parse_se_file() {
		try {
			parser = new file_parser(se_file);
			parser->read_file();
		} catch (file_parse_exception& e) {
			throw macroPP_error_exception(e.get_message());
		}
	} // End parse_se_file
	
	
	void macroPP::insert_expansion(string a, string b, string c) {
		new_file.open(expansion_file.c_str(), ios::app);	// Opens file and appends to end of file
		new_file << left << setfill(' ') << setw(12) << a << "\t" 
				 << left << setfill(' ') << setw(12) << b << "\t"
				 << left << setfill(' ') << setw(12) << c << "\n";
		new_file.close();
	} // End insert_expansion
	
	
	string macroPP::check_dot(string s) {
		if(s.length() == 0)	// Empty string
			return s;
		string::size_type found_dot;
		found_dot = s.find(".");
		if(found_dot != string::npos) {
			macro_size = s.substr(found_dot + 1);
			return s.substr(0, found_dot);
		}
		return s;
	} // End check_dot
	
	void macroPP::increment_counter(string key) {
		unsigned int *ctr = &MACROTAB.find(key)->second.counter;
		*ctr = *ctr + 1;
	} // End increment_counter
	
	
	string macroPP::int_to_string(int number) {
		ostringstream convert;
		convert << number;
		string num_string = convert.str();
		return num_string;
	} // End int_to_string
	
	
	string macroPP::replace_param(string key, string s) {		
		string num = int_to_string(MACROTAB.find(key)->second.counter);
		string::size_type slash;	// Pointer
		slash = s.find("\\@");	// Invocation counter substitution
		if(slash != string::npos)	// Found \@
			s.replace(slash, 2, num);
		
		slash = s.find("\\0");	// macro_size substitution
		if(slash != string::npos) // Found \0
			s.replace(slash, 2, macro_size);

		for(unsigned int i = 1; i <= 9; i++) {
			slash = s.find("\\" + int_to_string(i));
			if((slash != string::npos) && i < param_vec.size()) // Found and in range
				s.replace(slash, 2, param_vec[i]);
		} // End for loop
		return s;
	} // End replace_param
	
	
	void macroPP::substitute(string key) {
		vector<definition> vec = MACROTAB.find(key)->second.def_vec;
		for(unsigned int i = 1; i < vec.size() - 1; i++) { // Skip first and last line
			string label, opcode, operand;
			label = replace_param(key, vec[i].label);	// Replace proper parameters
			opcode = replace_param(key, vec[i].opcode);
			operand = replace_param(key, vec[i].operand);
			insert_expansion(label, opcode, operand);	// Write to file with expansion
		} // End for loop
	} // End substitute
	
	
	void macroPP::write_first_line(string s) {
		new_file.open(expansion_file.c_str(), ios::app);	// Opens file and appends to end of file
		new_file << "* " << s << "\n";
		new_file.close();
	} // End write_first_line
	
	
	void macroPP::write_last_line() {
		new_file.open(expansion_file.c_str(), ios::app);	// Opens file and appends to end of file
		new_file << "*\tEND OF MACRO EXPANSION\n";
		new_file.close();
	} // End write_last_line
		
	
	void macroPP::find_macro() {
		for(int i = 0; i < parser->size(); i++) {
			macro_size = "";	// Set as empty string
			string opcode = parser->get_token(i, 1);
			opcode = check_dot(opcode);
			
			if(MACROTAB.count(opcode) != 0) { // We have the macro defintion
				write_first_line(opcode);
				fill_param(parser->get_token(i, 2));	// Pass in operands
				substitute(opcode);		// Substitute the proper values
				increment_counter(opcode);
				write_last_line();
			}
			else
				write_to_disk(expansion_file, parser, i);
		} // End for loop
		
	} // End find_macro
	
	
	void macroPP::clean_files() {
		if(remove(se_file.c_str()) != 0)	// Deletes file
			throw_exception(-1, "Unable to delete file {" + se_file + "}"); // -1 is a dummy index
		rename(expansion_file.c_str(), se_file.c_str());
	} // End clean_files
	
	
	void macroPP::second_pass() {
		new_file.open(expansion_file.c_str());	// Open file
		parse_se_file();
		find_macro();	// Find macro, substitutes, and insert expansion
		clean_files();	// Delete and rename files
	} // End second_pass
	
	
	void macroPP::print_macros() {
		map<string,info>::iterator it;
		for(it = MACROTAB.begin(); it != MACROTAB.end(); it++) {
			vector<definition> vec = it->second.def_vec;	// Access value's info struct's def_vec
			vector<definition>::iterator val_it; // Iterates through def vec
			for(val_it = vec.begin(); val_it != vec.end(); val_it++) {
					cout << left << setfill(' ') << setw(10) << val_it->label << "\t" 
						 << left << setfill(' ') << setw(10) << val_it->opcode << "\t" 
						 << left << setfill(' ') << setw(10) << val_it->operand << "\n";
			} // End inner for loop
		} // End for loop
	} // End print_macros
	

	int main(int argc, char ** argv) {
		if (argc != 2) {	
			cout << "usage: " << argv[0] << " <filename>" << endl;
			return 1;
		}
		try {
			macroPP macro(argv[1]);
			macro.process(macro.parser);	// First pass grabs macro definitions
			macro.second_pass();			// Second pass invokes macro definitions
		} catch(macroPP_error_exception& e) {
			cout << e.get_message() << endl;
		} // End try-catch
		
		return 0;
	} // End main class
