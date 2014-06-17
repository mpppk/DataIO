#include <Log.h>
#include <gtest/gtest.h>
using namespace std;

inline int toInt(std::string s) {int v; std::istringstream sin(s);sin>>v;return v;}
template<class T> inline std::string toString(T x) {std::ostringstream sout;sout<<x;return sout.str();}

class LogTest : public ::testing::Test{
protected:

	virtual void SetUp(){
	}
};

TEST_F(LogTest, readAndWriteTest){
	mc::as::Log mlog;
	mlog.add("log1").add("log2").add("log3").write("test/output.txt");
}

TEST_F(LogTest, addDateTimeTest){
	mc::as::Log mlog;
	auto vec = mlog.addDateTime().toVec();
	cout << vec[0] << endl << vec[1] << endl;
	EXPECT_EQ(2, vec.size());
}

// Processが正しく動くかのテスト
TEST_F(LogTest, toVecTest){
	mc::as::Log mlog;
	auto vec = mlog.add("log1").clear().add("log2").add("log3").toVec();
	EXPECT_EQ("log2", vec[0]);
}

int main( int argc, char* argv[] ){
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
