// cpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <valarray>
#include <string>
#include <filesystem>



int gray(int n, int* m, int depth)
{
    int i, t = (1 << (depth - 1));

    if (depth == 0) {
        m[0] = 0;
    }else {
        //массив хранит десятичные записи двоичных слов
        for (i = 0; i < t; i++)
            m[t + i] = m[t - i - 1] + (1 << (depth - 1));
    }
    if (depth != n)
        gray(n, m, depth + 1);

    return 0;
}


unsigned int grayencode(unsigned int g)
{
    return g ^ (g >> 1);
}

unsigned int weight(unsigned int vec) {
    unsigned int weight = 0;
    while (vec) {
        weight += vec & 1;
        vec >>= 1;
    }
    return weight;
}


void writeMy(std::ofstream &out, std::vector<unsigned int> x) {
    for (int i = 0; i < x.size(); i++) {
        out << i << "\t" << x[i] << "\n";
    }
}

void readMy(std::ifstream& in, std::vector<unsigned int>& x, unsigned int& kmax) {
    kmax = 0;
    std::string bin;
    unsigned int dec, rem, k;
    unsigned int a;



    while (in >> bin)
    {
        dec = 0;
        //std::cout << "bin: " << bin << "\n";
        //std::cout << "nib: ";
        for (int i = bin.size() - 1; i >= 0; i--) {

            a = bin[i] - '0'; // преобразование char в соответствующее число
            dec += a * pow(2, bin.size() - i - 1);
            

        }

        for (int i = 0; i < bin.size(); i++) {
            a = bin[i] - '0'; // преобразование char в соответствующее число
            std::cout << a;
        }

        std::cout << "\n";

        std::cout << "dec: " << dec << "\n";

        std::cout << "\n";

        x.push_back(dec);

        /*dec = 0;
        k = 0;
        while (bin > 0) {
            rem = bin % 10;
            dec += rem * pow(2, k);
            bin = bin / 10;
            k += 1;
        }

        x.push_back(dec);
        kmax = fmax(k, kmax);*/
    }
    kmax = bin.size();
    std::cout << "kmax: " << kmax << "\n";
}




int main()
{
    /*std::cout << "Hello World!\n";

    std::ifstream infile("input.txt");

    unsigned int bin;
    unsigned int dec, rem, k, kmax;
    std::vector<unsigned int> decs;
    while (infile >> bin)
    {
        std::cout << bin << "\n";
        dec = 0;
        k = 0;
        while (bin > 0) {
            rem = bin % 10;
            dec += rem*pow(2, k);
            bin = bin / 10;
            k += 1;
        }
        std::cout << dec << "\n";
        decs.push_back(dec);
        kmax = fmax(k, kmax);


    }

    infile.close();

    std::cout <<  "\n";
    std::cout << "\n";

    for (int n : decs) {
        std::cout << n << "\n";
    }

    std::cout << "\n";
    std::cout << "\n";
    
    std::cout << "decs[0] ^ decs[1]:  "  << (decs[0] ^ decs[1]) << "\n";

    std::cout << "\n";
    std::cout << "\n";
    

    int gcode[8];

    gray(3, gcode, 0);

    std::cout << "graycode for 3 bits:" << "\n";
    for (int n : gcode) {
        std::cout << n << "\n";
    }

    std::cout << "\n";
    std::cout << "\n";
    

    unsigned int gchange;

    unsigned int g4 = grayencode(3);
    unsigned int g5 = grayencode(4);

    std::cout << "grayencode(3):  " << g4 << "\n";
    std::cout << "grayencode(4):  " << g5 << "\n";
    gchange = ~3 & (3 + 1);
    std::cout << "gchange 2-3:  " << gchange << "\n";
    std::cout << "g2 ^ gchange  " << (g4 ^ gchange) << "\n";



    */

    /*
    
    std::vector<unsigned int> vecs;
    std::ifstream infile("tst_24_8.txt");

    unsigned int kmax, kconstrain;

    unsigned int bin;
    infile >> bin;
    std::cout << bin << "\n";
    infile >> bin;
    std::cout << bin << "\n";
    */

    ////////// main code
    
    /*std::string path = "E:/MATLAB/Syntacore/cpp/cpp/out";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;




    }
    */
    
    
     
    
    std::vector<unsigned int> vecs;
    std::ifstream infile("tst_24_8.txt");
    unsigned int kmax, kconstrain;

    readMy(infile, vecs, kmax);
    infile.close();

    std::vector<unsigned int> weights(kmax + 1, 0);

    unsigned int vec = 0;
    unsigned int m;

    std::cout << "weights.size():  " << weights.size() << "\n";
    std::cout << "vecs.size()  " << vecs.size() << "\n";

    kconstrain = pow(2, kmax) - 1;
    std::cout << "kconstrain " << kconstrain << "\n";


    weights[0] += 1;
    for (unsigned int i = 0; i < pow(2, vecs.size()) - 1; i++) {
        m = log2( (~i & (i + 1))   );
        //std::cout << "m " << i << ": "<< m << "\n";
        vec = vec ^ vecs[m];

        //std::cout << "weight(vec) " << weight(vec) << "\n";
        weights[weight(vec)] += 1;
    }
    
    std::ofstream outfile("output.txt");
    writeMy(outfile, weights);
    outfile.close();
    

    
    ///////////////////////////////

    /*unsigned int m;
    for (unsigned int i = 0; i < pow(2, 3); i++) {
        m = ~i & (i + 1);
        std::cout << m << "\n";
        std::cout << log2(m) << "\n";
    }

    std::cout << "\n";
    std::cout << "\n";
    unsigned int gchange;
    gchange = ~3 & (3 + 1);

    std::cout << gchange << "\n";
    */








}




// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
