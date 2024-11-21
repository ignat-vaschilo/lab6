#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <string>
#include <sstream>
#include <type_traits>
#include <iomanip>

using namespace std;

struct bankDeposit {
    string name;
    double amount;
    string currencyType;
    double rate;
    double amountAfterYear;
};

vector<string> split(string str, char delim) {
    vector<std::string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

void writeToFile(string fileName, bool binary, vector<bankDeposit> deposits) {
    ofstream file;
    if (binary) {
        file.open(fileName, ios::binary | ios::app);
    } else {
        file.open(fileName, ios::app);
    }
    file << fixed << setprecision(2); 
    for (bankDeposit deposit: deposits) {
        if (binary) {
            file.write((char*)&deposit, sizeof(deposit));
        } else {
            file << deposit.name << ' ' << deposit.amount << ' ' << deposit.currencyType << ' '
                 << deposit.rate << ' ' << deposit.amountAfterYear << '\n';
        }
    }
    file.close();
}

void readFromFile(string fileName, bool binary) {
    ifstream file;
    if (binary) {
        file.open(fileName, ios::binary);
    } else {
        file.open(fileName);
    }

    if (!binary) {
        string line;
        while (getline(file, line)) {
            vector<string> tokens = split(line, ' ');
            
            if (tokens.size() != 5) {
                cout << "Ошибка: неверное количество значений в строке" << endl;
                continue;
            }

            try {
                string name = tokens[0];
                double amount = stod(tokens[1]);
                string currencyType = tokens[2];
                double rate = stod(tokens[3]);
                double amountAfterYear = stod(tokens[4]);
                
                cout << line << endl;
            }
            catch (const exception& e) {
                cout << "Ошибка преобразования типов в строке: " << e.what() << endl;
                continue;
            }
        }
    } else {
        bankDeposit deposit;
        while (file.read((char*)&deposit, sizeof(deposit))) {
            cout << deposit.name << ' ' << deposit.amount << ' ' << deposit.currencyType << ' '
                 << deposit.rate << ' ' << deposit.amountAfterYear << '\n';
        }
    }
    file.close();
}

void clearFile(string fileName) {
    ofstream file(fileName, ofstream::out | ofstream::trunc);
    file.close();
}

vector<bankDeposit> getRandomDeposits(int cnt, map<int, bankDeposit> depositMap) {
    vector<int> used;
    vector<bankDeposit> deposits;
    while (used.size() != cnt) {
        int rndm = (rand() % depositMap.size()) + 1;
        if (count(used.begin(), used.end(), rndm) <= 0) {
            depositMap[rndm].amountAfterYear = depositMap[rndm].amount + (depositMap[rndm].amount * depositMap[rndm].rate / 100);
            deposits.push_back(depositMap[rndm]);
            used.push_back(rndm);
        }
    }
    return deposits;
}

int main(int argc, char* argv[]) {
    cout << fixed;
    cout << setprecision(2);
    srand((unsigned) time(0));
    string mode = argv[1];
    bool binary = false;
    if (string(argv[2]) == "binary") {
        binary = true;
    }
    string fileName = argv[3];

    map<int, bankDeposit> depositMap;

    depositMap[1] = {"dep1", 1000000.0, "RUB", 4.5, 0.0};
    depositMap[2] = {"dep2", 1000.0, "USD", 5.0, 0.0};
    depositMap[3] = {"dep3", 12000.0, "EURO", 4.5, 0.0};

    if (mode == "write") {
        int cnt;
        cout << "Введите количество депозитов: ";
        cin >> cnt;
        if (cnt > depositMap.size()) {
            cout << "Депозитов меньше, чем вы ввели";
        }
        vector<bankDeposit> deposits = getRandomDeposits(cnt, depositMap);
        writeToFile(fileName, binary, deposits);
        return 0;
    } else if (mode == "read") {
        readFromFile(fileName, binary);
    } else {
        clearFile(fileName);
    }
}