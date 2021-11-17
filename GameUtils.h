#pragma once
#include <random>
#include <random>
#include <algorithm>
#include <ctime>

template<class T>
T GetDirectionVector(const T& begin, const T& end)
{
	return T{ begin.x - end.x, end.y - begin.y };
}

void GetRandomNum(int randNums[], int until, int from = 0)
{
    srand(static_cast<unsigned>(time(NULL)));
    for (int i = 0; i < until; i++)
    {
        bool same;
        do
        {
            same = false;
            randNums[i] = rand() % until + from;
            for (int check = 0; check < i; check++)
            {
                if (randNums[i] == randNums[check])
                {
                    same = true;
                    break;
                }
            }
        } while (same);
    }
}

template<class T>
void ShuffleSTL(T& arr)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(arr.begin(), arr.end(), g);
}