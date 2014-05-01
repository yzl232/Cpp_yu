
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <gzstream.h>
#include "ams.h"
#include "prng.h"
#include <ctime>
using namespace std;

/*** Functions for converting int to string ***/
string intToString(int a) {
	stringstream ss;
	ss << a;
	return ss.str();
}

string intToStringPad(int a, size_t length) {
	stringstream ss;
	ss << a;
	string s = ss.str();
	while (s.length() < length)
		s = "0" + s;

	return s;
}



string getWeekDay(int day, int month, int year){
	string weekdays[7] = { "Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri",
			"Satur" };

	std::tm time_in = { 0, 0, 0, // second, minute, hour
			day - 0, month - 1, year - 1900 }; // 1-based day, 0-based month, year since 1900

	std::time_t time_temp = std::mktime(&time_in);

	// the return value from localtime is a static global - do not call
	// this function from more than one thread!
	std::tm const *time_out = std::localtime(&time_temp);

	//std::cout << "I was born on (Sunday = 0) D.O.W. " << time_out->tm_wday << '\n';
	int weekDayNum = time_out->tm_wday;
	return weekdays[weekDayNum];
	
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

/*** Functions from Zhenglin ***/
int F0_precompute(int input, prng_type *prng) {
	long random1;
	random1 = (abs(prng_int(prng))) % 2 * 2 - 1;
	return (1 * random1);
}
long double F1_precompute(long input, prng_type *prng) {
	long random1;
	long double result = 1.00;
	random1 = (abs(prng_int(prng))) % 2 * 2 - 1;

	result = result * sqrt((long double) input) * random1;
	return result;
}

long double compute_intersect(long double a_b, long double a, long double b) {
	return (a_b - a - b) / 2;
}
long hash_S_LL(string s) { // string  hashed to a long integer
	long hash = 5381;
	int c;
	char *str = const_cast<char*> (s.c_str());

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

int main(int argc, char** argv) {

	for (int running_time = 0; running_time < 1; running_time++) {

		ofstream pFile;
		pFile.open(
				"/export/qhome/zyu/intersect/zhenglin/data/src/Four_8_experiments/ratio_five_percent_results_512.txt.txt",
				ios::app);

		if (pFile != NULL) {
			//  fputs ("fopen example\n",pFile);
			pFile << "Start running\n";
			int test1 = 1;
			int test2 = 2;

			pFile << "day test " + intToStringPad(test1, 4)
					+ " combination test  " + intToStringPad(test2, 4) + " "
					+ "anomolous \n";

			pFile.close();
			pFile.open(
					"/export/qhome/zyu/intersect/zhenglin/data/src/Four_8_experiments/ratio_five_percent_results_512.txt.txt",
					ios::app);

		}

		string base_path("/q/gp04/dpi/SQM2");
		string regions[] = { "NC/ILNWI", "NC/MININ", "NC/NSNMI", "NC/OHWPA",
				"NE/NWEND", "NE/NYCNJ", "NE/PANNJ", "NE/UPSNY", "NE/VAWVA",
				"NE/WADCM", "SC/ARNOK", "SC/MONKS", "SC/NTHTX", "SC/STHTX",
				"SE/ALMSL", "SE/GEORG", "SE/NCNSC", "SE/NTHFL", "SE/PRTRC",
				"SE/STHFL", "SE/TNNKY", "UK/AKRUK", "UK/ARLUK", "UK/ATCUK",
				"UK/BTCUK", "UK/BWYUK", "UK/CNCUK", "UK/STCUK", "UK/VNNUK",
				"UK/VTCUK", "WS/AZNMN", "WS/COUTW", "WS/LASCA", "WS/SDLVH",
				"WS/SFNSA", "WS/SORID" };
		int num_regions = 1; //36;
		prng_type *prng;
		prng = prng_Init(-time(NULL), 2);

		int day = 1, month = 1, year = 2014;

		int buckets = 512, depth = 1, s1 = 1;

	    string weekdays[7] = { "Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri",
	    			"Satur" };
	    string weekDay;
	    int weekDayNum;

		if (argc > 1)
			buckets = atoi(argv[1]);
		if (argc > 2)
			s1 = atoi(argv[2]);
		if (argc > 3)
			depth = atoi(argv[3]);

		pFile << "buckets = " << buckets << ", s1 = " << s1 << ", depth = "
				<< depth << endl;

		int numPairs = 147;

		vector<AMS_type *> sketch1F0(numPairs);
		vector<AMS_type *> sketch2F0(numPairs);
		vector<AMS_type *> sketch1F1(numPairs);
		vector<AMS_type *> sketch2F1(numPairs);

		double rel_err_F0 = 0.0, rel_err_F1 = 0.0, rel_err_Avg = 0.0;
		for (int reg = 0; reg < num_regions; ++reg) {
			// read in the pairs that we wish to sample
			// RNC(1)/service(17) pairs and item_ID(10)/domain pairs(18)
			vector<string> pair1, pair2;
			string
					sample_filename =
							"/export/qhome/zyu/intersect/zhenglin/data/src/Four_8_experiments/test_i_R_five_percent.txt";
			string line;
			ifstream sample_file(sample_filename.c_str());
			while (getline(sample_file, line)) {
				vector < string > tokens = split(line.c_str(), '|');
				pair1.push_back(tokens[4] + "|" + tokens[5] + "|" + tokens[6] + "|" + tokens[7]); // RNC and service
				pair2.push_back(tokens[0] + "|" + tokens[1] + "|" + tokens[2] + "|" + tokens[3]); // item_ID and domain
			}
			sample_file.close();

			vector<long long> realF0(numPairs, 0);
			vector<long long> realF1(numPairs, 0);

			for (int i = 0; i < numPairs; ++i) {
				sketch1F0[i] = AMS_Init(buckets, depth, s1);
				sketch2F0[i] = AMS_Init(buckets, depth, s1);
				sketch1F1[i] = AMS_Init(buckets, depth, s1);
				sketch2F1[i] = AMS_Init(buckets, depth, s1);
			}

			vector<long long> count1(numPairs, 0);
			vector<long long> count2(numPairs, 0);
			vector<long long> countBoth(numPairs, 0);

			string region = regions[reg];
			string super_region = region.substr(0, 2);
			string sub_region = region.substr(3);
			for (day = 1; day < 2; ++day) {
				for (int hr = 0; hr < 1; ++hr) {
					string hour = intToStringPad(hr, 2);
					struct tm epoch;
					epoch.tm_sec = 0;
					epoch.tm_min = 0;
					epoch.tm_hour = hr;
					epoch.tm_mday = day;
					epoch.tm_mon = month - 1;
					epoch.tm_year = year - 1900;
					string epoch_time = intToString(mktime(&epoch));
					
					
				    weekDay = getWeekDay(day, month ,year);

					string filename = base_path + "/" + region + "/FULL/"
							+ intToStringPad(year, 4) + "/" + intToStringPad(
							month, 2) + "/" + intToStringPad(day, 2) + "/RNC."
							+ super_region + "." + sub_region + ".PA."
							+ epoch_time + ".dat.gz";

					igzstream in(filename.c_str());
					string line;
					while (getline(in, line)) {
						vector < string > tokens = split(line.c_str(), '|');
						long long count = atoi(tokens[21].c_str());
						long long value = atoi(tokens[22].c_str());
						if (value < 0)
							value = -value; // prevents overflow but still gives a reasonable update
						long double v_F0 = F1_precompute(count, prng); // we need to total these values, so we use F1
						long double v_F1 = F1_precompute(value, prng);

						//pFile << count << " " << v_F0 << ", " << value << "  " << v_F1 << endl;
						//if (value < 0) { pFile << tokens[21].c_str() << "|" << tokens[22].c_str() << endl; exit(1); }

						for (int i = 0; i < numPairs; ++i) {
							bool inFirst = ( tokens[1] + "|" + tokens[17]  + "|" +   tokens[13]  + "|" + weekDay 	== pair1[i]);
							bool inSecond = (tokens[10] + "|" + tokens[18]   + "|" +   tokens[15]  + "|" + hour   == pair2[i]);
							long long flowID = hash_S_LL(line); // NOTE: we hash the entire descriptor (not just the flow ID)


							// code for keeping track of true intersection values
							if (inFirst && inSecond) {
								realF0[i] += count;
								realF1[i] += value;
								countBoth[i] += 1;
							}

							// code for inserting into first sketch
							if (inFirst) {
								// insert into first sketch
								AMS_Update(sketch1F0[i], flowID, v_F0);
								AMS_Update(sketch1F1[i], flowID, v_F1);
								count1[i] += 1;
							}

							// code for inserting into second sketch
							if (inSecond) {
								// insert into second sketch
								AMS_Update(sketch2F0[i], flowID, v_F0);
								AMS_Update(sketch2F1[i], flowID, v_F1);
								count2[i] += 1;
							}
						}
					}
				}
			}
			long double sumRelErrorF0 = 0.0, sumRelErrorF1 = 0.0,
					sumRelErrorAvg = 0.0;
			for (int i = 0; i < numPairs; ++i) {
				// initialize sum sketches
				AMS_type *sketchSumF0, *sketchSumF1;
				sketchSumF0 = AMS_Init(buckets, depth, s1);
				sketchSumF1 = AMS_Init(buckets, depth, s1);

				// add on both sketches
				AMS_AddOn(sketchSumF0, sketch1F0[i]);
				AMS_AddOn(sketchSumF0, sketch2F0[i]);
				AMS_AddOn(sketchSumF1, sketch1F1[i]);
				AMS_AddOn(sketchSumF1, sketch2F1[i]);

				// calculate estimated values
				long double aF0 = AMS_F2Est(sketch1F0[i]);
				long double bF0 = AMS_F2Est(sketch2F0[i]);
				long double abF0 = AMS_F2Est(sketchSumF0);
				//pFile << abF0 << " " << aF0 << " " << bF0 << endl;
				long double estF0 = compute_intersect(abF0, aF0, bF0);
				long double aF1 = AMS_F2Est(sketch1F1[i]);
				long double bF1 = AMS_F2Est(sketch2F1[i]);
				long double abF1 = AMS_F2Est(sketchSumF1);
				//pFile << abF1 << " " << aF1 << " " << bF1 << endl;
				long double estF1 = compute_intersect(abF1, aF1, bF1);

				//pFile << pair1[i] << "|" << pair2[i] << endl;
				//pFile << realF0[i] << " " << realF1[i] << endl;
				//pFile << estF0 << " " << estF1 << endl;
				//pFile << endl;

				pFile << "abF0 " << abF0 << " " << "aF0 " << aF0 << " "
						<< "bF0 " << bF0 << endl;

				pFile << "abF1 " << abF1 << " " << "aF1 " << aF1 << " "
						<< "bF1 " << bF1 << endl;
				//long double estF1 = compute_intersect(abF1, aF1, bF1);
				pFile << "pair1[i] " << pair1[i] << "***" << "pair2[i] "
						<< pair2[i] << endl;
				pFile << "realF0[i] " << realF0[i] << " " << "realF1 "
						<< realF1[i] << endl;
				pFile << "estF0 " << estF0 << " " << "estF1 " << estF1 << endl;
				

				sumRelErrorF0 += fabs(realF0[i] - estF0) / realF0[i];
				sumRelErrorF1 += fabs(realF1[i] - estF1) / realF1[i];
				double realAvg = ((double) realF1[i]) / realF0[i];
				sumRelErrorAvg += fabs(realAvg - estF1 / estF0) / realAvg;

				/* code for plotting error vs intersection rate */
				double ir = 0.0;
				if (count1[i] > count2[i])
					ir = ((double) countBoth[i]) / count1[i];
				else
					ir = ((double) countBoth[i]) / count2[i];

				pFile << "intersection ratio: " << ir << "\t" << "errors F0: "
						<< fabs(realF0[i] - estF0) / realF0[i] << "\t"
						<< "errors F1 " << fabs(realF1[i] - estF1) / realF1[i]
						<< "\t" << "errors F1/F0 " << fabs(realAvg - estF1
						/ estF0) / realAvg << endl;
				pFile << endl;

				// pFile << ir << "\t" << fabs(realF0[i] - estF0)/realF0[i] << "\t" << fabs(realF1[i] - estF1)/realF1[i] << "\t" << fabs(realAvg - estF1/estF0)/realAvg << endl;
				/* end code for plotting error vs intersection rate */

				//pFile << fabs(realF0[i] - estF0)/realF0[i] << " " << fabs(realF1[i] - estF1)/realF1[i] << " " << fabs(realAvg - estF1/estF0)/realAvg << endl;

				// free memory for summed sketches

				pFile.close();
				pFile.open(
						"/export/qhome/zyu/intersect/zhenglin/data/src/Four_8_experiments/ratio_five_percent_results_512.txt.txt",
						ios::app);
				AMS_Destroy(sketchSumF0);
				AMS_Destroy(sketchSumF1);
			}

			for (int i = 0; i < numPairs; ++i)
				pFile << count1[i] << "/" << count2[i] << "/" << countBoth[i]
						<< "\t";
			pFile << endl;

			pFile << "Average Relative Error F0 : " << sumRelErrorF0 / numPairs
					<< endl;
			pFile << "Average Relative Error F1 : " << sumRelErrorF1 / numPairs
					<< endl;
			pFile << "Average Relative Error Avg: " << sumRelErrorAvg
					/ numPairs << endl;

			rel_err_F0 += sumRelErrorF0 / numPairs;
			rel_err_F1 += sumRelErrorF1 / numPairs;
			rel_err_Avg += sumRelErrorAvg / numPairs;

			for (int i = 0; i < numPairs; ++i) {
				AMS_Destroy(sketch1F0[i]);
				AMS_Destroy(sketch1F1[i]);
				AMS_Destroy(sketch2F0[i]);
				AMS_Destroy(sketch2F1[i]);
			}
		}

		pFile << rel_err_F0 / num_regions << "\t" << rel_err_F1 / num_regions
				<< "\t" << rel_err_Avg / num_regions << endl;
		pFile.close();
		prng_Destroy(prng);

	}

	return 0;
}
