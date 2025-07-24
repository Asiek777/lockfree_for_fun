#include "pch.h"
#include "../../lockfree_for_fun/queue.h"
#include <thread>

namespace MultiThread {


	TEST(MultiThreadTest, OneWriterOneReader) {
		int size = 100;
		Queue<int> queue(size);

		std::thread reader([&] {
			for (int i = 0; i < size; i++) {
				while (queue.isEmpty()) {}
				EXPECT_EQ(queue.pop(), i);
			}
			});

		std::thread writer([&] {
			for (int i = 0; i < size; i++)
				queue.push(i);
			});

		reader.join();
		writer.join();
		EXPECT_EQ(queue.isEmpty(), true);
		EXPECT_ANY_THROW(queue.pop());
	}

	TEST(MultiThreadTest, TwoWriters) {
		int size = 100;
		Queue<int> queue(size*2);

		std::thread writer1([&] {
			for (int i = 0; i < size; i++)
				queue.push(i+1);
			});
		std::thread writer2([&] {
			for (int i = 0; i < size; i++)
				queue.push(i+1);
			});

		writer1.join();
		writer2.join();
		EXPECT_EQ(queue.count(), size*2);
		for (int i = 0; i < size * 2; i++)
			EXPECT_NE(queue.pop(), 0);
	}
}