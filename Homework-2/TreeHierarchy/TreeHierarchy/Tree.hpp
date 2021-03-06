#pragma once
#include<vector>
#include<list>
#include<queue>
#include<string>
#include"PriorityQueue.hpp"

using std::string;
using std::list;
using std::vector;
using std::queue;
using std::max;
using std::priority_queue;

class Tree
{
public:
	/// @brief Object lifetime

	Tree() : root(nullptr), fSize(0) {}

	~Tree() { clear(root); }

	Tree(const Tree& tree) : root(copy(tree.root)), fSize(tree.fSize) {}

	Tree(Tree&& tree) noexcept : Tree()
	{
		std::swap(root, tree.root);
		std::swap(fSize, tree.fSize);
	}

	Tree& operator=(const Tree& tree)
	{
		if (this != &tree)
		{
			clear(root);
			root = copy(tree.root);
			fSize = tree.fSize;
		}
		return *this;
	}

	Tree& operator=(Tree&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(root, rhs.root);
			std::swap(fSize, rhs.fSize);
		}
		return *this;
	}

public:
	/// @brief Public methods

	/// @brief Wrapper of inner method getSalaryOf
	/// @param who - employee salary that needs to be calculated
	/// @return salary of employee
	unsigned long getSalaryOf(const string& who) const { return getSalaryOf(root, who); }

	/// @brief Wrapper of inner method findParentKeyOf
	/// @param key - name of item
	/// @return The parent of the node with name <key>
	string findParentKeyOf(const string& key) const
	{
		return findParentKeyOf(root, key);
	}

	/// @brief Wrapper of inner method getNumberOfChildrenOf
	/// @param key - the key of the node for which we will determine the number of children
	/// @return the number of children
	int getNumberOfChildrenOf(const string& key) const {
		return getNumberOfChildrenOf(root, key);
	}

	/// @brief Wrapper of inner method find
	/// @param key - the key of the element we will be searching for
	/// @return true if the element is contained, false otherwise
	bool find(const string& key) const { return find(root, key); }

	/// @brief Wrapper of inner method insert
	/// @param who - the employee to be added underneath <boss>
	/// @param boss - the boss of <who>
	/// @return true if the operation succeeded, false otherwise
	bool insert(const string& who, const string& boss) { return insert(root, who, boss); }

	/// @brief Wrapper of inner method getOverloadedNodes
	/// @param level - overloadness factor 
	/// @return number of nodes having more than <level> descendants
	int getOverloadedNodes(int level) const { return getOverloadedNodes(root, level); }

	/// @brief Wrapper of inner method remove
	/// Follows a specific rule given in the description of the prblem
	/// @param key - the name of the element that we will be removing
	/// @return true if the operation succeeded, false otherwise
	bool remove(const string& key) { return remove(root, key); }

	/// @brief Wrapper of inner method height
	/// @return The height of the tree
	size_t height() const { return height(root); };

	/// @return number of nodes in the tree
	size_t size() const { return fSize; };

	/// @return true if tree is empty, false otherwise 
	bool empty() const { return fSize == 0; }

	/// @brief toString() method of tree
	/// @param root - the beginning of the tree
	/// @return Stringified tree
	/// TODO: remvoe pririty queue and use vector with std::sort
	string toString() const
	{
		if (!root)
			return "";

		queue<const Node*> front;
		string res;
		front.push(root);
		front.push(nullptr);

		while (true)
		{
			const Node* current = front.front();
			front.pop();
			if (!current)
			{
				if (front.empty())
					return res;
				front.push(nullptr);
			}
			else
			{
				PriorityQueue<Node*, LessByName> pq;

				for (Node* it = current->child; it; it = it->brother)
					pq.push(it);

				while (!pq.empty())
				{
					Node* el = pq.top();
					res += current->data + "-" + el->data + '\n';
					pq.pop();
					front.push(el);
				}
			}
		}

		return res;
	};

	/// @brief Wrapper of inner method modernizeTree
	void modernizeTree() { modernizeTree(root); }

	/// @brief Wrapper of inner method incorporateTree
	void incorporateTree() { incorporateTree(this->root); }

	/// @brief Wrapper of inner method join
	/// @param other - tree to be joined with *this
	/// @return Newly merged tree
	Tree join(const Tree& other) const { return join(*this, other); }

