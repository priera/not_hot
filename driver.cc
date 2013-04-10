#include <stdio.h>      
#include <unistd.h>     // for getopt
#include <cstdlib>

#include <basic.h>
#include <cool-tree.h>
#include <stringtab.h>
#include "ParserDriver.h"
#include "semant_driver.h"
#include "cgen_driver.h"

using namespace flags;
using namespace parsing;

FILE *fin;   // This is the file pointer from which the lexer reads its input.

extern int optind;  // used for option processing (man 3 getopt for more info)

void print_usage_and_exit(char* execname){
	cerr << "usage: " << execname << 
#ifdef DEBUG
	" [-lvpscOgtTr -o outname] [input-files]\n";
#else
	" [-OgtT -o outname] [input-files]\n";
#endif
	exit(1);
}

CompilerFlags check_flags(int argc, char ** argv){
	
	CompilerFlags flags = CompilerFlags(argc, argv);
	
	if (flags.wasError())
		print_usage_and_exit(argv[0]);
	
	return flags;
}

void open_file(char * filename) {
	fin = fopen(filename, "r");
	if (fin == NULL) {
		cerr << "Could not open input file " << filename << endl;
		exit(1);
	}
}

void check_semant(Results r){
	using namespace semant;
	
	SemantDriver driver(r.get_classes());
	driver.check_semantics();
	
	if (driver.errors() > 0) {
		cerr << "Semantical errors found\n";
		exit(1);
	}
	
	//r.get_root()->dump_with_types(cout, 0);

}

void code(ostream & out){
	using namespace cgen;

	Driver driver(out);
	driver.code();
}

Results parse_file(CompilerFlags flags, char * filename) {
	
	ParserDriver p(flags.yy_flex_debug->getState(), flags.cool_yydebug->getState(), filename);
	p.parse();
	Results *r = p.results();
	
	if (r->get_errorsCount() != 0) {
		cerr << "Compilation halted due to lex and parse errors\n";
		exit(1);
	}
	
	//r->get_root()->dump_with_types(cout,0);
	return *r;
	
}

void close_file() {
	fclose(fin);
}

ofstream * getOutputStream(int argc, char ** argv){
	char *out_filename = NULL;    // name of output assembly
	
	char *dot = strrchr(argv[1], '.');
	if (dot) *dot = '\0'; // strip off file extension
	out_filename = new char[strlen(argv[1])+8];
	strcpy(out_filename, argv[1]);
	strcat(out_filename, ".s");

	ofstream * s = new ofstream(out_filename);
	return s;
}

int main(int argc, char** argv) {
	
	CompilerFlags flags = check_flags(argc, argv);
	char * filename;
	Results r;
	
	//TODO: The compiler compiles the files file1.cl through filen.cl as if they were concatenated together.
	while (optind < argc) {
		filename = argv[optind];
		
		//cout << "#name \"" << filename << "\"" << endl;
		
		open_file(filename);
		
		r = parse_file(flags, filename);
		
		close_file();
		
		optind++;
	}
	
 	check_semant(r);
	
	code(*getOutputStream(argc, argv));
	
	exit(0);
}