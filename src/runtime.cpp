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

	instruction::instruction(instruction_kind i_kind) {
		insn_kind = i_kind;
	}

	instruction::~instruction() {

	}

	const instruction_kind instruction::get_instruction_kind() {
		return insn_kind;
	}

	runtime_variable::runtime_variable(storage s, string st) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = type_kind::TYPE_STRING;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = st;
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, int i) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = type_kind::TYPE_INT;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = "";
		integer = i;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, float f) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = type_kind::TYPE_DECIMAL;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = "";
		integer = 0;
		decimal = f;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, type_kind tk, vector<shared_ptr<runtime_variable>> lt) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = tk;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = lt;
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = type_kind::TYPE_DICT;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = d;
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, bool b) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = type_kind::TYPE_BOOLEAN;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = b;
	}

	runtime_variable::runtime_variable(storage s) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE), store(s) {
		t_kind = type_kind::TYPE_NIL;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
		str = "";
		integer = 0;
		decimal = 0;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		boolean = false;
	}

	runtime_variable::runtime_variable(storage s, shared_ptr<instruction> src) : instruction(src->get_instruction_kind()), store(s) {
		if(src->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
			shared_ptr<runtime_variable> rvar = static_pointer_cast<runtime_variable>(src);
			t_kind = rvar->t_kind;
			str = rvar->str;
			integer = rvar->integer;
			decimal = rvar->decimal;
			list_tuple = rvar->list_tuple;
			dict = rvar->dict;
			tp_kind = rvar->tp_kind;
		}
		t_kind = type_kind::TYPE_NIL;
		tp_kind = type_pure_kind::TYPE_PURE_NO;
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

	const type_kind runtime_variable::get_type_kind() {
		return t_kind;
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

	type_kind runtime_variable::set_type_kind(type_kind tk) {
		t_kind = tk;
		return t_kind;
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

	const type_pure_kind runtime_variable::get_type_pure_kind() {
		return tp_kind;
	}

	type_pure_kind runtime_variable::set_type_pure_kind(type_pure_kind tpk) {
		tp_kind = tpk;
		return tpk;
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
		cerr << "\033[31mError \033[0m";
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
		cerr << "\033[32m^\033[0m";
		cerr << "\n\n";
		exit(1);
		return false;
	}

	string runtime_utilities::trim(string s) {
		string temp = s;
		temp.erase(temp.begin(), std::find_if(temp.begin(), temp.end(), [](char c) -> bool { return !std::isspace(c); }));
		temp.erase(std::find_if(temp.rbegin(), temp.rend(), [](char c) -> bool { return !std::isspace(c); }).base(), temp.end());
		return temp;
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

	int runtime::find_instruction(int i) {
		for(int i = 0; i < instruction_list.size(); i++) {
			shared_ptr<instruction> insn = instruction_list[i];
			if(insn->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
				shared_ptr<runtime_variable> rv = static_pointer_cast<runtime_variable>(insn);
				storage store = rv->get_storage();
				if(store.get_storage_type_kind() == storage_type_kind::STORAGE_TYPE_REGISTER)
					if(store.get_register_number() == i)
						return -1;
			}
		}
		return -1;
	}

	bool runtime::run_program() {
		vector<vector<string>> vec_insn_list;
		for(int i = 0; i < code.size(); i++) {
			string temp = runtime_utilities().trim(code[i]);
			vec_insn_list.push_back(runtime_utilities().split_instruction_by_spaces(temp));
		}
		for(int i = 0; i < vec_insn_list.size(); i++) {
			vector<string> now = vec_insn_list[i];
			if(now.size() <= 0)
				runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::expected_instruction, now, i, -1);
			if(now[0] == vm_instruction_list::mov) {
				string operation = now[0];
				if(now.size() != 3)
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::incorrect_number_of_operands, now, i, 0);
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
						runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::identifier_not_previously_declared, now, i, 2);
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
							instruction_list.push_back(static_pointer_cast<instruction>(make_shared<runtime_variable>(storage(dest), src_insn)));
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
							instruction_list.push_back(static_pointer_cast<instruction>(make_shared<runtime_variable>(storage(stoi(dest)), src_insn)));
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
						if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_kind() == type_kind::TYPE_STRING) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_STRING);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_string(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_string());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_kind() == type_kind::TYPE_INT) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_INT);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_integer());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_kind() == type_kind::TYPE_DECIMAL) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_DECIMAL);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_decimal());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_kind() == type_kind::TYPE_BOOLEAN) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_BOOLEAN);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_boolean());
						}
						else if(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_type_kind() == type_kind::TYPE_NIL) {
							if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_kind() == type_kind::TYPE_STRING)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_string(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_string());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_kind() == type_kind::TYPE_DECIMAL)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_decimal());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_kind() == type_kind::TYPE_INT)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_integer());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_kind() == type_kind::TYPE_BOOLEAN)
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(static_pointer_cast<runtime_variable>(instruction_list[src_insn])->get_boolean());
							else if(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_type_kind() == type_kind::TYPE_NIL);
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
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_BOOLEAN);
							}
							else {
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_boolean(false);
								static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_BOOLEAN);
							}
						}
						else if(lk == literal_kind::LITERAL_STRING) {
							s.erase(0, 1);
							s.pop_back();
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_string(s);
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_STRING);
						}
						else if(lk == literal_kind::LITERAL_NIL)
							instruction_list[dest_insn] = make_shared<runtime_variable>(static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->get_storage());
						else if(lk == literal_kind::LITERAL_REGULAR_HEX_NUMBER) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_integer(stoi(s));
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_INT);
						}
						else if(lk == literal_kind::LITERAL_DECIMAL_NUMBER) {
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_decimal(stof(s));
							static_pointer_cast<runtime_variable>(instruction_list[dest_insn])->set_type_kind(type_kind::TYPE_DECIMAL);
						}
					}
				}
				else
					runtime_utilities().report_error_and_terminate_program(runtime_diagnostic_messages::malformed_operands, now, i, 2);
			}
		}
		//For testing purposes, in order to make sure register values are correct
		for(int i = 0; i < instruction_list.size(); i++) {
			shared_ptr<instruction> insn = instruction_list[i];
			if(insn->get_instruction_kind() == instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
				shared_ptr<runtime_variable> rvar = static_pointer_cast<runtime_variable>(insn);
				if(rvar->get_storage().get_storage_type_kind() == storage_type_kind::STORAGE_TYPE_REGISTER)
					cout << "r" << rvar->get_storage().get_register_number() << ' ';
				else
					cout << rvar->get_storage().get_identifier() << ' ';
				type_kind tk = rvar->get_type_kind();
				if(tk == type_kind::TYPE_BOOLEAN)
					cout << "Boolean: " << rvar->get_boolean() << '\n';
				else if(tk == type_kind::TYPE_STRING)
					cout << "String: " << rvar->get_string() << '\n';
				else if(tk == type_kind::TYPE_INT)
					cout << "Integer: " << rvar->get_integer() << '\n';
				else if(tk == type_kind::TYPE_DECIMAL)
					cout << "Decimal: " << rvar->get_decimal() << '\n';
			}
		}
		return true;
	}
}
