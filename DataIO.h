/**
 * @file DataIO.h
 * @brief DataIOの要約
 * @author まつむら
 */

#pragma once
// STL
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

//boostのライブラリ
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace mc{
	/**
	 * @class DataIO a b csvの入出力をするクラス
	 * @remarks 
	 */
	class DataIO{
	private:
		string filePass_;
		vector< vector<string> > contents_;

		// // 改行などの文字コードを取り除くための条件(詳しくは関数オブジェクト、叙述関数などでググる)
		// // remove_ifが上手く使えなかったので、使っていない
		// struct removeChar {
		// 	bool operator()(char c) const {
		// 		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
		// 	}
		// };

	public:
		/** @enum WriteOption 書き込み時のオプション*/
		enum WriteOption{
			/** 書きだし先のファイルを上書き(既存ファイルは失われる)*/
			out,
			/** 書きだし先のファイルの末尾に追加*/
			app,
			/** WriteOptionの最大値*/
			WriteOptionNum
		};

		/** CSVファイルをマージする際のオプション */
		enum mergeOption {
			/** マージする二つのCSVファイルの間にスペースを入れる */
			WITH_SPACE,
			/** マージする二つのCSVファイルの間にスペースを入れない */
			NO_SPACE
		};

		DataIO();

		/**
		 * stringひとつをCSVのひとつの要素として読み込む
		 * @param[in] contents 読み込むデータ
		 */
		DataIO(const vector< vector<string> > contents);

		/**
		 * 指定されたパスのCSVファイルを読み込む
		 * @param[in] tempfilePass 読み込むCSVファイルのパス
		 */
		DataIO(const string tempfilePass);

		/**
		 * 指定した要素を返す
		 * @param row 行
		 * @param  col 列
		 * @return 要素の値
		 */
		string& operator()(const int row, const int col);

		/**
		 * 受け取った文字列集合を保持する
		 * @param[in] contents 読み込むデータ
		 */
		void createByVec(const vector< vector<string> > contents);

		/**
		 * csvの内容を保持するvectorを返す
		 */
		vector< vector<string> > getCsvContents();

		/**
		 * csvの内容を表示する
		 */
		void showCsvContents();
		static void showCsvContents(vector< vector<string> > contents);
		/**
		 * 指定したパスのcsvファイルを読み込む
		 * @param readFilePass 読み込むcsvファイルのパス
		 */
		void readCsvFile(const string readFilePass);

		/**
		 * csvの内容を指定したパスに書き込む
		 * @param writeFilePass 書き込み先のパス
		 * @param wo            書き込む際のオプション.詳細はWriteOptionを参照
		 */
		void writeCsvFile(const string writeFilePass, WriteOption wo = out);

		/**
		 * csvの内容を指定したパスに書き込む
		 * @param writeFilePass 書き込み先のパス
		 * @param contents 		書き込む内容
		 * @param wo            書き込む際のオプション.詳細はWriteOptionを参照
		 */
		static void writeCsvFile(string writeFilePass, vector< vector<string> > contents, WriteOption wo = out);


		template<class T>
		 /**
		  * csvの指定した要素を変更する
		  * @param row   変更する要素の行
		  * @param col   変更する要素の列
		  * @param value 変更後の値
		  */
		void modCsvValue(const int row, const int col, const T value){
			stringstream ss;
			ss << value;
			contents_[row][col] = ss.str();
		}

		/**
		 * 指定した行に追加
		 * @param row         追加する行
		 * @param rowContents 追加する内容
		 */
		void addRow(const unsigned int row, const vector<string> rowContents);

		/**
		 * 最後に行を追加
		 * @param rowContents 追加する内容
		 */
		void pushBack(const vector<string> rowContents);
		/**
		 * 最後に行を追加(1列目に)
		 * @param paramName 1列目に書きこむ内容
		 * @param param     2列目に書きこむ内容
		 */
		void pushBack(const string paramName, const double param);
		/**
		 * 最後に行を追加(1列目に)
		 * @param paramName 1列目に書きこむ内容
		 * @param param     2列目に書きこむ内容
		 */
		void pushBack(const string paramName, const string param);

		void pushBack(const string element);

		/**
		 * 与えられた値で列を追加
		 * @param val 追加する内容
		 * @return 追加後のコンテンツ
		 */
		static vector< vector<string> > pushBackCol(const vector< vector<string> > contents, const double val);

		/**
		 * 与えられた値で列を追加
		 * @param val 追加する内容
		 * @return 追加後のコンテンツ
		 */
		static vector< vector<string> > pushBackCol(const vector< vector<string> > contents, const string val);

		void pushBackCol(const string val);
		void pushBackCol(const double val);

		/**
		 * 保持しているコンテンツを消す
		 */
		void clear();

		/**
		 * 横にcsvをマージする
		 * @param contents1 csvの内容
		 * @param contents2 右側に追加するcsvの内容
		 */
		static vector< vector<string> > mergeCSVToSide(const vector< vector<string> > contents1, const vector< vector<string> > contents2, mergeOption mo = WITH_SPACE);
		/**
		 * 下にcsvをマージする
		 * @param contents1 csvの内容
		 * @param contents2 下に追加するcsvの内容
		 */
		static vector< vector<string> > mergeCSVToBottom(const vector< vector<string> > contents1, const vector< vector<string> > contents2, mergeOption mo = WITH_SPACE);

		static vector< vector<string> > mergeCSVToBottom(const vector< vector< vector<string> > > contents, const mergeOption mo = WITH_SPACE);

		static vector< vector<string> > t(const vector< vector<string> > contents);
		DataIO& t();
	};


}



