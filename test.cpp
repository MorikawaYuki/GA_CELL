#include <bits/stdc++.h>
using namespace std;
void ConvertToBits(int val, vector<int> &ret)
{
    if (val < 0 || val > 255)
        return;
    while (val > 0)
    {
        ret.push_back(val % 2);
        val /= 2;
    }
    while (ret.size() < 8)
        ret.push_back(0);
}
void ConvertToInt(int &ret, vector<int> &val)
{
    ret = 0;
    for (int i = val.size() - 1; i >= 0; i--)
    {
        ret = ret * 2 + val[i];
    }
}

int main()
{
    int a;
    cin >> a;
    vector<int> v;
    ConvertToBits(a, v);
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
    ConvertToInt(a, v);
    cout << a << endl;
}