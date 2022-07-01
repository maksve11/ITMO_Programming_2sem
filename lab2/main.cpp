#define CURL_STATICLIB
#include <iostream>
#pragma warning(push,0)
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#pragma warning(pop)
#include <conio.h>
#include "all_currencies.h"
#include <exception>

using namespace std;

string previous_url = "";

// from request do string
size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
	((string*)userp)->append((char*)buffer, size * nmemb);
	return size * nmemb;
}

void parse_request_session(all_currencies& values, string url) {
	system("cls");
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	if (!curl)
	{
		throw exception("You can't working with curl library");
	}

	string buf;
	CURLcode response;
	response = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	if (response != CURLE_OK)
	{
		throw exception("Error when specifying certificate authentication");
	}

	response = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	if (response != CURLE_OK)
	{
		throw exception("Error with URL");
	}

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	if (response != CURLE_OK)
	{
		throw exception("Error while write data function assignment");
	}

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
	if (response != CURLE_OK)
	{
		throw exception("Error while passing buffer massive to callback");
	}

	response = curl_easy_perform(curl);
	if (response != CURLE_OK)
	{
		throw exception("Error while data transfering");
	}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	nlohmann::json json_buf;
	json_buf = nlohmann::json::parse(buf);

	string cur_date = json_buf["Date"];

	previous_url = json_buf["PreviousURL"];
	previous_url.insert(0, "https:");
	previous_url.erase(previous_url.find("/"), 1);

	for (auto element : json_buf["Valute"]) {
		string charcode_name = element["CharCode"];
		double nomin = element["Nominal"];
		string name = element["Name"];
		double cur = element["Value"];
		currency tmp(cur, nomin, cur_date, charcode_name, name);
		values.add_currency(tmp);
	}
}


int main() {
	try {
		SetConsoleOutputCP(CP_UTF8);
		string url;
		int outtime, option;
		cout << "Enter URL to get data from: ";
		cin >> url;
		cout << "Choose an option: <1> - get todays course; <2> - receive by day: ";
		cin >> option;
		if (option == 1)
		{
			cout << "Enter time to update currency date: ";
			cin >> outtime;
			all_currencies values;
			while (!_kbhit()) {
				parse_request_session(values, url);
				values.write_down_all();
				cout << "Print any button to exit program\n";
				Sleep(outtime * 1000);
			}
			values.cout_average();
			values.cout_median();
		}
		else
		{
			cout << "Enter the course viewing time: ";
			cin >> outtime;
			all_currencies values;
			while (!_kbhit()) {
				if (!previous_url.empty())
				{
					parse_request_session(values, previous_url);
					values.write_down_all();
					cout << "Print any button to exit program\n";
					Sleep(outtime * 1000);
				}
				else
				{
					parse_request_session(values, url);
					values.write_down_all();
					cout << "Print any button to exit program\n";
					Sleep(outtime * 1000);
				}
			}
			values.cout_average();
			values.cout_median();
		}
	}
	catch (exception err) {
		cout << err.what() << '\n';
	}
	return 0;
}
