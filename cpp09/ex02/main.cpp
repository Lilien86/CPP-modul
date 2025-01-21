	#include "PmergeMe.hpp"
#include <iomanip> // For std::fixed and std::setprecision

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <numbers>..." << std::endl;
		return 1;
	}

	try {
		std::vector<int> vectorContainer = build_container<std::vector<int> >((const char **)(argv + 1));
		std::list<int> listContainer = build_container<std::list<int> >((const char **)(argv + 1));

		std::cout << "Before: ";
		for (std::vector<int>::const_iterator it = vectorContainer.begin(); it != vectorContainer.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;

		clock_t startVector = clock();
		fordJohnsonMergeInsertSort(vectorContainer);
		clock_t endVector = clock();

		clock_t startList = clock();
		fordJohnsonMergeInsertSort(listContainer);
		clock_t endList = clock();

		std::cout << "After: ";
		for (std::vector<int>::const_iterator it = vectorContainer.begin(); it != vectorContainer.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;

		double timeVector = static_cast<double>(endVector - startVector) / CLOCKS_PER_SEC * 1e6;
		double timeList = static_cast<double>(endList - startList) / CLOCKS_PER_SEC * 1e6;

		std::cout << "Time to process a range of " << vectorContainer.size()
				  << " elements with std::vector : " << std::fixed << std::setprecision(5) << timeVector << " us" << std::endl;

		std::cout << "Time to process a range of " << listContainer.size()
				  << " elements with std::list : " << std::fixed << std::setprecision(5) << timeList << " us" << std::endl;

	} catch (int error) {
		std::cerr << "Error: Invalid input (" << error << ")" << std::endl;
		return 1;
	}

	return 0;
}