private:
	/// @brief Tree node descriptor
	struct Node
	{
		string data;
		Node* child, * brother, * parent;

		Node(const string& data, Node* parent = nullptr, Node* child = nullptr, Node* brother = nullptr)
			: data(data), parent(parent), child(child), brother(brother)
		{}

		/// @brief Check whether current node has a child with name <key>
		/// @param key - name to search for in the children of current node
		/// @return true if contained, false otherwise
		bool contains(const string& key) const
		{
			Node* it = child;
			while (it)
			{
				if (it->data == key)
					return true;

				it = it->brother;
			}

			return false;
		}

	} *root;

	/// @brief Used in priority queue comparator
	struct LessByName {
		bool operator()(Node* a, Node* b)
		{
			return a->data > b->data;
		}
	};

	size_t fSize;

private:
	/// @brief Clears the tree(deleting every node)
	/// @param root - the beginning of the tree
	void clear(Node*& root)
	{
		if (root)
		{
			clear(root->child);
			clear(root->brother);
			delete root;
			root = nullptr;
			fSize--;
		}
	}

	/// @brief Makes a copy of a tree given it's root
	/// @param root - the root of the tree that will be copied
	/// @return the copied tree
	Node* copy(Node* root)
	{
		return root ? new Node(root->data, root->parent, copy(root->child), copy(root->brother)) : root;
	}

	/// @brief Calculates the depth of the given node
	/// @param root - beginning of the tree
	/// @param key - name of node
	/// @param level - current level of tree
	/// @return depth of node
	int depthOf(Node* root, const string& key, int level = 0) const
	{
		if (root == nullptr)
			return 0;

		if (root->data == key)
			return level;

		return depthOf(root->child, key, level + 1) + depthOf(root->brother, key, level);
	}

	/// @brief Inserts element <who> underneath <boss>, if both elements exist, then 
	/// relations are rearranged
	/// @param root - the begining of the tree
	/// @param who - the employee to be added underneath <boss>
	/// @param boss - the boss of <who>
	/// @return true if the operation succeeded, false otherwise
	bool insert(Node*& root, const string& who, const string& boss)
	{
		if (fSize == 0)
		{
			root = new Node(boss);
			root->child = new Node(who, root);
			fSize += 2;
			return true;
		}

		if (!root)
			return false;

		if (root->data == boss)
		{
			Node* whoNode = findNodeByKey(this->root, who);

			if (!whoNode)
			{
				whoNode = new Node(who, root/*, nullptr, root->child*/);
				Node* it = root->child;

				if (it)
				{
					while (it->brother)
						it = it->brother;

					it->brother = whoNode;
				}
				else
				{
					root->child = whoNode;
				}

				fSize++;
			}
			else
			{
				if (!root->child)
				{
					root->child = whoNode;
				}
				else
				{
					Node* it = root->child->brother;

					if (it)
					{
						while (it->brother)
							it = it->brother;

						it->brother = whoNode;
					}
					else
					{
						root->child->brother = whoNode;
					}
				}

				Node* parentOfWho = whoNode->parent;
				whoNode->parent = root;

				if (parentOfWho->child->data == whoNode->data)
				{
					parentOfWho->child = whoNode->brother;
				}
				else
				{
					Node* it = parentOfWho->child;
					while (it->brother && it->brother->data != whoNode->data)
						it = it->brother;

					it->brother = whoNode->brother;
				}

				whoNode->brother = nullptr;
			}

			return true;
		}

		return insert(root->child, who, boss) || insert(root->brother, who, boss);
	}

	/// @brief Checks if the tree contains a node with given key
	/// @param root - the begining of the tree
	/// @param key - the key of the element that we are searching for
	/// @return true if the element is in the tree, false otherwise
	bool find(Node* root, const string& key) const
	{
		if (!root)
			return false;
		if (root->data == key)
			return true;
		return find(root->brother, key) || find(root->child, key);
	}

	/// @brief Removes node with <key> from the tree.
	/// Follows a specific rule given in the description of the prblem
	/// @param root - the begining of the tree
	/// @param key - the name of the element that we will be removing
	/// @return true if the operation succeeded, false otherwise
	bool remove(Node*& root, const string& key)
	{
		if (!root)
			return false;

		if (root->data == key)
		{
			Node* toDelete = root;
			if (!root->child)
			{
				Node* parentOfRoot = root->parent;
				root = root->brother;
				Node* it = root;
				it->parent = parentOfRoot;
				while (it->brother)
				{
					it = it->brother;
					it->parent = parentOfRoot;
				}
			}
			else if (root->brother)
			{
				Node* parentOfRemovedElement = root->parent;
				Node* it = root->child;
				it->parent = parentOfRemovedElement;
				while (it->brother)
				{
					it = it->brother;
					it->parent = parentOfRemovedElement;
				}

				it->brother = root->brother;
				root = root->child;
			}
			else
			{
				Node* parentOfRoot = root->parent;
				root = root->child;
				Node* it = root;
				it->parent = parentOfRoot;
				while (it->brother)
				{
					it = it->brother;
					it->parent = parentOfRoot;
				}
			}

			--fSize;
			delete toDelete;
			return true;
		}
		else
		{
			return remove(root->brother, key) || remove(root->child, key);
		}
	}

	/// @brief Finds the height of the tree(childwise)
	/// @param root - the beginning of the tree
	/// @return The height of the tree
	size_t height(Node* root) const
	{
		if (!root)
			return 0;

		return max(1 + height(root->child), height(root->brother));
	}

	/// @brief Finds node with name <key> and accesses its parent
	/// @param root - the begining of the tree
	/// @param key - element whoose parent we will search for
	/// @param res - string in which we will store the key of the parent
	/// @return true if the operation succeeded, false otherwise
	string findParentKeyOf(Node* root, const string& key) const
	{
		if (!root)
			return "";

		if (root->data == key)
			return root->parent ? root->parent->data : "";

		string tmp = findParentKeyOf(root->brother, key);
		if (!tmp.empty())
			return tmp;

		tmp = findParentKeyOf(root->child, key);
		return tmp;
	}

	/// @brief Finds element with name <key>
	/// @param root - the beginning of the tree
	/// @param key - the name of the element we will be looking for
	/// @param res - pointer to node in which we will store the found element
	/// @return true if the operation succeeded, false otherwise
	Node* findNodeByKey(Node* root, const string& key)
	{
		if (!root)
			return nullptr;

		if (root->data == key)
			return root;

		Node* tmp = findNodeByKey(root->brother, key);
		if (tmp)
			return tmp;

		tmp = findNodeByKey(root->child, key);
		return tmp;
	}

	/// @brief Given a name of node, counts all of its children
	/// @param root - beginning of the tree
	/// @param key - name of node
	/// @param res - variable in which result is stored
	/// @return true if the operation succeeded, false otherwise
	int getNumberOfChildrenOf(Node* root, const string& key) const
	{
		if (!root)
			return -1;

		if (root->data == key)
		{
			int res = 0;
			Node* it = root->child;
			if (it)
			{
				res++;
				while (it->brother)
				{
					res++;
					it = it->brother;
				}
			}

			return res;
		}

		int tmp = getNumberOfChildrenOf(root->brother, key);
		if (tmp != -1)
			return tmp;

		tmp = getNumberOfChildrenOf(root->child, key);
		return tmp;
	}

	/// @brief Calculates the size of tree(subtree) with given root
	/// @param root - beginning of the tree
	/// @param level - depth of current iteration
	/// @return number of nodes in tree
	int getSubtreeSize(Node* root, int level = 0) const
	{
		if (!root)
			return 0;

		/// We need to make sure that we enter the child of the root first
		if (level == 0)
			return 1 + getSubtreeSize(root->child, level + 1);
		/// Then we can spread out in all directions
		else
			return (1 + getSubtreeSize(root->child, level + 1) + getSubtreeSize(root->brother, level));
	}

	/// @brief Finds how many nodes have more than <level> descendants
	/// @param root - beginning of tree
	/// @param level - overloadness factor 
	/// @return number of nodes having more than <level> descendants
	int getOverloadedNodes(Node* root, int level) const
	{
		if (!root)
			return 0;

		if (getSubtreeSize(root) - 1 > level)
			return 1 + getOverloadedNodes(root->child, level) + getOverloadedNodes(root->brother, level);

		return getOverloadedNodes(root->child, level) + getOverloadedNodes(root->brother, level);
	}

	/// @brief Calculates the salary of the given node by the formula
	/// 500 * <num_direct_descendants> + 50 * <num_indirect_descendants>
	/// @param root - beginning of tree
	/// @param who - the person whoose salary needs to be calculated
	/// @return salary of node
	unsigned long getSalaryOf(Node* root, const string& who) const
	{
		if (!root)
			return 0;

		if (root->data == who)
		{
			unsigned long res = 0;
			Node* it = root->child;

			while (it)
			{
				res += 500 + 50 * (getSubtreeSize(it) - 1);
				it = it->brother;
			}

			return res;
		}

		return getSalaryOf(root->child, who) + getSalaryOf(root->brother, who);
	}

	/// @brief Modernizes the tree - the nodes at every odd level(counting from root level 0)
	/// get deleted and are replaced by their children
	/// @param root - the beginning node of the tree
	/// @param level - current level of the node
	void modernizeTree(Node* root, int level = 0)
	{
		if (!root)
			return;

		Node* rootBrother = root->brother;
		Node* rootChild = root->child;

		modernizeTree(rootChild, level + 1);
		modernizeTree(rootBrother, level);

		if (level % 2 == 1)
			if (root->child)
				remove(this->root, root->data);
	}

	/// @brief Finds the node with highest salary amongst the children of <root>
	/// If there are more than one nodes with same salary - get the first node lexicographically
	/// @param root - node which children will be checked 
	/// @return the child with highest salary(or lexicographically smallest)
	Node* getNodeToIncorporate(Node* root)
	{
		if (getNumberOfChildrenOf(root, root->data) < 2)
			return nullptr;

		Node* res = root->child;
		Node* it = root->child;
		while (it->brother)
		{
			int resSalary = getSalaryOf(res, res->data);
			int itBrotherSalary = getSalaryOf(it->brother, it->brother->data);
			if (resSalary < itBrotherSalary)
			{
				res = it->brother;
			}
			else if (resSalary == itBrotherSalary)
			{
				if (res->data > it->brother->data)
				{
					res = it->brother;
				}
			}

			it = it->brother;
		}

		return res;
	}

	/// @brief Incorporates the given node
	/// @param root - node to be incorporated
	void incorporateNode(Node* root)
	{
		Node* it = root->parent->child;
		Node* itBrother = it;
		while (itBrother)
		{
			if (itBrother->data != root->data)
			{
				itBrother = it->brother;
				insert(it->data, root->data);
			}
			else
			{
				itBrother = itBrother->brother;
			}

			it = itBrother;
		}
	}

	/// @brief Incorporate tree - for every group of employees with same parent
	/// get the employee with highest salary(or if there are more than one employee with same salary
	/// get the one with lexicographically lowest name) and make that employee boss of it's brothers
	/// @param root Beginning of the tree
	void incorporateTree(Node* root)
	{
		if (!root)
			return;

		Node* nodeToInc = getNodeToIncorporate(root);

		incorporateTree(root->child);
		incorporateTree(root->brother);

		if (nodeToInc)
			incorporateNode(nodeToInc);
	}

	/// @brief Creates vector of nodes of current tree in breath-first traversal order
	/// @return the vector of nodes in breath-first traversal order
	vector<Node*> bfs() const
	{
		vector<Node*> res;
		queue<Node*> front;
		front.push(root);

		while (!front.empty())
		{
			Node* current = front.front();
			front.pop();
			res.push_back(current);

			if (current)
			{
				Node* it = current->child;
				while (it)
				{
					front.push(it);
					it = it->brother;
				}
			}
		}
		return res;
	}

	/// @brief Given two trees, create a merged tree of the two
	/// @param left - left tree
	/// @param right - right tree
	/// @return Merged tree created from left + right
	Tree join(const Tree& left, const Tree& right) const
	{
		Tree res(left);

		for (Node* it : right.bfs())
		{
			if (!it->parent)
				continue;

			/// Parent of it in left tree( it is in right tree)
			Node* found = res.findNodeByKey(res.root, it->parent->data);
			Node* itInLeft = res.findNodeByKey(res.root, it->data);

			/// Check if element in right tree contains its parent in the left tree in its children
			if (itInLeft && !isValidJoin(itInLeft->parent, it))
				return Tree();

			if (!found->contains(it->data)) /// Is it contained in its parent in left tree
			{
				if (!res.find(it->data)) /// Is it contained somewhere in left tree
				{
					res.insert(it->data, found->data); // if not contained then just add it underneath its parent
				}
				else
				{
					string itParentKey = res.findParentKeyOf(it->data);
					int depthFound = res.depthOf(res.root, found->data);
					int depthItParent = res.depthOf(res.root, itParentKey);
					if (depthFound < depthItParent)
					{
						res.insert(it->data, found->data); /// If depth of found is lower than the parent of the existing element in left
															   /// then add it underneath found
					}
					else if (depthFound == depthItParent) /// if depths are equal, check their names lexicographically
					{
						if (found->data < itParentKey)
						{
							res.insert(it->data, found->data);
						}
					}
				}
			}
		}

		return res;
	}

	/// @brief Search for <toSearchFor> in <inSubTree> sub-tree
	/// @param toSearchFor - element to search for
	/// @param inSubTree - root of subtree
	/// @return true if valid, false if invalid
	bool isValidJoin(Node* toSearchFor, Node* inSubTree) const
	{
		if (!toSearchFor)
			return true;

		if (find(inSubTree, toSearchFor->data))
			return false;

		return isValidJoin(toSearchFor->parent, inSubTree);
	}
};