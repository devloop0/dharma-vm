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
#include <chrono>
#include "includes/unified.hpp"
#include "includes/file.hpp"
#include "includes/vm.hpp"
#include "includes/libkarmaffi.hpp"

#if _WIN32
#include <Windows.h>
#endif

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
using std::static_pointer_cast;

using namespace unified_includes;
using namespace libkarmaffi;
using namespace std::chrono;

namespace dharma_vm {

	extern vector<shared_ptr<runtime_variable>> global_import_table;

	class function;
	class runtime_type_information;
	class runtime;

	class runtime_type_information_list {
	public:
		const static runtime_type_information _int;
		const static runtime_type_information _decimal;
		const static runtime_type_information _boolean;
		const static runtime_type_information _string;
		const static runtime_type_information _list;
		const static runtime_type_information _tuple;
		const static runtime_type_information _dict;
		const static runtime_type_information _pure_int;
		const static runtime_type_information _pure_decimal;
		const static runtime_type_information _pure_string;
		const static runtime_type_information _pure_boolean;
		const static runtime_type_information _pure_list;
		const static runtime_type_information _pure_tuple;
		const static runtime_type_information _pure_dict;
		const static runtime_type_information _pure_nil;
		const static runtime_type_information _nil;
		const static runtime_type_information bad;
		const static runtime_type_information _func;
		const static runtime_type_information _pure_func;
	};

	enum runtime_type_kind {
		TYPE_INT, TYPE_DECIMAL, TYPE_BOOLEAN, TYPE_STRING, TYPE_LIST, TYPE_TUPLE, TYPE_DICT, TYPE_NIL, TYPE_FUNC,
		TYPE_ENUM, TYPE_ENUM_CHILD, TYPE_CUSTOM, TYPE_MODULE, TYPE_NONE
	};

