/**
 * Csvを簡単に扱うためのクラス
 *
 * ほにゃほにゃ
 */

// ユーザー定義
#include "DataIO.h"

using namespace std;

namespace mc{

	DataIO::DataIO(){}

	//コンストラクタ
	DataIO::DataIO(vector< vector<string> > contents){
		createByVec(contents);
	}

	//コンストラクタ
	DataIO::DataIO(string inputFilePass){
		DataIO::readCsvFile(inputFilePass);
	}

	string& DataIO::operator()(const int row, const int col){
		return contents_[row][col];
	}

	// vector< vector<string> >から読み込み
	void DataIO::createByVec(vector< vector<string> > contents){
		contents_ = contents;
	}

	// 指定したパスのcsvファイルを読み込む
	void DataIO::readCsvFile(string inputFilePass){
		cout << "reading csv file (from " << inputFilePass << ")" << endl;
		filePass_ = inputFilePass;
		ifstream ifs(filePass_.c_str());

		// 読み込みエラーチェック
		if(ifs.fail()) {
			throw runtime_error("File do not exist.file pass is " + inputFilePass);
		}

		//　ファイル読み込み
		string lineText;
		vector<string> row;
		// 1行ずつ読み込んでいく
		while(getline(ifs, lineText)) {
			// タブや改行を取り除く処理
			lineText.erase(remove(lineText.begin(), lineText.end(), '\t'), lineText.end());
			lineText.erase(remove(lineText.begin(), lineText.end(), '\r'), lineText.end());
			lineText.erase(remove(lineText.begin(), lineText.end(), '\n'), lineText.end());
			
			//カンマごとに要素をvectorに格納
			boost::algorithm::split(row, lineText, boost::is_any_of(","));

			//contentsにこの行をpush
			contents_.push_back(row);
		}
	}

	// csvの内容を指定したパスに書き込む
	void DataIO::writeCsvFile(string writeFilePass, WriteOption wo){
		writeCsvFile(writeFilePass, contents_, wo);

	}
	
	// csvの内容を指定したパスに書き込む
	void DataIO::writeCsvFile(string writeFilePass, vector< vector<string> > contents, WriteOption wo){
		ofstream ofs;
		if(wo == app){
			ofs.open(writeFilePass.c_str(), ios::app);
		}
		else{
			ofs.open(writeFilePass.c_str());
		}

		vector<string> row;
		for (int i = 0; i < contents.size(); i++){
			row = contents[i];

			//列数がゼロの行を含む場合は警告
			if(row.size() <= 0){
				cerr << "warning: this csv file contains zero element row\n";
				continue;
			}

			//カンマの数を合わせるために、最初の要素はループの外で書き込む
			ofs << row[0];
			for(int j =1; j < row.size(); j++){
				ofs << "," << row[j];
			}
			ofs << endl;
		}
		// delete(ofs);
	}

	//　csvの内容を保持するvectorを返す
	vector< vector<string> > DataIO::getCsvContents(){
		return contents_;
	}

	// csvの内容を表示する
	void DataIO::showCsvContents(){
		showCsvContents(contents_);
	}

	void DataIO::showCsvContents(vector< vector<string> > contents){
		vector<string> row;
		for(int i = 0; i < contents.size(); i++){
			row = contents[i];
			for(int j = 0; j < row.size(); j++){
				cout << row[j] << " ";
			}
			cout << endl;
		}
	}

	// 指定した場所に行を挿入
	void DataIO::addRow(const unsigned int row, const vector<string> rowContents){
		vector< vector<string> > ::iterator it = contents_.begin();

		// 不正な行が指定された場合は例外を投げる	
		// if(contents_.end() < it){
		if(contents_.size() < row){
			ostringstream os;
			os << "rowNum is too big.(in DataIO::addRow)" << endl
			   << "input rowNum is " << row << ", but Mat's row is " << contents_.size() << endl;
			throw out_of_range(os.str());
		}

		//最後の次の行が指定された場合は,pushBackを呼び出す
		if(contents_.size() == row){
			pushBack(rowContents);
			return;
		}

		// 目的の行までイテレータをずらす
		for (int i = 0; i < row; ++i)	it++;		

		// 行を挿入
		contents_.insert(it, rowContents);
	}

	// 最後に行を追加
	void DataIO::pushBack(const vector<string> rowContents){
		contents_.push_back(rowContents);
	}

	void DataIO::pushBack(const string paramName, const double param){
		if (0 < contents_.size() && contents_[0].size() <= 0 ){// csvファイルが1列以下の場合は例外
			ostringstream os;
			os << "csv colsNum is 1 or less. so you can't pushBack." << endl;
			throw out_of_range(os.str());
		}
		vector<string> tempRowContent;
		tempRowContent.push_back(paramName);
		tempRowContent.push_back(boost::lexical_cast<string>(param));
		contents_.push_back(tempRowContent);
	}

	void DataIO::pushBack(const string paramName, const string param){
		if (0 < contents_.size() && contents_[0].size() <= 0 ){// csvファイルが1列以下の場合は例外
			ostringstream os;
			os << "csv colsNum is 1 or less. so you can't pushBack." << endl;
			throw out_of_range(os.str());
		}
		vector<string> tempRowContent;
		tempRowContent.push_back(paramName);
		tempRowContent.push_back(param);
		contents_.push_back(tempRowContent);
	}

