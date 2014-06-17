#include <gtest/gtest.h>
// #include <GP.h>
#include <DataIO.h>
#include <Log.h>

using namespace std;

inline int toInt(std::string s) {int v; std::istringstream sin(s);sin>>v;return v;}
template<class T> inline std::string toString(T x) {std::ostringstream sout;sout<<x;return sout.str();}

// startValueから連番の値を振った行列を返す
vector< vector<string> > getTempMat(int startValue = 1, int rowsNum = 3, int colsNum = 3){
	int value = startValue;
	vector< vector<string> > contents;

	for (int row_i = 0; row_i < rowsNum; ++row_i){
		vector<string> content;
		for (int i = 0; i < colsNum; ++i){
			content.push_back(toString<int>(value));
			value++;
		}
		contents.push_back(content);
	}

	return contents;
}

vector<string> getTempVector(int v1, int v2, int v3){
	vector<string> v;
	v.push_back(toString<int>(v1));
	v.push_back(toString<int>(v2));
	v.push_back(toString<int>(v3));
	return v;
}

vector<string> getTempVector(){
	vector<string> content;
	content.push_back("1");
	content.push_back("2");
	content.push_back("3");
	return content;
}

class DataIOTest : public ::testing::Test{
protected:
	string fileDirPass;

	virtual void SetUp(){
		// ファイルの書きだし先
		fileDirPass = "./test/data/";
	}
};

// toVecRowsが正しく動くかのテスト
TEST_F(DataIOTest, toVecTest){
	mc::DataIO data(getTempMat());
	vector< vector<string> > v = data.toVecRows();
	// １行１列目は１のはず
	EXPECT_EQ(1, toInt(v[0][0]));
	// 3行3列目は9のはず
	EXPECT_EQ(9, toInt(v[2][2]));
}

// writeCsvFileがファイルを正しく書きだしているかのテスト
TEST_F(DataIOTest, readAndWriteFileTest){
	// readFileに不正なパスを与えた時に例外を吐くかのテスト
	mc::DataIO dataForChkException;
	ASSERT_THROW(dataForChkException.readCsvFile("noExistFile"), runtime_error);

	mc::DataIO writeData1(getTempMat());
	// 書き込む
	writeData1.writeCsvFile(fileDirPass + "csvFileTest.csv");
	// 読み込む
	mc::DataIO readData(fileDirPass + "csvFileTest.csv");

	// 読み込んだ値が正しいかチェック
	vector< vector<string> > v = readData.toVecRows();
	// １行１列目は１のはず
	EXPECT_EQ(1, toInt(v[0][0]));
	// 3行3列目は9のはず
	EXPECT_EQ(9, toInt(v[2][2]));

	// 追加書き込み
	mc::DataIO writeData2(getTempMat(10));
	writeData2.writeCsvFile(fileDirPass + "csvFileTest.csv", mc::DataIO::App);
	// 読み込み
	mc::DataIO readData2(fileDirPass + "csvFileTest.csv");

	// 読み込んだ値が正しいかチェック
	v = readData2.toVecRows();
	// 4行１列目は10のはず
	EXPECT_EQ(10, toInt(v[3][0]));
	// 6行3列目は19のはず
	EXPECT_EQ(18, toInt(v[5][2]));

	vector<string> logVec = {"log1", "log2", "log3"};
	mc::DataIO::writeColFile(fileDirPass + "csvFileTest2.csv", ",", logVec, mc::DataIO::App);
	mc::DataIO logData(fileDirPass + "csvFileTest2.csv");
	v = logData.toVecRows();
	EXPECT_EQ("log3", v[2][0]);

}

// modValueがただしく値を変更しているかのテスト
TEST_F(DataIOTest, modValueTest){
	mc::DataIO data(getTempMat());
	data.modValue<int>(0, 0, 10);
	data.modValue<int>(2, 2, 1);
	vector< vector<string> > v = data.toVecRows();

	// １行１列目は１0のはず
	EXPECT_EQ(10, toInt(v[0][0]));
	// 3行3列目は1のはず
	EXPECT_EQ(1, toInt(v[2][2]));
}

// 各種pushBackについて、正しく行が追加されるかどうかのテスト
TEST_F(DataIOTest, pushBackTest){
	mc::DataIO data(getTempMat());
	// double型で行を追加
	data.pushBack("testA", 1);
	// string型で行を追加
	data.pushBack("testB", "2");
	
	// vector<string>で行を追加
	vector<string> content;
	content.push_back("testC");
	content.push_back("3");
	data.pushBack(content);

	vector< vector<string> > v = data.toVecRows();

	// ３回行ったpushBackの結果をチェックする
	EXPECT_EQ("testA", v[3][0]);
	EXPECT_EQ(1, toInt(v[3][1]));

	EXPECT_EQ("testB", v[4][0]);
	EXPECT_EQ(2, toInt(v[4][1]));
	
	EXPECT_EQ("testC", v[5][0]);
	EXPECT_EQ(3, toInt(v[5][1]));

	// 一つの要素だけを追加するpushBack
	data.clear();
	data.pushBack("testD");
	data.pushBack("testE");
	
	EXPECT_EQ("testD", data(0, 0));
	EXPECT_EQ("testE", data(1, 0));
}

