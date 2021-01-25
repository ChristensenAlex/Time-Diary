/*
This is a project to help me gather data about myself.

The two features
 - Gather time diary data.
 - Gather communication data.

*/

#pragma once
#define __STDC_WANT_LIB_EXT1__ 1 // Includes localtime_s in <ctime>
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<ctime>

using namespace std;

void help() {
	string help_Prompt = "h    -help\nx    -exit\ni    -interpersonal\nd    -time diary";
	cout << help_Prompt << endl;
}

enum class Technique : char { Direct = 'D', NonHelp = 'N', NUL = 'U' };

ostream& operator<<(ostream& os, Technique& t) {
	switch (t)
	{
	case Technique::Direct: os << "Direct Technique"; break;
	case Technique::NonHelp: os << "Non-Help Technique"; break;
	case Technique::NUL: os << "NULL Technique"; break;
	default: os.setstate(ios_base::failbit);
	}
	return os;
}

istream& operator>>(istream& is, Technique& t) {
	string first_word;
	string second_word;
	cin >> first_word;
	cin >> second_word;
	if(first_word == "Direct")
		t = Technique::Direct;
	else if(first_word == "Non-Help")
		t = Technique::NonHelp;
	else if(first_word == "NULL")
		t = Technique::NUL;
	else is.setstate(ios_base::failbit);
	
	return is;
}

enum class Grade : char { F = 'F', D = 'D', C = 'C', B = 'B', A = 'A' }; // Enum class provides a strong type. Maybe add E as NULL.

ostream& operator<<(ostream& os, Grade& g) {
	switch (g)
	{
		case Grade::F: os << "F"; break;
		case Grade::D: os << "D"; break;
		case Grade::C: os << "C"; break;
		case Grade::B: os << "B"; break;
		case Grade::A: os << "A"; break;
		default: os.setstate(ios_base::failbit);
	}
	return os;
}

istream& operator>>(istream& is, Grade& g) {
	char ch; // I could pull the full line then grab the first two characters. If the second character doesn not exist then set to NULL. not + or -.
	is >> ch;
	switch (ch)
	{
		case 'F': g = Grade::F; break;
		case 'D': g = Grade::D; break;
		case 'C': g = Grade::C; break;
		case 'B': g = Grade::B; break;
		case 'A': g = Grade::A; break;
		default: is.setstate(ios_base::failbit); // Maybe put a while loop so user can retry.
	}
	return is;
}

void communication() {
	srand(time(0));
	int random = rand() % 2;
	Technique UsedTechnique = Technique::NUL;
	if (random == 0) {
		UsedTechnique = Technique::Direct;
		cout << "Use direct technique" << endl;
	}
	else {
		UsedTechnique = Technique::NonHelp;
		cout << "Use non-help technique" << endl;
	}

	cout << "What was your grade?" << endl;
	Grade g;
	cin >> g;

	// Save to a file
	ofstream file("communication.txt", ios::app);
	file << UsedTechnique << "~" << g << endl; // Maybe add time stamp. ~ is a delimiter.
	file.close();
}

class Date {
public:
	Date() {
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		year = 1900 + t.tm_year;
		month = 1 + t.tm_mon;
		day = t.tm_mday;
	}

	Date(int _year, int _month, int _day) : year(_year), month(_month), day(_day) { }

	int get_year() {
		return year;
	}

	int get_month() {
		return month;
	}

	int get_day() {
		return day;
	}

private:
	int year;
	int month;
	int day;
};

class Time {
public:
	Time() {
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		hour = 1 + t.tm_hour;
		minute = 1 + t.tm_min;
	}

	Time(int _hour, int _minute) : hour(_hour), minute(_minute) { }
	
	void set_hour(int _hour) {
		if (_hour < 0 || _hour > 23) {
			//throw an exception
		}

		else
			hour = _hour;
	}

	int get_hour() {
		return hour;
	}

	void set_minute(int _minute) {
		if (_minute < 0 || _minute > 59) {
			//throw an exception
		}

		else
			minute = _minute;
	}

	int get_minute() {
		return minute;
	}
	friend int operator-(const Time& c1, const Time& c2);
private:
	int hour; // The hour of the day between 0 and 23
	int minute; // The minute between 0 and 59
};

// returns the number of minutes between two times.
int operator-(const Time& t1, const Time& t2) {
	return (t2.hour - t1.hour) * 60 + t2.minute - t1.minute;
}

ostream& operator<<(ostream& os, Time& g) {
	if (g.get_minute() < 10) { // include an extra zero. 5:05 is not 5:5
		os << g.get_hour() << ":0" << g.get_minute();
	} else
		os << g.get_hour() << ":" << g.get_minute();
	return os;
}

istream& operator>>(istream& is, Time& g) {
	string line;
	cin >> line;
	if (line[1] == ':') {
		int hour = stoi(string(1, line[0])); // cast char to string to int
		g.set_hour(hour);
		int minute = stoi(string(1, line[2]) + string(1, line[3]));
		g.set_minute(minute);
	} 
	else if (line[2] == ':') {

		int hour = stoi(string(1, line[0]) + string(1, line[1]));
		g.set_hour(hour);
		int minute = stoi(string(1, line[3]) + string(1, line[4]));
		g.set_minute(minute);
	}
	else {
		is.setstate(ios_base::failbit);
	}
	return is;
}

// Entry contains data for a single entry in the time_diary.
struct entry {
	Date date;
	Time start;
	Time stop;
	int spent = -1;
	string task;
	string steps;
	string problem;
	string summary;
};

ostream& operator<<(ostream& os, entry& e) { // Comma is the deliminator
	os << e.task << ',' << e.steps << ',' << e.problem << ',' << e.start << ',' << e.stop << ',' << e.spent << ',' << e.summary << endl;
	return os;
}

void time_diary() {
	entry event;

	string purge;
	getline(cin, purge); // An extra newline is in cin. Maybe a different way to flush cin would be better.

	event.start = Time();

	cout << "What is the task?" << endl;
	getline(cin, event.task);

	cout << "What baby steps are needed to complete that task?" << endl;
	getline(cin, event.steps);

	cout << "What problems will you face?" << endl;
	getline(cin, event.problem);

	cout << "You are now working on the task. Press enter when you are done." << endl;
	getline(cin, purge);

	event.stop = Time();

	event.spent = event.start - event.stop;

	cout << "Write a summary of what happened. Include any follow up steps." << endl;
	getline(cin, event.summary);

	// print to file.
	ofstream file("TimeDiary.txt", ios::app);
	file << event << endl;
	file.close();
}

int main() {
	while (true) {
		cout << "Enter a command." << endl;
		char ch;
		cin >> ch;
		switch (ch) {
		case 'd':
			time_diary();
			break;
		case 'i':
			communication();
			break;
		case 'h':
			help();
			break;
		case 'x':
			return 0;
			break;
		default:
			cout << "Not a command I know.\nEnter h for help" << endl;
			break;
		}
	}
}
