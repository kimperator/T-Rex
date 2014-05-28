#include "trex.h"
#include <stdio.h>
#include <string.h>

#ifdef _UNICODE
#define trex_sprintf swprintf
#else
#define trex_sprintf sprintf
#endif

/* test edited by Kai Uwe Jesussek for parsing html*/

void trex_test(const TRexChar *regex, const TRexChar *text, const TRexBool expected) {
	const TRexChar *error = NULL;
	TRex *regex_compiled = trex_compile(_TREXC(regex), &error);

	// Check for regex compilation errors
	if (!regex_compiled) {
		trex_printf(_TREXC("Compilation error [%s]!\n"), error ? error : _TREXC("undefined"));
	}

	TRexBool result = trex_match(regex_compiled, text);

	#ifdef _DEBUG
		printf("DEBUG trex_test: result = %d\n", result);
		printf("DEBUG trex_test: expected = %d\n", expected);
	#endif

	// Print matching outcome compared to expected outcome
	if (result == expected) {
		if (result == TRex_True) {
			trex_printf("True positive: String '%s' matched.\n", regex);
		} else {
			trex_printf("True negative: No matches for '%s'.\n", regex);
		}
	} else {
		if (result == TRex_True) {
			trex_printf("FALSE POSITIVE: STRING '%s' SHOULD NOT HAVE MATCHED!\n", regex);
		} else {
			trex_printf("FALSE NEGATIVE: STRING '%s' SHOULD HAVE MATCHED.\n", regex);
		}
	}

	trex_free(regex_compiled);
}

int main(int argc, char* argv[])
{
	const TRexChar *begin,*end;
	TRexChar sTemp[200];
	const TRexChar *error = NULL;
	TRex *x = trex_compile(_TREXC("<a href=[\"|'](.*)[\"|']>(.*)</a>"),&error);
	if(x) {
		trex_sprintf(sTemp,_TREXC("<html><head></head><body><a href='link.html'>link</a></body></html>"));
		if(trex_search(x,sTemp,&begin,&end))
		{
			int i,n = trex_getsubexpcount(x);
			TRexMatch match;
			for(i = 0; i < n; i++)
			{
				TRexChar t[200];
				trex_getsubexp(x,i,&match);
				trex_sprintf(t,_TREXC("[%%d]%%.%ds\n"),match.len);
				trex_printf(t,i,match.begin);
			}
			trex_printf(_TREXC("match! %d sub matches\n"),trex_getsubexpcount(x));
		}
		else {
			trex_printf(_TREXC("no match!\n"));
		}
		trex_free(x);
	}
	else {
		trex_printf(_TREXC("compilation error [%s]!\n"),error?error:_TREXC("undefined"));
	}

	// Negative testing for strings that shouldn't match
	TRexChar regex_str[200];
	TRexChar test_string[200];
	trex_sprintf(test_string, _TREXC("Match some part of this string."));

	trex_printf("=============================\n");
	trex_printf("Simple matching string cases.\n");
	trex_printf("Test string: %s\n", test_string);

	trex_printf("Test 1: missing letter\n");
	trex_sprintf(regex_str, _TREXC("soe"));
	trex_test(regex_str, test_string, TRex_False);

	trex_printf("Test 2: missing space\n");
	trex_sprintf(regex_str, _TREXC("matchs"));
	trex_test(regex_str, test_string, TRex_False);

	trex_printf("Test 3a: case mismatch\n");
	trex_sprintf(regex_str, _TREXC("match"));
	trex_test(regex_str,test_string, TRex_False);
	trex_printf("Test 3b: case match\n");
	trex_sprintf(regex_str, _TREXC("Match"));
	trex_test(regex_str,test_string, TRex_True);

	trex_printf("Test 4: unused character\n");
	trex_sprintf(regex_str, _TREXC("!"));
	trex_test(regex_str, test_string, TRex_False);

	trex_printf("Test 5: single character\n");
	trex_sprintf(regex_str, _TREXC("."));
	trex_test(regex_str, test_string, TRex_True);
	// End negative testing

	return 0;
}
