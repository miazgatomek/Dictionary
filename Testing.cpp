#include "Testing.h"


Testing::Testing()
{
	std::cout << "\n---TESTS BEGIN---\n\n";
}

Testing::~Testing()
{
	std::cout << "\n\n\n---END OF TESTS---\n\n";
}


void Testing::insertTest()
{
	std::cout << "\n\n\n\t/////INSERT TEST/////\n";

	Dictionary<int, int> test1;

	test1.insert(1, 1);
	test1.insert(2, 1);
	test1.insert(3, 1);
	test1.insert(4, 1);
	test1.insert(5, 1);

	std::cout << "\n Inserting 5 nodes:";
    test1.display();

	std::cout << "\n";
}

void Testing::displayTest()
{
	std::cout << "\n\n\n\t/////DISPLAYING METHODS TEST/////\n";

	Dictionary<int, int> test1(2);
	test1.insert(-1, -2);

	std::cout << "\n Displaying three-node tree:";
	test1.display();

	std::cout << "\n Using displayNode(-1):";
	test1.displayNode(-1);

	std::cout << "\n Using displayNode(1001):";
	test1.displayNode(1001);

	std::cout << "\n Displaying empty tree:";
	
	Dictionary<int, int> test2;
	test2.display();

	std::cout << "\n";
}

void Testing::costructorsTest()
{
    std::cout << "\n\n\n\t/////CONSTRUCTORS TEST/////\n";

    std::cout << "\n Dictionary(5) test:";
    Dictionary<int, int> test1(5);
    test1.display();

    std::cout << "\n Copy constructor test:";
    Dictionary<int, int> test2(test1);
    test2.display();

    std::cout << "\n";
}

void Testing::deletingNodesTest()
{
	std::cout << "\n\n\n\t/////DELETING METHODS TEST/////\n";

	Dictionary<int, int> test(5);
	test.insert(150, 50);
	test.insert(700, 50);
	test.insert(500, 50);

	std::cout << "\n Default tree:";
	test.display();

	std::cout << "\n Tree after deleting 150, 700, 500:";
	test.deleteNode(150);
	test.deleteNode(700);
	test.deleteNode(500);
	test.display();

	std::cout << "\n Tree after destroy():";
	test.destroy();
	test.display();

	std::cout << "\n";
}

void Testing::containsKeyTest()
{
    std::cout << "\n\n\n\t/////CONTAINS KEY TEST/////\n";

    Dictionary<int, int> test(5);
    test.insert(-1,-1);

    std::cout << "\n Default tree:";
    test.display();

    std::cout << "\n Checking if key = -1 is in the tree:";

    if(test.containsKey(-1)) {
        std::cout << "\nKey = -1 is in the tree";
    }
    else {
        std::cout << "\nKey = -1 is not in the tree";
    }

    std::cout << "\n Checking if key = -2 is in the tree:";

    if(test.containsKey(-2)) {
        std::cout << "\nKey = -2 is in the tree";
    }
    else {
        std::cout << "\nKey = -2 is not in the tree";
    }

    std::cout << "\n";
}

void Testing::sizeTest()
{
	std::cout << "\n\n\n\t/////SIZE TEST/////\n";

	Dictionary<int, int> test(5);
	
	std::cout << "\n Default tree:";
	test.display();

	std::cout << "\n Returned size of tree: " << test.size();

	test.destroy();
	std::cout << "\n Returned size of already destroyed tree: " << test.size();

	std::cout << "\n";
}

void Testing::operatorsTest()
{
	std::cout << "\n\n\n\t/////OPERATORS TEST/////\n";

	Dictionary<int, int> test1(5);
	Dictionary<int, int> test2;

	std::cout << "\n Default tree:";
	test1.display();

	std::cout << "\n Empty tree after = by default tree:";
	test2 = test1;
	test2.display();

	std::cout << "\n Checking if the trees are == to each other:";

	if (test1 == test2) {
		std::cout << "\nThey are equal";
	}
	else {
		std::cout << "\nThey are not equal";
	}

	std::cout << "\n";
}

void Testing::iteratorTest()
{
	std::cout << "\n\n\n\t/////ITERATORS TEST/////\n";

	Dictionary<int, int> test(10);

	typename Dictionary<int, int>::Iterator it = test.begin();
	
	std::cout << "\n Default tree:";
	test.display();

	std::cout << "\n Displaying every node using iterator:";
	
	for(int i = 0; i < 10; i++) {
		it.display();
		it++;
	}

	std::cout << "\n";
}
