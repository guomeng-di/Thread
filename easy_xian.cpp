#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

//任务结构体
struct Task{
    int num;
    void(* callback)(int,ll);
    //函数指针，表示任务完成后要调用的函数
    //作用：计算完成后，自动调用这个函数通知结果
};

//使用到的全局变量
vector<thread> threads;//线程
queue<Task> task_queue;//任务队列
mutex mtx;
condition_variable cv;//条件变量,用于线程等待/唤醒
bool stop=0;

//阶乘函数
ll jiecheng(int num){
    if(num==1||num==0) return 1;
    return num*jiecheng(num-1);
}

//核心:线程执行函数
void worker(){
    while(1){
        Task task;
        {
    //1判断状态
            unique_lock<mutex> lock(mtx);
            ////1队列空,不停->等
            while(task_queue.empty()&&!stop)
              cv.wait(lock);
            //2队列空,停->退出
            if(task_queue.empty()&&stop)
              return ;
            //3队列不空,则取任务
            task=task_queue.front();
            task_queue.pop();
        }
    //2计算阶乘
    ll result=jiecheng(task.num);
    //3调用回调函数,说明计算完成,并打印结果
    if(task.callback!=NULL)
        task.callback(task.num,result);
    
    }
}

//往任务队列里添加任务
//把要计算的任务安全放进队列，并唤醒线程去执行！
void add_task(int num,void(*cb)(int,ll)){
    {
        lock_guard<mutex> lock(mtx);
        Task t;
        t.num=num;
        t.callback=cb;
        task_queue.push(t);
    }
    cv.notify_one();
}

//实现回调函数
void print_result(int num,ll result){
    lock_guard<mutex> lock(mtx);
    printf("完成计算!%d的阶乘结果为%lld\n",num,result);
    return ;
}

//主函数
int main(){
    //1创建进程
    for(int i=0;i<10;i++)
      threads.push_back(thread(worker));
      //创建一个线程,执行worker函数
    
    //2添加任务
    for(int i=1;i<=10;i++){
        cout<<"计算 "<<i<<endl;
        add_task(i,print_result);
        this_thread::sleep_for(100ms);
        // 模拟任务提交间隔
    }
    //主线程睡一会儿,目的:等待所有任务完成
    this_thread::sleep_for(2s);
    //任务完成,停止线程池
    {
        lock_guard<mutex> lock(mtx);
        stop=1;
    }
    cv.notify_all();
    //等待所有线程结束
    for(int i=0;i<threads.size();i++){
    if(threads[i].joinable()) threads[i].join();
    }
    return 0;
}