	void DataIO::pushBack(const string element){
		vector<string> tempContent;
		tempContent.push_back(element);
		contents_.push_back(tempContent);
	}

	// valの値で列を追加
	vector< vector<string> > DataIO::pushBackCol(const vector< vector<string> > contents, const string val){
		vector< vector<string> > retContents = contents;
		for (int i = 0; i < retContents.size(); ++i){
			retContents[i].push_back(val);
		}
		return retContents;
	}

	// valの値で列を追加
	vector< vector<string> > DataIO::pushBackCol(const vector< vector<string> > contents, const double val){
		return pushBackCol(contents, boost::lexical_cast<string>(val));
	}

	// valの値で列を追加
	void DataIO::pushBackCol(const string val){
		contents_ = pushBackCol(contents_, val);
	}

	// valの値で列を追加
	void DataIO::pushBackCol(const double val){
		contents_ = pushBackCol(contents_, val);
	}

	// 保持しているコンテンツを消す
	void DataIO::clear(){
		contents_.clear();
	}

	// csvを横にマージする
	vector< vector<string> > DataIO::mergeCSVToSide(const vector< vector<string> > contents1, const vector< vector<string> > contents2, mergeOption mo){
		vector< vector< string > > newContents;
		
		// contents1の行ごとに処理を行う
		for (int contents1Row_i = 0; contents1Row_i < contents1.size(); ++contents1Row_i){
			// contents2の行数をcontents1の処理している行が超えた場合は処理を終える
			if(contents2.size() <= contents1Row_i){
				newContents.push_back(contents1.at(contents1Row_i));
				continue;
			}else{
				vector<string> contents1Row, contents2Row;
				contents1Row = contents1.at(contents1Row_i);
				contents2Row = contents2.at(contents1Row_i);

				if(mo == WITH_SPACE)
					contents1Row.push_back("");

				// ２つめのcsvを繋げていく
				for (int contents2Col_i = 0; contents2Col_i < contents2Row.size(); ++contents2Col_i){
					contents1Row.push_back(contents2Row.at(contents2Col_i));
				}
				newContents.push_back(contents1Row);
			}
		}

		// contents2の方が行数が多い場合,残りを追加する
		for (int contents2Row_i = contents1.size(); contents2Row_i < contents2.size(); ++contents2Row_i){
			vector<string> contents2Row = contents2.at(contents2Row_i);

			// contents2の左を埋めるための、空白のvector<string>を作成
			vector<string> brankValues;
			for (int contents1Col_i = 0; contents1Col_i < contents1[0].size(); ++contents1Col_i){
				brankValues.push_back("");
			}
			brankValues.push_back("");

			// ２つめのcsvを繋げていく
			for (int contents2Col_i = 0; contents2Col_i < contents2Row.size(); ++contents2Col_i){
				brankValues.push_back(contents2Row.at(contents2Col_i));
			}
			newContents.push_back(brankValues);
		}
		return newContents;
	}

	vector< vector<string> > DataIO::mergeCSVToBottom(const vector< vector<string> > contents1, const vector< vector<string> > contents2, mergeOption mo){
		vector< vector<string> > newContents = contents1;
		// 間にスペースを入れる
		if (mo == WITH_SPACE){
			vector<string> v;
			v.push_back("");
			newContents.push_back(v);
		}
		for (int contents2Row_i = 0; contents2Row_i < contents2.size(); ++contents2Row_i){
			// cout << "dbg:mergeCSVToBottom contents2 row: " << contents2Row_i << endl;
			newContents.push_back(contents2.at(contents2Row_i));
		}
		return newContents;
	}

	vector< vector<string> > DataIO::mergeCSVToBottom(const vector< vector< vector<string> > > contents, const mergeOption mo){
		// マージするコンテンツが0の場合はエラー
		if(contents.size() < 1){
			throw invalid_argument("(in Csv::IO::mergeCSVToBottom) contents num is zero");
		}
		// マージするコンテンツが1の場合はwarningを出してそのまま返す
		if(contents.size() == 1){
			cerr << "(in DataIO::mergeCsvToBottom) contents num is only one" << endl;
			return contents[0];
		}

		// マージして返す
		vector< vector<string> > retContents;
		retContents = mergeCSVToBottom(contents[0], contents[1], mo);

		for(int i = 2; i < contents.size(); i++)	retContents = mergeCSVToBottom(retContents, contents[i], mo);
		return retContents;
	}

	// 転置する
	// すべての行が同じ数の要素を持っている前提
	vector< vector<string> > DataIO::t(const vector< vector<string> > contents){
		vector< vector<string> > retContents;

		for (int orgCol_i = 0; orgCol_i < contents[0].size(); ++orgCol_i){
			vector<string> content;
			for (int orgRow_i = 0; orgRow_i < contents.size(); ++orgRow_i){
				content.push_back(contents[orgRow_i][orgCol_i]);
			}
			retContents.push_back(content);
		}
		return retContents;
	}

	DataIO& DataIO::t(){
		contents_ = t(contents_);
		return *this;
	}
}

