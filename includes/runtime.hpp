#ifndef DHARMA_VM_RUNTIME_HPP
#define DHARMA_VM_RUNTIME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::map;

namespace dharma_vm {

	enum type_kind {
		TYPE_INT, TYPE_LIST, TYPE_DICT, TYPE_TUPLE, TYPE_STRING, TYPE_NUMBER, TYPE_DECIMAL, TYPE_NONE
	};

	enum instruction_kind {
		INSTRUCTION_RUNTIME_VARIABLE, INSTRUCTION_LABEL
	};

	enum operation_kind {
		OPERATION_MOV, OPERATION_INC, OPERATION_DEC, OPERATION_CMPL, OPERATION_NEG, OPERATION_POS, OPERATION_BNEG, OPERATION_TYOF, OPERATION_ADD, OPERATION_SUB,
		OPERATION_MUL, OPERATION_DIV, OPERATION_MOD, OPERATION_EXP, OPERATION_SHL, OPERATION_SHR, OPERATION_CEQU, OPERATION_SNEQU, OPERATION_CNEQU, OPERATION_GT,
		OPERATION_LT, OPERATION_GTE, OPERATION_LTE, OPERATION_BOR, OPERATION_BAND, OPERATION_EXOR, OPERATION_LOR, OPERATION_LAND, OPERATION_CAST, OPERATION_EXIT,
		OPERATION_JMP, OPERATION_LIST, OPERATION_TUPL, OPERATION_DICT, OPERATION_SEQU, OPERATION_SUBSL, OPERATION_SUBSLL, OPERATION_SUBSD
	};

	class instruction {
		instruction_kind insn_kind;
		public:
			instruction(instruction_kind i_kind);
			~instruction();
			const instruction_kind get_instruction_kind();
	};

	class runtime_variable : public instruction {
		type_kind t_kind;
		string str;
		int integer;
		float decimal;
		vector<shared_ptr<runtime_variable>> list_tuple;
		map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> dict;
		string register_identifier;
		public:
			runtime_variable(string ri, string s);
			runtime_variable(string ri, int i);
			runtime_variable(string ri, float f);
			runtime_variable(string ri, type_kind tk, vector<shared_ptr<runtime_variable>> lt);
			runtime_variable(string ri, map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d);
			~runtime_variable();
			string get_string();
			int get_integer();
			float get_decimal();
			const type_kind get_type_kind();
			vector<shared_ptr<runtime_variable>> get_list_tuple();
			map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> get_dict();
			int set_integer(int i);
			float set_decimal(float f);
			string set_string(string s);
			vector<shared_ptr<runtime_variable>> set_list_tuple(vector<shared_ptr<runtime_variable>> lt);
			map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> set_dict(map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d);
			type_kind set_type_kind(type_kind tk);
			string get_register_identifier();
	};

	class label : public instruction {
		int lab;
		int index;
		public:
			label(int l, int i);
			~label();
			int get_label_number();
			int get_instruction_list_index();
	};

	class runtime {
		vector<shared_ptr<instruction>> instruction_list;
		vector<string> code;
		public:
			runtime(vector<string> insn_list);
			~runtime();
			bool run_program();
	};
}

#endif
