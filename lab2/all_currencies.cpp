#include "all_currencies.h"

string currency::get_currency_name() const {
	return name;
}

string currency::get_currency_charcode() const {
	return charcode;
}

vector <double> currency::get_values() const {
	return values;
};

double currency::get_nominal() const {
	return nominal;
}

string currency::get_date() const {
	return date;
}

void currency::change_currency(const double& cur) {
	values.push_back(cur);
}

void currency::change_date(const string& date_) {
	date = date_;
}

void all_currencies::add_currency(currency& cur) {
	if (all.find(cur.get_currency_charcode()) == all.end())
	{
		all.insert({ cur.get_currency_charcode(), cur });
	}
	else
	{
		all[cur.get_currency_charcode()].change_date(cur.get_date());
		all[cur.get_currency_charcode()].change_currency(cur.get_values()[0]);
	}
}

HANDLE h;

void all_currencies::write_down_all() const {
	auto it = all.begin();
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 1);
	cout << it->second.get_date().erase(it->second.get_date().find("T")) << '\n';
	for (auto i : all) {
		SetConsoleTextAttribute(h, 15);
		cout << "CharCode: ";
		SetConsoleTextAttribute(h, 13);
		cout << i.second.get_currency_charcode();
		SetConsoleTextAttribute(h, 15);
		cout << " | Name: ";
		SetConsoleTextAttribute(h, 14);
		cout << i.second.get_currency_name();
		SetConsoleTextAttribute(h, 15);
		cout << " | Value: ";
		SetConsoleTextAttribute(h, 3);
		double diff = 0;
		cout << i.second.get_values()[i.second.get_values().size() - 1] / i.second.get_nominal() << ' ';
		if (i.second.get_values().size() >= 2)
			diff = (i.second.get_values()[i.second.get_values().size() - 1] - i.second.get_values()[i.second.get_values().size() - 2]) / i.second.get_nominal();
		SetConsoleTextAttribute(h, 15);
		cout << " | Difference: ";
		if (diff > 0)
		{
			SetConsoleTextAttribute(h, 2);
			cout << '+' << diff;
		}
		else if (diff < 0)
		{
			SetConsoleTextAttribute(h, 4);
			cout << '-' << abs(diff);
		}
		else
		{
			SetConsoleTextAttribute(h, 11);
			cout << diff;
		}
		cout << '\n';
	}
}

void all_currencies::cout_median() const {
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 2);
	cout << "----------------Medians----------------\n";
	for (auto& i : all) {
		h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, 15);
		vector <double> current_values = i.second.get_values();
		auto median = current_values.begin() + current_values.size() / 2;
		nth_element(current_values.begin(), median, current_values.end());
		cout << "CharCode: ";
		SetConsoleTextAttribute(h, 13);
		cout << i.second.get_currency_charcode();
		SetConsoleTextAttribute(h, 15);
		cout << " | Name: ";
		SetConsoleTextAttribute(h, 14);
		cout << i.second.get_currency_name();
		SetConsoleTextAttribute(h, 15);
		cout << " | Median = ";
		SetConsoleTextAttribute(h, 3);
		if (current_values.size() % 2 != 0)
			cout << current_values[(i.second.get_values().size()) / 2] << '\n';
		else
			cout << (current_values[(i.second.get_values().size()) / 2] + current_values[(i.second.get_values().size()) / 2 - 1]) / 2 << '\n';
	}
}

void all_currencies::cout_average() const {
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 2);
	cout << "----------------Average count----------------\n";
	for (auto i : all) {
		SetConsoleTextAttribute(h, 15);
		double count = 0;
		for (double j : i.second.get_values()) {
			count += j;
		}
		cout << "CharCode: ";
		SetConsoleTextAttribute(h, 13);
		cout << i.second.get_currency_charcode();
		SetConsoleTextAttribute(h, 15);
		cout << " | Name: ";
		SetConsoleTextAttribute(h, 14);
		cout << i.second.get_currency_name();
		SetConsoleTextAttribute(h, 15);
		cout << " | Average =  ";
		SetConsoleTextAttribute(h, 3);
		cout << count / i.second.get_values().size() << '\n';
	}
}