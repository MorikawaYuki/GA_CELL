#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cv;

class GA
{

  public:
    RNG random;
    int pop_size;
    vector<int> population;
    vector<int> fitness;
    double fitness_sum;
    double pcross = 0.8;
    double mutation = 0.01;
    int T;
    int best;
    GA(int pop_size)
    {
        T = 1;
        this->pop_size = pop_size;
        for (int i = 0; i < pop_size; i++)
            population.push_back(random.uniform(0, 256));
    }

    double Fitness(std::vector<int> &img)
    {
        fitness.clear();
        double max_score = -1;
        for (int idx = 0; idx < population.size(); idx++)
        {
            int threshold = population[idx];
            int w0_cnt = 0;
            double u0 = 0, u1 = 0;
            for (auto pixel : img)
            {
                if (pixel < threshold)
                {
                    w0_cnt++;
                    u0 += pixel;
                }
                else
                    u1 += pixel;
            }
            u0 /= w0_cnt;
            u1 /= (population.size() - w0_cnt);
            double w0 = (double)w0_cnt / img.size();
            double w1 = 1 - w0;
            double score = w0 * w1 * (u0 - u1) * (u0 - u1);
            fitness.push_back(score);
            if (score > max_score)
            {
                max_score = score;
                best = idx;
            }
        }
        fitness_sum = 0;
        for (auto score : fitness)
            fitness_sum += score;

        return max_score;
    }
    void Copy()
    {
        vector<int> copy;
        for (int i = 0; i < pop_size; i++)
        {
            double p = random.uniform(0.0, fitness_sum);
            int idx = -1;
            while (p > 0)
            {
                idx++;
                p -= fitness[idx];
            }
            copy.push_back(population[idx]);
        }
        population.swap(copy);
    }
    void Cross()
    {
        int c = pop_size * pcross;
        while (c > 0)
        {
            int idx1 = random.uniform(0, pop_size);
            int idx2 = random.uniform(0, pop_size);
            if (idx1 == idx2)
                continue;
            else
            {
                c -= 2;
                vector<int> bit1, bit2;
                ConvertToBits(population[idx1], bit1);
                ConvertToBits(population[idx2], bit2);
                for (int i = 0; i < 8; i++)
                {
                    if (random.uniform(0, 1) == 0)
                        continue;
                    else
                        swap(bit1[i], bit2[i]);
                }
                ConvertToInt(population[idx1], bit1);
                ConvertToInt(population[idx2], bit2);
            }
        }
    }
    static void ConvertToBits(int val, vector<int> &bit)
    {
        if (val < 0 || val > 255)
            return;
        while (val > 0)
        {
            bit.push_back(val % 2);
            val /= 2;
        }
        while (bit.size() < 8)
            bit.push_back(0);
    }
    static void ConvertToInt(int &ret, vector<int> &bit)
    {
        ret = 0;
        for (int i = bit.size() - 1; i >= 0; i--)
        {
            ret = ret * 2 + bit[i];
        }
    }
    void Mutate()
    {
        for (int i = 0; i < population.size(); i++)
        {
            vector<int> bit;
            ConvertToBits(population[i], bit);
            for (int j = 0; j < bit.size(); j++)
            {
                if (random.uniform(0.0, 1.0) <= mutation)
                    bit[j] = 1 - bit[j];
            }
            ConvertToInt(population[i], bit);
        }
    }
};

int main()
{
    Mat src = imread("cell.jpg", 1);
    Mat dst;
    cvtColor(src, dst, CV_BGR2GRAY);
    vector<int> img;
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
            img.push_back(dst.at<uchar>(i, j));
    }
    GA mGA(10);
    double ret = mGA.Fitness(img);
    while (mGA.T < 10)
    {
        mGA.T++;
        mGA.Copy();
        mGA.Cross();
        mGA.Mutate();
        ret = mGA.Fitness(img);
    }
    int threshold = mGA.population[mGA.best];
    int offset = 0;
    for (int i = 0; i < dst.rows; i++)
    {
        for (int j = 0; j < dst.cols; j++)
        {
            dst.at<uchar>(i, j) = img[offset] < threshold ? 0 : 255;
            offset++;
        }
    }
    imwrite("gray.jpg", dst);
}