#include "defaultseperator.h"
#include<map>
#include<QDebug>
using namespace std;

bool DefaultSeperator::valid(vector<vector<Person> > groups)
{
    for(int item=0;item<_mutex_nums.size();item++)
    {
        int mutex_num=_mutex_nums[item];
        for(int group_index=0;group_index<groups.size();group_index++)
        {
            map<int,int> t;
            for(auto& person:groups[group_index])
            {
                int person_group=person._mutex_group[item];
                if(person_group==INT32_MAX)
                {
                    continue;
                }
                if(t.find(person_group)!=t.end())
                {
                    t[person_group]++;
                    if(t[person_group]>mutex_num)
                    {
                        return false;
                    }
                }
                else
                {
                    t[person_group]=1;
                }
            }
        }
    }
    for(int item=0;item<_together_nums.size();item++)
    {
        int together_num=_together_nums[item];
        map<int,int> item_group_info;
        for(auto& group:groups)
        {
            for(auto& person:group)
            {
                auto person_item_group=person._together_group[item];
                if(person_item_group==INT32_MAX) continue;
                if(item_group_info.find(person_item_group)!=item_group_info.end())
                {
                    item_group_info[person_item_group]++;
                }
                else
                {
                    item_group_info[person_item_group]=1;
                }
            }
        }
        for(int group_index=0;group_index<groups.size();group_index++)
        {
            map<int,int> group_info;
            for(auto& person:groups[group_index])
            {
                auto person_item_group=person._together_group[item];
                if(person_item_group==INT32_MAX) continue;
                if(group_info.find(person_item_group)!=group_info.end())
                {
                    group_info[person_item_group]++;
                }
                else
                {
                    group_info[person_item_group]=1;
                }
            }
            for(auto i=group_info.begin();i!=group_info.end();i++)
            {
                if(i->second<(item_group_info[i->first]-together_num))
                {
                    return false;
                }
            }
        }
    }
    return true;
}
