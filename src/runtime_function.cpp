#include "../includes/runtime.hpp"

namespace dharma_vm {

	function::function(string fn, vector<string> fc, vector<string> fal, shared_ptr<runtime_variable> fv) {
		function_name = fn;
		function_code = fc;
		function_argument_list = fal;
		function_variable = fv;
	}

	function::~function() {

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

	const bool runtime::function_pass() {
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn_list = parse_instruction(string_instruction_list[i]);
			if (insn_list.size() > 0 && insn_list[0] == vm_instruction_list::func) {
				string fname = insn_list[1];
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, fname, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, fname, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), type_information_list::_func);
				if (find_instruction(fname).second);
				else
					instruction_list.push_back(rvar);
				vector<string> fal_temp(insn_list.begin() + 2, insn_list.end());
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
				function_list.push_back(make_shared<function>(fname, function_code, fal, rvar));
			}
		}
		return true;
	}

	shared_ptr<runtime_variable> runtime::run_function(shared_ptr<function> func, shared_ptr<runtime_variable> fvar, vector<shared_ptr<runtime_variable>> argument_list) {
		if (func == nullptr || fvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (fvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_IDENTIFIER && func->get_function_name() == fvar->get_storage_field().get_identifier());
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, fvar);
		if (func->get_function_argument_list().size() != argument_list.size())
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, fvar);
		vector<string> vec = func->get_function_argument_list();
		int save = instruction_list.size();
		for (int i = 0; i < vec.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, vec[i].substr(1, vec[i].length() - 2), storage_field_kind::STORAGE_FIELD_IDENTIFIER), argument_list[i]->get_integer(), argument_list[i]->get_decimal(), argument_list[i]->get_string(), argument_list[i]->get_boolean(),
				argument_list[i]->get_list_tuple(), argument_list[i]->get_dict(), argument_list[i]->get_type_information());
			instruction_list.push_back(rvar);
		}
		runtime r(func->get_function_code(), instruction_list, function_list);
		shared_ptr<runtime_variable> ret = r.run_program();
		instruction_list.erase(instruction_list.begin() + save, instruction_list.end());
		return ret;
	}
}