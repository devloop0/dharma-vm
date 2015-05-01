#ifndef DHARMA_VM_RUNTIME_HPP
#define DHARMA_VM_RUNTIME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <regex>

using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::regex;
using std::sregex_token_iterator;
using std::cerr;
using std::pair;
using std::static_pointer_cast;
using std::stoi;

namespace dharma_vm {

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
			const static string equ;
			const static string dict;
	};

	class runtime_diagnostic_messages {
		public:
			const static string expected_instruction;
			const static string incorrect_number_of_operands;
			const static string malformed_operands;
			const static string identifier_not_previously_declared;
			const static string literal_cannot_be_a_destination_operand;
			const static string expected_integer_or_floating_point_operands_for_this_instruction;
			const static string only_integers_are_acceptable_here;
			const static string register_not_previously_declared;
			const static string only_booleans_are_acceptable_here;
	};

	enum type_kind {
		TYPE_INT, TYPE_LIST, TYPE_DICT, TYPE_TUPLE, TYPE_STRING, TYPE_DECIMAL, TYPE_BOOLEAN, TYPE_NIL, TYPE_NONE
	};

	enum type_pure_kind {
		TYPE_PURE_YES, TYPE_PURE_NO, TYPE_PURE_NONE
	};

	enum type_class_kind {
		TYPE_CLASS_YES, TYPE_CLASS_NO, TYPE_CLASS_NONE
	};

	enum literal_kind {
		LITERAL_IDENTIFIER, LITERAL_REGULAR_HEX_NUMBER, LITERAL_DECIMAL_NUMBER, LITERAL_STRING, LITERAL_BOOLEAN, LITERAL_NIL,
		LITERAL_NONE
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

	enum storage_type_kind {
		STORAGE_TYPE_REGISTER, STORAGE_TYPE_IDENTIFIER
	};

	class storage {
		storage_type_kind st_kind;
		string identifier;
		int register_number;
		public:
			storage(string ident);
			storage(int reg_num);
			~storage();
			string get_identifier();
			int get_register_number();
			const storage_type_kind get_storage_type_kind();
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
			const bool operator==(type_information t_inf);
			const bool operator!=(type_information t_inf);
	};

	class runtime_variable : public instruction {
		type_information t_inf;
		string str;
		int integer;
		float decimal;
		bool boolean;
		vector<shared_ptr<runtime_variable>> list_tuple;
		map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> dict;
		storage store;
		public:
			runtime_variable(shared_ptr<runtime_variable> rvar);
			runtime_variable(runtime_variable& rvar);
			runtime_variable(storage s, string st);
			runtime_variable(storage s, int i);
			runtime_variable(storage s, float f);
			runtime_variable(storage s, bool b);
			runtime_variable(storage s, type_kind tk, vector<shared_ptr<runtime_variable>> lt);
			runtime_variable(storage s, map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d);
			runtime_variable(storage s, shared_ptr<instruction> src);
			runtime_variable(storage s);
			~runtime_variable();
			string get_string();
			bool get_boolean();
			int get_integer();
			float get_decimal();
			vector<shared_ptr<runtime_variable>> get_list_tuple();
			map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> get_dict();
			int set_integer(int i);
			float set_decimal(float f);
			string set_string(string s);
			bool set_boolean(bool b);
			vector<shared_ptr<runtime_variable>> set_list_tuple(vector<shared_ptr<runtime_variable>> lt);
			map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> set_dict(map<shared_ptr<runtime_variable>, shared_ptr<runtime_variable>> d);
			storage get_storage();
			type_information get_type_information();
			type_information set_type_information(type_information t);
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

	class runtime_utilities {
		public:
			runtime_utilities();
			~runtime_utilities();
			vector<string> split_instruction_by_spaces(string instruction);
			string trim(string s);
			bool report_error_and_terminate_program(string msg, vector<string> instruction, int line, int index);
	};

	class runtime {
		vector<shared_ptr<instruction>> instruction_list;
		vector<string> code;

		int find_instruction(string str);
		int find_instruction(int r);
		pair<literal_kind, string> recognize_identifier(string str);

		public:
			runtime(vector<string> insn_list);
			~runtime();
			bool run_program();
	};
}

#endif
