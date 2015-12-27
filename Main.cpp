#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <set>
#include <time.h>

struct wordStats {
	std::string firstWord;
	std::string secondWord;
	int count;
};

struct wordCount {
	std::string word;
	int count;
};

std::string textPath = "text.txt";
std::string textContent;

std::vector<wordStats> wStats;
std::vector<wordCount> wCount;
std::vector<std::string> words;

std::string outputString;

void error(std::string msg) {
	std::cout << msg << std::endl << "Press any key to exit the program" << std::endl;
	_getch();
	exit(0);
}

std::vector<std::string> splitString(std::string str) {
	std::vector<std::string> arr;
	arr.push_back("");

	int index = 0;
	int elem = 0;
	for (unsigned i = 0; i < str.length(); i++) {
		if (str[i] == ' ') {
			arr.push_back("");
			index++;
			elem = 0;
			continue;
		}
		
		arr[index].resize(elem + 1);
		arr[index][elem] = str[i];

		elem++;
	}

	return arr;
}

std::vector<std::string> removeEmpty(std::vector<std::string> arr) {
	arr.erase(std::remove_if(
		arr.begin(),
		arr.end(),
		[](const std::string& s) {
			return s == "";
		}),
		arr.end()
	);
	return arr;
}

void getWordCount() {
	// get unique
	std::set<std::string> uniqueWords;
	for (unsigned i = 0; i < words.size(); i++) {
		// convert to lowercase
		std::transform(words[i].begin(), words[i].end(), words[i].begin(), tolower);
		uniqueWords.insert(words[i]);
	}
	// copy into wordcount vector
	std::for_each(
		uniqueWords.begin(),
		uniqueWords.end(),
		[](const std::string& s) {
			// get number of occurances
		int occurances = std::count(words.begin(), words.end(), s);
			wCount.push_back({ s, occurances });
		}

	);
}

void getWordStats() {
	// create new vector of string with 2 consecutive words
	std::vector<std::string> twoWords;
	for (unsigned i = 0; i < words.size() - 1; i++) {
		twoWords.push_back(words[i] + " " + words[i + 1]);
	}
	// get unique
	std::set<std::string> uniqueTwoWords;
	for (unsigned i = 0; i < twoWords.size(); i++) {
		uniqueTwoWords.insert(twoWords[i]);
	}
	// for each in unique set, get count
	std::for_each(
		uniqueTwoWords.begin(),
		uniqueTwoWords.end(),
		[&twoWords](const std::string& s) {
		int occurances = std::count(twoWords.begin(), twoWords.end(), s);
		std::vector<std::string> split = splitString(s);
		wStats.push_back({ split[0], split[1], occurances });
	}
	);
}

std::string selectWord(std::vector<wordStats> possibilities) {
	std::vector<std::string> all;

	for (unsigned i = 0; i < possibilities.size(); i++) {
		// enter into array as many times as 'count' from wStats
		for (unsigned ntimes = 0; ntimes < possibilities[i].count; ntimes++) {
			all.push_back(possibilities[i].secondWord);
		}
	}

	// get random between 0 and size of all array
	return all[rand() % (all.size())];
}

int main() {
	srand(time(NULL));

	std::ifstream textFile(textPath); // open file
	if (!textFile.is_open()) {
		error("File couldn't be opened");
	}
	// get content
	std::string tmp;
	while (std::getline(textFile, tmp)) {
		textContent += tmp + "\n";
	}
	tmp.clear();
	// split into array
	words = splitString(textContent);
	// remove empty elements
	words = removeEmpty(words);
	// populate word counts
	getWordCount();
	// populate word stats
	getWordStats();

	while (true) {
		// GENERATION STARTS HERE//
		int wordCount = 0;
		std::string lastWord;
		std::cin >> lastWord;

		if (lastWord == "%RANDOM%") {
			// get random word to start, more frequent words have higher chance to start
			lastWord = words[rand() % words.size()];
		}

		std::string outputString = lastWord + " ";
		// get random word to start, more frequent words have higher chance to start
		/*std::string lastWord = words[rand() % words.size()];*/
		wordCount++;

		while (wordCount < 15 || lastWord.find_first_of('\n') == std::string::npos) {
			// get every entry in wStats where firstword == our starting word
			auto pred = [lastWord](wordStats& ws) {
				return ws.firstWord == lastWord;
			};
			// go through results
			std::vector<wordStats> results;
			for (auto it = wStats.begin(); (it = std::find_if(it, wStats.end(), pred)) != wStats.end(); it++) {
				int index = std::distance(wStats.begin(), it);
				results.push_back({ wStats[index].firstWord, wStats[index].secondWord, wStats[index].count });
			}
			if (results.size() == 0) {
				break;
			}
			// pick word to use
			lastWord = selectWord(results);
			outputString += lastWord + " ";
			wordCount++;
		}
		outputString = outputString.substr(0, outputString.find_last_of('\n')); // remove text after last newline
		std::cout << outputString << std::endl << std::endl;
	}

	_getch();
	return 0;
}