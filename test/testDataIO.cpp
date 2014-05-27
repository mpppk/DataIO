#include <gtest/gtest.h>
// #include <GP.h>
#include "../DataIO.h"

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

// getCsvContentsが正しく返すかのテスト
TEST_F(DataIOTest, getCsvContentsTest){
	mc::DataIO csv(getTempMat());
	vector< vector<string> > v = csv.getCsvContents();
	// １行１列目は１のはず
	EXPECT_EQ(1, toInt(v[0][0]));
	// 3行3列目は9のはず
	EXPECT_EQ(9, toInt(v[2][2]));
}

// writeCsvFileがファイルを正しく書きだしているかのテスト
TEST_F(DataIOTest, readAndWriteCsvFileTest){
	// readFileに不正なパスを与えた時に例外を吐くかのテスト
	mc::DataIO CsvForChkException;
	ASSERT_THROW(CsvForChkException.readCsvFile("noExistFile"), runtime_error);

	mc::DataIO writeCsv1(getTempMat());
	// 書き込む
	writeCsv1.writeCsvFile(fileDirPass + "csvFileTest.csv");
	// 読み込む
	mc::DataIO readCsvFile(fileDirPass + "csvFileTest.csv");

	// 読み込んだ値が正しいかチェック
	vector< vector<string> > v = readCsvFile.getCsvContents();
	// １行１列目は１のはず
	EXPECT_EQ(1, toInt(v[0][0]));
	// 3行3列目は9のはず
	EXPECT_EQ(9, toInt(v[2][2]));

	// 追加書き込み
	mc::DataIO writeCsv2(getTempMat(10));
	writeCsv2.writeCsvFile(fileDirPass + "csvFileTest.csv", mc::DataIO::app);
	// 読み込み
	mc::DataIO readCsv2(fileDirPass + "csvFileTest.csv");

	// 読み込んだ値が正しいかチェック
	v = readCsv2.getCsvContents();
	// 4行１列目は10のはず
	EXPECT_EQ(10, toInt(v[3][0]));
	// 6行3列目は19のはず
	EXPECT_EQ(18, toInt(v[5][2]));
}

// modCsvValueがただしく値を変更しているかのテスト
TEST_F(DataIOTest, modCsvValueTest){
	mc::DataIO csv(getTempMat());
	csv.modCsvValue<int>(0, 0, 10);
	csv.modCsvValue<int>(2, 2, 1);
	vector< vector<string> > v = csv.getCsvContents();

	// １行１列目は１0のはず
	EXPECT_EQ(10, toInt(v[0][0]));
	// 3行3列目は1のはず
	EXPECT_EQ(1, toInt(v[2][2]));
}

// 各種pushBackについて、正しく行が追加されるかどうかのテスト
TEST_F(DataIOTest, pushBackTest){
	mc::DataIO csv(getTempMat());
	// double型で行を追加
	csv.pushBack("testA", 1);
	// string型で行を追加
	csv.pushBack("testB", "2");
	
	// vector<string>で行を追加
	vector<string> content;
	content.push_back("testC");
	content.push_back("3");
	csv.pushBack(content);

	vector< vector<string> > v = csv.getCsvContents();

	// ３回行ったpushBackの結果をチェックする
	EXPECT_EQ("testA", v[3][0]);
	EXPECT_EQ(1, toInt(v[3][1]));

	EXPECT_EQ("testB", v[4][0]);
	EXPECT_EQ(2, toInt(v[4][1]));
	
	EXPECT_EQ("testC", v[5][0]);
	EXPECT_EQ(3, toInt(v[5][1]));

	// 一つの要素だけを追加するpushBack
	csv.clear();
	csv.pushBack("testD");
	csv.pushBack("testE");
	
	EXPECT_EQ("testD", csv(0, 0));
	EXPECT_EQ("testE", csv(1, 0));

}

// pushBackColが正しく列を追加しているかのテスト
TEST_F(DataIOTest, pushBackColTest){
	mc::DataIO csv(getTempMat());
	csv.pushBackCol(0);
	csv.pushBackCol("1");
	EXPECT_EQ("0", csv(0, 3));
	EXPECT_EQ("1", csv(2, 4));
}

