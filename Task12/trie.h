#include <map>
#include <string>
#include <vector>
#include <deque>
#include <set>

using std::map;
using std::make_pair;
using std::string;
using std::vector;
using std::deque;
using std::set;

class Node {
private:
	Node* parent;
	map<char, Node*> childs;
	int number;
	Node* SS; // суффиксная ссылка
	Node* out; // связь выхода
	char letter;
	// int depth; 

public:
	Node(Node* _parent) : parent(_parent), number(-1), SS(NULL), out(NULL) {}
	
	Node() : parent(NULL), number(-1), SS(NULL), out(NULL) {}

	// либо создает потомка по данной букве и его возвращает, либо возвращает уже имеющегося потомка
	Node* makeAndGetChild(char c) {
		map<char, Node*>::iterator iter = childs.find(c);
		if (iter != childs.end()) {
			return iter->second;
		}
		Node* child = new Node(this);
		// child->setDepth(depth + 1);
		childs.insert(make_pair(c, child));
		return child;
	}

	Node* getChild(char c) {
		map<char, Node*>::iterator iter = childs.find(c);
		if (iter != childs.end()) {
			return iter->second;
		}
		return NULL;
	}

	map<char, Node*>& getChilds() {
		return childs;
	}

	Node* getParent() {
		return parent;
	}

	Node* getSS() {
		return SS;
	}

	void setSS(Node* _SS) {
		SS = _SS;
	}
	
	bool iSLeaf() {
		return number != -1;
	}

	char getLetter() {
		return letter;
	}

	void setLetter(char c) {
		letter = c;
	}

	int getNumber() {
		return number;
	}

	void setNumber(int _number) {
		number = _number;
	}

	Node* getOut() {
		return out;
	}

	void setOut(Node* _out) {
		out = _out;
	}

	/*
	int getDepth() {
		return depth;
	}

	void setDepth(int _depth) {
		depth = _depth;
	}
	*/
};

Node* buildTrie(const vector<string>& words) {
	Node* root = new Node();
	// root->setDepth(0);
	
	for (size_t i = 0; i < words.size(); ++i) {
		Node* cur = root;
		for (size_t j = 0; j < words[i].size(); ++j) {
			cur = cur->makeAndGetChild(words[i].at(j));
			cur->setLetter(words[i].at(j));
		}
		cur->setNumber(static_cast<int>(i));
	}

	deque<Node*> d;
	d.push_back(root);

	while(!d.empty()) {
		Node* front = d.front();
		d.pop_front();
		front->setSS(root);

		map<char, Node*> childs = front->getChilds();
		for (map<char, Node*>::iterator iter = childs.begin(); iter != childs.end(); ++iter) {
			d.push_back(iter->second);
		}
		
		if (front == root || front->getParent() == root) {
			continue;
		}

		Node* SS = front->getParent()->getSS();
		char letter = front->getLetter();

		while (SS != root) {
			Node* childSS = SS->getChild(letter);
			if (childSS != NULL) {
				front->setSS(childSS);
				break;
			}
			SS = SS->getSS();
		}

		if (SS == root) {
			Node* childSS = SS->getChild(letter);
			if (childSS != NULL) {
				front->setSS(childSS);
			}
		}

		if (front->getSS()->iSLeaf()) {
			front->setOut(front->getSS());
		} else {
			front->setOut(front->getSS()->getOut());
		}
	}

	return root;
}

void findSubstrings(Node* root, const string& s, set<int>& result) {
	int size = static_cast<int>(s.size());
	int i = 0;
	Node* node = root;
	result.clear();

	while (i < size) {
		Node* out = node->getOut();
		while (out != NULL) {
			result.insert(out->getNumber());
			out = out->getOut();
		}

		Node* child = node->getChild(s[i]);
		if (child != NULL) {
			node = child;
			++i;
		} else {
			if (node == root) {
				++i;
			}
			node = node->getSS();
		}
		
		if (node->iSLeaf()) {
			result.insert(node->getNumber());
		}
	}
}

/*
void printTrie(Node* root) {
	deque<Node*> d;
	d.push_back(root);

	while (!d.empty()) {
		Node* node = d.front();
		d.pop_front();
		cout<<"letter: "<<node->getLetter()<<"\tSS letter: "<<node->getSS()->getLetter()<<"\tSS depth: "<<node->getSS()->getDepth();
		if (node->iSLeaf()) {
			cout<<"\tleaf\t";
		} else {
			cout<<"\tnole\t";
		}
		if (node->getParent() != NULL) {
			cout<<"parent: "<<node->getParent()->getLetter()<<"\t";
		} else {
			cout<<"no parent\t";
		}
		if (node->getOut() == NULL) {
			cout<<"NULL"<<endl;
		} else {
			cout<<node->getOut()->getLetter()<<" "<<node->getOut()->getDepth()<<endl;
		}
		map<char, Node*> m = node->getChilds();
		for (map<char, Node*>::iterator iter = m.begin(); iter != m.end(); ++iter) {
			d.push_back(iter->second);
		}
	}
}
*/