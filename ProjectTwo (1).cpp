#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


// define a structure to hold course information
struct Course {
	string courseId;
	string courseTitle;
	vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================


/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	// Start with an empty tree (no root node yet).
	root = nullptr;//root is equal to nullptr

}
BinarySearchTree::~BinarySearchTree() {
}
/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	// Public wrapper: start in-order traversal from the root. 
	inOrder(root);//Function being called to start at the root
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
	// Insert course into BST (by courseId). If tree is empty, new node becomes root;
 // otherwise recursively insert into left or right subtree.
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		addNode(root, course);
	}
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
	// Iterative BST search by courseId.
// Walk left if search key is smaller, right if larger, until found or nullptr.
	Node* current = root;// set current node equal to root

	// keep looping downwards until bottom reached or matching courseId found
   // if match found, return current course
	while (current != nullptr) {
		if (courseId == current->course.courseId) {
			return current->course;
		}
		else if (courseId < current->course.courseId) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	// Not found: return an "empty" Bid (bidId will be empty).
	Course course;
	return course;

}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {

	// Recursive insert helper for the BST (ordered by CourseId).
// If CourseId is smaller, go left; otherwise go right.
// Insert at the first available nullptr child.

	if (course.courseId < node->course.courseId) {
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		else {
			addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			addNode(node->right, course);
		}
	}
	


}

void BinarySearchTree::inOrder(Node* node) {
	//The base case, return immediatly(prevents infinite recursion)
	if (node == nullptr) {
		return;
	}
	inOrder(node->left);// Decends left subtree
	cout << node->course.courseId << ": " << node->course.courseTitle << endl;//When The left subtree is done then current is printed
	inOrder(node->right);//When current is done being printed then recursively descends  into the right subtree
}



/**
 * Load a CSV file containing courses into a BST
 *
 * @param csvPath the path to the CSV file to load
 */

bool loadCourses(string csvPath, BinarySearchTree* bst) {
	ifstream inFS;
	string line;
	// Tell the user what file we are attempting to load
	cout << "Loading CSV file " << csvPath << endl;
	// Open the file path/name the user entered
	inFS.open(csvPath);
	// If file can't be opened, stop and report failure
	if (!inFS.is_open()) {
		cout << "Could not open file " << csvPath << endl;
		return false;
	}
	// Read the CSV file one line at a time
	while (getline(inFS, line)) {
		Course course;// Course object for THIS row
		stringstream ss(line);     // Treat the row like an input stream
		string field;          // Temporary storage for each comma-separated value
		// 1) courseId
		getline(ss, field, ',');
		course.courseId = field;

		// 2) courseTitle
		getline(ss, field, ',');
		course.courseTitle = field;
		// 3) Remaining columns (if any) are prerequisites
		// Each one gets pushed into the prerequisites vector
		while (getline(ss, field, ',')) {

			course.prerequisites.push_back(field);
		}
		// Insert the fully built Course into the BST (keyed by courseId)
		bst->Insert(course);    
	}
	inFS.close();  // Done reading the file
	return true;
}

	/**
 * The one and only main() method
 */
int main() {
	// Define a binary search tree to hold all bids
	BinarySearchTree bst;

	string csvPath;
	string courseKey;
	bool loaded = false;

	int choice = 0;

	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			// Prevent loading twice 
			if (loaded) {
				cout << "Data already loaded." << endl;
				break;
			}
			// Complete the method call to load the courses
			cout << "Enter file name: ";
			cin >> csvPath;
			// loadCourses returns true if it opened/loaded successfully
			loaded = loadCourses(csvPath, &bst);
			
			break;

		case 2:
			// Print all courses in-order (BST traversal prints sorted by courseId)
			bst.InOrder();
			break;

		case 3:
			// Ask which course to print
			cout << "Enter Course Id: ";
			cin >> courseKey;

			{
				// Search BST for the courseId the user typed
				Course course = bst.Search(courseKey);
				// If courseId is NOT empty, it means search found a real course
				if (!course.courseId.empty()) {
					cout << course.courseId << ": " << course.courseTitle << endl;
					// Only print "Prerequisites:" label if there are any
					if (course.prerequisites.size() > 0) {
						cout << "Prerequisites:" << endl;
					}
					// Loop through prereq IDs, then try to find their titles too
					for (int i = 0; i < course.prerequisites.size(); i++) {
						string prereqId = course.prerequisites.at(i);
						// Look up each prerequisite courseId in the BST
						Course prereq = bst.Search(prereqId);
						// If found, print full prereq info
						if (!prereq.courseId.empty()) {
							cout << prereq.courseId << ": " << prereq.courseTitle << endl;
						}
						// If not found in file, at least print the prereqId string
						else {
							cout << prereqId << endl;
						}
					}
				}
				else {
					cout << "Course " << courseKey << " not found." << endl;
				}
			}
					break;
		case 9:
			break;


		default:
			cout << "Invalid option. Please enter 1, 2, 3, or 9." << endl;
			break;
		}
	}



				
		
		cout << "Good bye." << endl;

		return 0;
	}
	



	




