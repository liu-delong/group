#include "seperator.h"
#include <algorithm>
#include <random>
#include<ctime>
Seperator::Seperator()
{

}

vector<vector<Person> > Seperator::seperate(vector<Person> personList,int group_cnt)
{
    _stop=false;
    srand(time(0));
    vector<vector<Person>> res(group_cnt);
    do{
        res.clear();
        res.resize(group_cnt);
        std::random_shuffle(personList.begin(), personList.end());	// 打乱顺序，重新排序（随机序列）
        int person_a_group=personList.size()/group_cnt;
        for(size_t i=0;i<person_a_group*group_cnt;i++)
        {
            res[i/person_a_group].push_back(personList[i]);
        }
        for(size_t i=person_a_group*group_cnt;i<personList.size();i++)
        {
            res[i-person_a_group*group_cnt].push_back(personList[i]);
        }
    }while(!this->valid(res)&&!_stop);
    if(valid((res)))
    {
        return res;
    }
    else
    {
        res.clear();
        return res;
    }

}

void Seperator::stop()
{
    _stop=true;
}
