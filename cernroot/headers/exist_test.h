#include <sys/stat.h>
#include <unistd.h>
#include <string>

//Simple test to check whether file exists before reading in, to avoid crashing.
bool exists_test(const std::string& name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

//Because sometimes I don't know how to spell
bool exist_test(const std::string& name)
{
	return (exists_test(name));
}
