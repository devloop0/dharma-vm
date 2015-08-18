#include "includes/runtime.hpp"

namespace dharma_vm {

	const string runtime_diagnostic_messages::malformed_instruction = "Malformed instruction.";
	const string runtime_diagnostic_messages::incompatible_types = "Incompatible types";
	const string runtime_diagnostic_messages::fatal_error = "Fatal error.";
	const string runtime_diagnostic_messages::name_not_found = "Name not found.";
	const string runtime_diagnostic_messages::expected_sequence_for_subscript = "Expected sequence for subscript";
	const string runtime_diagnostic_messages::subscript_out_of_range = "Subscript out of range.";
	const string runtime_diagnostic_messages::unmodifiable_value = "Unmodifiable value.";
	const string runtime_diagnostic_messages::key_not_found = "Key not found.";
	const string runtime_diagnostic_messages::field_not_found = "Field not found.";
	const string runtime_diagnostic_messages::function_overload_not_found = "Function overload not found.";
	const string runtime_diagnostic_messages::structure_members_are_initialized_early = "Structure members are initialized early, and variable execution is not completed during structure initialization.";
	const string runtime_diagnostic_messages::import_file_not_found = "Import file not found.";
	const string runtime_diagnostic_messages::import_file_not_found_but_source_file_present = "Import file not found but source file present; if it is part of the standard library, "
		"please run 'karmac --compile-libkarma' to generate compiled versions of the source files.";

	runtime_type_information::runtime_type_information(runtime_type_kind tk, type_pure_kind tpk, type_class_kind tck, string cn) {
		t_kind = tk;
		tp_kind = tpk;
		tc_kind = tck;
		class_name = cn;
	}

	runtime_type_information::~runtime_type_information() {

	}

	const runtime_type_kind runtime_type_information::get_runtime_type_kind() {
		return t_kind;
	}

	const type_pure_kind runtime_type_information::get_type_pure_kind() {
		return tp_kind;
	}

	const type_class_kind runtime_type_information::get_type_class_kind() {
		return tc_kind;
	}

	string runtime_type_information::get_class_name() {
		return class_name;
	}

	const bool runtime_type_information::operator==(runtime_type_information ti) {
		return (t_kind == ti.t_kind && tc_kind == ti.tc_kind && tp_kind == ti.tp_kind && class_name == ti.class_name);
	}

	const bool runtime_type_information::operator!=(runtime_type_information ti) {
		return !(*this == ti);
	}

