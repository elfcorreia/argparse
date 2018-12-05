#ifndef ARGPARSE_HPP_
#define ARGPARSE_HPP_

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <cstdio>
#include <cstddef>
#include <iostream>

enum Action {
	ActionStore,
	ActionStoreConst,
	ActionAppend,
	ActionAppendConst,
	ActionCount,
	ActionHelp,
	ActionVersion,
};

struct Argument {
	std::vector<std::string> names;
	Action action;
	char nargs;
	std::string default_value;
	std::string const_value;
	std::string dest;
	std::initializer_list<std::string> choices;
	bool required;
	std::string help; 
	std::string metavar;
};

//split code
//http://www.cplusplus.com/faq/sequences/strings/split/

void split(std::vector<std::string> &result, const std::string& s) {
	result.clear();
	size_t current;
	size_t next = -1;
	do {
		next = s.find_first_not_of(" ", next + 1 );
		if (next == std::string::npos) break;
		next -= 1;
		current = next + 1;
		next = s.find_first_of(" ", current);
		result.push_back(s.substr(current, next - current));
	} while (next != std::string::npos);
}

class ArgumentBuilder {
private:
	Argument *_arg;
public:
	ArgumentBuilder(Argument* arg) 
		: _arg(arg) { }
	ArgumentBuilder(const ArgumentBuilder & other)
		: _arg(other._arg) { }
	~ArgumentBuilder() { }
	
	const ArgumentBuilder &store_const(std::string value) {
		_arg->action = ActionStoreConst;
		_arg->const_value = value;
		return *this;
	}

	const ArgumentBuilder &store_true() {
		_arg->action = ActionStoreConst;
		_arg->const_value = true;
		return *this;
	}
	
	const ArgumentBuilder &store_false() {
		_arg->action = ActionStoreConst;
		_arg->const_value = false;
		return *this;
	}

	const ArgumentBuilder &append() {
		_arg->action = ActionAppend;		
		return *this;
	}

	const ArgumentBuilder &append_const(std::string value) {
		_arg->action = ActionAppend;
		_arg->const_value = value;
		return *this;
	}
	
	const ArgumentBuilder &count() {
		_arg->action = ActionCount;
		return *this;
	}
	
	const ArgumentBuilder &version() {
		_arg->action = ActionVersion;
		return *this;
	}

	const ArgumentBuilder &help() {
		_arg->action = ActionHelp;
		return *this;
	}

	const ArgumentBuilder& nargs(char value) {
		_arg->nargs = value;
		return *this;
	}

	const ArgumentBuilder& default_value(std::string value) const {
		_arg->default_value = value;
		return *this;
	}

	const ArgumentBuilder& choices(std::initializer_list<std::string> &value) {
		_arg->choices = value;
		return *this;
	}

	const ArgumentBuilder& required() {
		_arg->required = true;
		return *this;
	}

	const ArgumentBuilder& help(std::string value) {
		_arg->help = value;
		return *this;
	} 

	const ArgumentBuilder& metavar(std::string value) {
		_arg->metavar = value;
		return *this;
	};
};

class ArgumentParser {
private:
	std::map<std::string, Argument*> _optional_arguments;
	std::vector<Argument*> _positional_arguments;
	bool _exit_on_failure;
	const char *_description;
public:	
	ArgumentParser(const char *description = nullptr, bool exit_on_failure=true) {
		_description = description;
		_exit_on_failure = exit_on_failure;
	}
	~ArgumentParser() { }

	void print_args() {
		for (auto it: _optional_arguments) {
			std::cout << it.first << ": " << it.second->default_value << ", ";
		}
		std::cout << std::endl;
	}

	const ArgumentBuilder add_argument(std::string name_or_flag) {
		Argument* a = new Argument;
		a->names.push_back(name_or_flag);
		a->action = ActionStore;
		a->dest = name_or_flag;
		a->nargs = '?';
		bool optional_argument = name_or_flag.at(0) == '-';
		if (optional_argument) {
			a->required = false;
			_optional_arguments[name_or_flag] = a;
			return ArgumentBuilder(a);
		} else {			
			a->required = true;
			_positional_arguments.push_back(a);
			return ArgumentBuilder(a);
		}
	}
	
	const ArgumentBuilder add_argument(std::string short_name, std::string long_name) {
		Argument *a = new Argument;
		a->names.push_back(short_name);
		a->names.push_back(long_name);
		a->action = ActionStore;
		a->dest = long_name;
		a->nargs = '?';
		a->required = false;
		_optional_arguments[long_name] = a;
		_optional_arguments[short_name] = a;
		return ArgumentBuilder(a);
	}
	
	std::map<std::string, std::string> parse_args(std::string args) {		
		std::vector<std::string> aux;
		split(aux, args);
		return parse_args(aux);
	}

	//std::map<std::string, Value> parse_args(int argc, char const *argv[]) {
	//		
	//}

	void handle_unknown_option(const std::string &arg) {
		if (_exit_on_failure) {
			fprintf(stderr, "Unknown option \"%s\"\n", arg.c_str());
			fprintf(stderr, "Use -h for help\n");
			exit(EXIT_FAILURE);		
		} else {
			throw "Unknown option";
		}
	}

	std::map<std::string, std::string> parse_args(std::vector<std::string> &args) {
		std::map<std::string, std::string> aux;
		for (auto it :_optional_arguments) {
			aux[it.first] = it.second->default_value;
		}		
		std::size_t i = 0;
		int cur_positional_argument = 0;		
		while (i < args.size()) {
			//printf("handling %d -> %s\n", i, args[i].c_str());
			bool optional_argument = args[i].at(0) == '-';
			std::string arg = args[i];

			if (optional_argument) {			
				if (_optional_arguments.find(arg) == _optional_arguments.end()) {
					handle_unknown_option(arg);
				}
				Argument *a = _optional_arguments[arg];
				switch (a->action) {
					case ActionStore: {
						//push n args
						aux[a->dest] = args[i + 1];
						if (a->choices.size() > 0) {
							//check choices
						}
						i += 2;
						break;
					}
				}
			} else {				
				if (cur_positional_argument >= _positional_arguments.size()) {
					handle_unknown_option(arg);
				}
				Argument *a = _positional_arguments[cur_positional_argument];				
				switch (a->action) {
					case ActionStore: {						
						aux[a->dest] = args[i];
						i += 1;
						break;
					}
				}
				cur_positional_argument += 1;
			}			
		}
		return aux;
	}

	void print_help() {

	}

};

#endif