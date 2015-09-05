#include "includes/runtime.hpp"

namespace dharma_vm {

	function::function(string fn, vector<string> fc, vector<string> fal, bool va, bool l, bool b) {
		function_name = fn;
		function_code = fc;
		function_argument_list = fal;
		va_args = va;
		lambda = l;
		builtin = b;
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

	const bool function::get_va_args() {
		return va_args;
	}

	const bool function::is_lambda() {
		return lambda;
	}

	const bool function::is_builtin() {
		return builtin;
	}

	shared_ptr<function> function::set_function(shared_ptr<function> f) {
		function_name = f->function_name;
		function_code = f->function_code;
		function_argument_list = f->function_argument_list;
		va_args = f->va_args;
		lambda = f->lambda;
		return make_shared<function>(*this);
	}

	const bool operator==(shared_ptr<function> dest, shared_ptr<function> src) {
		return dest->get_function_name() == src->get_function_name() && dest->get_function_argument_list() == src->get_function_argument_list() &&
			dest->get_function_code() == src->get_function_code() && dest->get_va_args() == src->get_va_args() && dest->is_lambda() == src->is_lambda();
	}
	
	const bool runtime::function_pass() {
		vector<shared_ptr<runtime_variable>> func_list;
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn_list = parse_instruction(string_instruction_list[i]);
			if (insn_list.size() > 0 && (insn_list[0] == vm_instruction_list::module || insn_list[0] == vm_instruction_list::imodule)) {
				i++;
				int module_balance = 0;
				while (i < string_instruction_list.size()) {
					vector<string> temp = parse_instruction(string_instruction_list[i]);
					if (temp.size() > 0 && (temp[0] == vm_instruction_list::imodule || temp[0] == vm_instruction_list::module))
						module_balance++;
					if (temp.size() > 0 && temp[0] == vm_instruction_list::emodule) {
						if (module_balance == 0)
							break;
						else
							module_balance--;
					}
					i++;
				}
				i--;
			}
			else if (insn_list.size() > 0 && (insn_list[0] == vm_instruction_list::func || insn_list[0] == vm_instruction_list::ifunc)) {
				string fname = insn_list[1];
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
				shared_ptr<function> f = make_shared<function>(fname, function_code, fal, va_args, false, false);
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, fname, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, fname, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
					make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(),
						vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_func, vector<shared_ptr<function>> { f });
				if (insn_list[0] == vm_instruction_list::ifunc)
					rvar->set_unmodifiable(true);
				bool added = false;
				for (int i = 0; i < func_list.size(); i++)
					if (func_list[i]->get_string() == rvar->get_string()) {
						func_list[i]->add_function(f);
						added = true;
						break;
					}
				if (!added)
					func_list.push_back(rvar);
			}
		}
		for (int i = 0; i < func_list.size(); i++)
			checked_insertion(func_list[i]);
		return true;
	}

	shared_ptr<runtime_variable> runtime::run_function(vector<shared_ptr<function>> func_list, shared_ptr<runtime_variable> fvar, vector<shared_ptr<runtime_variable>> argument_list,
		shared_ptr<runtime> ru) {
		shared_ptr<runtime> r = nullptr;
		if (fvar->get_module_runtime()->get_instruction_list().size() > 0)
			r = fvar->get_module_runtime();
		else
			r = ru;
		if (fvar->get_string().substr(0, builtins::builtin_runtime_dll_module_prefix.length()) == builtins::builtin_runtime_dll_module_prefix) {
			string str = fvar->get_string().substr(builtins::builtin_runtime_dll_module_prefix.length(), fvar->get_string().length());
			string dll_name = str.substr(0, str.find("@")), function_name = str.substr(str.find("@") + 1, str.length());
			typedef shared_ptr<locked_runtime_variable> (*karmaffi_t)(vector<shared_ptr<locked_runtime_variable>>);
			karmaffi_t kft = (karmaffi_t)GetProcAddress(LoadLibrary(dll_name.c_str()), function_name.c_str());
			return convert_from_ffi(kft(convert_to_ffi(argument_list)));
		}
		shared_ptr<function> func = nullptr;
		for (int i = 0; i < func_list.size(); i++) {
			if (func_list[i]->get_function_name() == fvar->get_string()) {
				if (func_list[i]->get_function_argument_list().size() == argument_list.size()) {
					func = func_list[i];
					break;
				}
				bool va_args = func_list[i]->get_va_args();
				int actual_args_size = argument_list.size();
				int lookup_args_size = func_list[i]->get_function_argument_list().size();
				if (!va_args);
				else {
					if (actual_args_size >= lookup_args_size) {
						func = func_list[i];
						break;
					}
				}
			}
		}
		vector<shared_ptr<runtime_variable>> temp;
		if (func == nullptr || fvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::function_overload_not_found, nullptr);
		if (func->get_function_name() == fvar->get_string());
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, fvar);
		if (func->is_builtin()) {
			if (func->get_function_name() == builtins::builtin_print) {
				for (int i = 0; i < argument_list.size(); i++)
					print(argument_list[i]);
				shared_ptr<runtime_variable> created_bool = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
						vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean,
					vector<shared_ptr<function>>());
				runtime_temporary_count++;
				return checked_insertion(created_bool);
			}
			else if (func->get_function_name() == builtins::builtin_exit && argument_list.size() == 2)
				return exit(argument_list[0], argument_list[1]);
			else if (func->get_function_name() == builtins::builtin_add && argument_list.size() == 3)
				return add(argument_list[0], argument_list[1], argument_list[2]);
			else if (func->get_function_name() == builtins::builtin_add && argument_list.size() == 2)
				return add(argument_list[0], argument_list[1]);
			else if (func->get_function_name() == builtins::builtin_insert && argument_list.size() == 3)
				return insert(argument_list[0], argument_list[1], argument_list[2]);
			else if (func->get_function_name() == builtins::builtin_remove && argument_list.size() == 3)
				return remove(argument_list[0], argument_list[1], argument_list[2]);
			else if (func->get_function_name() == builtins::builtin_remove && argument_list.size() == 2)
				return remove(argument_list[0], argument_list[1]);
			else if (func->get_function_name() == builtins::builtin_load_library && argument_list.size() == 1)
				return load_library(argument_list[0]);
		}
		vector<string> vec = func->get_function_argument_list();
		int save = instruction_list.size();
		if (func->get_va_args()) {
			vector<shared_ptr<runtime_variable>> excess_argument_list;
			for (int i = func->get_function_argument_list().size(); i < argument_list.size(); i++) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(argument_list[i]->get_storage_field(), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
					pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
						vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), argument_list[i]->get_runtime_type_information(), vector<shared_ptr<function>>());
				rvar = mov(rvar, argument_list[i], false);
				excess_argument_list.push_back(rvar);
			}
			temp.push_back(make_shared<runtime_variable>(storage_field(-1, builtins::builtin__va_args__, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, excess_argument_list,
				pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
					vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_tuple, vector<shared_ptr<function>>()));
		}
		else {
			if (func->get_function_argument_list().size() != argument_list.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, fvar);
			temp.push_back(make_shared<runtime_variable>(storage_field(-1, builtins::builtin__va_args__, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
				pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
					vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_tuple, vector<shared_ptr<function>>()));
		}
		for (int i = 0; i < vec.size(); i++) {
			shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, vec[i].substr(1, vec[i].length() - 2), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", false, vector<shared_ptr<runtime_variable>>(),
				pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
					vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), argument_list[i]->get_runtime_type_information(), vector<shared_ptr<function>>());
			rvar = mov(rvar, argument_list[i], false);
			temp.push_back(rvar);
		}
		shared_ptr<runtime_variable> ret = nullptr;
		if (func->is_lambda()) {
			fvar->get_module_runtime()->scope_stack.push_back(temp);
			shared_ptr<runtime> r = fvar->get_module_runtime();
			runtime run(func->get_function_code(), r->instruction_list, r->stacked_function_instruction_list, r->scope_stack, r->module_stack,
				r->added_lambda_instruction_list);
			ret = run.run_program();
			fvar->get_module_runtime()->scope_stack.pop_back();
			vector<shared_ptr<runtime_variable>> temp1 = fvar->get_module_runtime()->get_added_lambda_instruction_list();
		}
		else {
			r == nullptr ? stacked_function_instruction_list.push_back(temp) : r->stacked_function_instruction_list.push_back(temp);
			runtime run(func->get_function_code(), r == nullptr ? instruction_list : r->instruction_list, r == nullptr ? stacked_function_instruction_list : r->stacked_function_instruction_list,
				vector<vector<shared_ptr<runtime_variable>>>(), r == nullptr ? module_stack : r->module_stack, r == nullptr ? added_lambda_instruction_list : r->added_lambda_instruction_list);
			ret = run.run_program();
			vector<shared_ptr<runtime_variable>> temp1 = run.get_added_lambda_instruction_list();
			instruction_list.insert(instruction_list.end(), temp1.begin(), temp1.end());
			run.added_lambda_instruction_list.clear();
			r == nullptr ? stacked_function_instruction_list.pop_back() : r->stacked_function_instruction_list.pop_back();
		}
		return ret;
	}

	const bool runtime::struct_pass() {
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn_list = parse_instruction(string_instruction_list[i]);
			if (insn_list.size() > 0 && (insn_list[0] == vm_instruction_list::module || insn_list[0] == vm_instruction_list::imodule)) {
				i++;
				int module_balance = 0;
				while (i < string_instruction_list.size()) {
					vector<string> temp = parse_instruction(string_instruction_list[i]);
					if (temp.size() > 0 && (temp[0] == vm_instruction_list::imodule || temp[0] == vm_instruction_list::module))
						module_balance++;
					if (temp.size() > 0 && temp[0] == vm_instruction_list::emodule) {
						if (module_balance == 0)
							break;
						else
							module_balance--;
					}
					i++;
				}
				i--;
			}
			else if (insn_list.size() > 0 && (insn_list[0] == vm_instruction_list::struc || insn_list[0] == vm_instruction_list::istruc)) {
				string sname = insn_list[1];
				vector<string> member_list(insn_list.begin() + 2, insn_list.end());
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, sname, storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, sname, false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), nullptr,
					runtime_type_information(runtime_type_kind::TYPE_CUSTOM, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, sname), vector<shared_ptr<function>>());
				bool unmod = false;
				if (insn_list[0] == vm_instruction_list::istruc) {
					rvar->set_unmodifiable(true);
					unmod = true;
				}
				vector<string> struct_code;
				string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				int save = scope_stack.size() > 0 ? scope_stack[scope_stack.size() - 1].size() : (stacked_function_instruction_list.size() > 0 ? stacked_function_instruction_list[stacked_function_instruction_list.size() - 1].size() : instruction_list.size());
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
				scope_stack.push_back(vector<shared_ptr<runtime_variable>>());
				runtime r(struct_code, instruction_list, stacked_function_instruction_list, scope_stack, module_stack, added_lambda_instruction_list);
				shared_ptr<runtime_variable> rv = r.run_program();
				for_each(r.scope_stack[scope_stack.size() - 1].begin(), r.scope_stack[scope_stack.size() - 1].end(), [&](shared_ptr<runtime_variable> rvar) {
					if (rvar->get_storage_field().get_storage_field_kind() == storage_field_kind::STORAGE_FIELD_REGISTER_NUMBER);
					else {
						bool found = find(member_list.begin(), member_list.end(), rvar->get_storage_field().get_identifier()) != member_list.end();
						if (found) {
							if (rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES &&
								rvar->get_runtime_type_information().get_type_class_kind() == type_class_kind::TYPE_CLASS_NO);
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
							if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_int) {
								rvar->set_integer(0);
								rvar->set_runtime_type_information(runtime_type_information_list::_int);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_decimal) {
								rvar->set_decimal(0.0);
								rvar->set_runtime_type_information(runtime_type_information_list::_decimal);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_string) {
								rvar->set_string("");
								rvar->set_runtime_type_information(runtime_type_information_list::_string);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_boolean) {
								rvar->set_boolean(false);
								rvar->set_runtime_type_information(runtime_type_information_list::_boolean);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_list) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_runtime_type_information(runtime_type_information_list::_list);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_tuple) {
								rvar->set_list_tuple(vector<shared_ptr<runtime_variable>>());
								rvar->set_runtime_type_information(runtime_type_information_list::_tuple);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_dict) {
								rvar->set_dict(pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>());
								rvar->set_runtime_type_information(runtime_type_information_list::_dict);
							}
							else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_func) {
								rvar->set_runtime_type_information(runtime_type_information_list::_func);
							}
							else
								report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
							rvar_list.push_back(rvar);
						}
					}
				});
				if (member_list.size() != rvar_list.size())
					report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
				for (int i = 0; i < rvar_list.size(); i++)
					rvar_list[i]->set_unmodifiable(unmod);
				rvar->set_struct_enum_member_list(rvar_list, rvar->get_unique_id());
				scope_stack.pop_back();
				checked_insertion(rvar);
			}
		}
		return true;
	}

	const bool runtime::enum_pass() {
		for (int i = 0; i < string_instruction_list.size(); i++) {
			vector<string> insn = parse_instruction(string_instruction_list[i]);
			if (insn.size() > 0 && (insn[0] == vm_instruction_list::module || insn[0] == vm_instruction_list::imodule)) {
				i++;
				int module_balance = 0;
				while (i < string_instruction_list.size()) {
					vector<string> temp = parse_instruction(string_instruction_list[i]);
					if (temp.size() > 0 && (temp[0] == vm_instruction_list::imodule || temp[0] == vm_instruction_list::module))
						module_balance++;
					if (temp.size() > 0 && temp[0] == vm_instruction_list::emodule) {
						if (module_balance == 0)
							break;
						else
							module_balance--;
					}
					i++;
				}
				i--;
			}
			else if (insn.size() > 0 && insn[0] == vm_instruction_list::_enum) {
				shared_ptr<runtime_variable> rvar = make_shared<runtime_variable>(storage_field(-1, insn[1], storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, insn[1], false,
					vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
					make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
						vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information(runtime_type_kind::TYPE_ENUM, type_pure_kind::TYPE_PURE_NO, type_class_kind::TYPE_CLASS_YES, insn[1]),
					vector<shared_ptr<function>>());
				vector<shared_ptr<runtime_variable>> vec;
				for (int i = 2; i < insn.size(); i++) {
					shared_ptr<runtime_variable> rv = make_shared<runtime_variable>(storage_field(-1, insn[i], storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, insn[i], false,
						vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(),
						make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<vector<shared_ptr<runtime_variable>>>(),
							vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information(runtime_type_kind::TYPE_ENUM_CHILD, type_pure_kind::TYPE_PURE_NO,
								type_class_kind::TYPE_CLASS_YES, insn[1]), vector<shared_ptr<function>>());
					rv->set_unmodifiable(true);
					rv->set_enum_parent_unique_id(rvar->get_unique_id());
					vec.push_back(rv);
				}
				rvar->set_unmodifiable(true);
				rvar->set_struct_enum_member_list(vec, rvar->get_unique_id());
				checked_insertion(rvar);
				string_instruction_list.erase(string_instruction_list.begin() + i, string_instruction_list.begin() + i + 1);
				i--;
			}
		}
		return true;
	}
}