// pushBackColが正しく列を追加しているかのテスト
TEST_F(DataIOTest, pushBackColTest){
	mc::DataIO data(getTempMat());
	data.pushBackCol(0);
	data.pushBackCol("1");
	EXPECT_EQ("0", data(0, 3));
	EXPECT_EQ("1", data(2, 4));
}

// addRowが、指定した場所に行を追加するかどうかのテスト
TEST_F(DataIOTest, addRowTest){
	mc::DataIO data(getTempMat());
	ASSERT_THROW(data.addRow(4, getTempVector(10, 11, 12)), out_of_range);

	// 最後の行の次の行を指定した場合、pushBackが行われるか
	data.addRow(3, getTempVector(13, 14, 15));

	// 途中の行への挿入がうまくできるか
	data.addRow(3, getTempVector(16, 17, 18));
	// 最初の行への挿入がうまくできるか
	data.addRow(0, getTempVector(19, 20, 21));

	vector< vector<string> > v = data.toVecRows();
	EXPECT_EQ(16, toInt(v[4][0]));
	EXPECT_EQ(15, toInt(v[5][2]));
	EXPECT_EQ(19, toInt(v[0][0]));
}

// clearが正しく行われるかのテスト
TEST_F(DataIOTest, clearTest){
	mc::DataIO data(getTempMat());
	data.clear();
	data.pushBack(getTempVector(10, 11, 12));
	data.pushBack(getTempVector(13, 14, 15));

	vector< vector<string> > v = data.toVecRows();
	EXPECT_EQ(15, toInt(v[1][2]));
}

// mergeToSIdeによって正しく横方向にマージされるかのテスト
TEST_F(DataIOTest, mergeCSVTest){
	mc::DataIO csv1(getTempMat());
	mc::DataIO csv2(getTempMat(10));
	mc::DataIO csv3(getTempMat(20));

	vector< vector<string> > v1 = csv1.toVecRows();
	vector< vector<string> > v2 = csv2.toVecRows();

	// 横にマージする場合のテスト
	vector< vector<string> > sideResult = mc::DataIO::mergeToSide(v1, v2, mc::DataIO::NoSpace);
	vector< vector<string> > sideResultWithSpace = mc::DataIO::mergeToSide(v1, v2, mc::DataIO::WithSpace);

	EXPECT_EQ(12, toInt(sideResult[0][5]));
	EXPECT_EQ(18, toInt(sideResult[2][5]));
	EXPECT_EQ(12, toInt(sideResultWithSpace[0][6]));
	EXPECT_EQ(18, toInt(sideResultWithSpace[2][6]));

	// 縦にマージする場合のテスト
	vector< vector<string> > bottomResult = mc::DataIO::mergeToBottom(v1, v2, mc::DataIO::NoSpace);
	vector< vector<string> > bottomResultWithSpace = mc::DataIO::mergeToBottom(v1, v2, mc::DataIO::WithSpace);

	EXPECT_EQ(10, toInt(bottomResult[3][0]));
	EXPECT_EQ(18, toInt(bottomResult[5][2]));
	EXPECT_EQ(10, toInt(bottomResultWithSpace[4][0]));
	EXPECT_EQ(18, toInt(bottomResultWithSpace[6][2]));

	// 複数同時にマージする場合のテスト
	vector< vector< vector<string> > > data;
	data.push_back(csv1.toVecRows());
	data.push_back(csv2.toVecRows());
	data.push_back(csv3.toVecRows());
	vector< vector<string> > bottomsResult = mc::DataIO::mergeToBottom(data);

	EXPECT_EQ(1, toInt(bottomsResult[0][0]));
	EXPECT_EQ(10, toInt(bottomsResult[4][0]));
	EXPECT_EQ(20, toInt(bottomsResult[8][0]));
}

// toVecColsが正しく行われるかのテスト
TEST_F(DataIOTest, toVecColsTest){
	mc::DataIO data(getTempMat(1, 2, 4));
	vector< vector<string> > result = data.toVecCols();
	EXPECT_EQ(2, toInt(result[1][0]));
	EXPECT_EQ(8, toInt(result[3][1]));
}

// mkdirが正しく行われるかのテスト
// TEST_F(DataIOTest, makeDirTest){
// 	mc::DataIO::makeDir("testDir");
// 	string dirName = mc::DataIO::makeDir("testDir");
// 	cout << dirName << endl;
// }

class LogTest : public ::testing::Test{
protected:

	virtual void SetUp(){
	}
};

TEST_F(LogTest, readAndWriteTest){
	mc::Log mlog;
	mlog.add("log1").add("log2").add("log3").write("test/output.txt");
}

TEST_F(LogTest, addDateTimeTest){
	mc::Log mlog;
	auto vec = mlog.addDateTime().toVec();
	cout << vec[0] << endl << vec[1] << endl;
	EXPECT_EQ(2, vec.size());
}

// Processが正しく動くかのテスト
TEST_F(LogTest, toVecTest){
	mc::Log mlog;
	auto vec = mlog.add("log1").clear().add("log2").add("log3").toVec();
	EXPECT_EQ("log2", vec[0]);
}


int main( int argc, char* argv[] ){
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
