#ifndef DHARMA_VM_RUNTIME_HPP
#define DHARMA_VM_RUNTIME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <regex>
#include <sstream>

using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::cerr;
using std::to_string;
using std::pow;
using std::tuple;
using std::move;
using std::make_pair;
using std::get;
using std::stoi;
using std::regex;
using std::sregex_token_iterator;
using std::find_if;
using std::stringstream;
using std::getline;
using std::for_each;

namespace dharma_vm {

	class type_information;
	class runtime;

	class vm_instruction_list {
	public:
		const static string mov;
		const static string inc;
		const static string dec;
		const static string cmpl;
		const static string neg;
		const static string pos;
		const static string bneg;
		const static string tyof;
		const static string add;
		const static string mul;
		const static string div;
		const static string mod;
		const static string exp;
		const static string sub;
		const static string shl;
		const static string shr;
		const static string equ;
		const static string nequ;
		const static string gt;
		const static string lt;
		const static string gte;
		const static string lte;
		const static string band;
		const static string bor;
		const static string exor;
		const static string lor;
		const static string land;
		const static string cast;
		const static string exit;
		const static string jmp;
		const static string list;
		const static string tupl;
		const static string dict;
		const static string func;
		const static string efunc;
		const static string struc;
		const static string estruc;
		const static string istruc;
		const static string module;
		const static string imodule;
		const static string emodule;
		const static string imov;
		const static string ret;
		const static string _enum;
		const static string ifunc;
		const static string scope;
		const static string escope;
		const static string dmov;
		const static string lambda;
		const static string ilambda;
		const static string elambda;
		const static string brk;
		const static string cont;
	};

	class type_information_list {
	public:
		const static type_information _int;
		const static type_information _decimal;
		const static type_information _boolean;
		const static type_information _string;
		const static type_information _list;
		const static type_information _tuple;
		const static type_information _dict;
		const static type_information _pure_int;
		const static type_information _pure_decimal;
		const static type_information _pure_string;
		const static type_information _pure_boolean;
		const static type_information _pure_list;
		const static type_information _pure_tuple;
		const static type_information _pure_dict;
		const static type_information _pure_nil;
		const static type_information _nil;
		const static type_information bad;
		const static type_information _func;
		const static type_information _pure_func;
	};

	enum type_kind {
		TYPE_INT, TYPE_DECIMAL, TYPE_BOOLEAN, TYPE_STRING, TYPE_LIST, TYPE_TUPLE, TYPE_DICT, TYPE_NIL, TYPE_FUNC,
		TYPE_ENUM, TYPE_ENUM_CHILD, TYPE_CUSTOM, TYPE_MODULE, TYPE_NONE
	};

	enum type_pure_kind {
		TYPE_PURE_YES, TYPE_PURE_NO, TYPE_PURE_NONE
	};

	enum type_class_kind {
		TYPE_CLASS_YES, TYPE_CLASS_NO, TYPE_CLASS_NONE
	};

	class type_information {
		type_kind t_kind;
		type_pure_kind tp_kind;
		type_class_kind tc_kind;
		string class_name;
	public:
		type_information(type_kind tk, type_pure_kind tpk, type_class_kind tck, string cn);
		~type_information();
		const type_kind get_type_kind();
		const type_pure_kind get_type_pure_kind();
		const type_class_kind get_type_class_kind();
		string get_class_name();
		const bool operator==(type_information ti);
		const bool operator!=(type_information ti);
	};

	enum storage_field_kind {
		STORAGE_FIELD_REGISTER_NUMBER, STORAGE_FIELD_IDENTIFIER
	};

	struct storage_field {
		int register_number;
		string identifier;
		storage_field_kind sf_kind;
	public:
		storage_field(int rn, string i, storage_field_kind sfk);
		~storage_field();
		int get_register_number();
		string get_identifier();
		storage_field_kind get_storage_field_kind();
	};

