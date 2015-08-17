#include "includes/runtime.hpp"

namespace dharma_vm {
	
	shared_ptr<runtime_variable> runtime::print(shared_ptr<runtime_variable> rvar) {
		if (rvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (rvar->get_runtime_type_information() == runtime_type_information_list::_boolean) {
			cout << (rvar->get_boolean() ? "true" : "false");
		}
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_decimal)
			cout << rvar->get_decimal();
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_dict) {
			if (rvar->get_dict().first.size() != rvar->get_dict().second.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, rvar);
			cout << "{{{ ";
			for (int i = 0; i < rvar->get_dict().first.size(); i++) {
				print(rvar->get_dict().first[i]);
				cout << ": ";
				print(rvar->get_dict().second[i]);
				if (i != rvar->get_dict().first.size() - 1)
					cout << ", ";
			}
			cout << " }}}";
		}
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_list || rvar->get_runtime_type_information() == runtime_type_information_list::_tuple) {
			if (rvar->get_runtime_type_information() == runtime_type_information_list::_list)
				cout << "[[[ ";
			else
				cout << "<<< ";
			for (int i = 0; i < rvar->get_list_tuple().size(); i++) {
				print(rvar->get_list_tuple()[i]);
				if (i != rvar->get_list_tuple().size() - 1)
					cout << ", ";
			}
			if (rvar->get_runtime_type_information() == runtime_type_information_list::_list)
				cout << " ]]]";
			else
				cout << " >>>";
		}
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_int)
			cout << rvar->get_integer();
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_nil)
			cout << "Nil";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_func)
			cout << "Function: " << rvar->get_string();
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_boolean)
			cout << "[Boolean]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_decimal)
			cout << "[Decimal]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_dict)
			cout << "[Dict]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_func)
			cout << "[Function: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_int)
			cout << "[Integer]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_list)
			cout << "[List]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_nil)
			cout << "[Nil]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_string)
			cout << "[String]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_pure_tuple)
			cout << "[Tuple]";
		else if (rvar->get_runtime_type_information() == runtime_type_information_list::_string)
			cout << rvar->get_string();
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Struct: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Enum: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Enum Child: " << rvar->get_string() << "]";
		else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Module: " << rvar->get_string() << "]";
		else {
			vector<pair<shared_ptr<runtime_variable>, shared_ptr<runtime>>> results = find_builtin_function(instruction_list, nullptr, builtins::builtin__print__);
			bool success = false;
			if (results.size() > 0) {
				for (int j = 0; j < results.size(); j++) {
					pair<shared_ptr<runtime_variable>, shared_ptr<runtime>> res = results[j];
					for (int i = 0; i < res.first->get_function().size(); i++) {
						shared_ptr<runtime_variable> temp = run_function({ res.first->get_function()[i] }, res.first, { rvar }, res.second);
						if (temp->get_runtime_type_information() == runtime_type_information_list::_nil);
						else {
							print(temp);
							success = true;
							break;
						}
					}
					if (success)
						break;
				}
				if(!success)
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
			}
			else if (!success) {
				if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_CUSTOM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Struct: " << rvar->get_string();
				else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_MODULE && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Module: " << rvar->get_string();
				else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Enum: " << rvar->get_string();
				else if (rvar->get_runtime_type_information().get_runtime_type_kind() == runtime_type_kind::TYPE_ENUM_CHILD && rvar->get_runtime_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
					cout << "Enum Child: " << rvar->get_string();
				else
					report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
			}
			else
				report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
		}
		shared_ptr<runtime_variable> ret = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean,
			vector<shared_ptr<function>>());
		return checked_insertion(ret);
	}

	shared_ptr<runtime_variable> runtime::exit(shared_ptr<runtime_variable> exit_code, shared_ptr<runtime_variable> message) {
		if (exit_code == nullptr || message == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (exit_code->get_runtime_type_information() != runtime_type_information_list::_int)
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, exit_code);
		if (message->get_runtime_type_information() != runtime_type_information_list::_string)
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, message);
		print(message);
		std::exit(exit_code->get_integer());
		shared_ptr<runtime_variable> created_bool = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>()), runtime_type_information_list::_boolean,
			vector<shared_ptr<function>>());
		runtime_temporary_count++;
		return checked_insertion(created_bool);
	}
}