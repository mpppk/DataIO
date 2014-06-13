/**
 * Dataを簡単に扱うためのクラス
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
		create(contents);
	}

	//コンストラクタ
	DataIO::DataIO(string inputFilePass){
		DataIO::readCsvFile(inputFilePass);
	}

	string DataIO::operator()(const int row, const int col) const{
		return contents_[row][col];
	}

	// vector< vector<string> >から読み込み
	DataIO& DataIO::create(const vector< vector<string> > contents){
		contents_ = contents;
		return *this;
	}

	// 指定したパスのDataファイルを読み込む
	DataIO& DataIO::readFile(const string inputFilePass, const string splitWord){
		cout << "reading Data file (from " << inputFilePass << ")" << endl;
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
			boost::algorithm::split(row, lineText, boost::is_any_of(splitWord));

			//contentsにこの行をpush
			contents_.push_back(row);
		}
		return *this;
	}

	DataIO& DataIO::readCsvFile(const string readFilePass){
		return readFile(readFilePass, ",");
	}

	// Dataの内容を指定したパスに書き込む
	const DataIO& DataIO::writeFile(const string writeFilePass, const string splitWord, const WriteOption wo) const{
		writeFile(writeFilePass, splitWord, contents_, wo);
		return *this;
	}

	const DataIO& DataIO::writeCsvFile(const string writeFilePass, const WriteOption wo) const{
		return writeFile(writeFilePass, ",", wo);
	}

	// ---- static ----
	// Dataの内容を指定したパスに書き込む
	void DataIO::writeFile(const string writeFilePass, const string splitWord, const vector< vector<string> > contents, const WriteOption wo){
		ofstream ofs;
		if(wo == App)	ofs.open(writeFilePass.c_str(), App);
		else			ofs.open(writeFilePass.c_str());

		for (auto content : contents){
			//列数がゼロの行を含む場合は警告
			if(content.size() <= 0){
				cerr << "warning: this Data file contains zero element content\n";
				continue;
			}

			//カンマの数を合わせるために、最初の要素はループの外で書き込む
			ofs << content[0];
			for(int j =1; j < content.size(); j++){
				ofs << "," << content[j];
			}
			ofs << endl;
		}
	}

	//　Dataの内容を１行をvector<string>として返す
	vector< vector<string> > DataIO::toVecRows() const{
		return contents_;
	}

	//　Dataの内容を1列をvector<string>として返す
	vector< vector<string> > DataIO::toVecCols() const{
		return t(contents_);
	}

	//　Dataの内容を保持するvectorを返す
	vector< vector<string> > DataIO::toVec(const ToVecOption option) const{
		if(option == Row)		return toVecRows();
		else if(option == Col)	return toVecCols();
		throw invalid_argument("in DataIO::toVec");
	}

	// Dataの内容を表示する
	const DataIO& DataIO::show() const{
		show(contents_);
		return *this;
	}

	// ---- static ----
	void DataIO::show(const vector< vector<string> > contents){
		vector<string> row;
		for(auto content : contents){
			for(int j = 0; j < content.size(); j++){
				cout << content[j] << " ";
			}
			cout << endl;
		}
	}

	// 指定した場所に行を挿入
	DataIO& DataIO::addRow(const unsigned int row, const vector<string> rowContents){
		auto it = contents_.begin();

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
			return *this;
		}

		// 目的の行までイテレータをずらす
		for (int i = 0; i < row; ++i)	it++;		

		// 行を挿入
		contents_.insert(it, rowContents);
		return *this;
	}

	// 最後に行を追加
	DataIO& DataIO::pushBack(const vector<string> rowContents){
		contents_.push_back(rowContents);
		return *this;
	}

	DataIO& DataIO::pushBack(const string paramName, const double param){
		if (0 < contents_.size() && contents_[0].size() <= 0 ){// Dataファイルが1列以下の場合は例外
			ostringstream os;
			os << "Data colsNum is 1 or less. so you can't pushBack." << endl;
			throw out_of_range(os.str());
		}
		vector<string> tempRowContent;
		tempRowContent.push_back(paramName);
		tempRowContent.push_back(boost::lexical_cast<string>(param));
		contents_.push_back(tempRowContent);
		return *this;
	}

	DataIO& DataIO::pushBack(const string paramName, const string param){
		if (0 < contents_.size() && contents_[0].size() <= 0 ){// Dataファイルが1列以下の場合は例外
			ostringstream os;
			os << "Data colsNum is 1 or less. so you can't pushBack." << endl;
			throw out_of_range(os.str());
		}
		vector<string> tempRowContent;
		tempRowContent.push_back(paramName);
		tempRowContent.push_back(param);
		contents_.push_back(tempRowContent);
		return *this;
	}

	DataIO& DataIO::pushBack(const string element){
		vector<string> tempContent;
		tempContent.push_back(element);
		contents_.push_back(tempContent);
		return *this;
	}

	// ---- static ----
	// valの値で列を追加
	vector< vector<string> > DataIO::pushBackCol(const vector< vector<string> > contents, const string val){
		vector< vector<string> > retContents = contents;
		for (int i = 0; i < retContents.size(); ++i)	retContents[i].push_back(val);
		return retContents;
	}

	// ---- static ----
	// valの値で列を追加
	vector< vector<string> > DataIO::pushBackCol(const vector< vector<string> > contents, const double val){
		return pushBackCol(contents, boost::lexical_cast<string>(val));
	}

	// valの値で列を追加
	DataIO& DataIO::pushBackCol(const string val){
		contents_ = pushBackCol(contents_, val);
		return *this;
	}

	// valの値で列を追加
	DataIO& DataIO::pushBackCol(const double val){
		contents_ = pushBackCol(contents_, val);
		return *this;
	}

	// 保持しているコンテンツを消す
	DataIO& DataIO::clear(){
		contents_.clear();
		return *this;
	}

	// ---- static ----
	// Dataを横にマージする
	vector< vector<string> > DataIO::mergeToSide(const vector< vector<string> > contents1, const vector< vector<string> > contents2, const MergeOption mo){
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

				if(mo == WithSpace)
					contents1Row.push_back("");

				// ２つめのDataを繋げていく
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

			// ２つめのDataを繋げていく
			for (int contents2Col_i = 0; contents2Col_i < contents2Row.size(); ++contents2Col_i){
				brankValues.push_back(contents2Row.at(contents2Col_i));
			}
			newContents.push_back(brankValues);
		}
		return newContents;
	}

	// ---- static ----
	vector< vector<string> > DataIO::mergeToBottom(const vector< vector<string> > contents1, const vector< vector<string> > contents2, const MergeOption mo){
		vector< vector<string> > newContents = contents1;
		// 間にスペースを入れる
		if (mo == WithSpace){
			vector<string> v{""};
			newContents.push_back(v);
		}
		for (int i = 0; i < contents2.size(); ++i)	newContents.push_back(contents2.at(i));
		return newContents;
	}

	// ---- static ----
	vector< vector<string> > DataIO::mergeToBottom(const vector< vector< vector<string> > > contents, const MergeOption mo){
		// マージするコンテンツが0の場合はエラー
		if(contents.size() < 1)	throw invalid_argument("(in Data::IO::mergeToBottom) contents num is zero");
		// マージするコンテンツが1の場合はwarningを出してそのまま返す
		if(contents.size() == 1){
			cerr << "(in DataIO::mergeToBottom) contents num is only one" << endl;
			return contents[0];
		}

		// マージして返す
		vector< vector<string> > retContents;
		retContents = mergeToBottom(contents[0], contents[1], mo);

		for(int i = 2; i < contents.size(); i++)	retContents = mergeToBottom(retContents, contents[i], mo);
		return retContents;
	}

	// ---- static ----
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

	string DataIO::makeDir(string dirPass){
		int cnt = 0;
		string retOutputDirPass;
		while(1){
			ostringstream os;
			if(cnt == 0)	os << dirPass;
			else	os << dirPass << "(" << cnt << ")";
			struct stat sb;
			if (stat(os.str().c_str(), &sb) == -1){
				cout << os.str() << endl;
				if (mkdir(os.str().c_str(),
					S_IRUSR | S_IWUSR | S_IXUSR |         /* rwx */
					S_IRGRP | S_IWGRP | S_IXGRP |         /* rwx */
					S_IROTH | S_IXOTH | S_IXOTH) == 0){
					cout << "dir " << os.str() << " is made." << endl;
					retOutputDirPass = os.str();
					break;
				}else{
					cout << "mkdir error!!" << endl;
					exit(1);
				}
			}
			else{
				// cout << "dir already exist" << endl;
				cnt++;
			}
		}
		return retOutputDirPass;
	}


}

