#pragma once
#include <iostream>

//required for getting random numbers in createRandomNodes()
#include <stdlib.h>

//used in checkKeyInfoType()
#include <typeinfo>


template<typename Key, typename Info>
class Dictionary {

	struct Node {

		Node * right = nullptr;
		Node * left = nullptr;
		Node * parent = nullptr;
		
		Key key;
		Info info;
		
		short int balance = 0;

		Node(Key key, Info info) : key(key), info(info) {};
		Node(Key key, Info info, int balance) : key(key), info(info), balance(balance) {};
	};


	//the 'top' node of the tree
	Node * top = nullptr;


	/*-----------------------------------------
	*******************************************
	RECURSIVE METHODS (definitions at line 254)
	*******************************************
	-----------------------------------------*/


	void recDisplay(Node *, int);
	void recDestroy(Node *&);
	void recDeleteNode(Node *&, Key, Dictionary &);
	bool recInsert(Node *&, Key, Info);
	bool recContainsKey(Node *, Key);
	int recHeight(Node *);
	int recSize(Node *);
	
	//compare trees below each of node pointers
	bool compareTrees(Node*, Node*);
	
	//provides corect 'parent' pointer to every node
	void getParents(Node *&);

	//copies a tree below a given node
	Node * copyTree(Node *);
	
	//returns a pointer to node with a given key
	Node * find(Node *, Key);

	
	/*---------------------------------
	***********************************
	ROTATIONS (definitions at line 588)
	***********************************
	---------------------------------*/


	void L_rotation(Node *&);
	void R_rotation(Node *&);
	void RL_rotation(Node *&);
	void LR_rotation(Node *&);


	/*----------------------------
	******************************
	OTHER (definition at line 779)
	******************************
	----------------------------*/


	//checks if Key and Info are of type int
	//required in createRandomNodes(int) method
	bool checkKeyInfoType();

public:


	/*--------------------------------
	**********************************
	MEMBER METHODS (= operator at 755)
	**********************************
	--------------------------------*/


	//default constructor
	Dictionary() = default;

	//uses createRandomNodes(int) method
	Dictionary(int i) { this->createRandomNodes(i); }

	//copy constructor
	Dictionary(const Dictionary<Key, Info> &givenTree) { *this = givenTree; }

	//uses clear() method
	~Dictionary() { this->destroy(); }

	//operator =
	Dictionary& operator=(const Dictionary &);


	/*--------------------------------------
	****************************************
	PUBLIC METHODS (definitions at line 643)
	****************************************
	--------------------------------------*/


	//adds a node
	void insert(Key, Info);

	//dipslays whole tree
	void display();

	//displays node with given key
	void displayNode(Key);

	//deletes all nodes from the tree
	void destroy();

	//deletes node with given key
	void deleteNode(Key);

	//creates given number of random nodes
	void createRandomNodes(int);

	//checks if the tree contains a node with a given key
	bool containsKey(Key);

	//checks if tree is empty
	bool isEmpty();

	//returns height of the tree
	int height();

	//returns number of nodes in the tree
	int size();


	/*--------------------------------
	**********************************
	OPERATORS (definition at line 755)
	**********************************
	--------------------------------*/


	bool operator==(const Dictionary &);


	/*------------
	**************
	ITERATOR CLASS
	**************
	------------*/


	class Iterator {
		

		friend class Dictionary;
		mutable Node * iter = nullptr;


	public:


		Iterator() {}

		Iterator(Node * node) : iter(node) {}

		Iterator(const Iterator &given) : iter(given.iter) {}

		~Iterator() {}


		Key& getKey() { return this->iter->key; }
		Info& getInfo() { return this->iter->info; }


		Iterator& operator++() {

			if (this->iter->right != nullptr) {
				
				this->iter = this->iter->right;
				
				while (this->iter->left != nullptr) {
					this->iter = this->iter->left;
				}
			}
			else {
				
				Node * temp = this->iter->parent;

				while (temp != nullptr && this->iter == temp->right) {
					this->iter = temp;
					temp = temp->parent;
				}

				this->iter = temp;
			}

			return *this;
		}

		Iterator operator++(int) {
			Iterator temp(*this);
			++(*this);
			return temp;
		}

		//method created to check if the iterator works properly
		void display() {
			std::cout << "\nKey: " << this->iter->key << "\tInfo: " << this->iter->info;
		}

	};

	//constructs iterator pointing to bottom left
	Iterator begin() {
		
		if (this->top == nullptr) {
			return Iterator(this->top);
		}
		else {

			Node * temp = this->top;

			while (true) {
				
				if (temp->left != nullptr) {
					temp = temp->left;
				}
				else {
					return Iterator(temp);
				}
			}
		}
	}

};



/*-------
RECURSIVE
-------*/