// addRowが、指定した場所に行を追加するかどうかのテスト
TEST_F(DataIOTest, addRowTest){
	mc::DataIO csv(getTempMat());
	ASSERT_THROW(csv.addRow(4, getTempVector(10, 11, 12)), out_of_range);

	// 最後の行の次の行を指定した場合、pushBackが行われるか
	csv.addRow(3, getTempVector(13, 14, 15));

	// 途中の行への挿入がうまくできるか
	csv.addRow(3, getTempVector(16, 17, 18));
	// 最初の行への挿入がうまくできるか
	csv.addRow(0, getTempVector(19, 20, 21));

	vector< vector<string> > v = csv.getCsvContents();
	EXPECT_EQ(16, toInt(v[4][0]));
	EXPECT_EQ(15, toInt(v[5][2]));
	EXPECT_EQ(19, toInt(v[0][0]));
}

// clearが正しく行われるかのテスト
TEST_F(DataIOTest, clearTest){
	mc::DataIO csv(getTempMat());
	csv.clear();
	csv.pushBack(getTempVector(10, 11, 12));
	csv.pushBack(getTempVector(13, 14, 15));

	vector< vector<string> > v = csv.getCsvContents();
	EXPECT_EQ(15, toInt(v[1][2]));
}

// mergeCSVToSIdeによって正しく横方向にマージされるかのテスト
TEST_F(DataIOTest, mergeCSVTest){
	mc::DataIO csv1(getTempMat());
	mc::DataIO csv2(getTempMat(10));
	mc::DataIO csv3(getTempMat(20));

	vector< vector<string> > v1 = csv1.getCsvContents();
	vector< vector<string> > v2 = csv2.getCsvContents();

	// 横にマージする場合のテスト
	vector< vector<string> > sideResult = mc::DataIO::mergeCSVToSide(v1, v2, mc::DataIO::NO_SPACE);
	vector< vector<string> > sideResultWithSpace = mc::DataIO::mergeCSVToSide(v1, v2, mc::DataIO::WITH_SPACE);

	EXPECT_EQ(12, toInt(sideResult[0][5]));
	EXPECT_EQ(18, toInt(sideResult[2][5]));
	EXPECT_EQ(12, toInt(sideResultWithSpace[0][6]));
	EXPECT_EQ(18, toInt(sideResultWithSpace[2][6]));

	// 縦にマージする場合のテスト
	vector< vector<string> > bottomResult = mc::DataIO::mergeCSVToBottom(v1, v2, mc::DataIO::NO_SPACE);
	vector< vector<string> > bottomResultWithSpace = mc::DataIO::mergeCSVToBottom(v1, v2, mc::DataIO::WITH_SPACE);

	EXPECT_EQ(10, toInt(bottomResult[3][0]));
	EXPECT_EQ(18, toInt(bottomResult[5][2]));
	EXPECT_EQ(10, toInt(bottomResultWithSpace[4][0]));
	EXPECT_EQ(18, toInt(bottomResultWithSpace[6][2]));

	// 複数同時にマージする場合のテスト
	vector< vector< vector<string> > > data;
	data.push_back(csv1.getCsvContents());
	data.push_back(csv2.getCsvContents());
	data.push_back(csv3.getCsvContents());
	vector< vector<string> > bottomsResult = mc::DataIO::mergeCSVToBottom(data);

	EXPECT_EQ(1, toInt(bottomsResult[0][0]));
	EXPECT_EQ(10, toInt(bottomsResult[4][0]));
	EXPECT_EQ(20, toInt(bottomsResult[8][0]));
}

// 転置が正しく行われるかのテスト
TEST_F(DataIOTest, tTest){
	mc::DataIO csv(getTempMat(1, 2, 4));
	vector< vector<string> > result = csv.t().getCsvContents();
	EXPECT_EQ(2, toInt(result[1][0]));
	EXPECT_EQ(8, toInt(result[3][1]));
}

int main( int argc, char* argv[] ){
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
