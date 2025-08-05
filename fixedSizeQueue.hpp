#pragma once
#include <string>
#include <array>

constexpr int DEFAULT_SIZE = 128;

template <typename T, int SIZE = DEFAULT_SIZE>
class FixedSizeQueue {
	std::array<T, SIZE> buffer;
	std::array<std::atomic<bool>, SIZE> isWritten;
	std::atomic<unsigned int> front = 0, back = 0;

public:

	bool pop_or_empty(T& result);
	bool push_or_full(T val);
	bool isEmpty() const;
	bool isFull() const;
	int count() const;
};

template <typename T, int SIZE>
inline bool FixedSizeQueue<T, SIZE>::pop_or_empty(T& result)
{
	if (isEmpty()) 
		return false;
	unsigned int cur_front;// = front;
	unsigned read_front;
	do {
		cur_front = front.load(std::memory_order_acquire);
		read_front = (cur_front + 1) % SIZE;
		if (!isWritten[read_front])
			return false;
	} while (!front.compare_exchange_weak(cur_front, cur_front + 1));
	result = buffer[read_front];
	isWritten[read_front].store(false, std::memory_order_release);
	return true;
}

template <typename T, int SIZE>
bool FixedSizeQueue<T, SIZE>::push_or_full(T val)
{
	if (isFull()) 
		return false;
	
	int write_back = back++ % SIZE;
	buffer[write_back] = val;
	isWritten[write_back].store(true, std::memory_order_release);
	return true;
}

template <typename T, int SIZE>
inline bool FixedSizeQueue<T, SIZE>::isEmpty() const
{
	return front >= back;
}
template <typename T, int SIZE>
inline bool FixedSizeQueue<T, SIZE>::isFull() const
{
	return back >= front + SIZE;
}

template<typename T, int SIZE>
inline int FixedSizeQueue<T, SIZE>::count() const
{
	return back - front;
}