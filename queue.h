#pragma once
#include <vector>
#include <string>

const int DEFAULT_START_BUFFER_SIZE = 10;

template <typename T>
class Queue {
	std::vector<T> buffer;
	std::atomic<int> front = 0, back = 0;
	int size;


	void resize();

public:

	Queue(int _size = DEFAULT_START_BUFFER_SIZE) : size(_size), buffer(_size) {};
	T pop();
	void push(T val);
	bool isEmpty() const;
	int count() const;
};

template <typename T>
inline T Queue<T>::pop()
{
	if (front < back) {
		return buffer[front++ % size];
	}
	throw std::string("Poping from empty queue");
}

template <typename T>
void Queue<T>::push(T val)
{
	if (back - size >= front) {
		resize();
	}
	int write_back = back++;
	buffer[write_back % size] = val;
	return;

}

template <typename T>
inline bool Queue<T>::isEmpty() const
{
	return front >= back;
}

template <typename T>
inline void Queue<T>::resize()
{
	front = front % size;
	int oldSize = size;
	size *= 2;
	buffer.resize(size);
	int firstPartLen = back % (oldSize);
	std::memcpy(buffer.data() + oldSize, buffer.data(), firstPartLen * sizeof(T));
	back = front + oldSize;
}

template<typename T>
inline int Queue<T>::count() const
{
	return back - front;
}