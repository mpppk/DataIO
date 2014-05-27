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
	 * @class DataIO a b ファイルの入出力及び変更をを行うクラス
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

		/** Dataをマージする際のオプション */
		enum mergeOption {
			/** マージする二つのDataの間にスペースを入れる */
			WITH_SPACE,
			/** マージする二つのDataの間にスペースを入れない */
			NO_SPACE
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
		string& operator()(const int row, const int col);

		/**
		 * 受け取った文字列集合を保持する
		 * @param[in] contents 読み込むData
		 */
		DataIO& create(const vector< vector<string> > contents);

		/**
		 * Dataの内容を保持するvectorを返す
		 */
		vector< vector<string> > toVec();

		/**
		 * Dataの内容を表示する
		 */
		DataIO& show();
		static void show(vector< vector<string> > contents);
		/**
		 * 指定したパスのDataファイルを読み込む
		 * @param readFilePass 読み込むDataファイルのパス
		 */
		DataIO& readFile(const string readFilePass);

		/**
		 * Dataの内容を指定したパスに書き込む
		 * @param writeFilePass 書き込み先のパス
		 * @param wo            書き込む際のオプション.詳細はWriteOptionを参照
		 */
		DataIO& writeFile(const string writeFilePass, WriteOption wo = out);

		/**
		 * Dataの内容を指定したパスに書き込む
		 * @param writeFilePass 書き込み先のパス
		 * @param contents 		書き込む内容
		 * @param wo            書き込む際のオプション.詳細はWriteOptionを参照
		 */
		static void writeFile(string writeFilePass, vector< vector<string> > contents, WriteOption wo = out);


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
		 * 保持しているコンテンツを消す
		 */
		DataIO& clear();

		/**
		 * 横にDataをマージする
		 * @param contents1 Dataの内容
		 * @param contents2 右側に追加するDataの内容
		 */
		static vector< vector<string> > mergeToSide(const vector< vector<string> > contents1, const vector< vector<string> > contents2, mergeOption mo = WITH_SPACE);
		/**
		 * 下にDataをマージする
		 * @param contents1 Dataの内容
		 * @param contents2 下に追加するDataの内容
		 */
		static vector< vector<string> > mergeToBottom(const vector< vector<string> > contents1, const vector< vector<string> > contents2, mergeOption mo = WITH_SPACE);

		static vector< vector<string> > mergeToBottom(const vector< vector< vector<string> > > contents, const mergeOption mo = WITH_SPACE);

		static vector< vector<string> > t(const vector< vector<string> > contents);
		DataIO& t();
	};


}


