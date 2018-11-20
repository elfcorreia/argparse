#include "argparse2.hpp"
#include <iostream>

using namespace std;

void print(map<string, string> &m) {
	for (auto it: m) {
		cout << it.first << ": " << it.second << ", ";
	}
	cout << endl;
}

void test_name() {
	ArgumentParser parser;
	parser.add_argument("-f", "--foo");
	parser.add_argument("bar");	

	auto args = parser.parse_args("BAR");
	print(args);
	
	args = parser.parse_args("BAR --foo FOO");
	print(args);
	
	args = parser.parse_args("--foo FOO");
	print(args);
}

//void test_action() {
	////store
	//ArgumentParser parser;
	//parser.add_argument("--foo");
	//parser.parse_args("--foo 1");	
//
	////store_const
	//ArgumentParser parser;
	//parser.add_argument("--foo").store_const(42);
	//parser.parse_args("--foo");		
//
	////store_true
	//ArgumentParser parser;
	//parser.add_argument("--foo").store_true();
	//parser.add_argument("--bar").store_false();
	//parser.add_argument("--baz").store_false();
	//parser.parse_args("--foo --bar");
//
	////append
	//ArgumentParser parser;
	//parser.add_argument("--foo").append();
	//parser.parse_args("--foo 1 --foo 2");
//
	////append_const
	//ArgumentParser parser;
	//parser.add_argument("--f1").dest("filters").append_const(f1);
	//parser.add_argument("--f2").dest("filters").append_const(f2);
	//parser.parse_args("--f1 1 --f2 2");	
//
	////count
	//ArgumentParser parser;
	//parser.add_argument("--verbose", "-v").count();
	//parser.parse_args("-vvv");
//
	////help
//
	////version
	//ArgumentParser parser;
	//parser.add_argument("--version").version("1.0");
	//parser.parse_args("--version");
//}
//
//void test_nargs() {
	//ArgumentParser parser;
	//parser.add_argument("--foo").nargs(2);
	//parser.add_argument("bar").nargs(1);
	//parser.parse_args("c --foo a b");	
//
	//ArgumentParser parser;
	//parser.add_argument("--foo").nargs('?');
	//parser.add_argument("bar").nargs('?');
	//parser.parse_args("c --foo a b");
//
	//ArgumentParser parser;
	//parser.add_argument("infile").nargs('?').default(stdin);
	//parser.add_argument("outfile").nargs('?').default(stdout);
//
	//ArgumentParser parser;
	//parser.add_argument("--foo").nargs('*');
	//parser.add_argument("--bar").nargs('*');
	//parser.add_argument("baz").nargs('*');
//
	//ArgumentParser parser;
	//parser.add_argument("--foo").nargs('+');
	//parser.add_argument("--bar").nargs('+');
	//parser.add_argument("baz").nargs('+');
//
	//ArgumentParser parser;
	//parser.add_argument("--foo").nargs(remainder);
	//parser.add_argument("--bar").nargs(remainder);
	//parser.add_argument("baz").nargs(remainder);
//}



int main(int argc, char const *argv[])
{
	test_name();
	return 0;
}