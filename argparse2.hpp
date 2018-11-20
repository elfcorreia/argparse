#ifndef ARGPARSE_HPP_
#define ARGPARSE_HPP_

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <cstdio>
#include <cstddef>

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
	ArgumentBuilder(const ArgumentBuilder &other)
		: _arg(other._arg) { }
	~ArgumentBuilder() { }
	
	ArgumentBuilder &store_const(std::string value) {
		_arg->action = ActionStoreConst;
		_arg->const_value = value;
		return *this;
	}

	ArgumentBuilder &store_true() {
		_arg->action = ActionStoreConst;
		_arg->const_value = true;
		return *this;
	}
	
	ArgumentBuilder &store_false() {
		_arg->action = ActionStoreConst;
		_arg->const_value = false;
		return *this;
	}

	ArgumentBuilder &append() {
		_arg->action = ActionAppend;		
		return *this;
	}

	ArgumentBuilder &append_const(std::string value) {
		_arg->action = ActionAppend;
		_arg->const_value = value;
		return *this;
	}
	
	ArgumentBuilder &count() {
		_arg->action = ActionCount;
		return *this;
	}
	
	ArgumentBuilder &version() {
		_arg->action = ActionVersion;
		return *this;
	}

	ArgumentBuilder &help() {
		_arg->action = ActionHelp;
		return *this;
	}

	ArgumentBuilder& nargs(char value) {
		_arg->nargs = value;
		return *this;
	}

	ArgumentBuilder& default_value(std::string value) {
		_arg->default_value = value;
		return *this;
	}

	ArgumentBuilder& choices(std::initializer_list<std::string> value) {
		_arg->choices = value;
		return *this;
	}

	ArgumentBuilder& required() {
		_arg->required = true;
		return *this;
	}

	ArgumentBuilder& help(std::string value) {
		_arg->help = value;
		return *this;
	} 

	ArgumentBuilder& metavar(std::string value) {
		_arg->metavar = value;
		return *this;
	};
};

class ArgumentParser {
private:
	std::map<std::string, Argument> _optional_arguments;
	std::vector<Argument> _positional_arguments;
public:	
	ArgumentParser(std::string description = "") { }
	~ArgumentParser() { }

	const ArgumentBuilder add_argument(std::string name_or_flag) {
		Argument a;
		a.names.push_back(name_or_flag);
		a.action = ActionStore;
		a.dest = name_or_flag;
		a.nargs = '?';
		bool optional_argument = name_or_flag.at(0) == '-';
		if (optional_argument) {
			a.required = false;
			_optional_arguments[name_or_flag] = a;
			return ArgumentBuilder(&(_optional_arguments[name_or_flag]));
		} else {			
			a.required = true;
			_positional_arguments.push_back(a);
			return ArgumentBuilder(&(_positional_arguments[_positional_arguments.size() - 1]));
		}
	}
	
	const ArgumentBuilder add_argument(std::string short_name, std::string long_name) {
		Argument a;
		a.names.push_back(short_name);
		a.names.push_back(long_name);
		a.action = ActionStore;
		a.dest = long_name;
		a.nargs = '?';
		a.required = false;
		_optional_arguments[long_name] = a;
		_optional_arguments[short_name] = a;
		return ArgumentBuilder(&(_optional_arguments[short_name]));
	}
	
	std::map<std::string, std::string> parse_args(std::string args) {		
		std::vector<std::string> aux;
		split(aux, args);
		return parse_args(aux);
	}

	//std::map<std::string, Value> parse_args(int argc, char const *argv[]) {
//		
	//}

	std::map<std::string, std::string> parse_args(std::vector<std::string> &args) {
		std::map<std::string, std::string> aux;
		std::size_t i = 0;
		int cur_positional_argument = 0;		
		while (i < args.size()) {
			//printf("handling %d -> %s\n", i, args[i].c_str());
			bool optional_argument = args[i].at(0) == '-';

			if (optional_argument) {
				std::string key = std::string(args[i]);
				if (_optional_arguments.find(key) == _optional_arguments.end()) {
					fprintf(stderr, "option %s unknowed", args[i].c_str());
					exit(EXIT_FAILURE);
				}
				Argument a = _optional_arguments[key];
				switch (a.action) {
					case ActionStore: {
						aux[a.dest] = args[i + 1];
						i += 2;
						break;
					}
				}
			} else {				
				if (cur_positional_argument > _positional_arguments.size()) {
					fprintf(stderr, "option %s unknowed", args[i].c_str());
					exit(EXIT_FAILURE);
				}
				Argument a = _positional_arguments[cur_positional_argument];				
				switch (a.action) {
					case ActionStore: {						
						aux[a.dest] = args[i];
						i += 1;
						break;
					}
				}
				cur_positional_argument += 1;
			}			
		}
		return aux;
	}

	void print_help() { }

};

#endif