// cpp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <valarray>
#include <string>
#include <filesystem>
#include <thread>
#include <mutex>



unsigned long long grayencode(unsigned long long g)
{
    return g ^ (g >> 1);
}

unsigned long long weight(std::valarray<unsigned long long> vec) {
    unsigned long long weight = 0;
    for (unsigned long long i = 0; i < vec.size(); i++) {
        while (vec[i]) {
            weight += vec[i] & 1;
            vec[i] >>= 1;
        }
    }
    
    return weight;
}


void writeMy(std::ofstream &out, std::vector<unsigned long long> x) {
    for (int i = 0; i < x.size(); i++) {
        out << i << "\t" << x[i] << "\n";
    }
}

void readMy(std::ifstream& in, std::vector<std::valarray<unsigned long long> >& x, unsigned long long& kmax) {
    kmax = 0;
    std::string bin;
    std::valarray<unsigned long long> val;
    unsigned long long dec, rem, k, idx;
    unsigned long long kLimit = 32;
    int a;
    

    std::getline(in, bin);
    in.clear();
    in.seekg(0, std::ios::beg);
    val = std::valarray<unsigned long long>(bin.size());




    while (in >> bin)
    {
        
        dec = 0;
        k = 0;
        idx = 0;
        for (int i = bin.size() - 1; i >= 0; i--) {

            a = bin[i] - '0'; // преобразование char в соответствующее число
            dec += a * pow(2, bin.size() - i - idx*kLimit - 1);
            k += 1;
            if (k == kLimit || i == 0) {
                //вывод на экран
                /*std::cerr << "\n";

                std::cerr << "dec: " << dec << "\n";

                std::cerr << "\n";
                */
                val[idx] = dec;
                k = 0;
                idx += 1;
                dec = 0;
            }
        }

        // вывод на экран
        /*for (int i = 0; i < bin.size(); i++) {
            a = bin[i] - '0'; // преобразование char в соответствующее число
            std::cerr << a;
        }
        */

        

        x.push_back(val);
    }
    kmax = bin.size();
    std::cerr << "kmax: " << kmax << "\n";
}

void parallelFunc(int threadNum, unsigned long long start, unsigned long long end, std::mutex &mut_weights, 
    std::vector<std::valarray<unsigned long long> > vecs, std::vector<unsigned long long> &weights) {
    
    unsigned long long m;
    std::valarray<unsigned long long> vec(vecs[0].size());
    std::vector<unsigned long long> weights_internal(weights.size(), 0);

    unsigned long long k = 0;
    unsigned long long startGray = grayencode(start);
    while (startGray) {
        vec = vec ^ (vecs[k] * (startGray & 1));
        startGray >>= 1;
        k++;
    }



    for (unsigned long long i = start; i < end; i++) {
        m = log2((~i & (i + 1))); //номер изменившегося бита в коде Грея
        vec = vec ^ vecs[m];
        weights_internal[weight(vec)] += 1;
    }

    std::lock_guard<std::mutex> lock(mut_weights);
    for (int i = 0; i < weights.size(); i++) {
        weights[i] += weights_internal[i];
    }
}




int main(int argc, char* argv[])
{
    const int numThreads = 12;
    bool checkFiles = false; // нужно ли сверять файлы с эталоном

    std::vector<std::valarray<unsigned long long> > vecs;
    std::filesystem::path path = std::filesystem::path("E:/MATLAB/Syntacore/cpp/cpp/in").make_preferred();
    std::filesystem::path folderOut = std::filesystem::path("E:/MATLAB/Syntacore/cpp/cpp/outMy/").make_preferred();
    std::filesystem::path folderCheck = std::filesystem::path("E:/MATLAB/Syntacore/cpp/cpp/out/").make_preferred();

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cerr << entry.path() << std::endl;


        std::ifstream infile(entry.path());
        unsigned long long kmax;
        readMy(infile, vecs, kmax);
        infile.close();


        std::vector<unsigned long long> weights(kmax + 1, 0);
        std::mutex weights_mutex;

        std::valarray<unsigned long long> vec(vecs[0].size());
        unsigned long long m;

        std::cerr << "weights.size():  " << weights.size() << "\n";
        std::cerr << "vecs.size()  " << vecs.size() << "\n";

        
        
        //вычисляем векторные суммы и веса в нескольких потоках
        unsigned long long start[numThreads];
        unsigned long long end[numThreads];
        unsigned long long numOp = pow(2, vecs.size()) - 1;
        unsigned long long numOpOneProc = numOp / numThreads;
        unsigned long long rem = numOp % numThreads;


        for (auto i = 0; i < numThreads; i++) {
            start[i] = numOpOneProc * i;
        }
        for (auto i = 0; i < numThreads; i++) {
            end[i] = start[i] + numOpOneProc;
        }
        end[numThreads - 1] += rem;


        
        weights[0] += 1;
        std::thread threads[numThreads];
        for (int i = 0; i < numThreads; i++) {
            threads[i] = std::thread(parallelFunc, i, start[i], end[i], std::ref(weights_mutex), vecs, std::ref(weights));
        }
        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
        }
        
        std::filesystem::path pathOut = folderOut / entry.path().filename();
        std::cerr << pathOut << "\n";

        std::ofstream outfile(pathOut);
        writeMy(outfile, weights);
        outfile.close();
        
        vecs.clear();
        
        if (checkFiles) {
            //сравниваем получившийся файл с тестовым
            std::filesystem::path pathCheck = folderCheck / entry.path().filename();
            std::cerr << pathCheck << "\n";
            std::ifstream i1(pathOut), i2(pathCheck);
            if (!i1 || !i2) { std::cerr << "File not found"; return -1; }
            std::string s1((std::istreambuf_iterator<char>(i1)), std::istreambuf_iterator<char>());
            std::string s2((std::istreambuf_iterator<char>(i2)), std::istreambuf_iterator<char>());
            std::cerr << "Files identical: " << std::boolalpha << (s1 == s2) << "\n\n";
            i1.close();
            i2.close();
        }
        else {
            std::cout << "Output file written to " << pathOut << "\n\n";
        }
    }
   
}
