#pragma once
#include <unordered_map>
#include <string>
class Fuller {
private:
	unsigned int k;
	unsigned int id;
	float* vertices;
	unsigned int* india;
	std::unordered_map<std::string, unsigned int> map;
	void Emit(std::string v0, std::string v1, std::string v2);
	std::string Mid(std::string v0, std::string v1);
	void MakeFuller(std::string v0, std::string v1, std::string v2, int depth);
public:
	Fuller(int depth);
	~Fuller();
	void scale(float s);
	void translate(float x, float y, float z);
	void AddIndex(unsigned int n);
	void Print();
	inline float* getVertData() { return vertices; };
	inline unsigned int* getIndData() { return india; };
	inline unsigned int getVertSize() { return k; };
	inline unsigned int getIndSize() { return id; };
};
