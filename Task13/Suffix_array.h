#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using std::vector;
using std::string;
using std::sort;
using std::min;

class firstComparator {
private:
	string s;
	int size;
public:
	firstComparator() {}

	firstComparator(const string& _s) : s(_s), size(static_cast<int>(s.size())) {}

	bool operator() (int i, int j) {
		return s.at(i) < s.at(j);
	}
};

class secondComparator {
private:
	vector<int> classes;
	int size;
	int stepInPow;
public:
	secondComparator() {}

	secondComparator(vector<int>& _classes, int _size, int _stepInPow) : classes(_classes), size(_size), stepInPow(_stepInPow - 1) {}

	bool operator() (int i, int j) {
		if (classes[i] != classes[j]) {
			return classes[i] < classes[j];
		}
		if (i + stepInPow < size && j + stepInPow < size) {
			return classes[i + stepInPow] < classes[j + stepInPow];
		}
		if (i + stepInPow < size && j + stepInPow >= size) {
			return false;
		}
		return true;
	}
};

int getLog(int n) {
	if (n <= 1) {
		return 0;
	}
	int LOG = 1;
	int pow = 2; 
	while (pow <= n) {
		++LOG;
		pow *= 2;
	}
	return LOG;
}

void sortByOneSymbol(vector<int>& suffixs, const string& s, vector<int>& classes) {
	int size = static_cast<int>(s.size());
	firstComparator firstCmp = firstComparator(s);
	sort(suffixs.begin(), suffixs.end(), firstCmp);

	classes[suffixs[0]] = 0;
	for (int i = 1; i < size; ++i) {
		if (s[suffixs[i]] != s[suffixs[i - 1]]) {
			classes[suffixs[i]] = classes[suffixs[i - 1]] + 1;
		} else {
			classes[suffixs[i]] = classes[suffixs[i - 1]];
		}
	}
}

void sortBySeveralSymbols(vector<int>& suffixs, const string& s, vector<int>& classes) {
	int size = static_cast<int>(s.size());
	int LOG = getLog(size);
	int stepInPow = 2;
	bool next = true;
	
	for (int i = 1; i <= LOG && next; ++i) {
		secondComparator secondCmp = secondComparator(classes, size, stepInPow);
		sort(suffixs.begin(), suffixs.end(), secondCmp);

		next = false;
		classes[suffixs[0]] = 0;
		for (int j = 1; j < size; ++j) {
			if (suffixs[j] + stepInPow >= size || suffixs[j - 1] + stepInPow >= size) {
				classes[suffixs[j]] = classes[suffixs[j - 1]] + 1;
				continue;
			}
			if (classes[suffixs[j - 1] + stepInPow] != classes[suffixs[j] + stepInPow]) {
				classes[suffixs[j]] = classes[suffixs[j - 1]] + 1;
			} else {
				classes[suffixs[j]] = classes[suffixs[j - 1]];
				next = true;
			}
		}

		stepInPow *= 2;
	}
}

void buildSuffixArray(vector<int>& suffixs, const string& s) {
	int size = static_cast<int>(s.size());
	suffixs.resize(size);
	for (int i = 0; i < size; ++i) {
		suffixs[i] = i;
	}
	vector<int> classes(size);
	sortByOneSymbol(suffixs, s, classes);
	sortBySeveralSymbols(suffixs, s, classes);
}

int findCommonPrefix(const vector<int>& suffixs, const string& s, const string& substr, int pos, int from) {
	int size = static_cast<int>(s.size());
	int substrSize = static_cast<int>(substr.size());
	int MIN = min(size - pos, substrSize);

	for (int i = from; i < MIN; ++i) {
		if (s[pos + i] != substr[i]) {
			return i;
		}
	}
	return MIN;
}

void findSubstr(const vector<int>& suffixs, vector<bool>& positions, const string& s, const string& substr,
				int left, int right, int leftDepth, int rightDepth) {
	int mid = (left + right) / 2;
	int size = static_cast<int>(s.size());
	int substrSize = static_cast<int>(substr.size());
	int from = min(leftDepth, rightDepth);
	int midDepth = findCommonPrefix(suffixs, s, substr, suffixs[mid], from);

	if (left == right) {
		if (leftDepth == substrSize) { // substr является префиксом какого-то суффикса s
			positions[suffixs[left]] = true;
		}
		return;
	}
	
	if (left + 1 == right) {
		if (leftDepth == substrSize) {
			positions[suffixs[left]] = true;
		}
		if (rightDepth == substrSize) {
			positions[suffixs[right]] = true;
		}
		return;
	}

	if (midDepth == size - suffixs[mid] && midDepth == substrSize) { // substr полностью совпала с каким-то суффиксом s 
		positions[suffixs[mid]] = true;
		// запускаем поиск substr в правую часть suffixs с позиции mid
		findSubstr(suffixs, positions, s, substr, mid, right, midDepth, rightDepth);
		return;
	}

	if (midDepth < size - suffixs[mid] && midDepth == substrSize) { // substr является префиксом какого-то суффикса s
		positions[suffixs[mid]] = true;
		// запускаем поиск substr и в левую, и в правую части suffixs с позиции mid
		findSubstr(suffixs, positions, s, substr, left, mid, leftDepth, midDepth);
		findSubstr(suffixs, positions, s, substr, mid, right, midDepth, rightDepth);
		return;
	}

	if (midDepth == size - suffixs[mid] && midDepth < substrSize) {
		// запускаем поиск substr в правую часть suffixs с позиции mid
		findSubstr(suffixs, positions, s, substr, mid, right, midDepth, rightDepth);
		return;
	}

	// midDepth < size - suffixs[mid] && midDepth < substrSize
	if (s[suffixs[mid] + midDepth] > substr[midDepth]) {
		findSubstr(suffixs, positions, s, substr, left, mid, leftDepth, midDepth);
		// запускаем поиск substr в левую часть suffixs с позиции mid
	} else {
		// запускаем поиск substr в правую часть suffixs с позиции mid
		findSubstr(suffixs, positions, s, substr, mid, right, midDepth, rightDepth);
	}
}