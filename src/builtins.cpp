#include "../includes/runtime.hpp"

namespace dharma_vm {
	
	shared_ptr<runtime_variable> runtime::print(shared_ptr<runtime_variable> rvar) {
		if (rvar == nullptr)
			report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, nullptr);
		if (rvar->get_type_information() == type_information_list::_boolean)
			cout << rvar->get_boolean();
		else if (rvar->get_type_information() == type_information_list::_decimal)
			cout << rvar->get_decimal();
		else if (rvar->get_type_information() == type_information_list::_dict) {
			if (rvar->get_dict().first.size() != rvar->get_dict().second.size())
				report_error_and_terminate_program(runtime_diagnostic_messages::fatal_error, rvar);
			cout << "{{{ ";
			for (int i = 0; rvar->get_dict().first.size(); i++) {
				print(rvar->get_dict().first[i]);
				cout << ": ";
				print(rvar->get_dict().second[i]);
				if (i != rvar->get_dict().first.size() - 1)
					cout << ", ";
			}
			cout << " }}}";
		}
		else if (rvar->get_type_information() == type_information_list::_list || rvar->get_type_information() == type_information_list::_tuple) {
			if (rvar->get_type_information() == type_information_list::_list)
				cout << "[[[ ";
			else
				cout << "<<< ";
			for (int i = 0; i < rvar->get_list_tuple().size(); i++) {
				print(rvar->get_list_tuple()[i]);
				if (i != rvar->get_list_tuple().size() - 1)
					cout << ", ";
			}
			if (rvar->get_type_information() == type_information_list::_list)
				cout << " ]]]";
			else
				cout << " >>>";
		}
		else if (rvar->get_type_information() == type_information_list::_func)
			cout << "Function: " << rvar->get_string();
		else if (rvar->get_type_information() == type_information_list::_int)
			cout << rvar->get_integer();
		else if (rvar->get_type_information() == type_information_list::_nil)
			cout << "Nil";
		else if (rvar->get_type_information() == type_information_list::_pure_boolean)
			cout << "[Boolean]";
		else if (rvar->get_type_information() == type_information_list::_pure_decimal)
			cout << "[Decimal]";
		else if (rvar->get_type_information() == type_information_list::_pure_dict)
			cout << "[Dict]";
		else if (rvar->get_type_information() == type_information_list::_pure_func)
			cout << "[Function]";
		else if (rvar->get_type_information() == type_information_list::_pure_int)
			cout << "[Integer]";
		else if (rvar->get_type_information() == type_information_list::_pure_list)
			cout << "[List]";
		else if (rvar->get_type_information() == type_information_list::_pure_nil)
			cout << "[Nil]";
		else if (rvar->get_type_information() == type_information_list::_pure_string)
			cout << "[String]";
		else if (rvar->get_type_information() == type_information_list::_pure_tuple)
			cout << "[Tuple]";
		else if (rvar->get_type_information() == type_information_list::_string)
			cout << rvar->get_string();
		else if (rvar->get_type_information().get_type_kind() == type_kind::TYPE_CUSTOM && rvar->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
			cout << "Struct: " << rvar->get_string();
		else if (rvar->get_type_information().get_type_kind() == type_kind::TYPE_CUSTOM && rvar->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Struct]";
		else if (rvar->get_type_information().get_type_kind() == type_kind::TYPE_MODULE && rvar->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_NO)
			cout << "Module: " << rvar->get_string();
		else if (rvar->get_type_information().get_type_kind() == type_kind::TYPE_MODULE && rvar->get_type_information().get_type_pure_kind() == type_pure_kind::TYPE_PURE_YES)
			cout << "[Module]";
		else
			report_error_and_terminate_program(runtime_diagnostic_messages::incompatible_types, rvar);
		shared_ptr<runtime_variable> ret = make_shared<runtime_variable>(storage_field(-1, runtime_temporary_prefix + to_string(runtime_temporary_count), storage_field_kind::STORAGE_FIELD_IDENTIFIER), -1, -1, "", true,
			vector<shared_ptr<runtime_variable>>(), pair<vector<shared_ptr<runtime_variable>>, vector<shared_ptr<runtime_variable>>>(), vector<shared_ptr<runtime_variable>>(), make_shared<runtime>(vector<string>(), vector<shared_ptr<runtime_variable>>(), vector<shared_ptr<function>>(),
				vector<vector<shared_ptr<runtime_variable>>>(), vector<vector<shared_ptr<runtime_variable>>>()), type_information_list::_boolean);
		return checked_insertion(ret);
	}
}