	runtime_variable::runtime_variable(storage_field sf, int i, float d, string s, bool b, vector<shared_ptr<runtime_variable>> lt, pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> di,
		vector<shared_ptr<runtime_variable>> sml, shared_ptr<runtime> mr, runtime_type_information ti, vector<shared_ptr<function>> f) : t_inf(ti), s_field(sf) {
		integer = i;
		decimal = d;
		str = s;
		boolean = b;
		list_tuple = lt;
		dict = di;
		unmodifiable = false;
		struct_enum_member_list = sml;
		module_runtime = mr;
		func = f;
		unique_id = system_clock::now().time_since_epoch() / nanoseconds(1);
		enum_parent_unique_id = 0;
		unsigned long long hash = 0;
		if (str.length() > 0) {
			const char* ccs = str.c_str();
			while (*ccs)
				hash = hash * 101 + *ccs++;
			unique_id += hash;
		}
		if (t_inf.get_class_name().length() > 0) {
			const char* ccs = t_inf.get_class_name().c_str();
			while (*ccs)
				hash = hash * 101 + *ccs++;
			unique_id += hash;
		}
		for (int i = 0; i < str.length(); i++) {
			if (i + 1 < str.length() && str[i] == '\\' && str[i + 1] == 'n') {
				str.erase(i, 2);
				str.insert(i, "\n");
				i--;
			}
			else if (i + 1 < str.length() && str[i] == '\\' && str[i + 1] == 't') {
				str.erase(i, 2);
				str.insert(i, "\t");
				i--;
			}
			else if (i + 1 < str.length() && str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '\"')) {
				char c = str[i + 1];
				str.erase(i, 2);
				if (c == '\'')
					str.insert(i, "\'");
				else
					str.insert(i, "\"");
				i--;
			}
		}
	}

	runtime_variable::~runtime_variable() {

	}

	int runtime_variable::set_integer(int i) {
		integer = i;
		return integer;
	}

	float runtime_variable::set_decimal(float d) {
		decimal = d;
		return decimal;
	}

	string runtime_variable::set_string(string s) {
		str = s;
		return str;
	}

	bool runtime_variable::set_boolean(bool b) {
		boolean = b;
		return boolean;
	}

	vector<shared_ptr<runtime_variable>> runtime_variable::set_list_tuple(vector<shared_ptr<runtime_variable>> lt) {
		list_tuple.clear();
		for (int i = 0; i < lt.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(lt[i]->get_storage_field(), lt[i]->get_integer(), lt[i]->get_decimal(), lt[i]->get_string(), lt[i]->get_boolean(),
				lt[i]->get_list_tuple(), lt[i]->get_dict(), lt[i]->get_struct_enum_member_list(), lt[i]->get_module_runtime(), lt[i]->get_runtime_type_information(), lt[i]->get_function());
			rvar->set_unmodifiable(lt[i]->get_unmodifiable());
			rvar->set_unique_id(lt[i]->get_unique_id());
			list_tuple.push_back(rvar);
		}
		return list_tuple;
	}

	pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> runtime_variable::set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> d) {
		dict.first.clear();
		dict.second.clear();
		for (int i = 0; i < d.first.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(d.first[i]->get_storage_field(), d.first[i]->get_integer(), d.first[i]->get_decimal(), d.first[i]->get_string(), d.first[i]->get_boolean(),
				d.first[i]->get_list_tuple(), d.first[i]->get_dict(), d.first[i]->get_struct_enum_member_list(), d.first[i]->get_module_runtime(), d.first[i]->get_runtime_type_information(), d.first[i]->get_function());
			rvar->set_unmodifiable(d.first[i]->get_unmodifiable());
			rvar->set_unique_id(d.first[i]->get_unique_id());
			dict.first.push_back(rvar);
		}
		for (int i = 0; i < d.second.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(d.second[i]->get_storage_field(), d.second[i]->get_integer(), d.second[i]->get_decimal(), d.second[i]->get_string(), d.second[i]->get_boolean(),
				d.second[i]->get_list_tuple(), d.second[i]->get_dict(), d.second[i]->get_struct_enum_member_list(), d.second[i]->get_module_runtime(), d.second[i]->get_runtime_type_information(), d.second[i]->get_function());
			rvar->set_unmodifiable(d.second[i]->get_unmodifiable());
			rvar->set_unique_id(d.second[i]->get_unique_id());
			dict.second.push_back(rvar);
		}
		return dict;
	}

	runtime_type_information runtime_variable::set_runtime_type_information(runtime_type_information ti) {
		t_inf = ti;
		return t_inf;
	}

	int runtime_variable::get_integer() {
		return integer;
	}

	float runtime_variable::get_decimal() {
		return decimal;
	}

	string runtime_variable::get_string() {
		return str;
	}

	bool runtime_variable::get_boolean() {
		return boolean;
	}

	storage_field runtime_variable::get_storage_field() {
		return s_field;
	}

	vector<shared_ptr<runtime_variable>> runtime_variable::get_list_tuple() {
		return list_tuple;
	}

	pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> runtime_variable::get_dict() {
		return dict;
	}

	runtime_type_information runtime_variable::get_runtime_type_information() {
		return t_inf;
	}

	const bool runtime_variable::get_unmodifiable() {
		return unmodifiable;
	}

	bool runtime_variable::set_unmodifiable(bool b) {
		unmodifiable = b;
		return unmodifiable;
	}

	vector<shared_ptr<runtime_variable>> runtime_variable::get_struct_enum_member_list() {
		return struct_enum_member_list;
	}

	vector<shared_ptr<runtime_variable>> runtime_variable::set_struct_enum_member_list(vector<shared_ptr<runtime_variable>> sml, long ui, bool deep) {
		if (!deep) {
			struct_enum_member_list.clear();
			for (int i = 0; i < sml.size(); i++) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(sml[i]->get_storage_field(), sml[i]->get_integer(), sml[i]->get_decimal(), sml[i]->get_string(), sml[i]->get_boolean(),
					sml[i]->get_list_tuple(), sml[i]->get_dict(), sml[i]->get_struct_enum_member_list(), sml[i]->get_module_runtime(), sml[i]->get_runtime_type_information(), sml[i]->get_function());
				rvar->set_unmodifiable(sml[i]->get_unmodifiable());
				rvar->set_unique_id(sml[i]->get_unique_id());
				rvar->set_enum_parent_unique_id(ui);
				struct_enum_member_list.push_back(rvar);
			}
		}
		else
			struct_enum_member_list = sml;
		unique_id = ui;
		return struct_enum_member_list;
	}

	shared_ptr<runtime> runtime_variable::get_module_runtime() {
		return module_runtime;
	}

	shared_ptr<runtime> runtime_variable::set_module_runtime(shared_ptr<runtime> mr, long ui, bool deep) {
		if (!deep) {
			module_runtime->set_instruction_list(mr->get_instruction_list());
			module_runtime->set_scope_stack(mr->get_scope_stack());
			module_runtime->set_stacked_function_instruction_list(mr->get_stacked_function_instruction_list());
			module_runtime->set_string_instruction_list(mr->get_string_instruction_list());
			module_runtime->set_module_stack(mr->get_module_stack());
			module_runtime->set_added_lambda_instruction_list(mr->get_added_lambda_instruction_list());
		}
		else
			module_runtime = mr;
		unique_id = ui;
		return module_runtime;
	}

	storage_field::storage_field(int rn, string i, storage_field_kind sfk, bool b) {
		register_number = rn;
		identifier = i;
		sf_kind = sfk;
		builtin = b;
	}

	storage_field::~storage_field() {

	}

	int storage_field::get_register_number() {
		return register_number;
	}

	string storage_field::get_identifier() {
		return identifier;
	}

	storage_field_kind storage_field::get_storage_field_kind() {
		return sf_kind;
	}

	const bool storage_field::is_builtin() {
		return builtin;
	}

	bool report_error_and_terminate_program(string msg, shared_ptr<runtime_variable> rvar) {
		cerr << "Error: " << msg << '\n';
		string ident;
		if (rvar == nullptr) {
			cerr << runtime_diagnostic_messages::structure_members_are_initialized_early << '\n';
			exit(1);
		}
		if (rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
			ident = rvar->get_storage_field().get_identifier();
		else
			ident = "r" + to_string(rvar->get_storage_field().get_register_number());
		cerr << "\t" << ident << "; Type: '";
		if (rvar->get_runtime_type_information() == runtime_type_information_list::_int)
			cerr << "Int";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_int)
			cerr << "[Int]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_boolean)
			cerr << "Boolean";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_boolean)
			cerr << "[Boolean]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_string)
			cerr << "String";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_string)
			cerr << "[String]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_decimal)
			cerr << "Decimal";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_decimal)
			cerr << "[Decimal]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_list)
			cerr << "List";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_list)
			cerr << "[List]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_tuple)
			cerr << "Tuple";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_tuple)
			cerr << "[Tuple]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_dict)
			cerr << "Dict";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_dict)
			cerr << "[Dict]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_nil)
			cerr << "Nil";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_nil)
			cerr << "[Nil]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_func)
			cerr << "Function";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_func)
			cerr << "[Function]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
			cerr << rvar->get_runtime_type_information().get_class_name();
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cerr << '[' << rvar->get_runtime_type_information().get_class_name() << ']';
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::bad)
			cerr << "**Error**";
		cerr << ".'\n";
		cerr << runtime_diagnostic_messages::structure_members_are_initialized_early << '\n';
		exit(1);
		return true;
	}

	const runtime_type_information runtime_type_information_list::_int = runtime_type_information(runtime_type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_string = runtime_type_information(runtime_type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_boolean = runtime_type_information(runtime_type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_decimal = runtime_type_information(runtime_type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_list = runtime_type_information(runtime_type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_tuple = runtime_type_information(runtime_type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_dict = runtime_type_information(runtime_type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_nil = runtime_type_information(runtime_type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_int = runtime_type_information(runtime_type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_string = runtime_type_information(runtime_type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_boolean = runtime_type_information(runtime_type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_decimal = runtime_type_information(runtime_type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_list = runtime_type_information(runtime_type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_tuple = runtime_type_information(runtime_type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_dict = runtime_type_information(runtime_type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_nil = runtime_type_information(runtime_type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::bad = runtime_type_information(runtime_type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, "");
	const runtime_type_information runtime_type_information_list::_func = runtime_type_information(runtime_type_kind::TYPE_FUNC, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const runtime_type_information runtime_type_information_list::_pure_func = runtime_type_information(runtime_type_kind::TYPE_FUNC, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");

	shared_ptr<runtime_variable> operator+(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one + two);
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one + two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one + two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one + two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator-(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one - two);
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one - two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one - two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one - two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator*(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one * two);
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one * two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one * two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one * two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator/(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one / two);
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one / two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one / two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one / two);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> pow(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(std::pow(one, two));
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(std::pow(one, two));
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(std::pow(one, two));
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(std::pow(one, two));
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator%(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int && type_two == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one % two);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator|(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int && type_two == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one | two);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator^(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int && type_two == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one ^ two);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator&(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int && type_two == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one & two);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator>(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one > two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one > two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one > two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one > two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator<(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one < two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one < two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one < two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one < two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator>=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one >= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one >= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one >= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one >= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator<=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if ((type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_int) && (type_two == runtime_type_information_list::_decimal || type_two == runtime_type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one <= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one <= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one <= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one <= two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator==(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_decimal) {
				float two = src->get_decimal();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(one == 0);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_decimal) {
			float one = dest->get_decimal();
			if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_decimal) {
				float two = src->get_decimal();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(one == 0);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_string) {
			string one = dest->get_string();
			if (type_two == runtime_type_information_list::_string) {
				string two = src->get_string();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(one == "");
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_func) {
			string one = dest->get_string();
			if (type_two == runtime_type_information_list::_func) {
				string two = src->get_string();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_boolean) {
			bool one = dest->get_boolean();
			if (type_two == runtime_type_information_list::_boolean) {
				bool two = src->get_boolean();
				dest->set_boolean(one == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(one == false);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_nil) {
			if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(true);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(0 == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_boolean) {
				bool two = src->get_boolean();
				dest->set_boolean(false == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_string) {
				string two = src->get_string();
				dest->set_boolean("" == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_decimal) {
				float two = src->get_decimal();
				dest->set_boolean(0 == two);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_list) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_tuple) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_dict) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_list) {
			if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_list) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_tuple) {
			if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else if (type_two == runtime_type_information_list::_tuple) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_dict) {
			if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
				}
			else if (type_two == runtime_type_information_list::_dict) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if ((dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			string dest_class_name = dest->get_runtime_type_information().get_class_name();
			if ((src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES && src->get_runtime_type_information().get_runtime_type_kind() == dest->get_runtime_type_information().get_runtime_type_kind()) {
				string src_class_name = src->get_runtime_type_information().get_class_name();
				if (dest_class_name != src_class_name || dest->get_unique_id() != src->get_unique_id())
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			string dest_class_name = dest->get_runtime_type_information().get_class_name();
			if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
				string src_class_name = src->get_runtime_type_information().get_class_name();
				if (dest_class_name != src_class_name || dest->get_enum_parent_unique_id() != src->get_enum_parent_unique_id())
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
				if (dest->get_runtime_type_information().get_class_name() != src->get_runtime_type_information().get_class_name() ||
					dest->get_unique_id() != src->get_unique_id())
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
				dest->set_boolean(equals_equals(dest, src));
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES &&
			src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			if (dest->get_runtime_type_information() == src->get_runtime_type_information()) {
				runtime_type_kind tk = dest->get_runtime_type_information().get_runtime_type_kind();
				if (tk == runtime_type_kind::TYPE_ENUM || tk == runtime_type_kind::TYPE_ENUM_CHILD || tk == runtime_type_kind::TYPE_FUNC ||
					tk == runtime_type_kind::TYPE_MODULE || tk == runtime_type_kind::TYPE_CUSTOM)
					dest->set_boolean(dest->get_unique_id() == src->get_unique_id());
				else
					dest->set_boolean(true);
			}
			else
				dest->set_boolean(false);
			dest->set_runtime_type_information(runtime_type_information_list::_boolean);
			return dest;
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator!=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		return !(dest == src);
	}

	shared_ptr<runtime_variable> operator&&(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_boolean && type_two == runtime_type_information_list::_boolean);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		bool one = dest->get_boolean();
		bool two = src->get_boolean();
		dest->set_boolean(one && two);
		dest->set_runtime_type_information(runtime_type_information_list::_boolean);
		return dest;
	}

	shared_ptr<runtime_variable> operator||(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_boolean && type_two == runtime_type_information_list::_boolean);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		bool one = dest->get_boolean();
		bool two = src->get_boolean();
		dest->set_boolean(one || two);
		dest->set_runtime_type_information(runtime_type_information_list::_boolean);
		return dest;
	}

	shared_ptr<runtime_variable> operator!(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		bool one = dest->get_boolean();
		dest->set_boolean(!one);
		dest->set_runtime_type_information(runtime_type_information_list::_boolean);
		return dest;
	}

	shared_ptr<runtime_variable> operator-(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int || type_one == runtime_type_information_list::_decimal);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			dest->set_integer(-one);
			dest->set_runtime_type_information(runtime_type_information_list::_int);
			return dest;
		}
		else {
			float one = dest->get_decimal();
			dest->set_decimal(-one);
			dest->set_runtime_type_information(runtime_type_information_list::_decimal);
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
	}

	shared_ptr<runtime_variable> operator+(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int || type_one == runtime_type_information_list::_decimal);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == runtime_type_information_list::_int) {
			int one = dest->get_integer();
			dest->set_integer(+one);
			dest->set_runtime_type_information(runtime_type_information_list::_int);
			return dest;
		}
		else {
			float one = dest->get_decimal();
			dest->set_decimal(+one);
			dest->set_runtime_type_information(runtime_type_information_list::_decimal);
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
	}

	shared_ptr<runtime_variable> operator~(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		dest->set_integer(~one);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> inc(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		int one = dest->get_integer();
		dest->set_integer(one + 1);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> dec(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		if (type_one == runtime_type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		int one = dest->get_integer();
		dest->set_integer(one - 1);
		dest->set_runtime_type_information(runtime_type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> tyof(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		if (type_one == runtime_type_information_list::bad)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
		if (type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			return dest;
		else {
			dest->set_runtime_type_information(runtime_type_information(dest->get_runtime_type_information().get_runtime_type_kind(), type_pure_kind::TYPE_PURE_YES, dest->get_runtime_type_information().get_type_class_kind(), dest->get_runtime_type_information().get_class_name()));
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
	}

	shared_ptr<runtime_variable> cast(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (type_two.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, nullptr);
		if (type_two == runtime_type_information_list::_pure_int) {
			if (type_one == runtime_type_information_list::_int || type_one == runtime_type_information_list::_pure_int)
				return dest;
			else if (type_one == runtime_type_information_list::_decimal) {
				dest->set_integer(dest->get_decimal());
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				return dest;
			}
			else if (type_one == runtime_type_information_list::_pure_decimal) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_int);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_decimal) {
			if (type_one == runtime_type_information_list::_decimal || type_one == runtime_type_information_list::_pure_decimal)
				return dest;
			else if (type_one == runtime_type_information_list::_int) {
				dest->set_decimal(dest->get_integer());
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				return dest;
			}
			else if (type_one == runtime_type_information_list::_pure_int) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_decimal);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_boolean) {
			if (type_one == runtime_type_information_list::_boolean || type_one == runtime_type_information_list::_pure_boolean)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_string) {
			if (type_one == runtime_type_information_list::_string || type_one == runtime_type_information_list::_pure_string)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_list) {
			if (type_one == runtime_type_information_list::_list || type_one == runtime_type_information_list::_pure_list)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_tuple) {
			if (type_one == runtime_type_information_list::_tuple || type_one == runtime_type_information_list::_pure_tuple)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_dict) {
			if (type_one == runtime_type_information_list::_dict || type_one == runtime_type_information_list::_pure_dict)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == runtime_type_information_list::_pure_func) {
			if (type_one == runtime_type_information_list::_func || type_one == runtime_type_information_list::_pure_func)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && type_two.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES &&
			type_two.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			if(type_one == type_two || (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && type_one.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES &&
				(type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES || type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO))) {
				if (dest->get_unique_id() == src->get_unique_id() && type_one.get_class_name() == type_two.get_class_name())
					return dest;
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if ((type_two.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || type_two.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && type_two.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES &&
			type_two.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			if (type_one == type_two || (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) &&
				(type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO || type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) &&
				type_one.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES && type_one.get_class_name() == type_two.get_class_name() && type_one.get_runtime_type_kind() == type_two.get_runtime_type_kind()) {
				if(dest->get_unique_id() == src->get_unique_id())
					return dest;
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && type_two.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES && type_two.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			if ((type_one == type_two || (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES && type_one.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) ||
				(type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO && type_one.get_type_class_kind() == type_class_kind::TYPE_CLASS_YES)) &&
				type_one.get_class_name() == type_two.get_class_name()) {
				if(dest->get_unique_id() == src->get_unique_id())
					return dest;
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
	}

	shared_ptr<runtime_variable> mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src, bool dmov) {
		if (src == nullptr || dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		if (src->get_unmodifiable())
			dest->set_unmodifiable(src->get_unmodifiable());
		if (src->get_runtime_type_information() == runtime_type_information_list::_int) {
			dest->set_integer(src->get_integer());
			dest->set_runtime_type_information(runtime_type_information_list::_int);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_decimal) {
			dest->set_decimal(src->get_decimal());
			dest->set_runtime_type_information(runtime_type_information_list::_decimal);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_string) {
			dest->set_string(src->get_string());
			dest->set_runtime_type_information(runtime_type_information_list::_string);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_boolean) {
			dest->set_boolean(src->get_boolean());
			dest->set_runtime_type_information(runtime_type_information_list::_boolean);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_list) {
			dest->set_list_tuple(src->get_list_tuple());
			dest->set_runtime_type_information(runtime_type_information_list::_list);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_tuple) {
			dest->set_list_tuple(src->get_list_tuple());
			dest->set_runtime_type_information(runtime_type_information_list::_tuple);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_dict) {
			dest->set_dict(src->get_dict());
			dest->set_runtime_type_information(runtime_type_information_list::_dict);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_func) {
			dest->set_string(src->get_string());
			dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id());
			dest->set_runtime_type_information(runtime_type_information_list::_func);
			dest->set_function(src->get_function(), src->get_unique_id());
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if ((src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
			dest->set_struct_enum_member_list(src->get_struct_enum_member_list(), src->get_unique_id(), !dmov);
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_unique_id(src->get_unique_id());
			dest->set_string(src->get_string());
			return dest;
		}
		else if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
			dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id(), !dmov);
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_string(src->get_string());
			dest->set_unique_id(src->get_unique_id());
			dest->set_enum_parent_unique_id(src->get_enum_parent_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_string(src->get_string());
			dest->set_unique_id(src->get_unique_id());
			dest->set_enum_parent_unique_id(src->get_enum_parent_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id(), !dmov);
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if ((src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			dest->set_struct_enum_member_list(src->get_struct_enum_member_list(), src->get_unique_id(), !dmov);
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_unique_id(src->get_unique_id());
			dest->set_string(src->get_string());
			return dest;
		}
		else if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id(), !dmov);
			dest->set_runtime_type_information(src->get_runtime_type_information());
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_int) {
			dest->set_integer(src->get_integer());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_int);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_decimal) {
			dest->set_decimal(src->get_decimal());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_decimal);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_string) {
			dest->set_string(src->get_string());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_string);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_boolean) {
			dest->set_boolean(src->get_boolean());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_boolean);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_list) {
			dest->set_list_tuple(src->get_list_tuple());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_list);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_tuple) {
			dest->set_list_tuple(src->get_list_tuple());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_tuple);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_dict) {
			dest->set_dict(src->get_dict());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_dict);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_func) {
			dest->set_string(src->get_string());
			dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id());
			dest->set_runtime_type_information(runtime_type_information_list::_pure_func);
			dest->set_function(src->get_function(), src->get_unique_id());
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_nil) {
			if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
				dest->set_integer(0);
			else if (dest->get_runtime_type_information() == runtime_type_information_list::_decimal)
				dest->set_decimal(0);
			else if (dest->get_runtime_type_information() == runtime_type_information_list::_string)
				dest->set_string("");
			else if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean)
				dest->set_boolean(false);
			else if (dest->get_runtime_type_information() == runtime_type_information_list::_list || dest->get_runtime_type_information() == runtime_type_information_list::_tuple)
				dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
			else if (dest->get_runtime_type_information() == runtime_type_information_list::_dict)
				dest->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
			else if (dest->get_runtime_type_information() == runtime_type_information_list::bad)
				dest->set_runtime_type_information(runtime_type_information_list::_nil);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		else if (src->get_runtime_type_information() == runtime_type_information_list::_pure_nil) {
			dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
			dest->set_unique_id(src->get_unique_id());
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
	}

	shared_ptr<runtime_variable> strict_mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src, bool dmov) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		runtime_type_information type_one = dest->get_runtime_type_information();
		runtime_type_information type_two = src->get_runtime_type_information();
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		if (src->get_unmodifiable())
			dest->set_unmodifiable(src->get_unmodifiable());
		if (type_one == runtime_type_information_list::_int) {
			if (type_two == runtime_type_information_list::_decimal) {
				dest->set_integer(src->get_decimal());
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_int) {
				dest->set_integer(src->get_integer());
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_integer(0);
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_decimal) {
			if (type_two == runtime_type_information_list::_int) {
				dest->set_decimal(src->get_integer());
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_decimal) {
				dest->set_decimal(src->get_decimal());
				dest->set_runtime_type_information(runtime_type_information_list::_int);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_decimal(0.0);
				dest->set_runtime_type_information(runtime_type_information_list::_decimal);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_string) {
			if (type_two == runtime_type_information_list::_string) {
				dest->set_string(src->get_string());
				dest->set_runtime_type_information(runtime_type_information_list::_string);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_string("");
				dest->set_runtime_type_information(runtime_type_information_list::_string);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_boolean) {
			if (type_two == runtime_type_information_list::_boolean) {
				dest->set_boolean(src->get_boolean());
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_boolean(false);
				dest->set_runtime_type_information(runtime_type_information_list::_boolean);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_list) {
			if (type_two == runtime_type_information_list::_list) {
				dest->set_list_tuple(src->get_list_tuple());
				dest->set_runtime_type_information(runtime_type_information_list::_list);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
				dest->set_runtime_type_information(runtime_type_information_list::_list);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_tuple) {
			if (type_two == runtime_type_information_list::_tuple) {
				dest->set_list_tuple(src->get_list_tuple());
				dest->set_runtime_type_information(runtime_type_information_list::_tuple);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
				dest->set_runtime_type_information(runtime_type_information_list::_tuple);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_dict) {
			if (type_two == runtime_type_information_list::_dict) {
				dest->set_dict(src->get_dict());
				dest->set_runtime_type_information(runtime_type_information_list::_dict);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_nil) {
				dest->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
				dest->set_runtime_type_information(runtime_type_information_list::_dict);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_func) {
			if (type_two == runtime_type_information_list::_func) {
				dest->set_string(src->get_string());
				dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id());
				dest->set_runtime_type_information(runtime_type_information_list::_func);
				dest->set_function(src->get_function(), src->get_unique_id());
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
			if (type_two == type_one && dest->get_unique_id() == src->get_unique_id()) {
				dest->set_struct_enum_member_list(src->get_struct_enum_member_list(), src->get_unique_id(), !dmov);
				dest->set_runtime_type_information(src->get_runtime_type_information());
				dest->set_unique_id(src->get_unique_id());
				dest->set_string(src->get_string());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
			if (type_two == type_one && dest->get_unique_id() == src->get_unique_id()) {
				dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id(), !dmov);
				dest->set_runtime_type_information(src->get_runtime_type_information());
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_int) {
			if (type_two == runtime_type_information_list::_pure_int) {
				dest->set_integer(src->get_integer());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_int);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_func) {
			if (type_two == runtime_type_information_list::_pure_func) {
				dest->set_string(src->get_string());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_func);
				dest->set_function(src->get_function(), src->get_unique_id());
				dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id());
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_decimal) {
			if (type_two == runtime_type_information_list::_pure_decimal) {
				dest->set_decimal(src->get_decimal());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_decimal);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_string) {
			if (type_two == runtime_type_information_list::_pure_string) {
				dest->set_string(src->get_string());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_string);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_boolean) {
			if (type_two == runtime_type_information_list::_pure_boolean) {
				dest->set_boolean(src->get_boolean());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_boolean);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_list) {
			if (type_two == runtime_type_information_list::_pure_list) {
				dest->set_list_tuple(src->get_list_tuple());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_list);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_tuple) {
			if (type_two == runtime_type_information_list::_pure_tuple) {
				dest->set_list_tuple(src->get_list_tuple());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_tuple);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == runtime_type_information_list::_pure_dict) {
			if (type_two == runtime_type_information_list::_pure_dict) {
				dest->set_dict(src->get_dict());
				dest->set_runtime_type_information(runtime_type_information_list::_pure_dict);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else if (type_two == runtime_type_information_list::_pure_nil) {
				dest->set_runtime_type_information(runtime_type_information_list::_pure_nil);
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if ((type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			if (type_one == type_two && dest->get_unique_id() == src->get_unique_id()) {
				dest->set_struct_enum_member_list(src->get_struct_enum_member_list(), src->get_unique_id(), !dmov);
				dest->set_runtime_type_information(src->get_runtime_type_information());
				dest->set_unique_id(src->get_unique_id());
				dest->set_string(src->get_string());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
			if (type_one == type_two && dest->get_unique_id() == src->get_unique_id()) {
				dest->set_string(src->get_string());
				dest->set_unique_id(src->get_unique_id());
				dest->set_enum_parent_unique_id(src->get_enum_parent_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one.get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			if (type_one == type_two && dest->get_unique_id() == src->get_unique_id()) {
				dest->set_module_runtime(src->get_module_runtime(), src->get_unique_id(), !dmov);
				dest->set_runtime_type_information(src->get_runtime_type_information());
				dest->set_unique_id(src->get_unique_id());
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
	}

	const bool equals_equals(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dest->get_runtime_type_information() == runtime_type_information_list::_int) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_int)
				return dest->get_integer() == src->get_integer();
			else if (src->get_runtime_type_information() == runtime_type_information_list::_decimal)
				return dest->get_integer() == src->get_decimal();
			else if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return dest->get_integer() == 0;
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_decimal) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_decimal)
				return dest->get_decimal() == src->get_decimal();
			else if (src->get_runtime_type_information() == runtime_type_information_list::_int)
				return dest->get_decimal() == src->get_integer();
			else if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return dest->get_decimal() == 0.0;
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_boolean)
				return dest->get_boolean() == src->get_boolean();
			else if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return dest->get_boolean() == false;
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_func) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_func)
				return dest->get_string() == src->get_string() && dest->get_function() == src->get_function();
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_string) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_string)
				return dest->get_string() == src->get_string();
			else if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return dest->get_string() == "";
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_list) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return dest->get_list_tuple().size() == 0;
			else if (src->get_runtime_type_information() == runtime_type_information_list::_list) {
				if (dest->get_list_tuple().size() != src->get_list_tuple().size())
					return false;
				for (int i = 0; i < dest->get_list_tuple().size(); i++) {
					bool store = equals_equals(dest->get_list_tuple()[i], src->get_list_tuple()[i]);
					if (!store)
						return false;
				}
				return true;
			}
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_tuple) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return dest->get_list_tuple().size() == 0;
			else if (src->get_runtime_type_information() == runtime_type_information_list::_tuple) {
				if (dest->get_list_tuple().size() != src->get_list_tuple().size())
					return false;
				for (int i = 0; i < dest->get_list_tuple().size(); i++) {
					bool store = equals_equals(dest->get_list_tuple()[i], src->get_list_tuple()[i]);
					if (!store)
						return false;
				}
				return true;
			}
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_dict) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_nil) {
				vector<shared_ptr<runtime_variable>> key_list = dest->get_dict().first;
				vector<shared_ptr<runtime_variable>> value_list = dest->get_dict().second;
				return key_list.size() == 0 && value_list.size() == 0;
			}
			else if (src->get_runtime_type_information() == runtime_type_information_list::_dict) {
				vector<shared_ptr<runtime_variable>> dest_key_list = dest->get_dict().first;
				vector<shared_ptr<runtime_variable>> dest_value_list = dest->get_dict().second;
				vector<shared_ptr<runtime_variable>> src_key_list = src->get_dict().first;
				vector<shared_ptr<runtime_variable>> src_value_list = src->get_dict().second;
				if (dest_key_list.size() != src_key_list.size() || dest_value_list.size() != src_value_list.size())
					return false;
				for (int i = 0; i < dest_key_list.size(); i++) {
					bool store = equals_equals(dest_key_list[i], src_key_list[i]);
					if (!store)
						return false;
				}
				for (int i = 0; i < dest_value_list.size(); i++) {
					bool store = equals_equals(dest_value_list[i], src_value_list[i]);
					if (!store)
						return false;
				}
				return true;
			}
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == runtime_type_information_list::_nil) {
			if (src->get_runtime_type_information() == runtime_type_information_list::_int)
				return src->get_integer() == 0;
			else if (src->get_runtime_type_information() == runtime_type_information_list::_decimal)
				return src->get_decimal() == 0.0;
			else if (src->get_runtime_type_information() == runtime_type_information_list::_boolean)
				return src->get_boolean() == false;
			else if (src->get_runtime_type_information() == runtime_type_information_list::_string)
				return src->get_string() == "";
			else if (src->get_runtime_type_information() == runtime_type_information_list::_list || src->get_runtime_type_information() == runtime_type_information_list::_tuple)
				return src->get_list_tuple().size() == 0;
			else if (src->get_runtime_type_information() == runtime_type_information_list::_dict) {
				vector<shared_ptr<runtime_variable>> key_list = src->get_dict().first;
				vector<shared_ptr<runtime_variable>> value_list = src->get_dict().second;
				return key_list.size() == 0 && value_list.size() == 0;
			}
			else if (src->get_runtime_type_information() == runtime_type_information_list::_nil)
				return true;
			else
				return false;
		}
		else if (dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			string dest_class_name = dest->get_runtime_type_information().get_class_name();
			if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
				string src_class_name = src->get_runtime_type_information().get_class_name();
				if (dest_class_name != src_class_name)
					return false;
				if (dest->get_struct_enum_member_list().size() != src->get_struct_enum_member_list().size())
					return false;
				vector<shared_ptr<runtime_variable>> dsource = dest->get_struct_enum_member_list();
				vector<shared_ptr<runtime_variable>> ssource = src->get_struct_enum_member_list();
				for (int i = 0; i < dsource.size(); i++) {
					shared_ptr<runtime_variable> dvar = make_shared<runtime_variable>(dsource[i]->get_storage_field(), dsource[i]->get_integer(), dsource[i]->get_decimal(), dsource[i]->get_string(),
						dsource[i]->get_boolean(), dsource[i]->get_list_tuple(), dsource[i]->get_dict(), dsource[i]->get_struct_enum_member_list(), dsource[i]->get_module_runtime(), dsource[i]->get_runtime_type_information(),
						dsource[i]->get_function());
					dvar->set_unmodifiable(dsource[i]->get_unmodifiable());
					dvar->set_unique_id(dsource[i]->get_unique_id());
					shared_ptr<runtime_variable> svar = ssource[i];
					if (!((dvar == svar)->get_boolean()))
						return false;
				}
				return true;
			}
			else
				return false;
		}
		else if (dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			string dest_class_name = dest->get_runtime_type_information().get_class_name();
			if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
				string src_class_name = src->get_runtime_type_information().get_class_name();
				if (dest_class_name == src_class_name && dest->get_struct_enum_member_list().size() == src->get_struct_enum_member_list().size())
					return true;
			}
			return false;
		}
		else if (dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			string dest_class_name = dest->get_runtime_type_information().get_class_name();
			if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
				string src_class_name = src->get_runtime_type_information().get_class_name();
				if (dest_class_name == src_class_name && dest->get_string() == src->get_string())
					return true;
			}
			return false;
		}
		else if (dest->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && dest->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
			dest->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
			if (src->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && src->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
				src->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
				if (dest->get_runtime_type_information().get_class_name() != src->get_runtime_type_information().get_class_name())
					return false;
				if (dest->get_runtime_type_information().get_class_name().substr(0, builtins::builtin_runtime_file_module_prefix.length()) == builtins::builtin_runtime_file_module_prefix) {
					vector<vector<shared_ptr<runtime_variable>>> dto_loop = dest->get_module_runtime()->get_module_stack();
					vector<vector<shared_ptr<runtime_variable>>> sto_loop = src->get_module_runtime()->get_module_stack();
					if (dto_loop.size() != sto_loop.size())
						return false;
					for (int j = 0; j < dto_loop.size(); j++) {
						vector<shared_ptr<runtime_variable>> dsource = dto_loop[j];
						vector<shared_ptr<runtime_variable>> ssource = sto_loop[j];
						if (dsource.size() != ssource.size())
							return false;
						for (int i = 0; i < dsource.size(); i++) {
							shared_ptr<runtime_variable> dvar = make_shared<runtime_variable>(dsource[i]->get_storage_field(), dsource[i]->get_integer(), dsource[i]->get_decimal(), dsource[i]->get_string(),
								dsource[i]->get_boolean(), dsource[i]->get_list_tuple(), dsource[i]->get_dict(), dsource[i]->get_struct_enum_member_list(), dsource[i]->get_module_runtime(), dsource[i]->get_runtime_type_information(),
								dsource[i]->get_function());
							dvar->set_unmodifiable(dsource[i]->get_unmodifiable());
							dvar->set_unique_id(dsource[i]->get_unique_id());
							shared_ptr<runtime_variable> svar = ssource[i];
							if (!((dvar == svar)->get_boolean()))
								return false;
						}
					}
					vector<shared_ptr<runtime_variable>> dsource = dest->get_module_runtime()->get_instruction_list();
					vector<shared_ptr<runtime_variable>> ssource = src->get_module_runtime()->get_instruction_list();
					if (dsource.size() != ssource.size())
						return false;
					for (int i = 0; i < dsource.size(); i++) {
						shared_ptr<runtime_variable> dvar = make_shared<runtime_variable>(dsource[i]->get_storage_field(), dsource[i]->get_integer(), dsource[i]->get_decimal(), dsource[i]->get_string(),
							dsource[i]->get_boolean(), dsource[i]->get_list_tuple(), dsource[i]->get_dict(), dsource[i]->get_struct_enum_member_list(), dsource[i]->get_module_runtime(), dsource[i]->get_runtime_type_information(),
							dsource[i]->get_function());
						dvar->set_unmodifiable(dsource[i]->get_unmodifiable());
						dvar->set_unique_id(dsource[i]->get_unique_id());
						shared_ptr<runtime_variable> svar = ssource[i];
						if (!((dvar == svar)->get_boolean()))
							return false;
					}
					return true;
				}
				else {
					vector<shared_ptr<runtime_variable>> dsource = dest->get_module_runtime()->get_module_stack()[dest->get_module_runtime()->get_module_stack().size() - 1];
					vector<shared_ptr<runtime_variable>> ssource = src->get_module_runtime()->get_module_stack()[dest->get_module_runtime()->get_module_stack().size() - 1];
					if (dsource.size() != ssource.size())
						return false;
					for (int i = 0; i < dsource.size(); i++) {
						shared_ptr<runtime_variable> dvar = make_shared<runtime_variable>(dsource[i]->get_storage_field(), dsource[i]->get_integer(), dsource[i]->get_decimal(), dsource[i]->get_string(),
							dsource[i]->get_boolean(), dsource[i]->get_list_tuple(), dsource[i]->get_dict(), dsource[i]->get_struct_enum_member_list(), dsource[i]->get_module_runtime(), dsource[i]->get_runtime_type_information(),
							dsource[i]->get_function());
						dvar->set_unmodifiable(dsource[i]->get_unmodifiable());
						dvar->set_unique_id(dsource[i]->get_unique_id());
						shared_ptr<runtime_variable> svar = ssource[i];
						if (!((dvar == svar)->get_boolean()))
							return false;
					}
					return true;
				}
			}
			else
				return false;
		}
		else if (dest->get_runtime_type_information() == src->get_runtime_type_information() && dest->get_unique_id() == src->get_unique_id())
			return true;
		else
			return false;
		return false;
	}

	const bool not_equals(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		return !equals_equals(dest, src);
	}

	tuple<string, register_identifier_kind, runtime_type_kind> runtime::deduce_register_identifier_kind(string ident) {
		string temp = ident;
		if (temp[0] == 'r') {
			if ((int)temp.find('|') >= 0 || (int)temp.find('/') >= 0 || (int)temp.find('@') >= 0 || (int)temp.find(':') >= 0)
				return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX, runtime_type_kind::TYPE_NONE);
			temp.erase(0, 1);
			return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_REGISTER, runtime_type_kind::TYPE_NONE);
		}
		else if (temp[0] == '[') {
			temp.erase(0, 1);
			temp.pop_back();
			if (temp[0] == '$') {
				temp.erase(0, 1);
				if (temp[0] == '\'' || temp[0] == '\"') {
					if (temp[0] != temp[temp.length() - 1])
						report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
					temp.erase(0, 1);
					temp.pop_back();
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, runtime_type_kind::TYPE_STRING);
				}
				else if ((int)temp.find('.') >= 0)
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, runtime_type_kind::TYPE_DECIMAL);
				else if (isdigit(temp[0]))
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, runtime_type_kind::TYPE_INT);
				else if (temp == "true" || temp == "false")
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, runtime_type_kind::TYPE_BOOLEAN);
				else if (temp == "nil")
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, runtime_type_kind::TYPE_NIL);
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
			}
			else {
				if ((int)temp.find('|') >= 0 || (int)temp.find('/') >= 0 || (int)temp.find('@') >= 0 || (int)temp.find(':') >= 0)
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX, runtime_type_kind::TYPE_NONE);
				else
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_IDENTIFIER, runtime_type_kind::TYPE_NONE);
			}
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> runtime::deduce_runtime_variable(string ident, bool must_exist, bool dmov_override) {
		tuple<string, register_identifier_kind, runtime_type_kind> deduced_tuple = deduce_register_identifier_kind(ident);
		string deduced_string = get<0>(deduced_tuple);
		register_identifier_kind deduced_ri_kind = get<1>(deduced_tuple);
		auto temp_check_if_string_is_integer = [](string str) {
			if (str.length() == 0)
				return false;
			for (int i = 0; i < str.length(); i++)
				if (!isdigit(str[i]))
					return false;
			return true;
		};
		vector<int> exclude_list;
		if (deduced_ri_kind == register_identifier_kind::REGISTER_IDENTIFIER_REGISTER) {
			pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(stoi(deduced_string));
			if (dmov_override) {
				shared_ptr<runtime_variable> temp = make_shared<runtime_variable>(storage_field(stoi(deduced_string), "", storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER), -1, -1, "",
					false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
						vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::bad,
					vector<shared_ptr<function>>());
				return checked_insertion(temp);
			}
			else if (!pai.second) {
				if (must_exist) {
					report_error_and_terminate_program(runtime_diagnostic_messages::name_not_found, nullptr);
				}
				else {
					shared_ptr<runtime_variable> temp = make_shared<runtime_variable>(storage_field(stoi(deduced_string), "", storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER), -1, -1, "",
						false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
							vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::bad,
						vector<shared_ptr<function>>());
					return checked_insertion(temp);
				}
			}
			else
				return pai.first;
		}
		else if (deduced_ri_kind == register_identifier_kind::REGISTER_IDENTIFIER_IDENTIFIER) {
			pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(deduced_string);
			if (dmov_override) {
				shared_ptr<runtime_variable> temp = make_shared<runtime_variable>(storage_field(-1, deduced_string, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
					pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
						vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::bad,
					vector<shared_ptr<function>>());
				return checked_insertion(temp);
			}
			else if (!pai.second) {
				if (must_exist)
					report_error_and_terminate_program(runtime_diagnostic_messages::name_not_found, nullptr);
				else {
					shared_ptr<runtime_variable> temp = make_shared<runtime_variable>(storage_field(-1, deduced_string, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
						pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
							vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::bad, 
						vector<shared_ptr<function>>());
					return checked_insertion(temp);
				}
			}
			else
				return pai.first;
		}
		else if (deduced_ri_kind == register_identifier_kind::REGISTER_IDENTIFIER_LITERAL || deduced_ri_kind == register_identifier_kind::REGISTER_IDENTIFIER_NONE)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		shared_ptr<runtime_variable> base = nullptr;
		string temp;
		vector<string> token_list;
		for (int i = 0; i < deduced_string.length(); i++) {
			if (deduced_string[i] == '@') {
				if (temp.length() > 0)
					token_list.push_back(temp);
				temp.clear();
				token_list.push_back("@");
			}
			else if (deduced_string[i] == '|') {
				if (temp.length() > 0)
					token_list.push_back(temp);
				temp.clear();
				token_list.push_back("|");
			}
			else if (deduced_string[i] == '/') {
				if (temp.length() > 0)
					token_list.push_back(temp);
				temp.clear();
				i++;
				int count = 0;
				while (i < deduced_string.length()) {
					if (deduced_string[i] == '/') {
						if (count >= 2)
							break;
						temp.push_back(deduced_string[i]);
						count++;
					}
					else if (deduced_string[i] == '@' || deduced_string[i] == '|' || deduced_string[i] == ':')
						break;
					else
						temp.push_back(deduced_string[i]);
					i++;
				}
				token_list.push_back("/");
				token_list.push_back(temp);
				temp.clear();
				i--;
			}
			else if (deduced_string[i] == ':') {
				if (temp.length() > 0)
					token_list.push_back(temp);
				temp.clear();
				token_list.push_back(":");
				i++;
				while (i < deduced_string.length() && deduced_string[i] != '/' && deduced_string[i] != '@' && deduced_string[i] != '|' && deduced_string[i] != ':') {
					temp.push_back(deduced_string[i]);
					i++;
				}
				token_list.push_back(temp);
				temp.clear();
				i--;
			}
			else
				temp.push_back(deduced_string[i]);
		}
		if (temp.length() > 0)
			token_list.push_back(temp);
		shared_ptr<runtime> function_search = nullptr;
		for (int i = 0; i < token_list.size(); i++) {
			if (i == 0) {
				string temp = token_list[i];
				if (temp[0] == 'r' && temp_check_if_string_is_integer(temp.substr(1)));
				else {
					temp.insert(0, "[");
					temp.push_back(']');
				}
				base = deduce_runtime_variable(temp, true);
			}
			else if (token_list[i] == "@") {
				if (i + 1 >= token_list.size() || base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				bool base_immut = base->get_unmodifiable();
				if (base->get_runtime_type_information() == runtime_type_information_list::_list || base->get_runtime_type_information() == runtime_type_information_list::_tuple) {
					if (token_list[i + 1] == "size") {
						shared_ptr<runtime_variable> created_int = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), base->get_list_tuple().size(), -1, "", false,
							vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
								vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_int,
							vector<shared_ptr<function>>());
						base = checked_insertion(created_int);
						runtime_temporary_count++;
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
				}
				else if (base->get_runtime_type_information() == runtime_type_information_list::_dict)
					report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
				else if (base->get_runtime_type_information() == runtime_type_information_list::_string) {
					if (token_list[i + 1] == "size") {
						shared_ptr<runtime_variable> created_int = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), base->get_string().length(), -1, "", false,
							vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
								vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_int,
							vector<shared_ptr<function>>());
						base = checked_insertion(created_int);
						runtime_temporary_count++;
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
				}
				else if ((base->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM || base->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM) && base->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
					base->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
					shared_ptr<runtime_variable> rvar = nullptr;
					string to_find = token_list[i + 1];
					for (int i = 0; i < base->get_struct_enum_member_list().size(); i++)
						if (base->get_struct_enum_member_list()[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER &&
							base->get_struct_enum_member_list()[i]->get_storage_field().get_identifier() == to_find) {
							rvar = base->get_struct_enum_member_list()[i];
							break;
						}
					if (rvar == nullptr)
						report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
					base = rvar;
					if (base_immut)
						base->set_unmodifiable(true);
				}
				else if (base->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && base->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO &&
					base->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_YES) {
					string to_find = token_list[i + 1];
					pair<shared_ptr<runtime_variable>, bool> pai = make_pair(nullptr, false);
					if(base->get_runtime_type_information().get_class_name().substr(0, builtins::builtin_runtime_file_module_prefix.length()) == builtins::builtin_runtime_file_module_prefix)
						pai = base->get_module_runtime()->find_instruction(to_find, make_pair(true, false));
					else
						pai = base->get_module_runtime()->find_instruction(to_find, make_pair(base->get_runtime_type_information().get_class_name() == "^", true));
					if (!pai.second)
						report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
					function_search = base->get_module_runtime();
					base = pai.first;
					if (base_immut)
						base->set_unmodifiable(true);
				}
			}
			else if (token_list[i] == "|") {
				if (i + 1 >= token_list.size() || base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				string temp = token_list[i + 1];
				if (temp[0] == 'r' && temp_check_if_string_is_integer(temp.substr(1)));
				else {
					temp.insert(0, "[");
					temp.push_back(']');
				}
				bool base_immut = base->get_unmodifiable();
				shared_ptr<runtime_variable> rvar = deduce_runtime_variable(temp, true);
				if (base->get_runtime_type_information() == runtime_type_information_list::_list || base->get_runtime_type_information() == runtime_type_information_list::_tuple) {
					if (rvar->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
					if (rvar->get_integer() >= 0 && rvar->get_integer() < base->get_list_tuple().size())
						base = base->get_list_tuple()[rvar->get_integer()];
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, rvar);
				}
				else if (base->get_runtime_type_information() == runtime_type_information_list::_string) {
					if (rvar->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
					if (rvar->get_integer() >= 0 && rvar->get_integer() < base->get_string().length()) {
						shared_ptr<runtime_variable> created_string = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, string(1, base->get_string()[rvar->get_integer()]), false,
							vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
								vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_string,
							vector<shared_ptr<function>>());
						base = checked_insertion(created_string);
						runtime_temporary_count++;
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, rvar);
				}
				else if (base->get_runtime_type_information() == runtime_type_information_list::_dict) {
					if (base->get_dict().first.size() != base->get_dict().first.size())
						report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, base);
					if (base->get_dict().first.size() == 0)
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, base);
					vector<shared_ptr<runtime_variable>> key_list = base->get_dict().first;
					if (rvar->get_runtime_type_information() == key_list[0]->get_runtime_type_information());
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
					int index = -1;
					for (int i = 0; i < key_list.size(); i++)
						if (equals_equals(rvar, key_list[i])) {
							index = i;
							break;
						}
					if (index == -1)
						report_error_and_terminate_program(runtime_diagnostic_messages::key_not_found, rvar);
					base = base->get_dict().second[index];
				}
				else {
					vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__subscript__);
					if (results.size() > 0) {
						bool success = false;
						for (int j = 0; j < results.size(); j++) {
							pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
							for (int i = 0; i < res.first->get_function().size(); i++) {
								shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { base, rvar }, res.second);
								if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
								else {
									base = temp;
									success = true;
									break;
								}
							}
							if (success)
								break;
						}
						if (!success)
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
				}
				if (base_immut)
					base->set_unmodifiable(true);
			}
			else if (token_list[i] == "/") {
				if (i + 1 >= token_list.size() || base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				vector<string> start_end_step;
				string temp = token_list[i + 1];
				string buf;
				stringstream ss(temp);
				while (getline(ss, buf, '/'))
					start_end_step.push_back(buf);
				if (start_end_step.size() != 3)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				vector<string> start_end_step_final;
				for (int i = 0; i < start_end_step.size(); i++) {
					string s = start_end_step[i];
					if (s[0] == 'r' && temp_check_if_string_is_integer(s.substr(1)));
					else {
						s.insert(0, "[");
						s.push_back(']');
					}
					start_end_step_final.push_back(s);
				}
				shared_ptr<runtime_variable> start = deduce_runtime_variable(start_end_step_final[0], true);
				shared_ptr<runtime_variable> end = deduce_runtime_variable(start_end_step_final[1], true);
				shared_ptr<runtime_variable> step = deduce_runtime_variable(start_end_step_final[2], true);
				if (base->get_runtime_type_information() == runtime_type_information_list::_list || base->get_runtime_type_information() == runtime_type_information_list::_tuple) {
					if (start->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, start);
					else if (end->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, end);
					else if (step->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, step);
					if (start->get_integer() >= 0 && start->get_integer() < base->get_list_tuple().size());
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, start);
					if (end->get_integer() > start->get_integer() && end->get_integer() <= base->get_list_tuple().size());
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, end);
					if (step->get_integer() > 0);
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, step);
					shared_ptr<runtime_variable> created_list_tuple = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
						vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
							vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), base->get_runtime_type_information(),
						vector<shared_ptr<function>>());
					vector<shared_ptr<runtime_variable>> vec;
					for (int i = start->get_integer(); i < end->get_integer(); i += step->get_integer())
						vec.push_back(base->get_list_tuple()[i]);
					created_list_tuple->set_list_tuple(vec);
					base = checked_insertion(created_list_tuple);
					runtime_temporary_count++;
				}
				else if (base->get_runtime_type_information() == runtime_type_information_list::_string) {
					if (start->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, start);
					else if (end->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, end);
					else if (step->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, step);
					if (start->get_integer() >= 0 && start->get_integer() < base->get_string().length());
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, start);
					if (end->get_integer() > start->get_integer() && end->get_integer() >= base->get_string().length());
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, end);
					if (step->get_integer() > 0);
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, step);
					shared_ptr<runtime_variable> created_string = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
						vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
							vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), base->get_runtime_type_information(), 
						vector<shared_ptr<function>>());
					string s;
					for (int i = start->get_integer(); i < end->get_integer(); i += step->get_integer())
						s.push_back(base->get_string()[i]);
					created_string->set_string(s);
					base = checked_insertion(created_string);
					runtime_temporary_count++;
				}
				else {
					if (start->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, start);
					else if (end->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, end);
					else if (step->get_runtime_type_information() != runtime_type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, step);
					vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__slice__);
					if (results.size() > 0) {
						bool success = false;
						for (int j = 0; j < results.size(); j++) {
							pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
							for (int i = 0; i < res.first->get_function().size(); i++) {
								shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { base, start, end, step }, res.second);
								if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
								else {
									base = temp;
									success = true;
									break;
								}
							}
							if (success)
								break;
						}
						if (!success)
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
				}
			}
			else if (token_list[i] == ":") {
				if (base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				if (base->get_runtime_type_information() != runtime_type_information_list::_func &&
					base->get_runtime_type_information().get_runtime_type_kind() != runtime_type_kind::TYPE_CUSTOM)
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
				if (i + 1 >= token_list.size())
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				string temp = token_list[i + 1];
				vector<string> arg_list_temp;
				string buf;
				stringstream ss(temp);
				while (getline(ss, buf, ','))
					arg_list_temp.push_back(buf);
				vector<string> arg_list;
				for (int i = 0; i < arg_list_temp.size(); i++) {
					string tmp = arg_list_temp[i];
					if (tmp[0] == 'r' && temp_check_if_string_is_integer(tmp.substr(1)));
					else {
						tmp.insert(0, "[");
						tmp.push_back(']');
					}
					arg_list.push_back(tmp);
				}
				vector<shared_ptr<runtime_variable>> rvar_arg_list;
				for (int i = 0; i < arg_list.size(); i++)
					rvar_arg_list.push_back(deduce_runtime_variable(arg_list[i], true));
				string fname = base->get_string();
				vector<shared_ptr<function>> func = base->get_function();
				shared_ptr<runtime> parent = function_search == nullptr ? make_shared<runtime>(*this) : function_search;
				base = run_function(func, base, rvar_arg_list, function_search == nullptr ? nullptr : parent);
			}
		}
		return base;
	}

	pair<shared_ptr<runtime_variable>, bool> runtime::find_instruction(int reg) {
		for (int j = scope_stack.size() - 1; j >= 0; j--) {
			vector<shared_ptr<runtime_variable>> vec = scope_stack[j];
			for (int i = vec.size() - 1; i >= 0; i--)
				if (vec[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER)
					if (vec[i]->get_storage_field().get_register_number() == reg)
						return make_pair(vec[i], true);
		}
		if (stacked_function_instruction_list.size() > 0) {
			vector<shared_ptr<runtime_variable>> vec = stacked_function_instruction_list[stacked_function_instruction_list.size() - 1];
			for (int i = vec.size() - 1; i >= 0; i--)
				if (vec[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER)
					if (vec[i]->get_storage_field().get_register_number() == reg)
						return make_pair(vec[i], true);
		}
		for (int j = module_stack.size() - 1; j >= 0; j--) {
			vector<shared_ptr<runtime_variable>> vec = module_stack[j];
			for (int i = vec.size() - 1; i >= 0; i--)
				if (vec[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER)
					if (vec[i]->get_storage_field().get_register_number() == reg)
						return make_pair(vec[i], true);
		}
		for (int i = instruction_list.size() - 1; i >= 0; i--)
			if (instruction_list[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER) {
				if (instruction_list[i]->get_storage_field().get_register_number() == reg) {
					return make_pair(instruction_list[i], true);
				}
			}
		return make_pair(nullptr, false);
	}

	pair<shared_ptr<runtime_variable>, bool> runtime::find_instruction(string ident, pair<bool, bool> limits) {
		bool limit = limits.first;
		bool constrain = limits.second;
		if (limit)
			goto global;
		for (int j = scope_stack.size() - 1; j >= 0; j--) {
			vector<shared_ptr<runtime_variable>> vec = scope_stack[j];
			for (int i = vec.size() - 1; i >= 0; i--) {
				if (vec[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
					if (vec[i]->get_storage_field().get_identifier() == ident)
						return make_pair(vec[i], true);
			}
		}
		if (stacked_function_instruction_list.size() > 0) {
			vector<shared_ptr<runtime_variable>> vec = stacked_function_instruction_list[stacked_function_instruction_list.size() - 1];
			for (int i = vec.size() - 1; i >= 0; i--) {
				if (vec[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
					if (vec[i]->get_storage_field().get_identifier() == ident)
						return make_pair(vec[i], true);
			}
		}
		if (constrain) {
			if (module_stack.size() > 0) {
				vector<shared_ptr<runtime_variable>> search = module_stack[module_stack.size() - 1];
				for (int i = search.size() - 1; i >= 0; i--) {
					if (search[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
						if (search[i]->get_storage_field().get_identifier() == ident)
							return make_pair(search[i], true);
				}
			}
			return make_pair(nullptr, false);
		}
		else {
			for (int j = module_stack.size() - 1; j >= 0; j--) {
				vector<shared_ptr<runtime_variable>> vec = module_stack[j];
				for (int i = vec.size() - 1; i >= 0; i--) {
					if (vec[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
						if (vec[i]->get_storage_field().get_identifier() == ident)
							return make_pair(vec[i], true);
				}
			}
		}
		global:
		for (int i = instruction_list.size() - 1; i >= 0; i--)
			if (instruction_list[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER) {
				if (instruction_list[i]->get_storage_field().get_identifier() == ident)
					return make_pair(instruction_list[i], true);
			}
		if (ident == "^") {
			return make_pair(make_shared<runtime_variable>(storage_field(-1, "^", storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
				pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), instruction_list,
					vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), added_lambda_instruction_list), runtime_type_information(runtime_type_kind::TYPE_MODULE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, "^"),
				vector<shared_ptr<function>>()), true);
		}
		return make_pair(nullptr, false);
	}

	const string runtime::runtime_temporary_prefix = "__temp__runtime_";

	vector<string> runtime::parse_instruction(string s) {
		string temp = s;
		temp.erase(temp.begin(), find_if(temp.begin(), temp.end(), [](char c) -> bool { return !isspace(c); }));
		temp.erase(find_if(temp.rbegin(), temp.rend(), [](char c) -> bool { return !isspace(c); }).base(), temp.end());
		vector<string> ret;
		string str;
		for (int i = 0; i < temp.length(); i++) {
			if (temp[i] == '\'') {
				str.push_back(temp[i]);
				i++;
				while (temp[i] != '\'' && i != temp.length()) {
					if (temp[i] == '\\' && i + 1 != temp.length() && temp[i + 1] == '\'') {
						str.push_back('\\');
						str.push_back('\'');
						i++;
					}
					else
						str.push_back(temp[i]);
					i++;
				}
				if (i == temp.length())
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				str.push_back('\'');
			}
			else if (temp[i] == '\"') {
				str.push_back(temp[i]);
				i++;
				while (temp[i] != '\"' && i != temp.length()) {
					if (temp[i] == '\\' && i + 1 != temp.length() && temp[i + 1] == '\"') {
						str.push_back('\\');
						str.push_back('\"');
						i++;
					}
					else
						str.push_back(temp[i]);
					i++;
				}
				if (i == temp.length())
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				str.push_back('\"');
			}
			else if (temp[i] == ' ') {
				ret.push_back(str);
				str.clear();
			}
			else
				str.push_back(temp[i]);
		}
		ret.push_back(str);
		return ret;
	}

	runtime::runtime(vector<string> vec, vector<shared_ptr<runtime_variable>> il,
		vector<vector<shared_ptr<runtime_variable>>> sfil, vector<vector<shared_ptr<runtime_variable>>> ls,
		vector<vector<shared_ptr<runtime_variable>>> ms, vector<shared_ptr<runtime_variable>> alil) {
		string_instruction_list = vec;
		runtime_temporary_count = 0;
		instruction_list = il;
		stacked_function_instruction_list = sfil;
		scope_stack = ls;
		module_stack = ms;
		added_lambda_instruction_list = alil;
	}

	runtime::~runtime() {

	}

	shared_ptr<runtime_variable> runtime::run_program() {
		function_pass();
		struct_pass();
		enum_pass();
		vector<vector<string>> insn_list;
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> temp = parse_instruction(string_instruction_list[i]);
			insn_list.push_back(temp);
		}
		for (int i = 0; i < insn_list.size(); i++) {
			vector<string> temp = insn_list[i];
			if (temp.size() == 0);
			else if (temp.size() == 1 && temp[0] == vm_instruction_list::scope)
				scope_stack.push_back(vector<shared_ptr<runtime_variable>>());
			else if (temp.size() == 1 && temp[0] == vm_instruction_list::escope)
				scope_stack.pop_back();
			else if (temp.size() == 1);
			else if (temp[0] == vm_instruction_list::list || temp[0] == vm_instruction_list::tupl) {
				if (temp.size() < 2)
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				string op = temp[0];
				string d = temp[1];
				vector<shared_ptr<runtime_variable>> vec;
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, false);
				runtime_type_information t_inf = runtime_type_information_list::bad;
				for (int i = 2; i < temp.size(); i++) {
					shared_ptr<runtime_variable> rvar = deduce_runtime_variable(temp[i], true);
					if (i == 2) {
						t_inf = rvar->get_runtime_type_information();
						if (t_inf == runtime_type_information_list::_nil && op == vm_instruction_list::list)
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
						if (op == vm_instruction_list::tupl)
							rvar->set_unmodifiable(true);
					}
					else if (op == vm_instruction_list::list) {
						if (t_inf == rvar->get_runtime_type_information());
						else if ((t_inf == runtime_type_information_list::_int && rvar->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
							(t_inf == runtime_type_information_list::_decimal && rvar->get_runtime_type_information() == runtime_type_information_list::_int)) {
							if (t_inf == runtime_type_information_list::_int) {
								rvar->set_integer(rvar->get_decimal());
								rvar->set_runtime_type_information(runtime_type_information_list::_int);
							}
							else {
								rvar->set_decimal(rvar->get_integer());
								rvar->set_runtime_type_information(runtime_type_information_list::_decimal);
							}
						}
						else if (rvar->get_runtime_type_information() == runtime_type_information_list::_nil) {
							if (t_inf == runtime_type_information_list::_int) {
								rvar->set_integer(0);
								rvar->set_runtime_type_information(runtime_type_information_list::_int);
							}
							else if (t_inf == runtime_type_information_list::_decimal) {
								rvar->set_decimal(0.0);
								rvar->set_runtime_type_information(runtime_type_information_list::_decimal);
							}
							else if (t_inf == runtime_type_information_list::_boolean) {
								rvar->set_boolean(false);
								rvar->set_runtime_type_information(runtime_type_information_list::_boolean);
							}
							else if (t_inf == runtime_type_information_list::_string) {
								rvar->set_string("");
								rvar->set_runtime_type_information(runtime_type_information_list::_string);
							}
							else if (t_inf == runtime_type_information_list::_list) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_runtime_type_information(runtime_type_information_list::_list);
							}
							else if (t_inf == runtime_type_information_list::_tuple) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_runtime_type_information(runtime_type_information_list::_tuple);
							}
							else if (t_inf == runtime_type_information_list::_dict) {
								rvar->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
								rvar->set_runtime_type_information(runtime_type_information_list::_dict);
							}
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
						}
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
					}
					else if (op == vm_instruction_list::tupl)
						rvar->set_unmodifiable(true);
					vec.push_back(rvar);
				}
				dest->set_list_tuple(vec);
				dest->set_runtime_type_information(op == vm_instruction_list::list ? runtime_type_information_list::_list : runtime_type_information_list::_tuple);
			}
			else if (temp[0] == vm_instruction_list::dict) {
				if (temp.size() < 2)
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				string d = temp[1];
				vector<shared_ptr<runtime_variable>> key_list;
				vector<shared_ptr<runtime_variable>> value_list;
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, false);
				runtime_type_information key_t_inf = runtime_type_information_list::bad;
				runtime_type_information value_t_inf = runtime_type_information_list::bad;
				for (int i = 2; i < temp.size(); i++) {
					if (i % 2 == 0) {
						shared_ptr<runtime_variable> key = deduce_runtime_variable(temp[i], true);
						if (i == 2) {
							key_t_inf = key->get_runtime_type_information();
							if (key_t_inf == runtime_type_information_list::_nil)
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key);
							key_list.push_back(key);
						}
						else {
							if (key->get_runtime_type_information() == key_t_inf);
							else if ((key_t_inf == runtime_type_information_list::_int && key->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
								(key_t_inf == runtime_type_information_list::_decimal && key->get_runtime_type_information() == runtime_type_information_list::_int)) {
								if (key_t_inf == runtime_type_information_list::_int) {
									key->set_integer(key->get_decimal());
									key->set_runtime_type_information(runtime_type_information_list::_int);
								}
								else {
									key->set_decimal(key->get_integer());
									key->set_runtime_type_information(runtime_type_information_list::_decimal);
								}
							}
							else if (key->get_runtime_type_information() == runtime_type_information_list::_nil) {
								if (key_t_inf == runtime_type_information_list::_int) {
									key->set_integer(0);
									key->set_runtime_type_information(runtime_type_information_list::_int);
								}
								else if (key_t_inf == runtime_type_information_list::_decimal) {
									key->set_decimal(0.0);
									key->set_runtime_type_information(runtime_type_information_list::_decimal);
								}
								else if (key_t_inf == runtime_type_information_list::_boolean) {
									key->set_boolean(false);
									key->set_runtime_type_information(runtime_type_information_list::_boolean);
								}
								else if (key_t_inf == runtime_type_information_list::_string) {
									key->set_string("");
									key->set_runtime_type_information(runtime_type_information_list::_string);
								}
								else if (key_t_inf == runtime_type_information_list::_list) {
									key->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									key->set_runtime_type_information(runtime_type_information_list::_list);
								}
								else if (key_t_inf == runtime_type_information_list::_tuple) {
									key->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									key->set_runtime_type_information(runtime_type_information_list::_tuple);
								}
								else if (key_t_inf == runtime_type_information_list::_dict) {
									key->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
									key->set_runtime_type_information(runtime_type_information_list::_dict);
								}
								else
									report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key);
							}
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key);
							key_list.push_back(key);
						}
					}
					else {
						shared_ptr<runtime_variable> value = deduce_runtime_variable(temp[i], true);
						if (i == 3) {
							value_t_inf = value->get_runtime_type_information();
							if (value_t_inf == runtime_type_information_list::_nil)
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, value);
							value_list.push_back(value);
						}
						else {
							if (value->get_runtime_type_information() == value_t_inf);
							else if ((value_t_inf == runtime_type_information_list::_int && value->get_runtime_type_information() == runtime_type_information_list::_decimal) ||
								(value_t_inf == runtime_type_information_list::_decimal && value->get_runtime_type_information() == runtime_type_information_list::_int)) {
								if (value_t_inf == runtime_type_information_list::_int) {
									value->set_integer(value->get_decimal());
									value->set_runtime_type_information(runtime_type_information_list::_int);
								}
								else {
									value->set_decimal(value->get_integer());
									value->set_runtime_type_information(runtime_type_information_list::_decimal);
								}
							}
							else if (value->get_runtime_type_information() == runtime_type_information_list::_nil) {
								if (value_t_inf == runtime_type_information_list::_int) {
									value->set_integer(0);
									value->set_runtime_type_information(runtime_type_information_list::_int);
								}
								else if (value_t_inf == runtime_type_information_list::_decimal) {
									value->set_decimal(0.0);
									value->set_runtime_type_information(runtime_type_information_list::_decimal);
								}
								else if (value_t_inf == runtime_type_information_list::_boolean) {
									value->set_boolean(false);
									value->set_runtime_type_information(runtime_type_information_list::_boolean);
								}
								else if (value_t_inf == runtime_type_information_list::_string) {
									value->set_string("");
									value->set_runtime_type_information(runtime_type_information_list::_string);
								}
								else if (value_t_inf == runtime_type_information_list::_list) {
									value->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									value->set_runtime_type_information(runtime_type_information_list::_list);
								}
								else if (value_t_inf == runtime_type_information_list::_tuple) {
									value->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									value->set_runtime_type_information(runtime_type_information_list::_tuple);
								}
								else if (value_t_inf == runtime_type_information_list::_dict) {
									value->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
									value->set_runtime_type_information(runtime_type_information_list::_dict);
								}
								else
									report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, value);
							}
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, value);
							value_list.push_back(value);
						}
					}
				}
				if (value_list.size() == key_list.size());
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
				dest->set_dict(make_pair(key_list, value_list));
				dest->set_runtime_type_information(runtime_type_information_list::_dict);
			}
			else if (temp[0] == vm_instruction_list::lambda || temp[0] == vm_instruction_list::ilambda) {
				string f = temp[1];
				bool immut = temp[0] == vm_instruction_list::ilambda;
				i++;
				vector<string> function_code;
				int elambda_count = 0;
				while (i < insn_list.size()) {
					if (insn_list[i].size() > 0 && (insn_list[i][0] == vm_instruction_list::lambda || insn_list[i][0] == vm_instruction_list::ilambda))
						elambda_count++;
					if (insn_list[i].size() > 0 && insn_list[i][0] == vm_instruction_list::elambda) {
						if (elambda_count == 0)
							break;
						else {
							string code;
							for (int j = 0; j < insn_list[i].size(); j++)
								code += insn_list[i][j] + (j == insn_list[i].size() - 1 ? "" : " ");
							function_code.push_back(code);
							elambda_count--;
						}
					}
					else {
						string code;
						for (int j = 0; j < insn_list[i].size(); j++)
							code += insn_list[i][j] + (j == insn_list[i].size() - 1 ? "" : " ");
						function_code.push_back(code);
					}
					i++;
				}
				vector<string> fal_temp(temp.begin() + 2, temp.end());
				bool va_args = false;
				if (fal_temp.size() > 0 && fal_temp[fal_temp.size() - 1] == builtins::va_args_function_parameter) {
					fal_temp.erase(fal_temp.end() - 1, fal_temp.end());
					va_args = true;
				}
				vector<string> fal;
				for (int i = 0; i < fal_temp.size(); i++)
					fal.push_back("[" + fal_temp[i] + "]");
				shared_ptr<runtime> r = make_shared<runtime>(function_code, vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>());
				r->set_instruction_list(instruction_list);
				r->set_scope_stack(scope_stack);
				r->set_stacked_function_instruction_list(stacked_function_instruction_list);
				r->set_module_stack(module_stack);
				r->set_added_lambda_instruction_list(added_lambda_instruction_list);
				shared_ptr<function> func = make_shared<function>(f, function_code, fal, va_args, true);
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, f, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, f, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
					r, runtime_type_information_list::_func, vector<shared_ptr<function>> { func });
				rvar->set_unmodifiable(immut);
				checked_insertion(rvar);
				rvar->set_module_runtime(r, rvar->get_unique_id());
				r->checked_insertion(rvar);
				added_lambda_instruction_list.push_back(rvar);
			}
			else if (temp[0] == vm_instruction_list::module || temp[0] == vm_instruction_list::imodule) {
				if (temp.size() != 2)
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				string m = temp[1];
				bool immut = temp[0] == vm_instruction_list::imodule;
				insn_list.erase(insn_list.begin() + i, insn_list.begin() + i + 1);
				vector<string> module_code;
				int emodule_count = 0;
				while (i < insn_list.size()) {
					if (insn_list[i].size() > 0 && (insn_list[i][0] == vm_instruction_list::module || insn_list[i][0] == vm_instruction_list::imodule))
						emodule_count++;
					if (insn_list[i].size() > 0 && insn_list[i][0] == vm_instruction_list::emodule) {
						if (emodule_count == 0)
							break;
						else {
							string code;
							for (int j = 0; j < insn_list[i].size(); j++)
								code += insn_list[i][j] + (j == insn_list[i].size() - 1 ? "" : " ");
							module_code.push_back(code);
							emodule_count--;
						}
					}
					else {
						string code;
						for (int j = 0; j < insn_list[i].size(); j++)
							code += insn_list[i][j] + (j == insn_list[i].size() - 1 ? "" : " ");
						module_code.push_back(code);
					}
					insn_list.erase(insn_list.begin() + i, insn_list.begin() + i + 1);
				}
				insn_list.erase(insn_list.begin() + i, insn_list.begin() + i + 1);
				i--;
				runtime_type_information _module(runtime_type_kind::TYPE_MODULE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, m);
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, m, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, m, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
					make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(),
						vector<shared_ptr<runtime_variable>>()), _module, vector<shared_ptr<function>>());
				rvar->set_unmodifiable(immut);
				shared_ptr<runtime_variable>& dummy = checked_insertion(rvar);
				dummy->get_module_runtime()->instruction_list = instruction_list;
				dummy->get_module_runtime()->stacked_function_instruction_list = stacked_function_instruction_list;
				dummy->get_module_runtime()->scope_stack = vector<vector<shared_ptr<runtime_variable>>>();
				dummy->get_module_runtime()->set_module_stack(module_stack);
				dummy->get_module_runtime()->module_stack.push_back(vector<shared_ptr<runtime_variable>>());
				dummy->get_module_runtime()->string_instruction_list = module_code;
				dummy->get_module_runtime()->set_added_lambda_instruction_list(added_lambda_instruction_list);
				dummy->get_module_runtime()->run_program();
			}
			else if (temp[0] == vm_instruction_list::brk) {
				string label = temp[1];
				int pop_count = 0;
				while (i < insn_list.size()) {
					vector<string> inner_temp = insn_list[i];
					if (inner_temp.size() == 1 && inner_temp[0] == label)
						break;
					else if (inner_temp.size() > 0 && inner_temp[0] == vm_instruction_list::scope)
						pop_count--;
					else if (inner_temp.size() > 0 && inner_temp[0] == vm_instruction_list::escope)
						pop_count++;
					i++;
				}
				for (int i = 0; i < pop_count; i++)
					scope_stack.pop_back();
				i--;
			}
			else if (temp[0] == vm_instruction_list::cont) {
				string label = temp[1];
				int pop_count = 0;
				while (i >= 0) {
					vector<string> inner_temp = insn_list[i];
					if (inner_temp.size() == 1 && inner_temp[0] == label)
						break;
					else if (inner_temp.size() > 0 && inner_temp[0] == vm_instruction_list::scope)
						pop_count++;
					else if (inner_temp.size() > 0 && inner_temp[0] == vm_instruction_list::escope)
						pop_count--;
					i--;
				}
				for (int i = 0; i < pop_count; i++)
					scope_stack.pop_back();
				i--;
			}
			else if (temp.size() == 2) {
				string op = temp[0];
				string d = temp[1];
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, true);
				if (op == vm_instruction_list::bneg) {
					pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__boolean_negate__, make_pair(true, false));
					if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean)
						dest = !dest;
					else if (pai.second) {
						bool success = false;
						for (int i = 0; i < pai.first->get_function().size(); i++) {
							shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
							if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
							else {
								dest = mov(dest, temp, false);
								success = true;
								break;
							}
						}
						if (!success)
							dest = !dest;
					}
					else
						dest = !dest;
				}
				else if (op == vm_instruction_list::cmpl) {
					pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__complement__, make_pair(true, false));
					if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
						dest = ~dest;
					else if (pai.second) {
						bool success = false;
						for (int i = 0; i < pai.first->get_function().size(); i++) {
							shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
							if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
							else {
								dest = mov(dest, temp, false);
								success = true;
								break;
							}
						}
						if (!success)
							dest = ~dest;
					}
					else
						dest = ~dest;
				}
				else if (op == vm_instruction_list::tyof)
					dest = tyof(dest);
				else if (op == vm_instruction_list::poinc || op == vm_instruction_list::princ) {
					if (op == vm_instruction_list::princ) {
						pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__pre_increment__, make_pair(true, false));
						if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
							dest = inc(dest);
						else if (pai.second) {
							bool success = false;
							for (int i = 0; i < pai.first->get_function().size(); i++) {
								shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
								if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
								else {
									success = true;
									break;
								}
							}
							if (!success)
								dest = inc(dest);
						}
						else
							dest = inc(dest);
					}
					else {
						pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__post_increment__, make_pair(true, false));
						if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
							dest = inc(dest);
						else if (pai.second) {
							bool success = false;
							for (int i = 0; i < pai.first->get_function().size(); i++) {
								shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
								if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
								else {
									success = true;
									break;
								}
							}
							if (!success)
								dest = inc(dest);
						}
						else
							dest = inc(dest);
					}
				}
				else if (op == vm_instruction_list::podec || op == vm_instruction_list::prdec) {
					if (op == vm_instruction_list::prdec) {
						pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__pre_decrement__, make_pair(true, false));
						if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
							dest = dec(dest);
						else if (pai.second) {
							bool success = false;
							for (int i = 0; i < pai.first->get_function().size(); i++) {
								shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
								if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
								else {
									success = true;
									break;
								}
							}
							if (!success)
								dest = dec(dest);
						}
						else
							dest = dec(dest);
					}
					else {
						pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__post_decrement__, make_pair(true, false));
						if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
							dest = dec(dest);
						else if (pai.second) {
							bool success = false;
							for (int i = 0; i < pai.first->get_function().size(); i++) {
								shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
								if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
								else {
									success = true;
									break;
								}
							}
							if (!success)
								dest = dec(dest);
						}
						else
							dest = dec(dest);
					}
				}
				else if (op == vm_instruction_list::neg) {
					pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__numeric_negate__, make_pair(true, false));
					if (dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal)
						dest = -dest;
					else if (pai.second) {
						bool success = false;
						for (int i = 0; i < pai.first->get_function().size(); i++) {
							shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
							if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
							else {
								dest = mov(dest, temp, false);
								success = true;
								break;
							}
						}
						if (!success)
							dest = -dest;
					}
					else
						dest = -dest;
				}
				else if (op == vm_instruction_list::pos) {
					pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(builtins::builtin__numeric_positive__, make_pair(true, false));
					if (dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal)
						dest = +dest;
					else if (pai.second) {
						bool success = false;
						for (int i = 0; i < pai.first->get_function().size(); i++) {
							shared_ptr<runtime_variable> temp = run_function({ pai.first->get_function()[i] }, pai.first, { dest }, nullptr);
							if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
							else {
								dest = mov(dest, temp, false);
								success = true;
								break;
							}
						}
						if (!success)
							dest = +dest;
					}
					else
						dest = +dest;
				}
				else if (op == vm_instruction_list::ret)
					return dest;
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
			}
			else if (temp.size() == 3 && temp[0] == vm_instruction_list::imprt) {
				string file_name = temp[1] + file::dot + karma_lang_source_file_extension;
				string compiled_file_name = temp[1] + file::dot + dharma_vm_source_file_extension;
				string module_name = temp[2];
				bool source_file_present = false, compiled_file_present = false;
				file result("");
				for (int i = 0; i < default_include_search_path_list.size(); i++) {
					string sd = default_include_search_path_list[i];
					vector<file> all = search_directory(file(sd));
					for (int i = 0; i < all.size(); i++) {
						file f = all[i];
						if (f.is_file() && f.get_full_name() == file_name)
							source_file_present = true;
						else if (f.is_file() && f.get_full_name() == compiled_file_name && !compiled_file_present) {
							compiled_file_present = true;
							result = f;
							if (source_file_present)
								break;
						}
					}
				}
				if (!compiled_file_present) {
					if (source_file_present)
						report_error_and_terminate_program(runtime_diagnostic_messages::import_file_not_found_but_source_file_present, nullptr);
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::import_file_not_found, nullptr);
				}
				vm v(result);
				v.run();
				shared_ptr<runtime> prog = v.get_program();
				shared_ptr<runtime_variable> file_module = make_shared<runtime_variable>(storage_field(-1, module_name, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, module_name, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
					prog, runtime_type_information(runtime_type_kind::TYPE_MODULE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, builtins::builtin_runtime_file_module_prefix + module_name), vector<shared_ptr<function>>());
				checked_insertion(file_module);
			}
			else if (temp.size() == 3 && temp[0] == vm_instruction_list::jmp) {
				string d = temp[1];
				string label = temp[2];
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, true);
				if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean);
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
				if (dest->get_boolean()) {
					for (int j = 0; j < insn_list.size(); j++) {
						vector<string> insn = insn_list[j];
						if (insn.size() == 1 && insn[0] == label) {
							i = j - 1;
							break;
						}
					}
				}
			}
			else if (temp.size() == 3) {
				string op = temp[0];
				string d = temp[1];
				string s = temp[2];
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, false, temp[0] == vm_instruction_list::dmov || temp[0] == vm_instruction_list::imov);
				tuple<string, register_identifier_kind, runtime_type_kind> dtup = deduce_register_identifier_kind(d);
				tuple<string, register_identifier_kind, runtime_type_kind> tup = deduce_register_identifier_kind(s);
				bool complex_test = get<1>(dtup) == register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX ||
					(dest->get_runtime_type_information() != runtime_type_information_list::bad && dest->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER);
				if (get<1>(tup) == register_identifier_kind::REGISTER_IDENTIFIER_LITERAL) {
					if (op == vm_instruction_list::mov || op == vm_instruction_list::imov || op == vm_instruction_list::dmov);
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
					if (get<2>(tup) == runtime_type_kind::TYPE_DECIMAL) {
						if (complex_test) {
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
								dest->set_integer(stof(get<0>(tup)));
							else if (dest->get_runtime_type_information() == runtime_type_information_list::_decimal)
								dest->set_decimal(stof(get<0>(tup)));
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_decimal(stof(get<0>(tup)));
							dest->set_runtime_type_information(runtime_type_information_list::_decimal);
						}
					}
					else if (get<2>(tup) == runtime_type_kind::TYPE_INT) {
						string str = get<0>(tup);
						for (int i = 0; i < str.length(); i++) {
							if (str[i] == '\'') {
								str.erase(i, 1);
								i--;
							}
						}
						string prefix = get<0>(tup).substr(0, 2);
						int i;
						if (prefix == "0x" || prefix == "0X")
							i = strtol(str.substr(2).c_str(), nullptr, 16);
						else if (prefix == "0b" || prefix == "0B")
							i = strtol(str.substr(2).c_str(), nullptr, 2);
						else if (prefix == "0o" || prefix == "0O")
							i = strtol(str.substr(2).c_str(), nullptr, 8);
						else
							i = strtol(str.c_str(), nullptr, 10);
						if (complex_test) {
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
								dest->set_integer(i);
							else if (dest->get_runtime_type_information() == runtime_type_information_list::_decimal)
								dest->set_decimal(i);
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_integer(i);
							dest->set_runtime_type_information(runtime_type_information_list::_int);
						}
					}
					else if (get<2>(tup) == runtime_type_kind::TYPE_BOOLEAN) {
						if (complex_test) {
							if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean)
								dest->set_boolean(get<0>(tup) == "true" ? true : false);
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_boolean(get<0>(tup) == "true" ? true : false);
							dest->set_runtime_type_information(runtime_type_information_list::_boolean);
						}
					}
					else if (get<2>(tup) == runtime_type_kind::TYPE_STRING) {
						string str = get<0>(tup);
						for (int i = 0; i < str.length(); i++) {
							if (i + 1 < str.length() && str[i] == '\\' && str[i + 1] == 'n') {
								str.erase(i, 2);
								str.insert(i, "\n");
								i--;
							}
							else if (i + 1 < str.length() && str[i] == '\\' && str[i + 1] == 't') {
								str.erase(i, 2);
								str.insert(i, "\t");
								i--;
							}
							else if (i + 1 < str.length() && str[i] == '\\' && (str[i + 1] == '\'' || str[i + 1] == '\"')) {
								char c = str[i + 1];
								str.erase(i, 2);
								if (c == '\'')
									str.insert(i, "\'");
								else
									str.insert(i, "\"");
								i--;
							}
						}
						if (complex_test) {
							if (dest->get_runtime_type_information() == runtime_type_information_list::_string)
								dest->set_string(str);
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_string(str);
							dest->set_runtime_type_information(runtime_type_information_list::_string);
						}
					}
					else if (get<2>(tup) == runtime_type_kind::TYPE_NIL) {
						if (dest->get_runtime_type_information() == runtime_type_information_list::_int)
							dest->set_integer(0);
						else if (dest->get_runtime_type_information() == runtime_type_information_list::_decimal)
							dest->set_decimal(0);
						else if (dest->get_runtime_type_information() == runtime_type_information_list::_string)
							dest->set_string("");
						else if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean)
							dest->set_boolean(false);
						else if (dest->get_runtime_type_information() == runtime_type_information_list::_list || dest->get_runtime_type_information() == runtime_type_information_list::_tuple)
							dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
						else if (dest->get_runtime_type_information() == runtime_type_information_list::_dict)
							dest->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
						else if (dest->get_runtime_type_information() == runtime_type_information_list::bad)
							dest->set_runtime_type_information(runtime_type_information_list::_nil);
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
					dest->set_unmodifiable(op == vm_instruction_list::imov);
				}
				else {
					shared_ptr<runtime_variable> src = deduce_runtime_variable(s, true);
					if (op == vm_instruction_list::mov || op == vm_instruction_list::imov || op == vm_instruction_list::dmov) {
						if (get<1>(dtup) == register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX ||
							(dest->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER &&
								dest->get_runtime_type_information() != runtime_type_information_list::bad)) {
							auto use_standard_assignment = [](shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
								runtime_type_information lhs = dest->get_runtime_type_information(), rhs = src->get_runtime_type_information();
								if (rhs == runtime_type_information_list::_nil && (lhs == runtime_type_information_list::_int || lhs == runtime_type_information_list::_decimal ||
									lhs == runtime_type_information_list::_boolean || lhs == runtime_type_information_list::_string || lhs == runtime_type_information_list::_list ||
									lhs == runtime_type_information_list::_tuple || lhs == runtime_type_information_list::_dict))
									return true;
								if ((lhs == runtime_type_information_list::_int || lhs == runtime_type_information_list::_decimal) && (rhs == runtime_type_information_list::_int ||
									rhs == runtime_type_information_list::_boolean))
									return true;
								if (lhs == rhs) {
									runtime_type_kind tkind = lhs.get_runtime_type_kind();
									if (tkind == runtime_type_kind::TYPE_MODULE || tkind == runtime_type_kind::TYPE_CUSTOM || tkind == runtime_type_kind::TYPE_ENUM ||
										tkind == runtime_type_kind::TYPE_ENUM_CHILD)
										return dest->get_unique_id() == src->get_unique_id();
									return true;
								}
								return false;
							};
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__assign__);
							if (use_standard_assignment(dest, src))
								dest = strict_mov(dest, src, op == vm_instruction_list::dmov || op == vm_instruction_list::imov);
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = strict_mov(dest, src, op == vm_instruction_list::dmov || op == vm_instruction_list::imov);
							}
							else
								dest = strict_mov(dest, src, op == vm_instruction_list::dmov || op == vm_instruction_list::imov);
						}
						else
							dest = mov(dest, src, op == vm_instruction_list::dmov || op == vm_instruction_list::imov);
						if(!dest->get_unmodifiable())
							dest->set_unmodifiable(op == vm_instruction_list::imov);
					}
					else if (op == vm_instruction_list::add || op == vm_instruction_list::adde) {
						if (op == vm_instruction_list::add) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__add__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest + src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest + src;
							}
							else
								dest = dest + src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__add_equals__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest + src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest + src;
							}
							else
								dest = dest + src;
						}
					}
					else if (op == vm_instruction_list::sub || op == vm_instruction_list::sube) {
						if (op == vm_instruction_list::sub) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__subtract__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest - src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest - src;
							}
							else
								dest = dest - src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__subtract_equals__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest - src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest - src;
							}
							else
								dest = dest - src;
						}
					}
					else if (op == vm_instruction_list::mul || op == vm_instruction_list::mule) {
						if (op == vm_instruction_list::mul) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__multiply__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest * src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest * src;
							}
							else
								dest = dest * src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__multiply_equals__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest * src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest * src;
							}
							else
								dest = dest * src;
						}
					}
					else if (op == vm_instruction_list::div || op == vm_instruction_list::dive) {
						if (op == vm_instruction_list::div) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__divide__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest / src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest / src;
							}
							else
								dest = dest / src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__divide_equals__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = dest / src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest / src;
							}
							else
								dest = dest / src;
						}
					}
					else if (op == vm_instruction_list::mod || op == vm_instruction_list::mode) {
						if (op == vm_instruction_list::mod) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__modulus__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest % src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest % src;
							}
							else
								dest = dest % src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__modulus_equals__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest % src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest % src;
							}
							else
								dest = dest % src;
						}
					}
					else if (op == vm_instruction_list::exp || op == vm_instruction_list::expe) {
						if (op == vm_instruction_list::exp) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__exponent__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = pow(dest, src);
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = pow(dest, src);
							}
							else
								dest = pow(dest, src);
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__exponent_equals__);
							if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
								(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
								dest = pow(dest, src);
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = pow(dest, src);
							}
							else
								dest = pow(dest, src);
						}
					}
					else if (op == vm_instruction_list::band || op == vm_instruction_list::bande) {
						if (op == vm_instruction_list::band) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__binary_and__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest & src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest & src;
							}
							else
								dest = dest & src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__binary_and_equals__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest & src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest & src;
							}
							else
								dest = dest & src;
						}
					}
					else if (op == vm_instruction_list::bor || op == vm_instruction_list::bore) {
						if (op == vm_instruction_list::bor) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__binary_or__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest | src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest | src;
							}
							else
								dest = dest | src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__binary_or_equals__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest | src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest | src;
							}
							else
								dest = dest | src;
						}
					}
					else if (op == vm_instruction_list::exor || op == vm_instruction_list::exore) {
						if (op == vm_instruction_list::exor) {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__exclusive_or__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest ^ src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest ^ src;
							}
							else
								dest = dest ^ src;
						}
						else {
							vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__exclusive_or_equals__);
							if (dest->get_runtime_type_information() == runtime_type_information_list::_int && src->get_runtime_type_information() == runtime_type_information_list::_int)
								dest = dest ^ src;
							else if (results.size() > 0) {
								bool success = false;
								for (int j = 0; j < results.size(); j++) {
									pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
									for (int i = 0; i < res.first->get_function().size(); i++) {
										shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
										if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
										else {
											dest = mov(dest, temp, false);
											success = true;
											break;
										}
									}
									if (success)
										break;
								}
								if (!success)
									dest = dest ^ src;
							}
							else
								dest = dest ^ src;
						}
					}
					else if (op == vm_instruction_list::land) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__logical_and__);
						if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean || src->get_runtime_type_information() == runtime_type_information_list::_boolean)
							dest = dest && src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest && src;
						}
						else
							dest = dest && src;
					}
					else if (op == vm_instruction_list::lor) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__logical_or__);
						if (dest->get_runtime_type_information() == runtime_type_information_list::_boolean || src->get_runtime_type_information() == runtime_type_information_list::_boolean)
							dest = dest || src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest || src;
						}
						else
							dest = dest || src;
					}
					else if (op == vm_instruction_list::equ) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__equals_equals__);
						runtime_type_information dtype = dest->get_runtime_type_information();
						runtime_type_information stype = src->get_runtime_type_information();
						if (dtype == stype || (dtype.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES && stype.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) ||
							((dtype == runtime_type_information_list::_int || dtype == runtime_type_information_list::_decimal) &&
								(stype == runtime_type_information_list::_int || stype == runtime_type_information_list::_decimal)) ||
							(dtype == runtime_type_information_list::_nil && (stype == runtime_type_information_list::_nil || stype == runtime_type_information_list::_int ||
								stype == runtime_type_information_list::_boolean || stype == runtime_type_information_list::_string || stype == runtime_type_information_list::_list ||
								stype == runtime_type_information_list::_tuple || stype == runtime_type_information_list::_dict)) ||
							(stype == runtime_type_information_list::_nil && (dtype == runtime_type_information_list::_nil || dtype == runtime_type_information_list::_int ||
								dtype == runtime_type_information_list::_boolean || dtype == runtime_type_information_list::_string || dtype == runtime_type_information_list::_list ||
								dtype == runtime_type_information_list::_tuple || dtype == runtime_type_information_list::_dict)))
							dest = dest == src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest == src;
						}
						else
							dest = dest == src;
					}
					else if (op == vm_instruction_list::nequ) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__not_equals__);
						runtime_type_information dtype = dest->get_runtime_type_information();
						runtime_type_information stype = src->get_runtime_type_information();
						if (dtype == stype || (dtype.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES && stype.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) ||
							((dtype == runtime_type_information_list::_int || dtype == runtime_type_information_list::_decimal) &&
								(stype == runtime_type_information_list::_int || stype == runtime_type_information_list::_decimal)) ||
							(dtype == runtime_type_information_list::_nil && (stype == runtime_type_information_list::_nil || stype == runtime_type_information_list::_int ||
								stype == runtime_type_information_list::_boolean || stype == runtime_type_information_list::_string || stype == runtime_type_information_list::_list ||
								stype == runtime_type_information_list::_tuple || stype == runtime_type_information_list::_dict)) ||
							(stype == runtime_type_information_list::_nil && (dtype == runtime_type_information_list::_nil || dtype == runtime_type_information_list::_int ||
								dtype == runtime_type_information_list::_boolean || dtype == runtime_type_information_list::_string || dtype == runtime_type_information_list::_list ||
								dtype == runtime_type_information_list::_tuple || dtype == runtime_type_information_list::_dict)))
							dest = dest != src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest != src;
						}
						else
							dest = dest != src;
					}
					else if (op == vm_instruction_list::gt) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__greater_than__);
						if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
							(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
							dest = dest > src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest > src;
						}
						else
							dest = dest > src;
					}
					else if (op == vm_instruction_list::lt) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__less_than__);
						if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
							(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
							dest = dest < src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest < src;
						}
						else
							dest = dest < src;
					}
					else if (op == vm_instruction_list::lte) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__less_than_or_equal_to__);
						if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
							(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
							dest = dest <= src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest <= src;
						}
						else
							dest = dest <= src;
					}
					else if (op == vm_instruction_list::gte) {
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__greater_than_or_equal_to__);
						if ((dest->get_runtime_type_information() == runtime_type_information_list::_int || dest->get_runtime_type_information() == runtime_type_information_list::_decimal) &&
							(src->get_runtime_type_information() == runtime_type_information_list::_int || src->get_runtime_type_information() == runtime_type_information_list::_decimal))
							dest = dest >= src;
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = dest >= src;
						}
						else
							dest = dest >= src;
					}
					else if (op == vm_instruction_list::cast) {
						if (src->get_runtime_type_information().get_type_pure_kind() != type_pure_kind::TYPE_PURE_YES)
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						auto use_standard_cast = [](shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
							runtime_type_information lhs = dest->get_runtime_type_information(), rhs = src->get_runtime_type_information();
							bool one = lhs.get_runtime_type_kind() == rhs.get_runtime_type_kind() && lhs.get_type_class_kind() == rhs.get_type_class_kind() &&
								lhs.get_class_name() == rhs.get_class_name();
							bool two = (lhs.get_runtime_type_kind() == runtime_type_kind::TYPE_INT || lhs.get_runtime_type_kind() == runtime_type_kind::TYPE_DECIMAL) &&
								(rhs.get_runtime_type_kind() == runtime_type_kind::TYPE_INT || rhs.get_runtime_type_kind() == runtime_type_kind::TYPE_DECIMAL);
							if (two)
								return true;
							if (!one)
								return false;
							runtime_type_kind tkind = lhs.get_runtime_type_kind();
							if (tkind == runtime_type_kind::TYPE_MODULE || tkind == runtime_type_kind::TYPE_ENUM || tkind == runtime_type_kind::TYPE_ENUM_CHILD ||
								tkind == runtime_type_kind::TYPE_CUSTOM)
								return dest->get_unique_id() == src->get_unique_id();
							return one;
						};
						vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__cast__);
						if (use_standard_cast(dest, src))
							dest = cast(dest, src);
						else if (results.size() > 0) {
							bool success = false;
							for (int j = 0; j < results.size(); j++) {
								pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
								for (int i = 0; i < res.first->get_function().size(); i++) {
									shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { dest, src }, res.second);
									if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
									else {
										dest = mov(dest, temp, false);
										success = true;
										break;
									}
								}
								if (success)
									break;
							}
							if (!success)
								dest = cast(dest, src);
						}
						else
							dest = cast(dest, src);
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
				}
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
		}
		shared_ptr<runtime_variable> created_bool = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean,
			vector<shared_ptr<function>>());
		runtime_temporary_count++;
		return checked_insertion(created_bool);
	}

	vector<shared_ptr<runtime_variable>> runtime::get_instruction_list() {
		return instruction_list;
	}

	bool runtime::dump_runtime_variables(vector<shared_ptr<runtime_variable>> insn_list) {
		for (int i = 0; i < insn_list.size(); i++) {
			shared_ptr<runtime_variable> rvar = insn_list[i];
			if (rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
				cout << rvar->get_storage_field().get_identifier();
			else
				cout << 'r' << rvar->get_storage_field().get_register_number();
			cout << ' ';
			if (rvar->get_runtime_type_information() == runtime_type_information_list::_int)
				cout << "Int: " << rvar->get_integer();
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_list) {
				cout << "List: " << '{' << '\n';
				dump_runtime_variables(rvar->get_list_tuple());
				cout << '}';
			}
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_tuple) {
				cout << "Tuple: " << '{' << '\n';
				dump_runtime_variables(rvar->get_list_tuple());
				cout << '}';
			}
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_dict) {
				cout << "Dict: " << '{' << '\n' << '{' << '\n';
				dump_runtime_variables(rvar->get_dict().first);
				cout << '}' << '\n' << '{' << '\n';
				dump_runtime_variables(rvar->get_dict().second);
				cout << '}' << '\n' << '}';
			}
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_nil)
				cout << "Nil";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_decimal)
				cout << "Decimal: " << rvar->get_decimal();
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_string)
				cout << "String: " << rvar->get_string();
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_boolean)
				cout << "Boolean: " << rvar->get_boolean();
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_func)
				cout << "Function: " << rvar->get_string();
			else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
				cout << rvar->get_runtime_type_information().get_class_name() << ": {\n";
				dump_runtime_variables(rvar->get_struct_enum_member_list());
				cout << "}";
			}
			else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO) {
				cout << "Module " << rvar->get_runtime_type_information().get_class_name() << ": {\n";
				dump_runtime_variables(rvar->get_module_runtime()->get_instruction_list());
				cout << "}";
			}
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_int)
				cout << "[Int]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_list)
				cout << "[List]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_tuple)
				cout << "[Tuple]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_dict)
				cout << "[Dict]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_nil)
				cout << "[Nil]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_decimal)
				cout << "[Decimal]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_string)
				cout << "[String]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_boolean)
				cout << "[Boolean]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_func)
				cout << "[Function]";
			else if (rvar->get_runtime_type_information() == runtime_type_information_list::bad)
				cout << "Bad";
			else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
				cout << "[" << rvar->get_runtime_type_information().get_class_name() << "]";
			else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
				cout << "[Module]";
			cout << '\n';
		}
		cout << "============\n";
		return true;
	}

	vector<shared_ptr<runtime_variable>> runtime::set_instruction_list(vector<shared_ptr<runtime_variable>> insn_list) {
		instruction_list.clear();
		for (int i = 0; i < insn_list.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(insn_list[i]->get_storage_field(), insn_list[i]->get_integer(), insn_list[i]->get_decimal(), insn_list[i]->get_string(), insn_list[i]->get_boolean(),
				insn_list[i]->get_list_tuple(), insn_list[i]->get_dict(), insn_list[i]->get_struct_enum_member_list(), insn_list[i]->get_module_runtime(), insn_list[i]->get_runtime_type_information(), insn_list[i]->get_function());
			rvar->set_unmodifiable(insn_list[i]->get_unmodifiable());
			rvar->set_unique_id(insn_list[i]->get_unique_id());
			instruction_list.push_back(rvar);
		}
		return instruction_list;
	}

	vector<vector<shared_ptr<runtime_variable>>> runtime::set_stacked_function_instruction_list(vector<vector<shared_ptr<runtime_variable>>> sfil) {
		stacked_function_instruction_list.clear();
		for (int j = 0; j < sfil.size(); j++) {
			vector<shared_ptr<runtime_variable>> vec = sfil[j];
			stacked_function_instruction_list.push_back(vector<shared_ptr<runtime_variable>>());
			for (int i = 0; i < vec.size(); i++) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(vec[i]->get_storage_field(), vec[i]->get_integer(), vec[i]->get_decimal(), vec[i]->get_string(), vec[i]->get_boolean(),
					vec[i]->get_list_tuple(), vec[i]->get_dict(), vec[i]->get_struct_enum_member_list(), vec[i]->get_module_runtime(), vec[i]->get_runtime_type_information(), vec[i]->get_function());
				rvar->set_unmodifiable(vec[i]->get_unmodifiable());
				rvar->set_unique_id(vec[i]->get_unique_id());
				stacked_function_instruction_list[stacked_function_instruction_list.size() - 1].push_back(rvar);
			}
		}
		return stacked_function_instruction_list;
	}

	vector<vector<shared_ptr<runtime_variable>>> runtime::set_scope_stack(vector<vector<shared_ptr<runtime_variable>>> ssl) {
		scope_stack.clear();
		for (int j = 0; j < ssl.size(); j++) {
			vector<shared_ptr<runtime_variable>> vec = ssl[j];
			scope_stack.push_back(vector<shared_ptr<runtime_variable>>());
			for (int i = 0; i < vec.size(); i++) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(vec[i]->get_storage_field(), vec[i]->get_integer(), vec[i]->get_decimal(), vec[i]->get_string(), vec[i]->get_boolean(),
					vec[i]->get_list_tuple(), vec[i]->get_dict(), vec[i]->get_struct_enum_member_list(), vec[i]->get_module_runtime(), vec[i]->get_runtime_type_information(), vec[i]->get_function());
				rvar->set_unmodifiable(vec[i]->get_unmodifiable());
				rvar->set_unique_id(vec[i]->get_unique_id());
				scope_stack[scope_stack.size() - 1].push_back(rvar);
			}
		}
		return scope_stack;
	}

	vector<vector<shared_ptr<runtime_variable>>> runtime::get_scope_stack() {
		return scope_stack;
	}

	vector<vector<shared_ptr<runtime_variable>>> runtime::get_stacked_function_instruction_list() {
		return stacked_function_instruction_list;
	}

	vector<string> runtime::get_string_instruction_list() {
		return string_instruction_list;
	}

	shared_ptr<runtime_variable> runtime::checked_insertion(shared_ptr<runtime_variable> rvar) {
		if (scope_stack.size() > 0) {
			scope_stack[scope_stack.size() - 1].push_back(rvar);
			return scope_stack[scope_stack.size() - 1][scope_stack[scope_stack.size() - 1].size() - 1];
		}
		else if (stacked_function_instruction_list.size() > 0) {
			stacked_function_instruction_list[stacked_function_instruction_list.size() - 1].push_back(rvar);
			return stacked_function_instruction_list[stacked_function_instruction_list.size() - 1][stacked_function_instruction_list[stacked_function_instruction_list.size() - 1].size() - 1];
		}
		else if (module_stack.size() > 0) {
			module_stack[module_stack.size() - 1].push_back(rvar);
			return module_stack[module_stack.size() - 1][module_stack[module_stack.size() - 1].size() - 1];
		}
		else {
			instruction_list.push_back(rvar);
			return instruction_list[instruction_list.size() - 1];
		}
		return nullptr;
	}

	vector<shared_ptr<runtime_variable>> runtime::get_added_lambda_instruction_list() {
		return added_lambda_instruction_list;
	}

	vector<string> runtime::set_string_instruction_list(vector<string> vec) {
		string_instruction_list.clear();
		for (int i = 0; i < vec.size(); i++)
			string_instruction_list.push_back(vec[i]);
		return string_instruction_list;
	}

	vector<vector<shared_ptr<runtime_variable>>> runtime::set_module_stack(vector<vector<shared_ptr<runtime_variable>>> ms) {
		module_stack.clear();
		for (int j = 0; j < ms.size(); j++) {
			vector<shared_ptr<runtime_variable>> m = ms[j];
			module_stack.push_back(vector<shared_ptr<runtime_variable>>());
			for (int i = 0; i < m.size(); i++) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(m[i]->get_storage_field(), m[i]->get_integer(), m[i]->get_decimal(), m[i]->get_string(), m[i]->get_boolean(), m[i]->get_list_tuple(), m[i]->get_dict(),
					m[i]->get_struct_enum_member_list(), m[i]->get_module_runtime(), m[i]->get_runtime_type_information(), m[i]->get_function());
				rvar->set_unmodifiable(m[i]->get_unmodifiable());
				rvar->set_unique_id(m[i]->get_unique_id());
				module_stack[module_stack.size() - 1].push_back(rvar);
			}
		}
		return module_stack;
	}

	vector<vector<shared_ptr<runtime_variable>>> runtime::get_module_stack() {
		return module_stack;
	}

	vector<shared_ptr<function>> runtime_variable::set_function(vector<shared_ptr<function>> f, long ui) {
		func.clear();
		for (int i = 0; i < f.size(); i++)
			func.push_back(make_shared<function>(f[i]->get_function_name(), f[i]->get_function_code(), f[i]->get_function_argument_list(), f[i]->get_va_args(), f[i]->is_lambda()));
		unique_id = ui;
		return func;
	}

	vector<shared_ptr<function>> runtime_variable::get_function() {
		return func;
	}

	vector<shared_ptr<function>> runtime_variable::add_function(shared_ptr<function> f) {
		func.push_back(f);
		return func;
	}

	vector<shared_ptr<runtime_variable>> runtime::set_added_lambda_instruction_list(vector<shared_ptr<runtime_variable>> alil) {
		added_lambda_instruction_list.clear();
		for (int i = 0; i < alil.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(alil[i]->get_storage_field(), alil[i]->get_integer(), alil[i]->get_decimal(), alil[i]->get_string(), alil[i]->get_boolean(), alil[i]->get_list_tuple(),
				alil[i]->get_dict(), alil[i]->get_struct_enum_member_list(), alil[i]->get_module_runtime(), alil[i]->get_runtime_type_information(), alil[i]->get_function());
			rvar->set_unmodifiable(alil[i]->get_unmodifiable());
			rvar->set_unique_id(alil[i]->get_unique_id());
			added_lambda_instruction_list.push_back(rvar);
		}
		return added_lambda_instruction_list;
	}

	unsigned long long runtime_variable::get_unique_id() {
		return unique_id;
	}

	unsigned long long runtime_variable::set_unique_id(unsigned long long ui) {
		unique_id = ui;
		return unique_id;
	}

	unsigned long long runtime_variable::set_enum_parent_unique_id(unsigned long long ui) {
		enum_parent_unique_id = ui;
		return enum_parent_unique_id;
	}

	unsigned long long runtime_variable::get_enum_parent_unique_id() {
		return enum_parent_unique_id;
	}

	vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> runtime::find_builtin_function(vector<shared_ptr<runtime_variable>> to_search, 
		shared_ptr<runtime> r, string bf) {
		vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> ret;
		for (int i = 0; i < to_search.size(); i++) {
			if (to_search[i]->get_string() == bf && to_search[i]->get_runtime_type_information() == runtime_type_information_list::_func)
				ret.push_back(make_pair(to_search[i], r));
			else if (to_search[i]->get_runtime_type_information().get_class_name().substr(0, builtins::builtin_runtime_file_module_prefix.length()) == builtins::builtin_runtime_file_module_prefix) {
				vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> res = to_search[i]->get_module_runtime()->find_builtin_function(to_search[i]->get_module_runtime()->get_instruction_list(), to_search[i]->get_module_runtime(), bf);
				ret.insert(ret.end(), res.begin(), res.end());
			}
		}
		return ret;
	}
}