template<typename Key, typename Info>
void Dictionary<Key, Info>::recDisplay(Node * root, int space) {

	if (root == nullptr) {
		return;
	}

	space += 10;

	this->recDisplay(root->right, space);

	std::cout << "\n";

	for (int i = 10; i < space; i++) {
		std::cout << " ";
	}
	std::cout << root->key << "\n";

	this->recDisplay(root->left, space);
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::recDestroy(Node * &root) {

	if (root == nullptr) {
		return;
	}
	else {
		this->recDestroy(root->right);
		this->recDestroy(root->left);
		delete root;
	}
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::recDeleteNode(Node * &root, Key key, Dictionary<Key, Info> &givenTree) {

	if (root == nullptr) {
		return;
	}
	else if (root->key == key) {
		givenTree.recDeleteNode(root->right, key, givenTree);
		givenTree.recDeleteNode(root->left, key, givenTree);
	}
	else if (root->key != key) {
		givenTree.insert(root->key, root->info);
		givenTree.recDeleteNode(root->right, key, givenTree);
		givenTree.recDeleteNode(root->left, key, givenTree);
	}
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::recInsert(Node * &root, Key key, Info info) {

	if (root == nullptr) {
		root = new Node(key, info, 0);
		return true;
	}

	if (root->key < key) {

		//if the node isn't inserted
		//for example because the key is already taken
		//the whole method returns false

		if (!this->recInsert(root->right, key, info)) {
			return false;
		}

		//the insertion is accomplished positively
		//balance must be changed
		//in the two following cases
		//rotation is not required

		if (root->balance == -1) {
			root->balance = 0;
			return false;
		}

		if (root->balance == 0) {
			root->balance = 1;
			return true;
		}

		//if the method didn't return yet,
		//the tree is not balanced
		//and needs rotations

		if (root->balance == 1) {

			Node * temp1 = root->right;

			if (temp1->balance == 1) {
				root->balance = 0;
				temp1->balance = 0;
				this->L_rotation(root);
				return false;
			}
			else if (temp1->balance == -1) {

				Node * temp2 = temp1->left;

				switch (temp2->balance) {

				case -1:
					root->balance = 0;
					temp1->balance = 1;
					break;

				case 0:
					root->balance = 0;
					temp1->balance = 0;
					break;

				case 1:
					root->balance = -1;
					temp1->balance = 0;
					break;

				default:
					break;
				}

				temp2->balance = 0;
				this->RL_rotation(root);
				return false;
			}
		}
	}

	if (root->key > key) {

		//if the node isn't inserted
		//for example because the key is already taken
		//the whole method returns false

		if (!this->recInsert(root->left, key, info)) {
			return false;
		}

		//the insertion is accomplished positively
		//balance must be changed
		//in the two following cases
		//rotation is not required

		if (root->balance == 1) {
			root->balance = 0;
			return false;
		}

		if (root->balance == 0) {
			root->balance = -1;
			return true;
		}

		//if the method didn't return yet,
		//the tree is not balanced
		//and needs rotations

		if (root->balance == -1) {

			Node * temp1 = root->left;

			if (temp1->balance == -1) {
				root->balance = 0;
				temp1->balance = 0;
				this->R_rotation(root);
				return false;
			}

			if (temp1->balance == 1) {

				Node * temp2 = temp1->right;

				switch (temp2->balance) {

				case -1:
					root->balance = 1;
					temp1->balance = 0;
					break;

				case 0:
					root->balance = 0;
					temp1->balance = 0;
					break;

				case 1:
					root->balance = 0;
					temp1->balance = -1;
					break;

				default:
					break;
				}

				temp2->balance = 0;
				this->LR_rotation(root);
				return false;
			}
		}
	}
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::recContainsKey(Node * root, Key key) {

	if (root == nullptr) {
		return false;
	}
	else if (root->key == key) {
		return true;
	}
	else if (root->key > key) {
		return this->recContainsKey(root->left, key);
	}
	else {
		return this->recContainsKey(root->right, key);
	}
}


template<typename Key, typename Info>
int Dictionary<Key, Info>::recHeight(Node * root) {

	if (root == nullptr) {
		return 0;
	}

	int right, left;

	right = this->height(root->right);
	left = this->height(root->left);

	return 1 + (left > right ? left : right);
}


template<typename Key, typename Info>
int Dictionary<Key, Info>::recSize(Node * root) {

	if (root == nullptr) {
		return 0;
	}

	return 1 + this->recSize(root->left) + this->recSize(root->right);
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::compareTrees(Node * lhs, Node * rhs) {

	if (lhs == rhs) {
		return true;
	}
	else if (rhs == nullptr || lhs == nullptr) {
		return false;
	}

	bool result = (lhs->key == rhs->key) && (lhs->info == rhs->info);

	return result && compareTrees(lhs->left, rhs->left) && compareTrees(lhs->right, rhs->right);
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::getParents(Node * &root) {

	//the case shouldn't happen
	if (root == nullptr) {
		return;
	}

	//node has no children
	if (root->right == nullptr && root->left == nullptr) {
		return;
	}

	//node has a 'son'
	if (root->right != nullptr) {
		root->right->parent = root;
		this->getParents(root->right);
	}

	//node has a 'daughter'
	if (root->left != nullptr) {
		root->left->parent = root;
		this->getParents(root->left);
	}
}


template<typename Key, typename Info>
typename Dictionary<Key, Info>::Node * Dictionary<Key, Info>::copyTree(Node * root) {

	if (root == nullptr) {
		return nullptr;
	}

	Node * temp = new Node(root->key, root->info, root->balance);

	temp->left = this->copyTree(root->left);
	temp->right = this->copyTree(root->right);

	return temp;
}


template<typename Key, typename Info>
typename Dictionary<Key, Info>::Node * Dictionary<Key, Info>::find(Node * root, Key key) {

	if (root == nullptr) {
		return nullptr;
	}
	else if (root->key == key) {
		return root;
	}
	else if (root->key > key) {
		return this->find(root->left, key);
	}
	else {
		return this->find(root->right, key);
	}
}


/*-------
ROTATIONS
-------*/


template<typename Key, typename Info>
void Dictionary<Key, Info>::L_rotation(Node *&root) {

	if (root == nullptr || root->right == nullptr) {
		return;
	}

	Node * temp = root->right;

	root->right = temp->left;
	temp->left = root;
	root = temp;
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::R_rotation(Node *&root) {

	if (root == nullptr || root->left == nullptr) {
		return;
	}

	Node * temp = root->left;

	root->left = temp->right;
	temp->right = root;
	root = temp;
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::LR_rotation(Node *&root) {

	Node * temp = root->left;
	this->L_rotation(temp);
	root->left = temp;
	this->R_rotation(root);
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::RL_rotation(Node *&root) {

	Node * temp = root->right;
	this->R_rotation(temp);
	root->right = temp;
	this->L_rotation(root);
}


/*------------
PUBLIC METHODS
------------*/


template<typename Key, typename Info>
void Dictionary<Key, Info>::insert(Key key, Info info) {

	//key must be unique
    if(this->containsKey(key)) {
        std::cout << "\nKey already taken: " << key;
        return;
    }

	//recursion
	this->recInsert(this->top, key, info);
	
	//getting a 'parent' pointer for every node
	this->getParents(this->top);
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::display() {

	if (this->isEmpty()) {
		std::cout << "\nTree is empty";
	}
	else {
		std::cout << "\n-----------------------------";
		this->recDisplay(this->top, 0);
		std::cout << "\n-----------------------------";
	}
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::displayNode(Key key) {

	Node * temp = this->find(this->top, key);

	if (temp == nullptr) {
		std::cout << "\nThere is no node with Key = " << key;
		return;
	}
	else {
		std::cout << "\nKey: " << temp->key << "\tInfo: " << temp->info
			<< "\tBalance: " << temp->balance;
		if (temp->parent != nullptr) {
			std::cout << "\tParent Key: " << temp->parent->key;
		}
	}
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::destroy() {

	this->recDestroy(this->top);
	
	this->top = nullptr;
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::deleteNode(Key key) {
	
	Dictionary<Key, Info> temp;

	this->recDeleteNode(this->top, key, temp);

	*this = temp;
}


template<typename Key, typename Info>
void Dictionary<Key, Info>::createRandomNodes(int number) {

	if (!this->checkKeyInfoType()) {
		std::cout << "\nOnly int,int types are appropriate (createRandomNodes)";
		return;
	}
	else if (number <= 0) {
		std::cout << "\nWrong parameter passed (createRandomNodes)";
		return;
	}
	else {
		for (int i = 0; i < number; i++) {
			int randomKey = rand() % 1000 + 1;
			int randomInfo = rand() % 1000 + 1;
			this->insert(randomKey, randomInfo);
		}
	}
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::containsKey(Key key) { return this->recContainsKey(this->top, key); }


template<typename Key, typename Info>
bool Dictionary<Key, Info>::isEmpty() { return this->top == nullptr; }


template<typename Key, typename Info>
int Dictionary<Key, Info>::height() { return this->recHeight(this->top); }


template<typename Key, typename Info>
int Dictionary<Key, Info>::size() { return this->recSize(this->top); }


/*-------
OPERATORS
-------*/


template<typename Key, typename Info>
Dictionary<Key, Info>& Dictionary<Key, Info>::operator=(const Dictionary<Key, Info> &rhs) {

	if (*this == rhs) {
		return *this;
	}

	this->destroy();

	this->top = copyTree(rhs.top);

	return *this;
}


template<typename Key, typename Info>
bool Dictionary<Key, Info>::operator==(const Dictionary<Key, Info> &rhs) { return this->compareTrees(this->top, rhs.top); }


/*---
OTHER
---*/


template<typename Key, typename Info>
bool Dictionary<Key, Info>::checkKeyInfoType() {

	if (typeid(Key) == typeid(int) && typeid(Info) == typeid(int)) {
		return true;
	}
	else {
		return false;
	}
}
