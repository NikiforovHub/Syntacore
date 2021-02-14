// cpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <valarray>
#include <string>
#include <filesystem>



unsigned long long grayencode(unsigned long long g)
{
    return g ^ (g >> 1);
}

unsigned long long weight(unsigned long long vec) {
    unsigned long long weight = 0;
    while (vec) {
        weight += vec & 1;
        vec >>= 1;
    }
    return weight;
}


void writeMy(std::ofstream &out, std::vector<unsigned long long> x) {
    for (int i = 0; i < x.size(); i++) {
        out << i << "\t" << x[i] << "\n";
    }
}

void readMy(std::ifstream& in, std::vector<unsigned long long>& x, unsigned long long& kmax) {
    kmax = 0;
    std::string bin;
    unsigned long long dec, rem, k;
    int a;



    while (in >> bin)
    {
        dec = 0;
        for (int i = bin.size() - 1; i >= 0; i--) {

            a = bin[i] - '0'; // преобразование char в соответствующее число
            dec += a * pow( 2, bin.size() - i - 1);
            

        }

        // вывод на экран
        /*for (int i = 0; i < bin.size(); i++) {
            a = bin[i] - '0'; // преобразование char в соответствующее число
            std::cout << a;
        }
        
        std::cout << "\n";

        std::cout << "dec: " << dec << "\n";

        std::cout << "\n";
        */

        x.push_back(dec);
    }
    kmax = bin.size();
    std::cout << "kmax: " << kmax << "\n";
}




int main()
{ 
    std::vector<unsigned long long> vecs;  
    std::string path = "E:/MATLAB/Syntacore/cpp/cpp/in";
    std::string folderOut = "E:/MATLAB/Syntacore/cpp/cpp/outMy/";
    std::string folderCheck = "E:/MATLAB/Syntacore/cpp/cpp/out/";


    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;

        
        std::ifstream infile(entry.path());
        unsigned long long kmax;
        readMy(infile, vecs, kmax);
        infile.close();

        std::vector<unsigned long long> weights(kmax + 1, 0);
        unsigned long long vec = 0;
        unsigned long long m;

        std::cout << "weights.size():  " << weights.size() << "\n";
        std::cout << "vecs.size()  " << vecs.size() << "\n";

        //вычисляем векторыне суммы и веса
        weights[0] += 1;
        for (unsigned long long i = 0; i < pow(2, vecs.size()) - 1; i++) {
            m = log2((~i & (i + 1))); //номер изменившегося бита в коде Грея
            vec = vec ^ vecs[m];
            weights[weight(vec)] += 1;
        }

        std::string pathOut = folderOut + entry.path().filename().u8string();
        std::cout << pathOut << "\n";


        std::ofstream outfile(pathOut);
        writeMy(outfile, weights);
        outfile.close();

        vecs.clear();


        //сравниваем получившийся файл с тестовым

        std::string pathCheck = folderCheck + entry.path().filename().u8string();
        std::cout <<  pathCheck << "\n";
        std::ifstream i1(pathOut), i2(pathCheck);
        if (!i1 || !i2) { std::cerr << "File not found"; return -1; }
        std::string s1((std::istreambuf_iterator<char>(i1)), std::istreambuf_iterator<char>());
        std::string s2((std::istreambuf_iterator<char>(i2)), std::istreambuf_iterator<char>());
        std::cout << "Files identical: " << std::boolalpha << (s1 == s2) << "\n\n";
        i1.close();
        i2.close();
    }
   
}
