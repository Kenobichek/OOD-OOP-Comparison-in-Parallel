#include <iostream>
#include <thread>

void doWork() {
	std::cout << "hello\n";
}

int main() {
    std::thread worker(doWork);
    worker.join();
    return 0;
}
