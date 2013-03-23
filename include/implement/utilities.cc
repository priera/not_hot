#include "utilities.h"

//////////////////////////////////////////////////////////////////////////////
//
//  utilities.c
//
//  General support code for lexer and parser.
//
//  This file contains:
//      fatal_error            print an error message and exit
//      print_escaped_string   print a string showing escape characters
//      print_cool_token       print a cool token and its semantic value
//      dump_cool_token        dump a readable token representation
//      strdup                 duplicate a string (missing from some libraries)
//
///////////////////////////////////////////////////////////////////////////////

// #define CHECK_TABLES

// sm: fixed an off-by-one error here; code assumed there were 80 spaces, but
// in fact only 79 spaces were there; I've made it 80 now
//                                1         2         3         4         5         6         7
//                      01234567890123456789012345678901234567890123456789012345678901234567890123456789
static char *padding = "                                                                                ";      // 80 spaces for padding

void fatal_error(char *msg)
{
   cerr << msg;
   exit(1);
}


void print_escaped_string(ostream& str, const char *s)
{
  while (*s) {
    switch (*s) {
    case '\\' : str << "\\\\"; break;
    case '\"' : str << "\\\""; break;
    case '\n' : str << "\\n"; break;
    case '\t' : str << "\\t"; break;
    case '\b' : str << "\\b"; break;
    case '\f' : str << "\\f"; break;

    default:
      if (isprint(*s))
	str << *s;
      else 
	// 
	// Unprintable characters are printed using octal equivalents.
	// To get the sign of the octal number correct, the character
	// must be cast to an unsigned char before coverting it to an
	// integer.
	//
	str << '\\' << oct << setfill('0') << setw(3)
	    << (int) ((unsigned char) (*s))
	    << dec << setfill(' ');
      break;
    }
    s++;
  }
}

// //
// // Decstations don't have strdup in the library.
// //
// char *strdup(const char *s)
// {
//   char *news;
// 
//   if (s == NULL) return(NULL);
// 
//   news = (char *)malloc(strlen(s)+1);
//   strcpy(news, s);
//   return(news);
// }

///////////////////////////////////////////////////////////////////////////
//
// pad
//
// function to add pad
//
///////////////////////////////////////////////////////////////////////////
char *pad(int n) {
    if (n > 80) return padding;
    if (n <= 0)  return "";
    return padding+(80-n);
}

/**Returns the string "a" to lower-case string, and returns the new string*/
char * changeCase(char * a) {     
	
	char *ret = new char[strlen(a) +1];
	unsigned int i = 0;
	
	for (i=0; i<strlen(a); i++) {
		ret[i] = (char)tolower(a[i]);
	}
	
	ret[strlen(a)] = 0;
	return ret;
};
