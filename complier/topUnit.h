#pragma warning(disable:4786)
#include <string>


class topUnit
{
public:
	topUnit(std::istream&, std::string);
	void compile();
private:
	std::istream& input;
	std::string name;
};