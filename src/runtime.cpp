#include "../includes/runtime.hpp"

namespace dharma_vm {

	const string vm_instruction_list::mov = "mov";
	const string vm_instruction_list::inc = "inc";
	const string vm_instruction_list::dec = "dec";
	const string vm_instruction_list::cmpl = "cmpl";
	const string vm_instruction_list::neg = "neg";
	const string vm_instruction_list::pos = "pos";
	const string vm_instruction_list::bneg = "bneg";
	const string vm_instruction_list::tyof = "tyof";
	const string vm_instruction_list::add = "add";
	const string vm_instruction_list::mul = "mul";
	const string vm_instruction_list::div = "div";
	const string vm_instruction_list::mod = "mod";
	const string vm_instruction_list::exp = "exp";
	const string vm_instruction_list::sub = "sub";
	const string vm_instruction_list::shl = "shl";
	const string vm_instruction_list::shr = "shr";
	const string vm_instruction_list::equ = "equ";
	const string vm_instruction_list::nequ = "nequ";
	const string vm_instruction_list::lt = "lt";
	const string vm_instruction_list::lte = "lte";
	const string vm_instruction_list::gt = "gt";
	const string vm_instruction_list::gte = "gte";
	const string vm_instruction_list::bor = "bor";
	const string vm_instruction_list::band = "band";
	const string vm_instruction_list::exor = "exor";
	const string vm_instruction_list::land = "land";
	const string vm_instruction_list::lor = "lor";
	const string vm_instruction_list::cast = "cast";
	const string vm_instruction_list::exit = "exit";
	const string vm_instruction_list::jmp = "jmp";
	const string vm_instruction_list::list = "list";
	const string vm_instruction_list::tupl = "tupl";
	const string vm_instruction_list::dict = "dict";
	const string vm_instruction_list::func = "func";
	const string vm_instruction_list::efunc = "efunc";

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

	type_information::type_information(type_kind tk, type_pure_kind tpk, type_class_kind tck, string cn) {
		t_kind = tk;
		tp_kind = tpk;
		tc_kind = tck;
		class_name = cn;
	}

	type_information::~type_information() {

	}

	const type_kind type_information::get_type_kind() {
		return t_kind;
	}

	const type_pure_kind type_information::get_type_pure_kind() {
		return tp_kind;
	}

	const type_class_kind type_information::get_type_class_kind() {
		return tc_kind;
	}

	string type_information::get_class_name() {
		return class_name;
	}

	const bool type_information::operator==(type_information ti) {
		return (t_kind == ti.t_kind && tc_kind == ti.tc_kind && tp_kind == ti.tp_kind);
	}

	const bool type_information::operator!=(type_information ti) {
		return !(*this == ti);
	}

	runtime_variable::runtime_variable(storage_field sf, int i, float d, string s, bool b, vector<shared_ptr<runtime_variable>> lt, pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> di, type_information ti) : t_inf(ti), s_field(sf) {
		integer = i;
		decimal = d;
		str = s;
		boolean = b;
		list_tuple = lt;
		dict = di;
		if (t_inf == type_information_list::_tuple)
			unmodifiable = true;
		else
			unmodifiable = false;
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
		list_tuple = lt;
		return list_tuple;
	}

	pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> runtime_variable::set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>> d) {
		dict = d;
		return dict;
	}

	type_information runtime_variable::set_type_information(type_information ti) {
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

	type_information runtime_variable::get_type_information() {
		return t_inf;
	}

	const bool runtime_variable::get_unmodifiable() {
		return unmodifiable;
	}

	bool runtime_variable::set_unmodifiable(bool b) {
		unmodifiable = b;
		return unmodifiable;
	}

	storage_field::storage_field(int rn, string i, storage_field_kind sfk) {
		register_number = rn;
		identifier = i;
		sf_kind = sfk;
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

	bool report_error_and_terminate_program(string msg, shared_ptr<runtime_variable> rvar) {
		cerr << "Error: " << msg << '\n';
		string ident;
		if (rvar == nullptr)
			exit(1);
		if (rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
			ident = rvar->get_storage_field().get_identifier();
		else
			ident = "r" + to_string(rvar->get_storage_field().get_register_number());
		cerr << "\t" << ident << "; Type: '";
		if (rvar->get_type_information() == type_information_list::_int)
			cerr << "Int";
		else if (rvar->get_type_information() == type_information_list::_pure_int)
			cerr << "[Int]";
		else if (rvar->get_type_information() == type_information_list::_boolean)
			cerr << "Boolean";
		else if (rvar->get_type_information() == type_information_list::_pure_boolean)
			cerr << "[Boolean]";
		else if (rvar->get_type_information() == type_information_list::_string)
			cerr << "String";
		else if (rvar->get_type_information() == type_information_list::_pure_string)
			cerr << "[String]";
		else if (rvar->get_type_information() == type_information_list::_decimal)
			cerr << "Decimal";
		else if (rvar->get_type_information() == type_information_list::_pure_decimal)
			cerr << "[Decimal]";
		else if (rvar->get_type_information() == type_information_list::_list)
			cerr << "List";
		else if (rvar->get_type_information() == type_information_list::_pure_list)
			cerr << "[List]";
		else if (rvar->get_type_information() == type_information_list::_tuple)
			cerr << "Tuple";
		else if (rvar->get_type_information() == type_information_list::_pure_tuple)
			cerr << "[Tuple]";
		else if (rvar->get_type_information() == type_information_list::_dict)
			cerr << "Dict";
		else if (rvar->get_type_information() == type_information_list::_pure_dict)
			cerr << "[Dict]";
		else if (rvar->get_type_information() == type_information_list::_nil)
			cerr << "Nil";
		else if (rvar->get_type_information() == type_information_list::_pure_nil)
			cerr << "[Nil]";
		else if (rvar->get_type_information() == type_information_list::bad)
			cerr << "**Error**";
		cerr << ".'";
		exit(1);
		return true;
	}

	const type_information type_information_list::_int = type_information(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_string = type_information(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_boolean = type_information(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_decimal = type_information(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_list = type_information(type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_tuple = type_information(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_dict = type_information(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_nil = type_information(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_int = type_information(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_string = type_information(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_boolean = type_information(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_decimal = type_information(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_list = type_information(type_kind::TYPE_LIST, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_tuple = type_information(type_kind::TYPE_TUPLE, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_dict = type_information(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_nil = type_information(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::bad = type_information(type_kind::TYPE_NONE, type_pure_kind::TYPE_PURE_NONE, type_class_kind::TYPE_CLASS_NONE, "");
	const type_information type_information_list::_func = type_information(type_kind::TYPE_FUNC, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
	const type_information type_information_list::_pure_func = type_information(type_kind::TYPE_FUNC, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");

	shared_ptr<runtime_variable> operator+(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) || (type_two == type_information_list::_decimal && type_two == type_information_list::_int) ||
			(type_one == type_two && type_one == type_information_list::_int) || (type_one == type_two && type_one == type_information_list::_decimal));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one + two);
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one + two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one + two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one + two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator-(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one - two);
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one - two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one - two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one - two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator*(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one * two);
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one * two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one * two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one * two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator/(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(one / two);
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one / two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(one / two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(one / two);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> pow(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_integer(std::pow(one, two));
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(std::pow(one, two));
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_decimal(std::pow(one, two));
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_decimal(std::pow(one, two));
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator%(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_int && type_two == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one % two);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator|(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_int && type_two == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one | two);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator^(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_int && type_two == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one ^ two);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator&(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_int && type_two == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		int two = src->get_integer();
		dest->set_integer(one & two);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> operator>(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one > two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one > two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one > two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one > two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator<(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one < two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one < two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one < two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one < two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator>=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one >= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one >= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one >= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one >= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator<=(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if ((type_one == type_information_list::_decimal || type_one == type_information_list::_int) && (type_two == type_information_list::_decimal || type_two == type_information_list::_int));
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one <= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one <= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		else {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one <= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else {
				float two = src->get_decimal();
				dest->set_boolean(one <= two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> operator==(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_decimal) {
				float two = src->get_decimal();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_boolean(one == 0);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_decimal) {
			float one = dest->get_decimal();
			if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_decimal) {
				float two = src->get_decimal();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_boolean(one == 0);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_string) {
			string one = dest->get_string();
			if (type_two == type_information_list::_string) {
				string two = src->get_string();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_boolean(one == "");
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_func) {
			string one = dest->get_string();
			if (type_two == type_information_list::_func) {
				string two = src->get_string();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_boolean) {
			bool one = dest->get_boolean();
			if (type_two == type_information_list::_boolean) {
				bool two = src->get_boolean();
				dest->set_boolean(one == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_boolean(one == false);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_nil) {
			if (type_two == type_information_list::_nil) {
				dest->set_boolean(true);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_int) {
				int two = src->get_integer();
				dest->set_boolean(0 == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_boolean) {
				bool two = src->get_boolean();
				dest->set_boolean(false == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_string) {
				string two = src->get_string();
				dest->set_boolean("" == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_decimal) {
				float two = src->get_decimal();
				dest->set_boolean(0 == two);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_list) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_tuple) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_dict) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_list) {
			if (type_two == type_information_list::_nil) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_list) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_tuple) {
			if (type_two == type_information_list::_nil) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_tuple) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_dict) {
			if (type_two == type_information_list::_nil) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_dict) {
				dest->set_boolean(equals_equals(dest, src));
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (dest->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES &&
			src->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
			dest->set_boolean(dest->get_type_information() == src->get_type_information());
			dest->set_type_information(type_information_list::_boolean);
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
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_boolean && type_two == type_information_list::_boolean);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		bool one = dest->get_boolean();
		bool two = src->get_boolean();
		dest->set_boolean(one && two);
		dest->set_type_information(type_information_list::_boolean);
		return dest;
	}

	shared_ptr<runtime_variable> operator||(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_one == type_information_list::_boolean && type_two == type_information_list::_boolean);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		bool one = dest->get_boolean();
		bool two = src->get_boolean();
		dest->set_boolean(one || two);
		dest->set_type_information(type_information_list::_boolean);
		return dest;
	}

	shared_ptr<runtime_variable> operator!(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dest->get_type_information() == type_information_list::_boolean);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		bool one = dest->get_boolean();
		dest->set_boolean(!one);
		dest->set_type_information(type_information_list::_boolean);
		return dest;
	}

	shared_ptr<runtime_variable> operator-(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		if (type_one == type_information_list::_int || type_one == type_information_list::_decimal);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (type_one == type_information_list::_int) {
			int one = dest->get_integer();
			dest->set_integer(-one);
			dest->set_type_information(type_information_list::_int);
			return dest;
		}
		else {
			float one = dest->get_decimal();
			dest->set_decimal(-one);
			dest->set_type_information(type_information_list::_decimal);
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
	}

	shared_ptr<runtime_variable> operator~(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		if (type_one == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		int one = dest->get_integer();
		dest->set_integer(~one);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> inc(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		if (type_one == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		int one = dest->get_integer();
		dest->set_integer(one + 1);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> dec(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		if (type_one == type_information_list::_int);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		int one = dest->get_integer();
		dest->set_integer(one - 1);
		dest->set_type_information(type_information_list::_int);
		return dest;
	}

	shared_ptr<runtime_variable> tyof(shared_ptr<runtime_variable> dest) {
		if (dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		if (type_one == type_information_list::bad)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
		if (type_one.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			return dest;
		else {
			dest->set_type_information(type_information(dest->get_type_information().get_type_kind(), type_pure_kind::TYPE_PURE_YES, dest->get_type_information().get_type_class_kind(), dest->get_type_information().get_class_name()));
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, dest);
	}

	shared_ptr<runtime_variable> cast(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (type_two.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES);
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, nullptr);
		if (type_two == type_information_list::_pure_int) {
			if (type_one == type_information_list::_int || type_one == type_information_list::_pure_int)
				return dest;
			else if (type_one == type_information_list::_decimal) {
				dest->set_integer(dest->get_decimal());
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else if (type_one == type_information_list::_pure_decimal) {
				dest->set_type_information(type_information_list::_pure_int);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == type_information_list::_pure_decimal) {
			if (type_one == type_information_list::_decimal || type_one == type_information_list::_pure_decimal)
				return dest;
			else if (type_one == type_information_list::_int) {
				dest->set_decimal(dest->get_integer());
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else if (type_one == type_information_list::_pure_int) {
				dest->set_type_information(type_information_list::_pure_decimal);
				return dest;
			}
		}
		else if (type_two == type_information_list::_pure_boolean) {
			if (type_one == type_information_list::_boolean || type_one == type_information_list::_pure_boolean)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == type_information_list::_pure_string) {
			if (type_one == type_information_list::_string || type_one == type_information_list::_pure_string)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == type_information_list::_pure_list) {
			if (type_one == type_information_list::_list || type_one == type_information_list::_pure_list)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == type_information_list::_pure_tuple) {
			if (type_one == type_information_list::_tuple || type_one == type_information_list::_pure_tuple)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_two == type_information_list::_pure_dict) {
			if (type_one == type_information_list::_dict || type_one == type_information_list::_pure_dict)
				return dest;
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
	}

	shared_ptr<runtime_variable> mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (src == nullptr || dest == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		if (src->get_type_information() == type_information_list::_int) {
			dest->set_integer(src->get_integer());
			dest->set_type_information(type_information_list::_int);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_decimal) {
			dest->set_decimal(src->get_decimal());
			dest->set_type_information(type_information_list::_decimal);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_string) {
			dest->set_string(src->get_string());
			dest->set_type_information(type_information_list::_string);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_boolean) {
			dest->set_boolean(src->get_boolean());
			dest->set_type_information(type_information_list::_boolean);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_list) {
			dest->set_list_tuple(src->get_list_tuple());
			dest->set_type_information(type_information_list::_list);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_tuple) {
			dest->set_list_tuple(src->get_list_tuple());
			dest->set_type_information(type_information_list::_tuple);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_dict) {
			dest->set_dict(src->get_dict());
			dest->set_type_information(type_information_list::_dict);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_func) {
			dest->set_string(src->get_string());
			dest->set_type_information(type_information_list::_func);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_int) {
			dest->set_type_information(type_information_list::_pure_int);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_decimal) {
			dest->set_type_information(type_information_list::_pure_decimal);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_string) {
			dest->set_type_information(type_information_list::_pure_string);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_boolean) {
			dest->set_type_information(type_information_list::_pure_boolean);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_list) {
			dest->set_type_information(type_information_list::_pure_list);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_tuple) {
			dest->set_type_information(type_information_list::_pure_tuple);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_dict) {
			dest->set_type_information(type_information_list::_pure_dict);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_func) {
			dest->set_type_information(type_information_list::_pure_func);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_nil) {
			if (dest->get_type_information() == type_information_list::_int)
				dest->set_integer(0);
			else if (dest->get_type_information() == type_information_list::_decimal)
				dest->set_decimal(0);
			else if (dest->get_type_information() == type_information_list::_string)
				dest->set_string("");
			else if (dest->get_type_information() == type_information_list::_boolean)
				dest->set_boolean(false);
			else if (dest->get_type_information() == type_information_list::_list || dest->get_type_information() == type_information_list::_tuple)
				dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
			else if (dest->get_type_information() == type_information_list::_dict)
				dest->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
			else if (dest->get_type_information() == type_information_list::bad)
				dest->set_type_information(type_information_list::_nil);
			return dest;
		}
		else if (src->get_type_information() == type_information_list::_pure_nil) {
			dest->set_type_information(type_information_list::_pure_nil);
			return dest;
		}
		report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
	}

	shared_ptr<runtime_variable> strict_mov(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		if (dest == nullptr || src == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		type_information type_one = dest->get_type_information();
		type_information type_two = src->get_type_information();
		if (dest->get_unmodifiable())
			report_error_and_terminate_program(runtime_diagnostic_messages::unmodifiable_value, dest);
		dest->set_unmodifiable(src->get_unmodifiable());
		if (type_one == type_information_list::_int) {
			if (type_two == type_information_list::_decimal) {
				dest->set_integer(src->get_decimal());
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else if (type_two == type_information_list::_int) {
				dest->set_integer(src->get_integer());
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_integer(0);
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_decimal) {
			if (type_two == type_information_list::_int) {
				dest->set_decimal(src->get_integer());
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else if (type_two == type_information_list::_decimal) {
				dest->set_decimal(src->get_decimal());
				dest->set_type_information(type_information_list::_int);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_decimal(0.0);
				dest->set_type_information(type_information_list::_decimal);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_string) {
			if (type_two == type_information_list::_string) {
				dest->set_string(src->get_string());
				dest->set_type_information(type_information_list::_string);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_string("");
				dest->set_type_information(type_information_list::_string);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_boolean) {
			if (type_two == type_information_list::_boolean) {
				dest->set_boolean(src->get_boolean());
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_boolean) {
				dest->set_boolean(false);
				dest->set_type_information(type_information_list::_boolean);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_list) {
			if (type_two == type_information_list::_list) {
				dest->set_list_tuple(src->get_list_tuple());
				dest->set_type_information(type_information_list::_list);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
				dest->set_type_information(type_information_list::_list);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_tuple) {
			if (type_two == type_information_list::_tuple) {
				dest->set_list_tuple(src->get_list_tuple());
				dest->set_type_information(type_information_list::_tuple);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
				dest->set_type_information(type_information_list::_tuple);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_dict) {
			if (type_two == type_information_list::_dict) {
				dest->set_dict(src->get_dict());
				dest->set_type_information(type_information_list::_dict);
				return dest;
			}
			else if (type_two == type_information_list::_nil) {
				dest->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
				dest->set_type_information(type_information_list::_dict);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_func) {
			if (type_two == type_information_list::_func) {
				dest->set_string(src->get_string());
				dest->set_type_information(type_information_list::_func);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_int) {
			if (type_two == type_information_list::_pure_int) {
				dest->set_type_information(type_information_list::_pure_int);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_func) {
			if (type_two == type_information_list::_pure_func) {
				dest->set_type_information(type_information_list::_pure_func);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_decimal) {
			if (type_two == type_information_list::_pure_decimal) {
				dest->set_type_information(type_information_list::_pure_decimal);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_string) {
			if (type_two == type_information_list::_pure_string) {
				dest->set_type_information(type_information_list::_pure_string);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_boolean) {
			if (type_two == type_information_list::_pure_boolean) {
				dest->set_type_information(type_information_list::_pure_boolean);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_list) {
			if (type_two == type_information_list::_pure_list) {
				dest->set_type_information(type_information_list::_pure_list);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_tuple) {
			if (type_two == type_information_list::_pure_tuple) {
				dest->set_type_information(type_information_list::_pure_tuple);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
				return dest;
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
		}
		else if (type_one == type_information_list::_pure_dict) {
			if (type_two == type_information_list::_pure_dict) {
				dest->set_type_information(type_information_list::_pure_dict);
				return dest;
			}
			else if (type_two == type_information_list::_pure_nil) {
				dest->set_type_information(type_information_list::_pure_nil);
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
		if (dest->get_type_information() == type_information_list::_int) {
			if (src->get_type_information() == type_information_list::_int)
				return dest->get_integer() == src->get_integer();
			else if (src->get_type_information() == type_information_list::_decimal)
				return dest->get_integer() == src->get_decimal();
			else if (src->get_type_information() == type_information_list::_nil)
				return dest->get_integer() == 0;
			else
				return false;
		}
		else if (dest->get_type_information() == type_information_list::_decimal) {
			if (src->get_type_information() == type_information_list::_decimal)
				return dest->get_decimal() == src->get_decimal();
			else if (src->get_type_information() == type_information_list::_int)
				return dest->get_decimal() == src->get_integer();
			else if (src->get_type_information() == type_information_list::_nil)
				return dest->get_decimal() == 0.0;
			else
				return false;
		}
		else if (dest->get_type_information() == type_information_list::_boolean) {
			if (src->get_type_information() == type_information_list::_boolean)
				return dest->get_boolean() == src->get_boolean();
			else if (src->get_type_information() == type_information_list::_nil)
				return dest->get_boolean() == false;
			else
				return false;
		}
		else if (dest->get_type_information() == type_information_list::_func) {
			if (src->get_type_information() == type_information_list::_func)
				return dest->get_string() == src->get_string();
			else
				return false;
		}
		else if (dest->get_type_information() == type_information_list::_string) {
			if (src->get_type_information() == type_information_list::_string)
				return dest->get_string() == src->get_string();
			else if (src->get_type_information() == type_information_list::_nil)
				return dest->get_string() == "";
			else
				return false;
		}
		else if (dest->get_type_information() == type_information_list::_list) {
			if (src->get_type_information() == type_information_list::_nil)
				return dest->get_list_tuple().size() == 0;
			else if (src->get_type_information() == type_information_list::_list) {
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
		else if (dest->get_type_information() == type_information_list::_tuple) {
			if (src->get_type_information() == type_information_list::_nil)
				return dest->get_list_tuple().size() == 0;
			else if (src->get_type_information() == type_information_list::_tuple) {
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
		else if (dest->get_type_information() == type_information_list::_dict) {
			if (src->get_type_information() == type_information_list::_nil) {
				vector<shared_ptr<runtime_variable>> key_list = dest->get_dict().first;
				vector<shared_ptr<runtime_variable>> value_list = dest->get_dict().second;
				return key_list.size() == 0 && value_list.size() == 0;
			}
			else if (src->get_type_information() == type_information_list::_dict) {
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
		else if (dest->get_type_information() == type_information_list::_nil) {
			if (src->get_type_information() == type_information_list::_int)
				return src->get_integer() == 0;
			else if (src->get_type_information() == type_information_list::_decimal)
				return src->get_decimal() == 0.0;
			else if (src->get_type_information() == type_information_list::_boolean)
				return src->get_boolean() == false;
			else if (src->get_type_information() == type_information_list::_string)
				return src->get_string() == "";
			else if (src->get_type_information() == type_information_list::_list || src->get_type_information() == type_information_list::_tuple)
				return src->get_list_tuple().size() == 0;
			else if (src->get_type_information() == type_information_list::_dict) {
				vector<shared_ptr<runtime_variable>> key_list = src->get_dict().first;
				vector<shared_ptr<runtime_variable>> value_list = src->get_dict().second;
				return key_list.size() == 0 && value_list.size() == 0;
			}
			else if (src->get_type_information() == type_information_list::_nil)
				return true;
			else
				return false;
		}
		else if (dest->get_type_information() == src->get_type_information())
			return true;
		else
			return false;
		return false;
	}

	const bool not_equals(shared_ptr<runtime_variable> dest, shared_ptr<runtime_variable> src) {
		return !equals_equals(dest, src);
	}

	tuple<string, register_identifier_kind, type_kind> runtime::deduce_register_identifier_kind(string ident) {
		string temp = ident;
		if (temp[0] == 'r') {
			if ((int)temp.find('|') >= 0 || (int)temp.find('/') >= 0 || (int)temp.find('@') >= 0 || (int)temp.find(':') >= 0)
				return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX, type_kind::TYPE_NONE);
			temp.erase(0, 1);
			return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_REGISTER, type_kind::TYPE_NONE);
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
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, type_kind::TYPE_STRING);
				}
				else if ((int)temp.find('.') >= 0)
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, type_kind::TYPE_DECIMAL);
				else if (isdigit(temp[0]))
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, type_kind::TYPE_INT);
				else if (temp == "true" || temp == "false")
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, type_kind::TYPE_BOOLEAN);
				else if (temp == "nil")
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_LITERAL, type_kind::TYPE_NIL);
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
			}
			else {
				if ((int)temp.find('|') >= 0 || (int)temp.find('/') >= 0 || (int)temp.find('@') >= 0 || (int) temp.find(':') >= 0)
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX, type_kind::TYPE_NONE);
				else
					return make_tuple(temp, register_identifier_kind::REGISTER_IDENTIFIER_IDENTIFIER, type_kind::TYPE_NONE);
			}
		}
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
	}

	shared_ptr<runtime_variable> runtime::deduce_runtime_variable(string ident, bool must_exist) {
		tuple<string, register_identifier_kind, type_kind> deduced_tuple = deduce_register_identifier_kind(ident);
		string deduced_string = get<0>(deduced_tuple);
		register_identifier_kind deduced_ri_kind = get<1>(deduced_tuple);
		if (deduced_ri_kind == register_identifier_kind::REGISTER_IDENTIFIER_REGISTER) {
			pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(stoi(deduced_string));
			if (!pai.second) {
				if (must_exist) {
					report_error_and_terminate_program(runtime_diagnostic_messages::name_not_found, nullptr);
				}
				else {
					shared_ptr<runtime_variable> temp = make_shared<runtime_variable>(storage_field(stoi(deduced_string), "", storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER), -1, -1, "",
						false, vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::bad);
					instruction_list.push_back(temp);
					return instruction_list[instruction_list.size() - 1];
				}
			}
			else
				return pai.first;
		}
		else if (deduced_ri_kind == register_identifier_kind::REGISTER_IDENTIFIER_IDENTIFIER) {
			pair<shared_ptr<runtime_variable>, bool> pai = find_instruction(deduced_string);
			if (!pai.second) {
				if (must_exist)
					report_error_and_terminate_program(runtime_diagnostic_messages::name_not_found, nullptr);
				else {
					shared_ptr<runtime_variable> temp = make_shared<runtime_variable>(storage_field(-1, deduced_string, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
						pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::bad);
					instruction_list.push_back(temp);
					return instruction_list[instruction_list.size() - 1];
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
				if(temp.length() > 0)
					token_list.push_back(temp);
				temp.clear();
				token_list.push_back("@");
			}
			else if (deduced_string[i] == '|') {
				if(temp.length() > 0)
					token_list.push_back(temp);
				temp.clear();
				token_list.push_back("|");
			}
			else if (deduced_string[i] == '/') {
				if(temp.length() > 0)
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
		for (int i = 0; i < token_list.size(); i++) {
			if (i == 0) {
				string temp = token_list[i];
				if (temp[0] == 'r' && stoi(temp.substr(1)) > 0);
				else {
					temp.insert(0, "[");
					temp.push_back(']');
				}
				base = deduce_runtime_variable(temp, true);
			}
			else if (token_list[i] == "@") {
				if (i + 1 >= token_list.size() || base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				if (base->get_type_information() == type_information_list::_list || base->get_type_information() == type_information_list::_tuple) {
					if (token_list[i + 1] == "size") {
						shared_ptr<runtime_variable> created_int = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), base->get_list_tuple().size(), -1, "", false,
							vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::_int);
						instruction_list.push_back(created_int);
						base = instruction_list[instruction_list.size() - 1];
						runtime_temporary_count++;
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
				}
				else if (base->get_type_information() == type_information_list::_dict)
					report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
				else if (base->get_type_information() == type_information_list::_string) {
					if (token_list[i + 1] == "size") {
						shared_ptr<runtime_variable> created_int = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), base->get_string().length(), -1, "", false,
							vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::_int);
						instruction_list.push_back(created_int);
						base = instruction_list[instruction_list.size() - 1];
						runtime_temporary_count++;
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::field_not_found, base);
				}
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
			}
			else if (token_list[i] == "|") {
				if (i + 1 >= token_list.size() || base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				string temp = token_list[i + 1];
				if (temp[0] == 'r' && stoi(temp.substr(1)) > 0);
				else {
					temp.insert(0, "[");
					temp.push_back(']');
				}
				shared_ptr<runtime_variable> rvar = deduce_runtime_variable(temp, true);
				if (base->get_type_information() == type_information_list::_list || base->get_type_information() == type_information_list::_tuple) {
					if (rvar->get_type_information() != type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
					if (rvar->get_integer() >= 0 && rvar->get_integer() < base->get_list_tuple().size())
						base = base->get_list_tuple()[rvar->get_integer()];
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, rvar);
				}
				else if (base->get_type_information() == type_information_list::_string) {
					if (rvar->get_type_information() == type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
					if (rvar->get_integer() >= 0 && rvar->get_integer() < base->get_string().length()) {
						shared_ptr<runtime_variable> created_string = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, string(1, base->get_string()[rvar->get_integer()]), false,
							vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::_string);
						instruction_list.push_back(created_string);
						base = instruction_list[instruction_list.size() - 1];
						runtime_temporary_count++;
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, rvar);
				}
				else if (base->get_type_information() == type_information_list::_dict) {
					if (base->get_dict().first.size() != base->get_dict().first.size())
						report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, base);
					if (base->get_dict().first.size() == 0)
						report_error_and_terminate_program(runtime_diagnostic_messages::subscript_out_of_range, base);
					vector<shared_ptr<runtime_variable>> key_list = base->get_dict().first;
					if (rvar->get_type_information() == key_list[0]->get_type_information());
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
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
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
					if (s[0] == 'r' && stoi(s.substr(1)) > 0);
					else {
						s.insert(0, "[");
						s.push_back(']');
					}
					start_end_step_final.push_back(s);
				}
				shared_ptr<runtime_variable> start = deduce_runtime_variable(start_end_step_final[0], true);
				shared_ptr<runtime_variable> end = deduce_runtime_variable(start_end_step_final[1], true);
				shared_ptr<runtime_variable> step = deduce_runtime_variable(start_end_step_final[2], true);
				if (base->get_type_information() == type_information_list::_list || base->get_type_information() == type_information_list::_tuple) {
					if (start->get_type_information() != type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, start);
					else if (end->get_type_information() != type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, end);
					else if (step->get_type_information() != type_information_list::_int)
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
						vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), base->get_type_information());
					vector<shared_ptr<runtime_variable>> vec;
					for (int i = start->get_integer(); i < end->get_integer(); i += step->get_integer())
						vec.push_back(base->get_list_tuple()[i]);
					created_list_tuple->set_list_tuple(vec);
					instruction_list.push_back(created_list_tuple);
					base = instruction_list[instruction_list.size() - 1];
					runtime_temporary_count++;
				}
				else if (base->get_type_information() == type_information_list::_string) {
					if (start->get_type_information() != type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, start);
					else if (end->get_type_information() != type_information_list::_int)
						report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, end);
					else if (step->get_type_information() != type_information_list::_int)
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
						vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), base->get_type_information());
					string s;
					for (int i = start->get_integer(); i < end->get_integer(); i += step->get_integer())
						s.push_back(base->get_string()[i]);
					created_string->set_string(s);
					instruction_list.push_back(created_string);
					base = instruction_list[instruction_list.size() - 1];
					runtime_temporary_count++;
				}
			}
			else if (token_list[i] == ":") {
				if (base == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				if (base->get_type_information() != type_information_list::_func)
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, base);
				if (i + 1 >= token_list.size() || base == nullptr)
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
					if (tmp[0] == 'r' && stoi(tmp.substr(1)) > 0);
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
				shared_ptr<runtime_variable> fvar = nullptr;
				for (int i = 0; i < instruction_list.size(); i++) {
					shared_ptr<runtime_variable> rvar = instruction_list[i];
					if (rvar->get_type_information() == type_information_list::_func && rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER &&
						rvar->get_storage_field().get_identifier() == fname) {
						fvar = rvar;
						break;
					}
				}
				if (fvar == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::function_overload_not_found, base);
				shared_ptr<function> func = nullptr;
				for (int i = 0; i < function_list.size(); i++) {
					if (function_list[i]->get_function_argument_list().size() == rvar_arg_list.size()) {
						func = function_list[i];
						break;
					}
				}
				if (func == nullptr)
					report_error_and_terminate_program(runtime_diagnostic_messages::function_overload_not_found, fvar);
				base = run_function(func, fvar, rvar_arg_list);
			}
		}
		return base;
	}

	pair<shared_ptr<runtime_variable>, bool> runtime::find_instruction(int reg) {
		for (int i = 0; i < instruction_list.size(); i++)
			if (instruction_list[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER) {
				if (instruction_list[i]->get_storage_field().get_register_number() == reg) {
					return make_pair(instruction_list[i], true);
				}
			}
		return make_pair(nullptr, false);
	}

	pair<shared_ptr<runtime_variable>, bool> runtime::find_instruction(string ident) {
		for (int i = 0; i < instruction_list.size(); i++)
			if (instruction_list[i]->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER) {
				if (instruction_list[i]->get_storage_field().get_identifier() == ident)
					return make_pair(instruction_list[i], true);
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

	runtime::runtime(vector<string> vec, vector<shared_ptr<runtime_variable>> il, vector<shared_ptr<function>> fl) {
		string_instruction_list = vec;
		runtime_temporary_count = 0;
		function_list = fl;
		instruction_list = il;
	}

	runtime::~runtime() {

	}

	shared_ptr<runtime_variable> runtime::run_program() {
		function_pass();
		vector<vector<string>> insn_list;
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> temp = parse_instruction(string_instruction_list[i]);
			insn_list.push_back(temp);
		}
		for (int i = 0; i < insn_list.size(); i++) {
			vector<string> temp = insn_list[i];
			if (temp.size() == 0);
			if (temp[0] == vm_instruction_list::list || temp[0] == vm_instruction_list::tupl) {
				if (temp.size() < 2)
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				string op = temp[0];
				string d = temp[1];
				vector<shared_ptr<runtime_variable>> vec;
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, false);
				type_information t_inf = type_information_list::bad;
				for (int i = 2; i < temp.size(); i++) {
					shared_ptr<runtime_variable> rvar = deduce_runtime_variable(temp[i], true);
					if (i == 2) {
						t_inf = rvar->get_type_information();
						if (t_inf == type_information_list::_nil && op == vm_instruction_list::list)
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
						if (op == vm_instruction_list::tupl)
							rvar->set_unmodifiable(true);
					}
					else if (op == vm_instruction_list::list) {
						if (t_inf == rvar->get_type_information());
						else if ((t_inf == type_information_list::_int && rvar->get_type_information() == type_information_list::_decimal) ||
							(t_inf == type_information_list::_decimal && rvar->get_type_information() == type_information_list::_int)) {
							if (t_inf == type_information_list::_int) {
								rvar->set_integer(rvar->get_decimal());
								rvar->set_type_information(type_information_list::_int);
							}
							else {
								rvar->set_decimal(rvar->get_integer());
								rvar->set_type_information(type_information_list::_decimal);
							}
						}
						else if (rvar->get_type_information() == type_information_list::_nil) {
							if (t_inf == type_information_list::_int) {
								rvar->set_integer(0);
								rvar->set_type_information(type_information_list::_int);
							}
							else if (t_inf == type_information_list::_decimal) {
								rvar->set_decimal(0.0);
								rvar->set_type_information(type_information_list::_decimal);
							}
							else if (t_inf == type_information_list::_boolean) {
								rvar->set_boolean(false);
								rvar->set_type_information(type_information_list::_boolean);
							}
							else if (t_inf == type_information_list::_string) {
								rvar->set_string("");
								rvar->set_type_information(type_information_list::_string);
							}
							else if (t_inf == type_information_list::_list) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_type_information(type_information_list::_list);
							}
							else if (t_inf == type_information_list::_tuple) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_type_information(type_information_list::_tuple);
							}
							else if (t_inf == type_information_list::_dict) {
								rvar->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
								rvar->set_type_information(type_information_list::_dict);
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
				dest->set_type_information(op == vm_instruction_list::list ? type_information_list::_list : type_information_list::_tuple);
			}
			else if (temp[0] == vm_instruction_list::dict) {
				if (temp.size() < 2)
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
				string d = temp[1];
				vector<shared_ptr<runtime_variable>> key_list;
				vector<shared_ptr<runtime_variable>> value_list;
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, false);
				type_information key_t_inf = type_information_list::bad;
				type_information value_t_inf = type_information_list::bad;
				for (int i = 2; i < temp.size(); i++) {
					if (i % 2 == 0) {
						shared_ptr<runtime_variable> key = deduce_runtime_variable(temp[i], true);
						if (i == 2) {
							key_t_inf = key->get_type_information();
							if (key_t_inf == type_information_list::_nil)
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, key);
							key_list.push_back(key);
						}
						else {
							if (key->get_type_information() == key_t_inf);
							else if ((key_t_inf == type_information_list::_int && key->get_type_information() == type_information_list::_decimal) ||
								(key_t_inf == type_information_list::_decimal && key->get_type_information() == type_information_list::_int)) {
								if (key_t_inf == type_information_list::_int) {
									key->set_integer(key->get_decimal());
									key->set_type_information(type_information_list::_int);
								}
								else {
									key->set_decimal(key->get_integer());
									key->set_type_information(type_information_list::_decimal);
								}
							}
							else if (key->get_type_information() == type_information_list::_nil) {
								if (key_t_inf == type_information_list::_int) {
									key->set_integer(0);
									key->set_type_information(type_information_list::_int);
								}
								else if (key_t_inf == type_information_list::_decimal) {
									key->set_decimal(0.0);
									key->set_type_information(type_information_list::_decimal);
								}
								else if (key_t_inf == type_information_list::_boolean) {
									key->set_boolean(false);
									key->set_type_information(type_information_list::_boolean);
								}
								else if (key_t_inf == type_information_list::_string) {
									key->set_string("");
									key->set_type_information(type_information_list::_string);
								}
								else if (key_t_inf == type_information_list::_list) {
									key->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									key->set_type_information(type_information_list::_list);
								}
								else if (key_t_inf == type_information_list::_tuple) {
									key->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									key->set_type_information(type_information_list::_tuple);
								}
								else if (key_t_inf == type_information_list::_dict) {
									key->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
									key->set_type_information(type_information_list::_dict);
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
							value_t_inf = value->get_type_information();
							if (value_t_inf == type_information_list::_nil)
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, value);
							value_list.push_back(value);
						}
						else {
							if (value->get_type_information() == value_t_inf);
							else if ((value_t_inf == type_information_list::_int && value->get_type_information() == type_information_list::_decimal) ||
								(value_t_inf == type_information_list::_decimal && value->get_type_information() == type_information_list::_int)) {
								if (value_t_inf == type_information_list::_int) {
									value->set_integer(value->get_decimal());
									value->set_type_information(type_information_list::_int);
								}
								else {
									value->set_decimal(value->get_integer());
									value->set_type_information(type_information_list::_decimal);
								}
							}
							else if (value->get_type_information() == type_information_list::_nil) {
								if (value_t_inf == type_information_list::_int) {
									value->set_integer(0);
									value->set_type_information(type_information_list::_int);
								}
								else if (value_t_inf == type_information_list::_decimal) {
									value->set_decimal(0.0);
									value->set_type_information(type_information_list::_decimal);
								}
								else if (value_t_inf == type_information_list::_boolean) {
									value->set_boolean(false);
									value->set_type_information(type_information_list::_boolean);
								}
								else if (value_t_inf == type_information_list::_string) {
									value->set_string("");
									value->set_type_information(type_information_list::_string);
								}
								else if (value_t_inf == type_information_list::_list) {
									value->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									value->set_type_information(type_information_list::_list);
								}
								else if (value_t_inf == type_information_list::_tuple) {
									value->set_list_tuple(vector<shared_ptr<runtime_variable>>());
									value->set_type_information(type_information_list::_tuple);
								}
								else if (value_t_inf == type_information_list::_dict) {
									value->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
									value->set_type_information(type_information_list::_dict);
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
				dest->set_type_information(type_information_list::_dict);
			}
			else if (temp.size() == 2) {
				string op = temp[0];
				string d = temp[1];
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, true);
				if (op == vm_instruction_list::bneg)
					dest = !dest;
				else if (op == vm_instruction_list::cmpl)
					dest = ~dest;
				else if (op == vm_instruction_list::tyof)
					dest = tyof(dest);
				else if (op == vm_instruction_list::inc)
					dest = inc(dest);
				else if (op == vm_instruction_list::dec)
					dest = dec(dest);
				else if (op == vm_instruction_list::neg)
					dest = -dest;
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
			}
			else if (temp.size() == 3) {
				string op = temp[0];
				string d = temp[1];
				string s = temp[2];
				shared_ptr<runtime_variable> dest = deduce_runtime_variable(d, false);
				tuple<string, register_identifier_kind, type_kind> dtup = deduce_register_identifier_kind(d);
				tuple<string, register_identifier_kind, type_kind> tup = deduce_register_identifier_kind(s);
				bool complex_test = get<1>(dtup) == register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX ||
					(dest->get_type_information() != type_information_list::bad && dest->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER);
				if (get<1>(tup) == register_identifier_kind::REGISTER_IDENTIFIER_LITERAL) {
					if (op == vm_instruction_list::mov);
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
					if (get<2>(tup) == type_kind::TYPE_DECIMAL) {
						if (complex_test) {
							if (dest->get_type_information() == type_information_list::_int)
								dest->set_integer(stof(get<0>(tup)));
							else if (dest->get_type_information() == type_information_list::_decimal)
								dest->set_decimal(stof(get<0>(tup)));
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_decimal(stof(get<0>(tup)));
							dest->set_type_information(type_information_list::_decimal);
						}
					}
					else if (get<2>(tup) == type_kind::TYPE_INT) {
						if (complex_test) {
							if (dest->get_type_information() == type_information_list::_int)
								dest->set_integer(stoi(get<0>(tup)));
							else if (dest->get_type_information() == type_information_list::_decimal)
								dest->set_decimal(stoi(get<0>(tup)));
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_integer(stoi(get<0>(tup)));
							dest->set_type_information(type_information_list::_int);
						}
					}
					else if (get<2>(tup) == type_kind::TYPE_BOOLEAN) {
						if (complex_test) {
							if (dest->get_type_information() == type_information_list::_boolean)
								dest->set_boolean(get<0>(tup) == "true" ? true : false);
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_boolean(get<0>(tup) == "true" ? true : false);
							dest->set_type_information(type_information_list::_boolean);
						}
					}
					else if (get<2>(tup) == type_kind::TYPE_STRING) {
						if (complex_test) {
							if (dest->get_type_information() == type_information_list::_string)
								dest->set_string(get<0>(tup));
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
						}
						else {
							dest->set_string(get<0>(tup));
							dest->set_type_information(type_information_list::_string);
						}
					}
					else if (get<2>(tup) == type_kind::TYPE_NIL) {
						if (dest->get_type_information() == type_information_list::_int)
							dest->set_integer(0);
						else if (dest->get_type_information() == type_information_list::_decimal)
							dest->set_decimal(0);
						else if (dest->get_type_information() == type_information_list::_string)
							dest->set_string("");
						else if (dest->get_type_information() == type_information_list::_boolean)
							dest->set_boolean(false);
						else if (dest->get_type_information() == type_information_list::_list || dest->get_type_information() == type_information_list::_tuple)
							dest->set_list_tuple(vector<shared_ptr<runtime_variable>>());
						else if (dest->get_type_information() == type_information_list::_dict)
							dest->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
						else if (dest->get_type_information() == type_information_list::bad)
							dest->set_type_information(type_information_list::_nil);
						else
							report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, dest);
					}
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				}
				else {
					shared_ptr<runtime_variable> src = deduce_runtime_variable(s, true);
					if (op == vm_instruction_list::mov) {
						if (get<1>(dtup) == register_identifier_kind::REGISTER_IDENTIFIER_COMPLEX ||
							(dest->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER &&
								dest->get_type_information() != type_information_list::bad))
							dest = strict_mov(dest, src);
						else
							dest = mov(dest, src);
					}
					else if (op == vm_instruction_list::add)
						dest = dest + src;
					else if (op == vm_instruction_list::sub)
						dest = dest - src;
					else if (op == vm_instruction_list::mul)
						dest = dest * src;
					else if (op == vm_instruction_list::div)
						dest = dest / src;
					else if (op == vm_instruction_list::mod)
						dest = dest % src;
					else if (op == vm_instruction_list::exp)
						dest = pow(dest, src);
					else if (op == vm_instruction_list::band)
						dest = dest & src;
					else if (op == vm_instruction_list::bor)
						dest = dest | src;
					else if (op == vm_instruction_list::exor)
						dest = dest ^ src;
					else if (op == vm_instruction_list::land)
						dest = dest && src;
					else if (op == vm_instruction_list::lor)
						dest = dest || src;
					else if (op == vm_instruction_list::equ)
						dest = dest == src;
					else if (op == vm_instruction_list::nequ)
						dest = dest != src;
					else if (op == vm_instruction_list::gt)
						dest = dest > src;
					else if (op == vm_instruction_list::lt)
						dest = dest < src;
					else if (op == vm_instruction_list::lte)
						dest = dest <= src;
					else if (op == vm_instruction_list::gte)
						dest = dest >= src;
					else if (op == vm_instruction_list::cast)
						dest = cast(dest, src);
					else
						report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, dest);
				}
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::malformed_instruction, nullptr);
			dump_runtime_variables(instruction_list);
			cout << '\n';
		}
		shared_ptr<runtime_variable> created_bool = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::_boolean);
		instruction_list.push_back(created_bool);
		runtime_temporary_count++;
		return instruction_list[instruction_list.size() - 1];
	}

	bool runtime::dump_runtime_variables(vector<shared_ptr<runtime_variable>> insn_list) {
		for (int i = 0; i < insn_list.size(); i++) {
			shared_ptr<runtime_variable> rvar = insn_list[i];
			if (rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER)
				cout << rvar->get_storage_field().get_identifier();
			else
				cout << 'r' << rvar->get_storage_field().get_register_number();
			cout << ' ';
			if (rvar->get_type_information() == type_information_list::_int)
				cout << "Int: " << rvar->get_integer();
			else if (rvar->get_type_information() == type_information_list::_list) {
				cout << "List: " << '{' << '\n';
				dump_runtime_variables(rvar->get_list_tuple());
				cout << '}';
			}
			else if (rvar->get_type_information() == type_information_list::_tuple) {
				cout << "Tuple: " << '{' << '\n';
				dump_runtime_variables(rvar->get_list_tuple());
				cout << '}';
			}
			else if (rvar->get_type_information() == type_information_list::_dict) {
				cout << "Dict: " << '{' << '\n' << '{' << '\n';
				dump_runtime_variables(rvar->get_dict().first);
				cout << '}' << '\n' << '{' << '\n';
				dump_runtime_variables(rvar->get_dict().second);
				cout << '}' << '\n' << '}';
			}
			else if (rvar->get_type_information() == type_information_list::_nil)
				cout << "Nil";
			else if (rvar->get_type_information() == type_information_list::_decimal)
				cout << "Decimal: " << rvar->get_decimal();
			else if (rvar->get_type_information() == type_information_list::_string)
				cout << "String: " << rvar->get_string();
			else if (rvar->get_type_information() == type_information_list::_boolean)
				cout << "Boolean: " << rvar->get_boolean();
			else if (rvar->get_type_information() == type_information_list::_func)
				cout << "Function: " << rvar->get_string();
			else if (rvar->get_type_information() == type_information_list::_pure_int)
				cout << "[Int]";
			else if (rvar->get_type_information() == type_information_list::_pure_list)
				cout << "[List]";
			else if (rvar->get_type_information() == type_information_list::_pure_tuple)
				cout << "[Tuple]";
			else if (rvar->get_type_information() == type_information_list::_pure_dict)
				cout << "[Dict]";
			else if (rvar->get_type_information() == type_information_list::_pure_nil)
				cout << "[Nil]";
			else if (rvar->get_type_information() == type_information_list::_pure_decimal)
				cout << "[Decimal]";
			else if (rvar->get_type_information() == type_information_list::_pure_string)
				cout << "[String]";
			else if (rvar->get_type_information() == type_information_list::_pure_boolean)
				cout << "[Boolean]";
			else if (rvar->get_type_information() == type_information_list::_pure_func)
				cout << "[Function]";
			else if (rvar->get_type_information() == type_information_list::bad)
				cout << "Bad";
			cout << '\n';
		}
		cout << "============\n";
		return true;
	}
}