#include <Log.h>
using namespace std;

template<class T> inline std::string toString(T x) {std::ostringstream sout;sout<<x;return sout.str();}

namespace mc{
	Log& Log::add(string message){
		logs_.push_back(message);
		return *this;
	}

	Log& Log::addDateTime(){
		// 現在日時と時刻を取得
		time_t t = time(0);
		struct tm * now = localtime( & t );
		int year = now->tm_year + 1900;
		int month = now->tm_mon + 1;
		int day = now->tm_mday;
		int hour = now->tm_hour;
		int min = now->tm_min; int sec = now->tm_sec;
		string dateLog = "date," + toString(year) + "_" + toString(month) + "_" + toString(day);
		string timeLog = "time," + toString(day) + ":" + toString(hour) + ":" + toString(min);
		add(dateLog);
		add(timeLog);
		return *this;
	}

	vector<string> Log::toVec() const{
		return logs_;
	}

	Log& Log::clear(){
		logs_.clear();
		return *this;
	}

	const Log& Log::write(string filePass) const{
		ofstream ofs;
		ofs.open(filePass.c_str());

		for (auto row : logs_){
			//列数がゼロの行を含む場合は警告
			if(row.size() <= 0){
				cerr << "warning: this Data file contains zero element row\n";
				continue;
			}
			ofs << row << endl;
		}
		return *this;
	}
}