	class runtime_variable {
		int integer;
		float decimal;
		string str;
		bool boolean;
		vector<shared_ptr<runtime_variable>> list_tuple;
		pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> dict;
		vector<shared_ptr<runtime_variable>> struct_enum_member_list;
		shared_ptr<runtime> module_runtime;
		type_information t_inf;
		storage_field s_field;
		bool unmodifiable;
	public:
		runtime_variable(storage_field s_field, int i, float d, string s, bool b, vector<shared_ptr<runtime_variable>> lt, pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> di,
			vector<shared_ptr<runtime_variable>> sml, shared_ptr<runtime> mr, type_information ti);
		~runtime_variable();
		int get_integer();
		float get_decimal();
		string get_string();
		bool get_boolean();
		vector<shared_ptr<runtime_variable>> get_list_tuple();
		pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> get_dict();
		int set_integer(int i);
		float set_decimal(float f);
		string set_string(string s);
		bool set_boolean(bool b);
		vector<shared_ptr<runtime_variable>> set_list_tuple(vector<shared_ptr<runtime_variable>> lt);
		pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> d);
		type_information get_type_information();
		type_information set_type_information(type_information t);
		storage_field get_storage_field();
		const bool get_unmodifiable();
		bool set_unmodifiable(bool b);
		vector<shared_ptr<runtime_variable>> get_struct_enum_member_list();
		vector<shared_ptr<runtime_variable>> set_struct_enum_member_list(vector<shared_ptr<runtime_variable>> sml);
		shared_ptr<runtime> get_module_runtime();
		shared_ptr<runtime> set_module_runtime(shared_ptr<runtime> mr);
		storage_field set_storage_field(storage_field sf);
		shared_ptr<runtime_variable> function_parameter_mov(shared_ptr<runtime_variable> &rvar);
	};

	const bool equals_equals(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	const bool not_equals(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator+(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator-(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator*(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator/(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator%(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator^(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator&(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator|(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> pow(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator<(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator>(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator>=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator<=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator==(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator!=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator&&(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator||(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator!(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> operator-(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> operator~(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> tyof(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> cast(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> inc(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> dec(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> strict_mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> function_parameter_mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);

	bool report_error_and_terminate_program(string msg, shared_ptr<runtime_variable> rvar);

	class runtime_diagnostic_messages {
	public:
		const static string incompatible_types;
		const static string malformed_instruction;
		const static string fatal_error;
		const static string name_not_found;
		const static string expected_sequence_for_subscript;
		const static string subscript_out_of_range;
		const static string unmodifiable_value;
		const static string key_not_found;
		const static string field_not_found;
		const static string function_overload_not_found;
		const static string structure_members_are_initialized_early;
	};

	const vector<string> list_field_list = { "size" };
	const vector<string> tuple_field_list = { "size" };
	const vector<string> dict_field_list = {};
	const vector<string> string_field_list = { "size" };

	enum register_identifier_kind {
		REGISTER_IDENTIFIER_REGISTER, REGISTER_IDENTIFIER_IDENTIFIER, REGISTER_IDENTIFIER_COMPLEX, REGISTER_IDENTIFIER_LITERAL, REGISTER_IDENTIFIER_NONE
	};

	class function {
		string function_name;
		vector<string> function_code;
		vector<string> function_argument_list;
		shared_ptr<runtime_variable> function_variable;
		bool va_args;
		bool lambda;
	public:
		function(string fn, vector<string> fc, vector<string> fal, shared_ptr<runtime_variable> fv, bool va, bool l);
		~function();
		string get_function_name();
		vector<string> get_function_code();
		vector<string> get_function_argument_list();
		shared_ptr<runtime_variable> get_function_variable();
		const bool get_va_args();
		const bool is_lambda();
	};

	class builtins {
		public:
			const static string builtin__va_args__;
			const static string va_args_function_parameter;
			const static string builtin_print;
	};

	class runtime {
		vector<shared_ptr<runtime_variable>> instruction_list;
		vector<shared_ptr<function>> function_list;
		vector<string> string_instruction_list;
		const static string runtime_temporary_prefix;
		int runtime_temporary_count;
		vector<vector<shared_ptr<runtime_variable>>> stacked_function_instruction_list;
		vector<vector<shared_ptr<runtime_variable>>> scope_stack;
		vector<shared_ptr<function>> added_lambda_function_list;
		vector<shared_ptr<runtime_variable>> added_lambda_instruction_list;

		vector<string> parse_instruction(string insn);
		tuple<string, register_identifier_kind, type_kind> deduce_register_identifier_kind(string ident);
		shared_ptr<runtime_variable> deduce_runtime_variable(string ident, bool must_exist, bool dmov_override = false);
		const bool function_pass();
		shared_ptr<runtime_variable> run_function(shared_ptr<function> func, shared_ptr<runtime_variable> fvar, vector<shared_ptr<runtime_variable>> argument_list);
		shared_ptr<runtime_variable> checked_insertion(shared_ptr<runtime_variable> rvar);
		const bool struct_pass();
		const bool enum_pass();

		shared_ptr<runtime_variable> print(shared_ptr<runtime_variable> rvar);
		public:
			runtime(vector<string> vec, vector<shared_ptr<runtime_variable>> il, vector<shared_ptr<function>> fl, vector<vector<shared_ptr<runtime_variable>>> sfil,
				vector<vector<shared_ptr<runtime_variable>>> ls);
			~runtime();
			shared_ptr<runtime_variable> run_program();
			bool dump_runtime_variables(vector<shared_ptr<runtime_variable>> insn_list);
			pair<shared_ptr<runtime_variable>, bool> find_instruction(string ident);
			pair<shared_ptr<runtime_variable>, bool> find_instruction(int reg);
			vector<shared_ptr<runtime_variable>> get_instruction_list();
			vector<shared_ptr<function>> get_function_list();
			vector<shared_ptr<runtime_variable>> set_instruction_list(vector<shared_ptr<runtime_variable>> insn_list);
			vector<shared_ptr<function>> set_function_list(vector<shared_ptr<function>> fn_list);
			vector<vector<shared_ptr<runtime_variable>>> get_stacked_function_instruction_list();
			vector<vector<shared_ptr<runtime_variable>>> get_scope_stack();
			vector<vector<shared_ptr<runtime_variable>>> set_stacked_function_instruction_list(vector<vector<shared_ptr<runtime_variable>>> sfil);
			vector<vector<shared_ptr<runtime_variable>>> set_scope_stack(vector<vector<shared_ptr<runtime_variable>>> ssl);
			vector<string> get_string_instruction_list();
			vector<string> set_string_instruction_list(vector<string> vec);
			vector<shared_ptr<runtime_variable>> get_added_lambda_instruction_list();
			vector<shared_ptr<function>> get_added_lambda_function_list();
	};
}

#endif