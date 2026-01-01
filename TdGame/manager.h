#pragma once

template<typename T>
class Manager {
public:
	static T* instance() {
		if (manager == nullptr)
			manager = new T();

		return manager;
	}

private:
	static T* manager;

protected:
	Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;

	~Manager() = default;

};

template <typename T>
T* Manager<T>::manager = nullptr;