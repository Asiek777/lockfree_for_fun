#include "pch.h"
#include "../../lockfree_for_fun/fixedSizeQueue.hpp"
#include <queue>
#include <random>
#include <thread>

namespace SingleThread {

	TEST(SingleThreadFixedSize, RandomOps) {
		constexpr int buffer_size = 128;
		FixedSizeQueue<double, buffer_size> queue;
		std::queue<double> stl_queue;
		constexpr int stepCount = 100000;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> nextNumber(
			std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
		std::uniform_int_distribution<int> nextOp(0, 1);

		for (int i = 0; i < stepCount; i++) {
			if (nextOp(mt) && !queue.isFull()) {
				double val = nextNumber(mt);
				queue.push_or_full(val);
				stl_queue.push(val);
				EXPECT_EQ(queue.count(), stl_queue.size());
			}
			else if (!queue.isEmpty()) {
				double firstVal;
				queue.pop_or_empty(firstVal);
				double secondVal = stl_queue.front();
				stl_queue.pop();
				EXPECT_EQ(firstVal, secondVal);
				EXPECT_EQ(queue.count(), stl_queue.size());
			}
		}
	}
}

namespace MultiThread {
	TEST(MultiThreadFixedSize, ManyWritersOneReader) {
		constexpr int writerCount = 15;
		constexpr int size = 100;
		FixedSizeQueue<short, size* writerCount> queue;
		std::vector<std::thread> writers(writerCount);

		auto writerFun = [&] {
			for (int i = 0; i < size; i++)
				queue.push_or_full(i + 1);
			};

		auto readerFun = [&] {
			for (int i = 0; i < size * writerCount; i++) {
				short val;
				while (!queue.pop_or_empty(val))
					EXPECT_NE(val, 0);
			}
			};

		for (int i = 0; i < writerCount; i++)
			writers[i] = std::thread(writerFun);

		auto reader = std::thread(readerFun);

		for (int i = 0; i < writerCount; i++)
			writers[i].join();
		reader.join();

		EXPECT_TRUE(queue.isEmpty());
	}
}