#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

namespace mc{
	class Log{
	private:
		vector<string> logs_;
	public:
		// ---------- ログ関係 ----------
		// ログを追加
		Log& add(string message);

		// 現在の日時と時刻をログに追加
		Log& addDateTime();

		// 現在のログを取得
		vector<string> toVec() const;
		// ログを消す
		Log& clear();
		// ログを書き込む
		const Log& write(string filePass) const;
	};
}
