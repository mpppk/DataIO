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
#include <sys/stat.h> // mkdir

//boostのライブラリ
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace mc{
	/**
	 * @class DataIO a b ファイルの入出力及び変更をを行うクラス
	 * @remarks 
	 */
	class DataIO{
	public:
		enum ToVecOption{
		    Row,
		    Col,
		    ToVecOptionNum
		};

	private:
		string filePass_;
		vector< vector<string> > contents_;
		vector<string> headerRow_;
		vector<string> headerCol_;
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
			Out,
			/** 書きだし先のファイルの末尾に追加*/
			App,
			/** WriteOptionの最大値*/
			WriteOptionNum
		};

		/** Dataをマージする際のオプション */
		enum MergeOption {
			/** マージする二つのDataの間にスペースを入れる */
			WithSpace,
			/** マージする二つのDataの間にスペースを入れない */
			NoSpace
		};

		DataIO();

		/**
		 * stringひとつをDataのひとつの要素として読み込む
		 * @param[in] contents 読み込むData
		 */
		DataIO(const vector< vector<string> > contents);

		/**
		 * 指定されたパスのDataファイルを読み込む
		 * @param[in] tempfilePass 読み込むDataファイルのパス
		 */
		DataIO(const string tempfilePass);

		/**
		 * 指定した要素を返す
		 * @param row 行
		 * @param  col 列
		 * @return 要素の値
		 */
		string operator()(const int row, const int col) const;

		/**
		 * 受け取った文字列集合を保持する
		 * @param[in] contents 読み込むData
		 */
		DataIO& create(const vector< vector<string> > contents);

		/**
		 * Dataの内容を１行をvector<string>として返す
		 */
		vector< vector<string> > toVecRows() const;

		/**
		 * Dataの内容を１列をvector<string>として返す
		 */
		vector< vector<string> > toVecCols() const;

		/**
		 * Dataの内容を保持するvectorを返す
		 */
		vector< vector<string> > toVec(const ToVecOption option) const;

		/**
		 * Dataの内容を表示する
		 */
		const DataIO& show() const;
		static void show(const vector< vector<string> > contents);
		/**
		 * 指定したパスのDataファイルを読み込む
		 * @param readFilePass 読み込むDataファイルのパス
		 */
		DataIO& readFile(const string readFilePass, const string splitWord);
		DataIO& readCsvFile(const string readFilePass);

		/**
		 * Dataの内容を指定したパスに書き込む
		 * @param writeFilePass 書き込み先のパス
		 * @param wo            書き込む際のオプション.詳細はWriteOptionを参照
		 */
		const DataIO& writeFile(const string writeFilePass, const string splitWord, const WriteOption wo = Out) const;
		const DataIO& writeCsvFile(const string writeFilePass, const WriteOption wo = Out) const;

		/**
		 * Dataの内容を指定したパスに書き込む
		 * @param writeFilePass 書き込み先のパス
		 * @param contents 		書き込む内容
		 * @param wo            書き込む際のオプション.詳細はWriteOptionを参照
		 */
		static void writeFile(const string writeFilePass, const string splitWord, const vector< vector<string> > contents, const WriteOption wo = Out);
		static void writeColFile(const string writeFilePass, const string splitWord, const vector<string> contents, const WriteOption wo = Out);


		template<class T>
		 /**
		  * Dataの指定した要素を変更する
		  * @param row   変更する要素の行
		  * @param col   変更する要素の列
		  * @param value 変更後の値
		  */
		DataIO& modValue(const int row, const int col, const T value){
			stringstream ss;
			ss << value;
			contents_[row][col] = ss.str();
			return *this;
		}

		/**
		 * 指定した行に追加
		 * @param row         追加する行
		 * @param rowContents 追加する内容
		 */
		DataIO& addRow(const unsigned int row, const vector<string> rowContents);

		/**
		 * 最後に行を追加
		 * @param rowContents 追加する内容
		 */
		DataIO& pushBack(const vector<string> rowContents);
		/**
		 * 最後に行を追加(1列目に)
		 * @param paramName 1列目に書きこむ内容
		 * @param param     2列目に書きこむ内容
		 */
		DataIO& pushBack(const string paramName, const double param);
		/**
		 * 最後に行を追加(1列目に)
		 * @param paramName 1列目に書きこむ内容
		 * @param param     2列目に書きこむ内容
		 */
		DataIO& pushBack(const string paramName, const string param);

		DataIO& pushBack(const string element);

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

		DataIO& pushBackCol(const string val);
		DataIO& pushBackCol(const double val);

		/**
		 * 指定した行/列を切り出す。その部分はデータから削除される
		 */
		vector<string> cutUpRow(const int row);
		vector<string> cutUpCol(const int col);

		/**
		 * 指定した行/列をヘッダに指定する。データとしての管理はしなくなる
		 */
		DataIO& setHeaderRow(const int row = 0);
		DataIO& setHeaderCol(const int col = 0);

		string getHeaderRow(const int index);
		string getHeaderCol(const int index);

		/**
		 * 保持しているコンテンツを消す
		 */
		DataIO& clear();

		/**
		 * 横にDataをマージする
		 * @param contents1 Dataの内容
		 * @param contents2 右側に追加するDataの内容
		 */
		static vector< vector<string> > mergeToSide(const vector< vector<string> > contents1, const vector< vector<string> > contents2, const MergeOption mo = WithSpace);
		/**
		 * 下にDataをマージする
		 * @param contents1 Dataの内容
		 * @param contents2 下に追加するDataの内容
		 */
		static vector< vector<string> > mergeToBottom(const vector< vector<string> > contents1, const vector< vector<string> > contents2, const MergeOption mo = WithSpace);

		static vector< vector<string> > mergeToBottom(const vector< vector< vector<string> > > contents, const MergeOption mo = WithSpace);

		static vector< vector<string> > t(const vector< vector<string> > contents);

		static string makeDir(string dirPass);
	};


}



