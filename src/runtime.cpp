#include "../includes/runtime.hpp"

using namespace dharma_vm;

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
	const string vm_instruction_list::nequ = "nequ";
	const string vm_instruction_list::dict = "dict";

	const string runtime_diagnostic_messages::expected_instruction = "Expected an instruction.";
	const string runtime_diagnostic_messages::incorrect_number_of_operands = "Incorrect number of operands.";
	const string runtime_diagnostic_messages::malformed_operands = "Malformed operands for this operation.";
	const string runtime_diagnostic_messages::identifier_not_previously_declared = "Identifier not previously declared.";
	const string runtime_diagnostic_messages::literal_cannot_be_a_destination_operand = "Expected a storable operand.";
	const string runtime_diagnostic_messages::expected_integer_or_floating_point_operands_for_this_instruction = "Expected integer or floating point operands for this instruction.";
	const string runtime_diagnostic_messages::only_integers_are_acceptable_here = "Only integers are acceptable for this instruction.";
	const string runtime_diagnostic_messages::register_not_previously_declared = "Register not previously declared.";
	const string runtime_diagnostic_messages::only_booleans_are_acceptable_here = "Only booleans are acceptable for this instruction.";

	instruction::instruction(instruction_kind i_kind) {
		insn_kind = i_kind;
	}

	instruction::~instruction() {

	}

	const instruction_kind instruction::get_instruction_kind() {
		return insn_kind;
	}

	runtime_variable::runtime_variable(shared_ptr<runtime_variable> rvar) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), t_inf(rvar->t_inf), store(rvar->store) {
		str = rvar->str;
		integer = rvar->integer;
		decimal = rvar->decimal;
		list_tuple = rvar->list_tuple;
		dict = rvar->dict;
		boolean = rvar->boolean;
	}

	runtime_variable::runtime_variable(runtime_variable& rvar) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), t_inf(rvar.t_inf), store(rvar.store) {
		str = rvar.str;
		integer = rvar.integer;
		decimal = rvar.decimal;
		list_tuple = rvar.list_tuple;
		dict = rvar.dict;
		boolean = rvar.boolean;
	}

	runtime_variable::runtime_variable(storage s, string st) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = st;
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, int i) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = "";
		integer = i;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, float f) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = "";
		integer = 0;
		decimal = f;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, type_kind tk, vector<shared_ptr<runtime_variable>> lt) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(tk, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = lt;
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(type_kind::TYPE_DICT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = d;
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, bool b) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = b;
	}

	runtime_variable::runtime_variable(storage s) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s), t_inf(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "") {
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, shared_ptr<instruction> src) : instruction(src->get_instruction_kind()), store(s), t_inf(type_information(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "")) {
		if(src->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
			shared_ptr<runtime_variable> rvar = static_pointer_cast<runtime_variable>(src);
			str = rvar->str;
			integer = rvar->integer;
			decimal = rvar->decimal;
			list_tuple = rvar->list_tuple;
			dict = rvar->dict;
			t_inf = rvar->t_inf;
		}
		t_inf = type_information(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::~runtime_variable() {

	}

	string runtime_variable::get_string() {
		return str;
	}

	int runtime_variable::get_integer() {
		return integer;
	}

	float runtime_variable::get_decimal() {
		return decimal;
	}

	bool runtime_variable::get_boolean() {
		return boolean;
	}

	type_information runtime_variable::get_type_information() {
		return t_inf;
	}

	vector<shared_ptr<runtime_variable>> runtime_variable::get_list_tuple() {
		return list_tuple;
	}

	map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> runtime_variable::get_dict() {
		return dict;
	}

	string runtime_variable::set_string(string s) {
		str = s;
		return str;
	}

	int runtime_variable::set_integer(int i) {
		integer = i;
		return integer;
	}

	float runtime_variable::set_decimal(float f) {
		decimal = f;
		return decimal;
	}

	bool runtime_variable::set_boolean(bool b) {
		boolean = b;
		return b;
	}

	type_information runtime_variable::set_type_information(type_information t) {
		t_inf = t;
		return t;
	}

	vector<shared_ptr<runtime_variable>> runtime_variable::set_list_tuple(vector<shared_ptr<runtime_variable>> lt) {
		list_tuple = lt;
		return list_tuple;
	}

	map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> runtime_variable::set_dict(map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d) {
		dict = d;
		return dict;
	}

	storage runtime_variable::get_storage() {
		return store;
	}

	storage::storage(string ident) {
		st_kind = storage_type_kind::STORAGE_TYPE_IDENTIFIER;
		identifier = ident;
		register_number = -1;
	}

	storage::storage(int reg_num) {
		st_kind = storage_type_kind::STORAGE_TYPE_REGISTER;
		identifier = "";
		register_number = reg_num;
	}

	storage::~storage() {

	}

	string storage::get_identifier() {
		return identifier;
	}

	int storage::get_register_number() {
		return register_number;
	}

	const storage_type_kind storage::get_storage_type_kind() {
		return st_kind;
	}

	label::label(int l, int i) : instruction(instruction_kind::INSTRUCTION_LABEL) {
		lab = l;
		index = i;
	}

	label::~label() {

	}

	int label::get_label_number() {
		return lab;
	}

	int label::get_instruction_list_index() {
		return index;
	}

	runtime_utilities::runtime_utilities() {

	}

	runtime_utilities::~runtime_utilities() {

	}

	vector<string> runtime_utilities::split_instruction_by_spaces(string instruction) {
		vector<string> ret;
		regex rgx("\\s+");
		sregex_token_iterator iter(instruction.begin(), instruction.end(), rgx, -1);
		sregex_token_iterator end;
		while(iter != end) {
			ret.push_back(*iter);
			++iter;
		}
		return ret;
	}

	bool runtime_utilities::report_error_and_terminate_program(string msg, vector<string> instruction, int line, int index) {
#if WIN32
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		bool colored = true;
		if (handle == INVALID_HANDLE_VALUE) {
			cerr << "Could not output colored diagnostics. Defaulting to regular, non-colored output. (Perhaps you do not have the file kernel32.dll)?\n\n";
			colored = false;
		}
		if (colored) {
			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
			GetConsoleScreenBufferInfo(handle, &console_screen_buffer_info);
			SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cerr << "Error ";
			SetConsoleTextAttribute(handle, console_screen_buffer_info.wAttributes);
		}
		else
			cerr << "Error ";
#else
		cerr << "\033[31mError \033[0m";
#endif
		string print;
		int caret = 0;
		for(int i = 0; i < instruction.size(); i++) {
			print += instruction[i] + " ";
			if(i < index) caret += instruction[i].length() + 1;
		}
		print = runtime_utilities().trim(print);
		int column = caret;
		cerr << '[' << line << ':' << column << "]: " << msg << "\n";
		cerr << "Region given here for reference:\n\t";
		cerr << print << "\n\t";
		for(int i = 0; i < caret; i++)
			cerr << ' ';
#if WIN32
		HANDLE handle2 = GetStdHandle(STD_OUTPUT_HANDLE);
		bool colored2 = true;
		if (handle2 == INVALID_HANDLE_VALUE) {
			cerr << "Could not output colored diagnostics. Defaulting to regular, non-colored output. (Perhaps you do not have the file kernel32.dll?)\n\n";
			colored2 = false;
		}
		if (colored2) {
			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info2;
			GetConsoleScreenBufferInfo(handle2, &console_screen_buffer_info2);
			SetConsoleTextAttribute(handle2, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cerr << "^";
			SetConsoleTextAttribute(handle2, console_screen_buffer_info2.wAttributes);
		}
		else
			cerr << "^";
#else
		cerr << "\033[32m^\033[0m";
#endif
		cerr << "\n\n";
		exit(1);
		return false;
	}

	string runtime_utilities::trim(string s) {
		string temp = s;
		temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), [](char c) -> bool { return !isspace(c); }));
		temp.erase(std::find_if(temp.rbegin(), temp.rend(), [](char c) -> bool { return !isspace(c); }).base(), temp.end());
		return temp;
	}

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

	const bool type_information::operator==(type_information t_inf) {
		return t_inf.t_kind == t_kind && t_inf.tp_kind == tp_kind && t_inf.tc_kind == tc_kind && t_inf.class_name == class_name;
	}

	const bool type_information::operator!=(type_information t_inf) {
		return !(*this == t_inf);
	}

	runtime::runtime(vector<string> insn_list) {
		instruction_list = vector<shared_ptr<instruction>>();
		code = insn_list;
	}

	runtime::~runtime() {

	}

	pair<literal_kind, string> runtime::recognize_identifier(string str) {
		if(str[0] == '$') {
			string temp = str;
			temp.erase(0,1);
			if(temp == "true" || temp == "false")
				return make_pair(literal_kind::LITERAL_BOOLEAN, temp);
			else if(temp == "nil")
				return make_pair(literal_kind::LITERAL_NIL, temp);
			else if(temp[0] == '\"' || temp[0] == '\'')
				return make_pair(literal_kind::LITERAL_STRING, temp);
			else if((int) temp.find('.') >= 0)
				return make_pair(literal_kind::LITERAL_DECIMAL_NUMBER, temp);
			else
				return make_pair(literal_kind::LITERAL_REGULAR_HEX_NUMBER, temp);
		}
		else
			return make_pair(literal_kind::LITERAL_IDENTIFIER, str);
		return make_pair(literal_kind::LITERAL_NONE, str);
	}

	int runtime::find_instruction(string str) {
		for(int i = 0; i < instruction_list.size(); i++) {
			shared_ptr<instruction> insn = instruction_list[i];
			if(insn->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
				shared_ptr<runtime_variable> rv = static_pointer_cast<runtime_variable>(insn);
				storage store = rv->get_storage();
				if(store.get_storage_type_kind() == storage_type_kind::STORAGE_TYPE_IDENTIFIER)
					if(store.get_identifier() == str)
						return i;
			}
		}
		return -1;
	}

	int runtime::find_instruction(int r) {
		for(int i = 0; i < instruction_list.size(); i++) {
			shared_ptr<instruction> insn = instruction_list[i];
			if(insn->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
				shared_ptr<runtime_variable> rv = static_pointer_cast<runtime_variable>(insn);
				storage store = rv->get_storage();
				if(store.get_storage_type_kind() == storage_type_kind::STORAGE_TYPE_REGISTER)
					if(store.get_register_number() == r)
						return i;
			}
		}
		return -1;
	}

	bool runtime::run_program() {
		type_information _int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
		type_information _decimal(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
		type_information _string(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
		type_information _nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
		type_information _boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_NO, "");
		type_information _pure_int(type_kind::TYPE_INT, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
		type_information _pure_decimal(type_kind::TYPE_DECIMAL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
		type_information _pure_string(type_kind::TYPE_STRING, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
		type_information _pure_nil(type_kind::TYPE_NIL, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
		type_information _pure_boolean(type_kind::TYPE_BOOLEAN, type_pure_kind::TYPE_PURE_YES, type_class_kind::TYPE_CLASS_NO, "");
		vector<vector<string>> vec_insn_list;
		for(int i = 0; i < code.size(); i++) {
			string temp = runtime_utilities().trim(code[i]);
			vec_insn_list.push_back(runtime_utilities().split_instruction_by_spaces(temp));
		}
		for(int i = 0; i < vec_insn_list.size(); i++) {
			vector<string> now = vec_insn_list[i];
			if(now.size() <= 0)
				runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::expected_instruction, now, i, -1);
			if(now.size() == 3) {
				string operation = now[0];
				string dest = now[1];
				string src = now[2];
				bool src_ident = false;
				bool dest_ident = false;
				bool src_register = false;
				bool dest_register = false;
				if(dest[0] == '[') {
					dest.erase(0,1);
					dest.pop_back();
					dest_ident = true;
				}
				else if(dest[0] == 'r') {
					dest.erase(0,1);
					dest_register = true;
				}
				else
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
				if(src[0] == '[') {
					src.erase(0,1);
					src.pop_back();
					src_ident = true;
				}
				else if(src[0] == 'r') {
					src.erase(0,1);
					src_register = true;
				}
				else
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
				int src_insn = -1;
				pair<literal_kind, string> src_pair;
				int dest_insn = -1;
				if(src_ident) {
					pair<literal_kind, string> pai = recognize_identifier(src);
					if(pai.first == literal_kind::LITERAL_NONE)
						runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
					if(pai.first == literal_kind::LITERAL_IDENTIFIER) {
						int res = find_instruction(src);
						if(res == -1)
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::identifier_not_previously_declared, now, i, 2);
						src_insn = res;
					}
					else
						src_pair = pai;
				}
				else if(src_register) {
					int res = find_instruction(stoi(src));
					if(res == -1)
						runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::register_not_previously_declared, now, i, 2);
					src_insn = res;
				}
				else
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
				if(dest_ident) {
					pair<literal_kind, string> pai = recognize_identifier(dest);
					int res = find_instruction(dest);
					if(pai.first != literal_kind::LITERAL_IDENTIFIER)
						runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
					if(res == -1) {
						if(src_ident) {
							literal_kind lk = src_pair.first;
							string s = src_pair.second;
							shared_ptr<runtime_variable> rvar = nullptr;
							if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER)
								rvar = make_shared<runtime_variable>(storage(dest), stoi(s));
							else if(lk == literal_kind::LITERAL_DECIMAL_NUMBER)
								rvar = make_shared<runtime_variable>(storage(dest), stof(s));
							else if(lk == literal_kind::LITERAL_NONE)
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
							else
								rvar = make_shared<runtime_variable>(storage(dest), s);
							instruction_list.push_back(static_pointer_cast<instruction>(rvar));
							dest_insn = instruction_list.size() - 1;
						}
						else if(src_register) {
							instruction_list.push_back(static_pointer_cast<instruction>(make_shared<runtime_variable>(storage(dest), instruction_list[src_insn])));
							dest_insn = instruction_list.size() - 1;
						}
					}
					else
						dest_insn = res;
				}
				else if(dest_register) {
					int res = find_instruction(stoi(dest));
					if(res == -1) {
						if(src_ident) {
							literal_kind lk = src_pair.first;
							string s = src_pair.second;
							shared_ptr<runtime_variable> rvar = nullptr;
							if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER)
								rvar = make_shared<runtime_variable>(storage(stoi(dest)), stoi(s));
							else if(lk == literal_kind::LITERAL_DECIMAL_NUMBER)
								rvar = make_shared<runtime_variable>(storage(stoi(dest)), stof(s));
							else if(lk == literal_kind::LITERAL_NONE)
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
							else
								rvar = make_shared<runtime_variable>(storage(stoi(dest)), s);
							instruction_list.push_back(static_pointer_cast<instruction>(rvar));
							dest_insn = instruction_list.size() - 1;
						}
						else if(src_register) {
							instruction_list.push_back(static_pointer_cast<instruction>(make_shared<runtime_variable>(storage(stoi(dest)), instruction_list[src_insn])));
							dest_insn = instruction_list.size() - 1;
						}
					}
					else
						dest_insn = res;
				}
				else
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
				if(operation == vm_instruction_list::mov) {
					if(src_insn != -1) {
						if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_information() == _string) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_string);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_string(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_string());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_information() == _int) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_int);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_integer());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_information() == _decimal) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_decimal);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_decimal());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_information() == _boolean) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_boolean());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_information() == _nil) {
							if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_information() == _string)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_string(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_string());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_information() == _decimal)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_decimal());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_information() == _int)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_integer());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_information() == _boolean)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_boolean());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_information() == _nil);
							else
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
						}
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
					}
					else {
						literal_kind lk = src_pair.first;
						string s = src_pair.second;
						if(lk == literal_kind::LITERAL_NONE || lk == literal_kind::LITERAL_IDENTIFIER)
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 0);
						else if(lk == literal_kind::LITERAL_BOOLEAN) {
							if(s == "true") {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(true);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
						}
						else if(lk == literal_kind::LITERAL_STRING) {
							s.erase(0, 1);
							s.pop_back();
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_string(s);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_string);
						}
						else if(lk == literal_kind::LITERAL_NIL)
							instruction_list[dest_insn] = make_shared<runtime_variable>(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_storage());
						else if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(stoi(s));
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_int);
						}
						else if(lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(stof(s));
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_decimal);
						}
					}
				}
				else if(operation == vm_instruction_list::add || operation == vm_instruction_list::sub ||
						operation == vm_instruction_list::mul || operation == vm_instruction_list::div ||
						operation == vm_instruction_list::exp) {
					string op = operation;
					if(src_insn != -1) {
						shared_ptr<runtime_variable> temp = static_pointer_cast<runtime_variable>(instruction_list[src_insn]);
						if(temp->get_type_information() == _int || temp->get_type_information() == _decimal);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::expected_integer_or_floating_point_operands_for_this_instruction, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _int || dtemp->get_type_information() == _decimal);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::expected_integer_or_floating_point_operands_for_this_instruction, now, i, 1);
						if(dtemp->get_type_information() == _int) {
							int temp1 = dtemp->get_integer();
							if(temp->get_type_information() == _decimal) {
								float temp2 = temp->get_decimal();
								float res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_decimal);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(res);
							}
							else {
								int temp2 = temp->get_integer();
								int res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(res);
							}
						}
						else {
							float temp1 = dtemp->get_decimal();
							if(temp->get_type_information() == _decimal) {
								float temp2 = temp->get_decimal();
								float res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(res);
							}
							else {
								int temp2 = temp->get_integer();
								float res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(res);
							}
						}
					}
					else {
						literal_kind lk = src_pair.first;
						string s = src_pair.second;
						if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER || lk == literal_kind::LITERAL_DECIMAL_NUMBER);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::expected_integer_or_floating_point_operands_for_this_instruction, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _int || dtemp->get_type_information() == _decimal);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::expected_integer_or_floating_point_operands_for_this_instruction, now, i, 1);
						if(dtemp->get_type_information() == _int) {
							int temp1 = dtemp->get_integer();
							if(lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
								float temp2 = stof(s);
								float res = temp1;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_decimal);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(res);
							}
							else {
								int temp2 = stoi(s);
								int res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(res);
							}
						}
						else {
							float temp1 = dtemp->get_decimal();
							if(lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
								float temp2 = stof(s);
								float res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(res);
							}
							else {
								int temp2 = stoi(s);
								float res;
								if(op == vm_instruction_list::add)
									res = temp1 + temp2;
								else if(op == vm_instruction_list::sub)
									res = temp1 - temp2;
								else if(op == vm_instruction_list::mul)
									res = temp1 * temp2;
								else if(op == vm_instruction_list::div)
									res = temp1 / temp2;
								else
									res = std::pow(temp1, temp2);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(res);
							}
						}
					}
				}
				else if(operation == vm_instruction_list::lte || operation == vm_instruction_list::lt ||
					operation == vm_instruction_list::gte || operation == vm_instruction_list::gt) {
					string op = operation;
					if(src_insn != -1) {
						shared_ptr<runtime_variable> temp = static_pointer_cast<runtime_variable>(instruction_list[src_insn]);
						if(temp->get_type_information() == _decimal || temp->get_type_information() == _int);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _int || dtemp->get_type_information() == _decimal);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
						if(dtemp->get_type_information() == _int && temp->get_type_information() == _decimal) {
							int one = dtemp->get_integer();
							float two = temp->get_decimal();
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _decimal && temp->get_type_information() == _int) {
							float one = dtemp->get_decimal();
							int two = temp->get_integer();
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _int && temp->get_type_information() == _decimal) {
							int one = dtemp->get_integer();
							int two = temp->get_integer();
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _decimal && temp->get_type_information() == _decimal) {
							float one = dtemp->get_decimal();
							float two = temp->get_decimal();
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _int && temp->get_type_information() == _int) {
							int one = dtemp->get_integer();
							int two = temp->get_integer();
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 0);
					}	
					else {
						string s = src_pair.second;
						literal_kind lk = src_pair.first;
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _int || dtemp->get_type_information() == _decimal);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
						if(lk == literal_kind::LITERAL_DECIMAL_NUMBER || lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
						if(dtemp->get_type_information() == _int && lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
							int one = dtemp->get_integer();
							float two = stof(s);
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _decimal && lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
							float one = dtemp->get_decimal();
							int two = stoi(s);
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _decimal && lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
							float one = dtemp->get_decimal();
							float two = stof(s);
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(dtemp->get_type_information() == _int && lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
							int one = dtemp->get_integer();
							int two = stoi(s);
							bool res;
							if(op == vm_instruction_list::gte)
								res = one >= two;
							else if(op == vm_instruction_list::gt)
								res = one > two;
							else if(op == vm_instruction_list::lte)
								res = one <= two;
							else
								res = one < two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 0);
					}
				}
				else if(operation == vm_instruction_list::mod || operation == vm_instruction_list::shr || operation == vm_instruction_list::shl ||
						operation == vm_instruction_list::band || operation == vm_instruction_list::bor || operation == vm_instruction_list::exor) {
					string op = operation;
					if(src_insn != -1) {
						shared_ptr<runtime_variable> temp = static_pointer_cast<runtime_variable>(instruction_list[src_insn]);
						if(temp->get_type_information() == _int);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_integers_are_acceptable_here, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _int);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_integers_are_acceptable_here, now, i, 1);
						int one = dtemp->get_integer();
						int two = temp->get_integer();
						int res;
						if(op == vm_instruction_list::mod)
							res = one % two;
						else if(op == vm_instruction_list::shr)
							res = one >> two;
						else if(op == vm_instruction_list::shl)
							res = one << two;
						else if(op == vm_instruction_list::band)
							res = one & two;
						else if(op == vm_instruction_list::bor)
							res = one | two;
						else
							res = one ^ two;
						static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_int);
					}
					else {
						literal_kind lk = src_pair.first;
						string s = src_pair.second;
						if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_integers_are_acceptable_here, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _int);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_integers_are_acceptable_here, now, i, 1);
						int one = dtemp->get_integer();
						int two = stoi(s);
						int res;
						if(op == vm_instruction_list::mod)
							res = one % two;
						else if(op == vm_instruction_list::shr)
							res = one >> two;
						else if(op == vm_instruction_list::shl)
							res = one << two;
						else if(op == vm_instruction_list::band)
							res = one & two;
						else if(op == vm_instruction_list::bor)
							res = one | two;
						else
							res = one ^ two;
						static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_int);
					}
				}
				else if(operation == vm_instruction_list::lor || operation == vm_instruction_list::land) {
					string op = operation;
					if(src_insn != -1) {
						shared_ptr<runtime_variable> temp = static_pointer_cast<runtime_variable>(instruction_list[src_insn]);
						if(temp->get_type_information() == _boolean);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_booleans_are_acceptable_here, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _boolean);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_booleans_are_acceptable_here, now, i, 1);
						bool one = dtemp->get_boolean();
						bool two = temp->get_boolean();
						bool res;
						if(op == vm_instruction_list::lor)
							res = one || two;
						else
							res = one && two;
						static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
					}
					else {
						string s = src_pair.second;
						literal_kind lk = src_pair.first;
						if(lk == literal_kind::LITERAL_BOOLEAN);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_booleans_are_acceptable_here, now, i, 2);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(dtemp->get_type_information() == _boolean);
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::only_booleans_are_acceptable_here, now, i, 2);
						bool one = dtemp->get_boolean();
						bool two = s == "true" ? true : false;
						bool res;
						if(op == vm_instruction_list::lor)
							res = one || two;
						else
							res = one && two;
						static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
					}
				}
				else if(operation == vm_instruction_list::equ || operation == vm_instruction_list::nequ) {
					string op = operation;
					if(src_insn != -1) {
						shared_ptr<runtime_variable> temp = static_pointer_cast<runtime_variable>(instruction_list[src_insn]);
						type_information t_inf2 = temp->get_type_information();
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						type_information t_inf1 = dtemp->get_type_information();
						if(t_inf1 == t_inf2) {
							if(t_inf1 == _int) {
								int one = dtemp->get_integer();
								int two = temp->get_integer();
								bool res = (op == vm_instruction_list::equ ? one == two : one != two);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _decimal) {
								float one = dtemp->get_decimal();
								float two = temp->get_decimal();
								bool res = (op == vm_instruction_list::equ ? one == two : one != two);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _nil) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? true : false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _boolean) {
								bool one = dtemp->get_boolean();
								bool two = temp->get_boolean();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _string) {
								string one = dtemp->get_string();
								string two = temp->get_string();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(true);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}		
							else
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 0);
						}
						else if(t_inf1 == _nil) {
							if(t_inf2 == _int) {
								int one = 0;
								int two = temp->get_integer();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf2 == _decimal) {
								int one = 0.0;
								int two = temp->get_decimal();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf2 == _nil) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? true : false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf2 == _boolean) {
								bool one = false;
								bool two = temp->get_boolean();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf2 == _string) {
								string one = "";
								string two = temp->get_string();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
						}
						else if(t_inf2 == _nil) {
							if(t_inf1 == _int) {
								int one = 0;
								int two = dtemp->get_integer();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _decimal) {
								int one = 0.0;
								int two = dtemp->get_decimal();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _nil) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? true : false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _boolean) {
								bool one = false;
								bool two = dtemp->get_boolean();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf1 == _string) {
								string one = "";
								string two = dtemp->get_string();
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
						}
						else if((t_inf1 == _int && t_inf2 == _decimal) || (t_inf1 == _decimal && t_inf2 == _int)) {
							float one;
							float two;
							bool res;
							if(t_inf1 == _int)
								one = dtemp->get_integer();
							else
								one = dtemp->get_decimal();
							if(t_inf2 == _int)
								two = temp->get_integer();
							else
								two = temp->get_decimal();
							res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf1.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES && t_inf2.get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? false : true);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 0);
					}
					else {
						string s = src_pair.second;
						literal_kind lk = src_pair.first;
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						type_information t_inf = dtemp->get_type_information();
						if(t_inf == _int && lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
							int one = dtemp->get_integer();
							int two = stoi(s);
							bool res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf == _decimal && literal_kind::LITERAL_DECIMAL_NUMBER) {
							float one = dtemp->get_decimal();
							float two = stof(s);
							bool res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf == _int && lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
							float one = dtemp->get_integer();
							float two = stof(s);
							bool res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf == _decimal && lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
							float one = dtemp->get_decimal();
							float two = stoi(s);
							bool res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf == _boolean && lk == literal_kind::LITERAL_BOOLEAN) {
							bool one = dtemp->get_boolean();
							bool two = s == "true" ? true : false;
							bool res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf == _string && lk == literal_kind::LITERAL_STRING) {
							string one = dtemp->get_string();
							string two = s;
							bool res = op == vm_instruction_list::equ ? one == two : one != two;
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
						}
						else if(t_inf == _nil) {
							if(lk == literal_kind::LITERAL_NIL) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? true : false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(lk == literal_kind::LITERAL_BOOLEAN) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? dtemp->get_boolean() == false : dtemp->get_boolean() != false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(lk == literal_kind::LITERAL_STRING) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? dtemp->get_string() == "" : dtemp->get_string() != "");
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? dtemp->get_decimal() == 0.0 : dtemp->get_decimal() != 0.0);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? dtemp->get_integer() == 0 : dtemp->get_integer() != 0);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
						}
						else if(lk == literal_kind::LITERAL_NIL) {
							if(t_inf == _nil) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(op == vm_instruction_list::equ ? true : false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf == _boolean) {
								bool one = dtemp->get_boolean();
								bool two = false;
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf == _string) {
								string one = dtemp->get_string();
								string two = "";
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf == _int) {
								int one = dtemp->get_integer();
								int two = 0;
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else if(t_inf == _decimal) {
								float one = dtemp->get_decimal();
								float two = 0.0;
								bool res = op == vm_instruction_list::equ ? one == two : one != two;
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(res);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_boolean);
							}
							else
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 1);
						}
					}
				}
				else if(operation == vm_instruction_list::cast) {
					string op = operation;
					if(src_insn != -1) {
						shared_ptr<runtime_variable> temp = static_pointer_cast<runtime_variable>(instruction_list[src_insn]);
						shared_ptr<runtime_variable> dtemp = static_pointer_cast<runtime_variable>(instruction_list[dest_insn]);
						if(temp->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES) {
							if(dtemp->get_type_information() == _int && temp->get_type_information() == _pure_int);
							else if(dtemp->get_type_information() == _decimal && temp->get_type_information() == _pure_decimal);
							else if(dtemp->get_type_information() == _int && temp->get_type_information() == _pure_decimal) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(dtemp->get_integer());
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_decimal);
							}
							else if(dtemp->get_type_information() == _decimal && temp->get_type_information() == _int) {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer((int) dtemp->get_decimal());
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_information(_int);
							}
							else if(dtemp->get_type_information() == _string && temp->get_type_information() == _pure_string);
							else if(dtemp->get_type_information() == _boolean && temp->get_type_information() == _pure_boolean);
							else 
								runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 0);
						}
						else
							runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
					}
					else
						runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
				}
				else
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
			}
			else
				runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::incorrect_number_of_operands, now, i, 0);
			//For testing purposes, in order to make sure register values are correct
			for(int i = 0; i < instruction_list.size(); i++) {
				shared_ptr<instruction> insn = instruction_list[i];
				if(insn->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
					shared_ptr<runtime_variable> rvar = static_pointer_cast<runtime_variable>(insn);
					if(rvar->get_storage().get_storage_type_kind() == storage_type_kind::STORAGE_TYPE_REGISTER)
						cout << "r" << rvar->get_storage().get_register_number() << ' ';
					else
						cout << rvar->get_storage().get_identifier() << ' ';
					type_information ti = rvar->get_type_information();
					if(ti == _boolean)
						cout << "Boolean: " << rvar->get_boolean() << '\n';
					else if(ti == _string)
						cout << "String: " << rvar->get_string() << '\n';
					else if(ti == _int)
						cout << "Integer: " << rvar->get_integer() << '\n';
					else if(ti == _decimal)
						cout << "Decimal: " << rvar->get_decimal() << '\n';
					else if(ti == _nil)
						cout << "Nil\n";
				}
			}
			cout << "====================\n";
		}
		return true;
	}
}
