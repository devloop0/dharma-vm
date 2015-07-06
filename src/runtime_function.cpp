#include "../includes/runtime.hpp"

namespace dharma_vm {

	function::function(string fn, vector<string> fc, vector<string> fal, shared_ptr<runtime_variable> fv, bool va) {
		function_name = fn;
		function_code = fc;
		function_argument_list = fal;
		function_variable = fv;
		va_args = va;
	}

	function::~function() {

	}

	shared_ptr<runtime_variable> function::get_function_variable() {
		return function_variable;
	}

	string function::get_function_name() {
		return function_name;
	}

	vector<string> function::get_function_code() {
		return function_code;
	}

	vector<string> function::get_function_argument_list() {
		return function_argument_list;
	}

	const bool function::get_va_args() {
		return va_args;
	}

	const bool runtime::function_pass() {
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn_list = parse_instruction(string_instruction_list[i]);
			if (insn_list.size() > 0 && insn_list[0] == vm_instruction_list::func) {
				string fname = insn_list[1];
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, fname, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, fname, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), nullptr, type_information_list::_func);
				if (find_instruction(fname).second);
				else
					instruction_list.push_back(rvar);
				vector<string> fal_temp(insn_list.begin() + 2, insn_list.end());
				bool va_args = false;
				if (fal_temp.size() > 0 && fal_temp[fal_temp.size() - 1] == builtins::va_args_function_parameter) {
					fal_temp.erase(fal_temp.end() - 1, fal_temp.end());
					va_args = true;
				}
				vector<string> fal;
				for (int i = 0; i < fal_temp.size(); i++)
					fal.push_back("[" + fal_temp[i] + "]");
				vector<string> function_code;
				string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				while (i < string_instruction_list.size()) {
					vector<string> temp = parse_instruction(string_instruction_list[i]);
					if (temp.size() > 0 && temp[0] == vm_instruction_list::efunc)
						break;
					else
						function_code.push_back(string_instruction_list[i]);
					string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				}
				string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				i--;
				function_list.push_back(make_shared<function>(fname, function_code, fal, rvar, va_args));
			}
		}
		return true;
	}

	shared_ptr<runtime_variable> runtime::run_function(shared_ptr<function> func, shared_ptr<runtime_variable> fvar, vector<shared_ptr<runtime_variable>> argument_list) {
		if (func == nullptr || fvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (func->get_function_name() == fvar->get_string());
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, fvar);
		if (func->get_function_name() == builtins::builtin_print) {
			for (int i = 0; i < argument_list.size(); i++)
				print(argument_list[i]);
		}
		vector<string> vec = func->get_function_argument_list();
		int save = instruction_list.size();
		if (func->get_va_args()) {
			vector<shared_ptr<runtime_variable>> excess_argument_list;
			for (int i = func->get_function_argument_list().size(); i < argument_list.size(); i++) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(argument_list[i]->get_storage_field(), argument_list[i]->get_integer(), argument_list[i]->get_decimal(), argument_list[i]->get_string(), argument_list[i]->get_boolean(),
					argument_list[i]->get_list_tuple(), argument_list[i]->get_dict(), argument_list[i]->get_struct_member_list(), argument_list[i]->get_module_runtime(), argument_list[i]->get_type_information());
				excess_argument_list.push_back(rvar);
			}
			instruction_list.push_back(make_shared<runtime_variable>(storage_field(-1, builtins::builtin__va_args__, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, excess_argument_list,
				pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), nullptr, type_information_list::_tuple));
		}
		else {
			if (func->get_function_argument_list().size() != argument_list.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, fvar);
		}
		for (int i = 0; i < vec.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, vec[i].substr(1, vec[i].length() - 2), storage_field_kind::STORAGE_FIELD_IDENTIFIER), argument_list[i]->get_integer(), argument_list[i]->get_decimal(), argument_list[i]->get_string(), argument_list[i]->get_boolean(),
				argument_list[i]->get_list_tuple(), argument_list[i]->get_dict(), argument_list[i]->get_struct_member_list(), argument_list[i]->get_module_runtime(), argument_list[i]->get_type_information());
			instruction_list.push_back(rvar);
		}
		runtime r(func->get_function_code(), instruction_list, function_list);
		shared_ptr<runtime_variable> ret = r.run_program();
		instruction_list.insert(instruction_list.end(), r.instruction_list.begin(), r.instruction_list.end());
		instruction_list.erase(instruction_list.begin() + save, instruction_list.end());
		return ret;
	}

	const bool runtime::struct_pass() {
		int save_begin = instruction_list.size();
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn_list = parse_instruction(string_instruction_list[i]);
			if (insn_list.size() > 0 && (insn_list[0] == vm_instruction_list::struc || insn_list[0] == vm_instruction_list::istruc)) {
				string sname = insn_list[1];
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, sname, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), nullptr, type_information(type_kind::TYPE_CUSTOM, type_pure_kind::TYPE_PURE_NO,
						type_class_kind::TYPE_CLASS_YES, sname));
				if (insn_list[0] == vm_instruction_list::istruc)
					rvar->set_unmodifiable(true);
				instruction_list.push_back(rvar);
			}
		}
		int save_end = instruction_list.size();
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn_list = parse_instruction(string_instruction_list[i]);
			if (insn_list.size() > 0 && (insn_list[0] == vm_instruction_list::struc || insn_list[0] == vm_instruction_list::istruc)) {
				string sname = insn_list[1];
				vector<string> member_list(insn_list.begin() + 2, insn_list.end());
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, sname, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), nullptr, 
					type_information(type_kind::TYPE_CUSTOM, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, sname));
				if (insn_list[0] == vm_instruction_list::istruc)
					rvar->set_unmodifiable(true);
				vector<string> struct_code;
				string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				int save = instruction_list.size();
				vector<shared_ptr<runtime_variable>> rvar_list;
				while (i < string_instruction_list.size()) {
					vector<string> temp = parse_instruction(string_instruction_list[i]);
					if (temp.size() > 0 && temp[0] == vm_instruction_list::estruc)
						break;
					else
						struct_code.push_back(string_instruction_list[i]);
					string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				}
				string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				i--;
				runtime r(struct_code, instruction_list, function_list);
				shared_ptr<runtime_variable> rv = r.run_program();
				instruction_list.insert(instruction_list.end(), r.instruction_list.begin(), r.instruction_list.end());
				for_each(instruction_list.begin() + save, instruction_list.end(), [&](shared_ptr<runtime_variable> rvar) {
					if (rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER);
					else {
						bool found = find(member_list.begin(), member_list.end(), rvar->get_storage_field().get_identifier()) != member_list.end();
						if (found) {
							if (rvar->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES &&
								rvar->get_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_NO);
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
							if (rvar->get_type_information() == type_information_list::_pure_int) {
								rvar->set_integer(0);
								rvar->set_type_information(type_information_list::_int);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_decimal) {
								rvar->set_decimal(0.0);
								rvar->set_type_information(type_information_list::_decimal);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_string) {
								rvar->set_string("");
								rvar->set_type_information(type_information_list::_string);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_boolean) {
								rvar->set_boolean(false);
								rvar->set_type_information(type_information_list::_boolean);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_list) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_type_information(type_information_list::_list);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_tuple) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_type_information(type_information_list::_tuple);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_dict) {
								rvar->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
								rvar->set_type_information(type_information_list::_dict);
							}
							else if (rvar->get_type_information() == type_information_list::_pure_func) {
								rvar->set_type_information(type_information_list::_func);
							}
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
							rvar_list.push_back(rvar);
						}
					}
				});
				instruction_list.erase(instruction_list.begin() + save, instruction_list.end());
				if (member_list.size() != rvar_list.size())
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				rvar->set_struct_member_list(rvar_list);
				instruction_list.push_back(rvar);
			}
		}
		instruction_list.erase(instruction_list.begin() + save_begin, instruction_list.begin() + save_end);
		return true;
	}
}