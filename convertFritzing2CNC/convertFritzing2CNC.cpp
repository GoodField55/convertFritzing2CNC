// convertFritzing2CNC.cpp 
//      Friting から出力した基板データ（ガーバーデータ）を
//      oriminpcb.exe に対応したフォーマットに変換する
//
//      ガーバーデータは　c:\tempcnc ディレクトリに保存しておく
//      変換後のファイル名は、変換前ファイル名の頭に　"modify_" が付加される

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = filesystem;

int main()
{
    const char* dir = "c:\\temp_cnc";       // 対象ファイルが保存されているディレクトリ
    const int maxnum = 20;                  // 探索ファイル数上限

    wstring ws[maxnum];
    string s[maxnum];
    int filenum = 0;

    // c:\\temp_cncディレクトリ直下に含まれる全ファイルを出力
    for (const fs::directory_entry& x : fs::directory_iterator(dir)) {
        //cout << x.path() << endl;
        ws[filenum] = x.path();
        string str(ws[filenum].begin(), ws[filenum].end()); // wstring型から　string型へ変換
        s[filenum] = str;
        filenum++;
        if (filenum >= maxnum) break;
    }

    for (int n = 0; n < filenum; n++) {
        //cout << n << ", " << s[n] << endl;

        string fullpath = s[n];
        //string fullpath = "12345678.txt";
        int path_i = fullpath.find_last_of("\\");
        int ext_i = fullpath.find_last_of(".");
        string pathname = fullpath.substr(0, path_i + 1);
        string extname = fullpath.substr(ext_i, fullpath.size() - ext_i);
        string filename = fullpath.substr(path_i + 1, ext_i - path_i - 1);
        string new_filename = pathname + "modify_" + filename + extname;    // 修正後書き込み用ファイル名
        cout << pathname << " , " << filename << " , " << extname << endl;

        ifstream rfs;
        rfs.open(fullpath);
        if (!rfs) {
            cout << "入力ファイルオープン失敗" << fullpath << endl;
            break;
        }
        ofstream wfs;
        wfs.open(new_filename);
        if (!rfs) {
            cout << "出力ファイルオープン失敗" << new_filename << endl;
            break;
        }
        if (fullpath.find("drill") != string::npos) {     // ドリルデータのファイルの場合
            while (true) {
                string buf;
                getline(rfs, buf);
                if (!rfs) break;
                if (buf[0] == 'X') {                           // 先頭文字が　'X' の場合
                    string word1 = buf.substr(7, 6);           // Y座標  小数点以下４桁目を削除
                    string word0 = buf.substr(0, 6);            // X座標  小数点以下４桁目を削除
                    wfs << word0 << word1 << endl;
                    cout << word0 << word1 << endl;
                }
                else {
                    wfs << buf << endl;
                    cout << buf << endl;
                }
            }
        }
        else {                                  // ドリルデータ以外のファイルの場合
            while (true) {
                string buf;
                getline(rfs, buf);
                if (!rfs) break;
                if (!(buf.find("SFA1.0B1.0") != string::npos ||
                        buf.find("LNCOPPER0") != string::npos ||
                        buf.find("LNCONTOUR") != string::npos)){        // 左記文字列を含む場合は何もしない
                    wfs << buf << endl;
                    cout << buf << endl;
                }
            }
        }
        wfs.close();
        rfs.close();
    }
}



// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
