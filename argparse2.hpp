#ifndef ARGPARSE_HPP_
#define ARGPARSE_HPP_

#include <string>
#include <variant>
#include <vector>
#include <map>

typedef std::variant<int, double, char, bool, const char*> Value;
//union Value {
	//int i;
	//char c;
	//float f;
	//double d;
	//bool b;
//};

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
	Value default_value;
	Value const_value;
	std::initializer_list<Value> choices;
	bool required;
	std::string help; 
	std::string metavar;
};

class ArgumentBuilder {
private:
	Argument *_arg;
public:
	ArgumentBuilder(Argument* arg) 
		: _arg(arg) { }
	ArgumentBuilder(const ArgumentBuilder &other)
		: _arg(other._arg) { }
	~ArgumentBuilder() { }
	
	ArgumentBuilder &store_const(Value value) {
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

	ArgumentBuilder &append_const(Value value) {
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

	ArgumentBuilder& default_value(Value value) {
		_arg->default_value = value;
		return *this;
	}

	ArgumentBuilder& choices(std::initializer_list<Value> value) {
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
	std::vector<Argument*> _arguments;
public:	
	ArgumentParser(std::string description = "") { }
	~ArgumentParser() { }

	const ArgumentBuilder add_argument(std::string name_or_flag) {
		Argument *a = new Argument();
		a->names.push_back(name_or_flag);
		_arguments.push_back(a);
		return ArgumentBuilder(a);
	}
	
	const ArgumentBuilder add_argument(std::string short_name, std::string long_name) {
		Argument *a = new Argument();
		_arguments.push_back(a);
		a->names.push_back(short_name);
		a->names.push_back(long_name);
		return ArgumentBuilder(a);
	}
	
	std::map<std::string, Value> parse_args(std::string args) {
		return parse_args(0, nullptr);
	}

	std::map<std::string, Value> parse_args(int argc, char const *argv[]) {
		int i = 0;
		while (i < argc) {
			//if (argv[i])
		}
		std::map<std::string, Value> aux;
		return aux;
	}

	void print_help() { }

};

#endif