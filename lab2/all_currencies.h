#ifndef LAB2_ALL_CURRENCIES_H
#define LAB2_ALL_CURRENCIES_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <Windows.h>

using namespace std;

class currency {
private:
	string name, charcode, date;
	vector <double> values;
	double nominal;

public:
	currency() {}

	currency(vector <double> cur, double nomin, string names, string charcode_name, string date_) : name(move(names)), charcode(move(charcode_name)), values(move(cur)), nominal(move(nomin)), date(move(date_))
	{}

	currency(double value, double nomin, string date_, string charcode_name, string names) : name(move(names)), charcode(move(charcode_name)), nominal(move(nomin)), date(move(date_)) {
		values.push_back(value);
	}

	string get_currency_name() const;

	string get_currency_charcode() const;

	double get_nominal() const;

	vector<double> get_values() const;

	void change_currency(const double& cur);

	void change_date(const string& date_);

	string get_date() const;
};

class all_currencies {
private:
	unordered_map<string, currency> all;

public:
	all_currencies() = default;

	void add_currency(currency& cur);

	void write_down_all() const;

	void cout_median() const;

	void cout_average() const;
};
#endif
