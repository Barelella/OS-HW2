#ifndef LOG_H_
#define LOG_H_

#include <fstream>

class Log {
private:
	std::ofstream logStream;
	pthread_mutex_t lock;

public:
	Log(const char* fileName);
	~Log();
//	friend std::ostream& operator<<(Log& log, std::ostream&);
//	friend std::ostream& operator<<(Log& log, std::string);
//	friend std::ostream& operator<<(Log& log, int);

	template <class T>
	friend std::ostream& operator<<(Log& log, T input);

	friend std::ostream& operator<<(Log& log, int);
	friend std::ostream& operator<<(Log& log, double);
	friend std::ostream& operator<<(Log& log, char *);
	friend std::ostream& operator<<(Log& log, const char *);



};

#endif /* LOG_H_ */
