#include <bits/stdc++.h>
using namespace std;
const int n=1e5;
//4 合并两个有序数组
vector<int> merge(vector<int>& left,vector<int>& right){
    vector<int> result;
    result.resize(left.size()+right.size());
    int q=0,m=0;
    for(int i=0;i<result.size();i++){
        if(q<left.size()&&m<right.size()){
            result[i]=(left[q]<=right[m])?left[q++]:right[m++];
        }else if(q<left.size()){
            result[i]=left[q++];
        }else result[i]=right[m++];
    }
    return result;
}
//3 线程任务:排序
void sortt(vector<int>& data){
    sort(data.begin(),data.end());
}
//4 验证/检查是否排序正确
bool is_sorted(const vector<int>& arr) {
    for (int i=1;i<arr.size();i++) {
        if(arr[i]<arr[i-1]) return 0;
    }
    return 1;
}
int main(){
    //1 生成数据
     vector<int> a;

    srand((unsigned int)time(NULL));
    for(int i=0;i<n;i++){
        int num=rand()%n+1;
        a.push_back(num);
    }
    cout<<"初始:"<<endl;
    for(int i=0;i<a.size();i++) cout<<a[i]<<" ";
     cout<<endl;

    //2-1 检测CPU核心数,由此决定开启多少个线程
//2-1-1 尝试获取CPU核心数
    unsigned int num_threads=std::thread::hardware_concurrency();
//2-1-2 处理获取失败的情况
    if(num_threads==0) num_threads=2;

    //2-2 将大数组分割成num_threads个小数组
    vector<vector<int>> arr;//二维 一行一个小数组
    auto arr_size=n/num_threads;
    auto remaind=n%num_threads;
    //对于剩余的数据,一个数组分一个,分完为止
    int zhizhen=0;
    for(int i=0;i<num_threads;i++){
        auto curr_size=arr_size+((i<remaind)?1:0);
        vector<int> a1(curr_size);
        for(int j=0;j<curr_size;j++){
            a1[j]=a[j+zhizhen];
        }zhizhen+=curr_size;
        arr.push_back(a1);
    }
    //2-3 创建线程并启动排序
    vector<thread> threads;
    for(int i=0;i<num_threads;i++){
        threads.emplace_back(sortt,ref(arr[i]));
    }
    for(auto& t:threads){
        if(t.joinable()) t.join();
    }
    //4 合并阶段:将小数组合并
    vector<int> last_result=arr[0];
    for(int i=1;i<num_threads;i++){
        last_result=merge(last_result,arr[i]);
    }
    //
    cout<<"结果:"<<endl;
    for(int i=0;i<last_result.size();i++)
     cout<<last_result[i]<<' ';
     cout<<endl;

    if(is_sorted(last_result)) cout<<"正确!"<<endl;
    else cout<<"错误"<<endl;
        return 0;
}