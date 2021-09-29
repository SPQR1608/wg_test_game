#pragma once

template<class T>
T GetDirectionVector(const T& begin, const T& end)
{
	return T{ begin.x - end.x, end.y - begin.y };
}

void GetRandomNum(int randNums[], int size)
{
    for (int i = 0; i < size; i++)
    {
        bool same;
        do
        {
            same = false;
            randNums[i] = rand() % size + 1;
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