	class runtime_type_information {
		runtime_type_kind t_kind;
		type_pure_kind tp_kind;
		type_class_kind tc_kind;
		string class_name;
	public:
		runtime_type_information(runtime_type_kind tk, type_pure_kind tpk, type_class_kind tck, string cn);
		~runtime_type_information();
		const runtime_type_kind get_runtime_type_kind();
		const type_pure_kind get_type_pure_kind();
		const type_class_kind get_type_class_kind();
		string get_class_name();
		const bool operator==(runtime_type_information ti);
		const bool operator!=(runtime_type_information ti);
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
		runtime_type_information t_inf;
		storage_field s_field;
		bool unmodifiable;
		vector<shared_ptr<function>> func;
		unsigned long long unique_id;
		unsigned long long enum_parent_unique_id;
	public:
		runtime_variable(storage_field s_field, int i, float d, string s, bool b, vector<shared_ptr<runtime_variable>> lt, pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> di,
			vector<shared_ptr<runtime_variable>> sml, shared_ptr<runtime> mr, runtime_type_information ti, vector<shared_ptr<function>> fl);
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
		runtime_type_information get_runtime_type_information();
		runtime_type_information set_runtime_type_information(runtime_type_information t);
		storage_field get_storage_field();
		const bool get_unmodifiable();
		bool set_unmodifiable(bool b);
		vector<shared_ptr<runtime_variable>> get_struct_enum_member_list();
		vector<shared_ptr<runtime_variable>> set_struct_enum_member_list(vector<shared_ptr<runtime_variable>> sml, long ui, bool deep = false);
		shared_ptr<runtime> get_module_runtime();
		shared_ptr<runtime> set_module_runtime(shared_ptr<runtime> mr, long ui, bool deep = false);
		vector<shared_ptr<function>> set_function(vector<shared_ptr<function>> fl, long ui);
		vector<shared_ptr<function>> get_function();
		vector<shared_ptr<function>> add_function(shared_ptr<function> f);
		unsigned long long set_unique_id(unsigned long long ui);
		unsigned long long get_unique_id();
		unsigned long long set_enum_parent_unique_id(unsigned long long ui);
		unsigned long long get_enum_parent_unique_id();
		storage_field set_storage_field(storage_field sf);
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
	shared_ptr<runtime_variable> operator<<(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> operator>>(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
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
	shared_ptr<runtime_variable> operator+(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> operator~(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> tyof(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> cast(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src);
	shared_ptr<runtime_variable> mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src, bool dmov);
	shared_ptr<runtime_variable> inc(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> dec(shared_ptr<runtime_variable> dest);
	shared_ptr<runtime_variable> strict_mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src, bool dmov);
	const bool operator==(shared_ptr<function> dest, shared_ptr<function> src);

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
		const static string key_already_exists;
		const static string field_not_found;
		const static string function_overload_not_found;
		const static string structure_members_are_initialized_early;
		const static string import_file_not_found;
		const static string import_file_not_found_but_source_file_present;
		const static string use_the_add_function_to_add_an_element_to_an_empty_list;
		const static string cannot_remove_from_an_empty_dictionary;
		const static string native_function_not_found;
		const static string unsupported_type_to_export_to_native;
		const static string cannot_take_the_type_of_an_already_pure_type;
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
		bool va_args;
		bool lambda;
		bool builtin;
	public:
		function(string fn, vector<string> fc, vector<string> fal, bool va, bool l, bool b);
		~function();
		string get_function_name();
		vector<string> get_function_code();
		vector<string> get_function_argument_list();
		const bool get_va_args();
		const bool is_lambda();
		shared_ptr<function> set_function(shared_ptr<function> f);
		const bool is_builtin();
	};

	class runtime {
		vector<shared_ptr<runtime_variable>> instruction_list;
		vector<string> string_instruction_list;
		const static string runtime_temporary_prefix;
		int runtime_temporary_count;
		vector<vector<shared_ptr<runtime_variable>>> stacked_function_instruction_list;
		vector<vector<shared_ptr<runtime_variable>>> scope_stack;
		vector<vector<shared_ptr<runtime_variable>>> module_stack;
		vector<shared_ptr<runtime_variable>> added_lambda_instruction_list;

		vector<string> parse_instruction(string insn);
		tuple<string, register_identifier_kind, runtime_type_kind> deduce_register_identifier_kind(string ident);
		shared_ptr<runtime_variable> deduce_runtime_variable(string ident, bool must_exist, bool dmov_override = false);
		const bool function_pass();
		shared_ptr<runtime_variable> run_function(vector<shared_ptr<function>> func_list, shared_ptr<runtime_variable> fvar, vector<shared_ptr<runtime_variable>> argument_list, shared_ptr<runtime> r);
		shared_ptr<runtime_variable> checked_insertion(shared_ptr<runtime_variable> rvar);
		const bool struct_pass();
		const bool enum_pass();

		vector<shared_ptr<locked_runtime_variable>> convert_to_ffi(vector<shared_ptr<runtime_variable>> argument_list);
		shared_ptr<runtime_variable> convert_from_ffi(shared_ptr<locked_runtime_variable> result);

		shared_ptr<runtime_variable> print(shared_ptr<runtime_variable> rvar);
		shared_ptr<runtime_variable> exit(shared_ptr<runtime_variable> exit_code, shared_ptr<runtime_variable> message);
		shared_ptr<runtime_variable> add(shared_ptr<runtime_variable> dict, shared_ptr<runtime_variable> key, shared_ptr<runtime_variable> value);
		shared_ptr<runtime_variable> add(shared_ptr<runtime_variable> list_string, shared_ptr<runtime_variable> element);
		shared_ptr<runtime_variable> insert(shared_ptr<runtime_variable> list_string, shared_ptr<runtime_variable> pos, shared_ptr<runtime_variable> element);
		shared_ptr<runtime_variable> remove(shared_ptr<runtime_variable> list_string_dict, shared_ptr<runtime_variable> key_index);
		shared_ptr<runtime_variable> remove(shared_ptr<runtime_variable> list_string, shared_ptr<runtime_variable> start, shared_ptr<runtime_variable> end);
		shared_ptr<runtime_variable> load_library(shared_ptr<runtime_variable> dll_name);
		shared_ptr<runtime_variable> input(shared_ptr<runtime_variable> prompt);
		shared_ptr<runtime_variable> is_pure(shared_ptr<runtime_variable> rvar);

		vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> find_special_function(vector<shared_ptr<runtime_variable>> to_search, shared_ptr<runtime> r, string bf);
		public:
			runtime(vector<string> vec, vector<shared_ptr<runtime_variable>> il, vector<vector<shared_ptr<runtime_variable>>> sfil,
				vector<vector<shared_ptr<runtime_variable>>> ls, vector<vector<shared_ptr<runtime_variable>>> ms, vector<shared_ptr<runtime_variable>> alil);
			~runtime();
			shared_ptr<runtime_variable> run_program();
			bool dump_runtime_variables(vector<shared_ptr<runtime_variable>> insn_list);
			pair<shared_ptr<runtime_variable>, bool> find_instruction(string ident, pair<bool,bool> limits = make_pair(false, false));
			pair<shared_ptr<runtime_variable>, bool> find_instruction(int reg);
			vector<shared_ptr<runtime_variable>> get_instruction_list();
			vector<shared_ptr<runtime_variable>> set_instruction_list(vector<shared_ptr<runtime_variable>> insn_list);
			vector<vector<shared_ptr<runtime_variable>>> get_stacked_function_instruction_list();
			vector<vector<shared_ptr<runtime_variable>>> get_scope_stack();
			vector<vector<shared_ptr<runtime_variable>>> set_stacked_function_instruction_list(vector<vector<shared_ptr<runtime_variable>>> sfil);
			vector<vector<shared_ptr<runtime_variable>>> set_scope_stack(vector<vector<shared_ptr<runtime_variable>>> ssl);
			vector<string> get_string_instruction_list();
			vector<string> set_string_instruction_list(vector<string> vec);
			vector<vector<shared_ptr<runtime_variable>>> get_module_stack();
			vector<vector<shared_ptr<runtime_variable>>> set_module_stack(vector<vector<shared_ptr<runtime_variable>>> ms);
			vector<shared_ptr<runtime_variable>> set_added_lambda_instruction_list(vector<shared_ptr<runtime_variable>> alil);
			vector<shared_ptr<runtime_variable>> get_added_lambda_instruction_list();
	};
}

#endif