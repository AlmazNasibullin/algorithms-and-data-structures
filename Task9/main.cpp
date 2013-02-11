#include "treap.h"
#include <string>

using std::cin;
using std::string;

void Handler(int size, Node* root, vector<int>& mas) {
	string in;
	bool contin = true;
	while (contin) {
		cin>>in;
		if (in == "get") {
			int k;
			cin>>k;
			if (k <= 0 || k > size) {
				cerr<<"Bad index"<<endl;
				exit(1);
			}
			cout<<Find_k_order_statistic(k, root)<<endl;
		} else if (in == "update") {
			int k;
			int val;
			cin>>k>>val;
			if (k <= 0 || k > size) {
				cerr<<"Bad index"<<endl;
				exit(1);
			}

			int key = mas[k - 1];
			Delete(key, &root);

			for (int i = 0; i < size; ++i) { // случай, когда изменяемый ключ не единственный в массиве
				if (i != k - 1 && mas[i] == key) {
					Node* y = new Node(key, rand());
					Insert(y, &root);
				}
			}

			mas[k - 1] = val;
			Node* x = new Node(val, rand());
			Insert(x, &root);
		} else if (in == "exit") {
			contin = false;
		} else {
			cerr<<"Bad comand"<<endl;
			exit(1);
		}
	}
}

int main() {
	int size;
	cin>>size;
	vector<int> mas(size);

	for (int i = 0; i < size; ++i) {
		cin>>mas[i];
	}

	Node* tree = Build_Tree(mas);
	if (!tree) {
		cout<<"Empty array"<<endl;
		return 0;
	}

	Handler(size, tree, mas);
	
	return 0;
}