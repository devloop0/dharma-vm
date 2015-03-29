#include "../includes/runtime.hpp"

using namespace dharma_vm;

namespace dharma_vm {

	instruction::instruction(instruction_kind i_kind) {
		insn_kind = i_kind;
	}

	instruction::~instruction() {

	}

	const instruction_kind instruction::get_instruction_kind() {
		return insn_kind;
	}

	runtime_variable::runtime_variable(string ri, string s) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
		t_kind = type_kind::TYPE_STRING;
		str = s;
		integer = -1;
		decimal = -1;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		register_identifier = ri;
	}

	runtime_variable::runtime_variable(string ri, int i) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
		t_kind = type_kind::TYPE_INT;
		str = "";
		integer = i;
		decimal = -1;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		register_identifier = ri;
	}

	runtime_variable::runtime_variable(string ri, float f) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
		t_kind = type_kind::TYPE_DECIMAL;
		str = "";
		integer = -1;
		decimal = f;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		register_identifier = ri;
	}

	runtime_variable::runtime_variable(string ri, type_kind tk, vector<shared_ptr<runtime_variable>> lt) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
		t_kind = tk;
		str = "";
		integer = -1;
		decimal = -1;
		list_tuple = lt;
		dict = map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>>();
		register_identifier = ri;
	}

	runtime_variable::runtime_variable(string ri, map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d) : instruction(instruction_kind::INSTRUCTION_RUNTIME_VARIABLE) {
		t_kind = type_kind::TYPE_DICT;
		str = "";
		integer = -1;
		decimal = -1;
		list_tuple = vector<shared_ptr<runtime_variable>>();
		dict = d;
		register_identifier = ri;
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

	string runtime_variable::get_register_identifier() {
		return register_identifier;
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

	runtime::runtime(vector<string> insn_list) {
		instruction_list = vector<shared_ptr<instruction>>();
		code = insn_list;
	}

	runtime::~runtime() {

	}

	bool runtime::run_program() {

	}
}
