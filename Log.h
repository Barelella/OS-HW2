#ifndef LOG_H_
#define LOG_H_

#include <fstream>
#include <sstream>

class Log {
private:
	std::ofstream logStream;
	pthread_mutex_t lock;

public:
	Log(const char* fileName);
	~Log();

	void Write(std::string str);
	void WriteLine(std::string str);
	void WriteLine(std::stringstream&);

};

#endif /* LOG_H_ */
