/*** Code for sampling some 4-tuples randomly
 Author: Ashwin Lall
 Date started: 2013-11-16
 ***/

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <gzstream.h>
#include <stdlib.h>

using namespace std;

/*** Functions for converting int to string ***/
string intToString(int a) {
	stringstream ss;
	ss << a;
	return ss.str();
}

string intToStringPad(int a, int length) {
	stringstream ss;
	ss << a;
	string s = ss.str();
	while (s.length() < length)
		s = "0" + s;

	return s;
}

/*** Functions for string tokenizer ***/
// from: http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

vector<string> split(const string &s, char delim) {
	vector < string > elems;
	split(s, delim, elems);
	return elems;
}

int main() {
	string base_path("/q/gp04/dpi/SQM2");
	string regions[] = { "NC/ILNWI", "NC/MININ", "NC/NSNMI", "NC/OHWPA",
			"NE/NWEND", "NE/NYCNJ", "NE/PANNJ", "NE/UPSNY", "NE/VAWVA",
			"NE/WADCM", "SC/ARNOK", "SC/MONKS", "SC/NTHTX", "SC/STHTX",
			"SE/ALMSL", "SE/GEORG", "SE/NCNSC", "SE/NTHFL", "SE/PRTRC",
			"SE/STHFL", "SE/TNNKY", "UK/AKRUK", "UK/ARLUK", "UK/ATCUK",
			"UK/BTCUK", "UK/BWYUK", "UK/CNCUK", "UK/STCUK", "UK/VNNUK",
			"UK/VTCUK", "WS/AZNMN", "WS/COUTW", "WS/LASCA", "WS/SDLVH",
			"WS/SFNSA", "WS/SORID" };
	int num_regions = 36;

	int day = 1, month = 1, year = 2014;

	// we will perform reservoir sample of all lines
	int reservoirSize = 100, i = 1, j;

	for (int region_number = 0; region_number < num_regions; ++region_number) {
		i = 1;
		vector < string > reservoir(reservoirSize, "");
		for (int hr = 0; hr < 24; ++hr) {
			string region = regions[region_number];
			string super_region = region.substr(0, 2);
			string sub_region = region.substr(3);

			string hour = intToStringPad(hr, 2);

			struct tm epoch;
			epoch.tm_sec = 0;
			epoch.tm_min = 0;
			epoch.tm_hour = hr;
			epoch.tm_mday = day;
			epoch.tm_mon = month - 1;
			epoch.tm_year = year - 1900;
			string epoch_time = intToString(mktime(&epoch));

			cout << region << " " << hour << " " << epoch_time << endl;

			string filename = base_path + "/" + region + "/FULL/"
					+ intToStringPad(year, 4) + "/" + intToStringPad(month, 2)
					+ "/" + intToStringPad(day, 2) + "/RNC." + super_region
					+ "." + sub_region + ".PA." + epoch_time + ".dat.gz";
			//cout << filename << endl;


			igzstream in(filename.c_str());
			string line;
			while (getline(in, line)) // in case file is empty
			{
				j = rand() % i;
				i++;
				if (j < reservoirSize) {
					vector < string > tokens = split(line.c_str(), '|');
					//cout << line << endl;
					//cout << tokens[0] << " " << tokens[1] << " " << tokens[10] << " " << tokens[17] << " " << tokens[18] << " " << tokens[21] << " " << tokens[22] << endl;
					//sample_file <<  tokens[0] << "|" << tokens[1] << "|" << tokens[10] << "|" << tokens[17] << "|" << tokens[18] << endl;
					reservoir[j] = line + "\n"; //tokens[0] + "|" + tokens[1] + "|" + tokens[10] + "|" + tokens[17] + "|" + tokens[18] + "\n";
				}
			}
		}

		// write the final reservoir to a file
		ofstream sample_file;
		string sample_filename = intToString(region_number + 1) + "-"
				+ intToStringPad(year, 4) + "-" + intToStringPad(month, 2)
				+ "-" + intToStringPad(day, 2) + "-sample.txt";
		sample_file.open(sample_filename.c_str());
		for (int i = 0; i < reservoirSize; ++i)
			sample_file << reservoir[i];
		sample_file.close();
	}

	return 0;
}
