#include <iostream>
#include <vector>
#include <ctime>

using std::cout;
using std::endl;
using std::cerr;
using std::vector;

struct Node {
	int key;
	int priority;
	Node* left;
	Node* right;
	int size;
	
	Node() : left(NULL), right(NULL), size(1) {}
	Node(int k, int p) : key(k), priority(p), left(NULL), right(NULL), size(1) {}
	Node(int k, int p, Node* l, Node* r) : key(k), priority(p), left(l), right(r) {
		size = 1;
		size += l == NULL ? 0 : l->size;
		size += r == NULL ? 0 : r->size;
	}
};

int get_size(Node* node) {
	return node == NULL ? 0 : node->size;
}

void Print(Node* root) {
	if (root != NULL && root->left != NULL) {
		Print(root->left);
	}
	if (root != NULL) {
		cout<<root->key<<" "<<root->priority<<" "<<root->size<<" ";
		if (root->left != NULL) {
			cout<<root->left->key<<" ";
		} else {
			cout<<"no left ";
		}
		if (root->right != NULL) {
			cout<<root->right->key<<" ";
		} else {
			cout<<"no right ";
		}
		cout<<endl;
	}
	if (root != NULL && root->right != NULL) {
		Print(root->right);
	}
}

Node* Merge(Node* l, Node* r) {
	if (l == NULL) {
		return r;
	}
	if (r == NULL) {
		return l;
	}
	if (l->priority > r->priority) {
		l->right = Merge(l->right, r);
		
		l->size = 1 + get_size(l->left) + get_size(l->right);
		return l;
	} else {
		r->left = Merge(l, r->left);
		
		r->size = 1 + get_size(r->left) + get_size(r->right);
		return r;
	}
}

void Split(Node* t, int key, Node** l, Node** r) {
	if (t == NULL) {
		*l = NULL;
		*r = NULL;
		return;
	}
	if (t->key <= key) {
		*l = t;
		Split(t->right, key, &((*l)->right), r);
		
		(*l)->size = 1 + get_size((*l)->left) + get_size((*l)->right);
	} else {
		*r = t;
		Split(t->left, key, l, &((*r)->left));

		(*r)->size = 1 + get_size((*r)->left) + get_size((*r)->right);
	}
}

void Insert(Node* x, Node** root) {
	if (*root == NULL) {
		*root = x;
		return;
	}
	Node* l = NULL;
	Node* r = NULL;

	Split(*root, x->key, &l, &r);
	*root = Merge(l, x);
	*root = Merge(*root, r);
}

void Delete(int key, Node** root) {
	Node* l1 = NULL;
	Node* r1 = NULL;
	Node* l2 = NULL;
	Node* r2 = NULL;

	Split(*root, key - 1, &l1, &r1);
	Split(r1, key, &l2, &r2);
	*root = Merge(l1, r2);
}

Node* Build_Tree(const vector<int>& mas) {
	srand(time(NULL));

	int size = static_cast<int>(mas.size());
	
	if (mas.empty()) {
		return NULL;
	}

	Node* root = NULL;

	for (int i = 0; i < size; ++i) {
		Node* x = new Node(mas[i], rand());
		Insert(x, &root);
	}

	return root;
}

int Find_k_order_statistic(int k, Node* root) {
	Node* node = root;

	if (node == NULL) {
		cerr<<"Find_k_order_statistic: bad tree"<<endl;
		exit(1);
	}

	while (!(node->left == NULL && k == 1 || node->left != NULL && node->left->size + 1 == k)) {
		if (node->left != NULL && k <= node->left->size) {
			node = node->left;
		} else {
			if (node->left != NULL) {
				k -= (node->left->size + 1);
				node = node->right;
			} else {
				--k;
				node = node->right;
			}
		}
	}
	
	return node->key;
}