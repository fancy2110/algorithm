/**
 *  哲学家吃饭问题，竞争条件下共享对象的互斥问题。
 *  限制条件：
 *  1、每个哲学家只能那自己左右的筷子 
 *  2、每个哲学家需要两双筷子才能吃饭
 *  3、每个哲学家吃饭时间随机
 *  4、不能让任何一个哲学家永远吃不上饭
 *  5、筷子数量与哲学家数量相等
 *  寻找合适的并发控制策略模拟上述问题
 */
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define random(x) (rand()%x)

//每个人需要的获取的饭

char* lock_state = NULL;

int package_size = 100000;
int person_count = 1000;

struct person {
    int index;
    int package;
    pthread_mutex_t** chopsticks; 
    pthread_cond_t ** conds;
};

void print_lock_state(int index)
{
    printf("Philosopher:%d, %s\n",index, lock_state);
}
/**
 *  哲学家
 *  @param i int , 哲学家编号, 可以拿i 以及 i+1, 两支筷子
 */
void * job(void * args) 
{
    struct person * philosophier = (struct person *) args;
    int index = philosophier->index;
    pthread_mutex_t** chopsticks = philosophier->chopsticks;

    while (philosophier->package <= package_size) 
    {
        // pthread_mutex_t* left = chopsticks[index];                 
        // int left_state = EBUSY;
        // do {
        //     int left_state = pthread_mutex_trylock(left);
        // } while (left_state == EBUSY);
        // lock_state[index] = 'L';

        // int next_index = (index + 1) % person_count;
        // pthread_mutex_t * right = chopsticks[next_index];
        // int right_state = pthread_mutex_trylock(right);
        // if (right_state == EBUSY)
        // {
        //     lock_state[index] = '0';
        //     pthread_mutex_unlock(left);
        //     continue;
        // }
        // lock_state[next_index] = 'L';

         int number = 1;
        // int cost = 0;
        //printf("Philosopher:%d, state:%s, eat:%d, wait:%d\n", index,lock_state, number, cost);
        //std::cout << "Philosopher " << index << ", eat:" << number << " and cost:" << cost << std::endl;
        //sleep(cost);
        philosophier->package += number;

        // lock_state[next_index] = '0';
        // pthread_mutex_unlock(right);
        // lock_state[index] = '0';
        // pthread_mutex_unlock(left);
    }

    if (philosophier->package > package_size) {
        philosophier->package = package_size;
    }

    return (void *) 0;
}


int main(int argc, char ** argv) {
    int count = person_count;

    pthread_mutex_t ** chopsticks = new pthread_mutex_t*[count];
    pthread_cond_t ** conds = new pthread_cond_t*[count];

    lock_state = new char[count+1];
    //memset(chopsticks, 0, sizeof(pthread_mutex_t *) / sizeof(char) * count);
    memset(lock_state, '0', sizeof(char) * count);
    lock_state[count] = '\0';

    pthread_t ** tids = new pthread_t* [count];
    struct person ** philosophiers = new struct person*[count];
    for(int i=0; i < count; i++)
    {
        pthread_mutex_t * p = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(p, NULL);
        chopsticks[i] = p;

        tids[i] = new pthread_t;
        philosophiers[i] = new struct person;

        conds[i] = new pthread_cond_t; 
        pthread_cond_init(conds[i], NULL);
    }

    for (int i=0; i < count; i++) {
        pthread_t * tid = tids[i];
        struct person * philosophier = philosophiers[i];
        philosophier->index = i;
        philosophier->package = 0;
        philosophier->chopsticks = chopsticks;
        philosophier->conds = conds;
        int err = pthread_create(tid, NULL, job, philosophier);
        if (err != 0) 
        {
            printf("Create thread error");
            break;
        }
    }

    for(int i=0; i < count; i++)
    {
        pthread_join(*tids[i], NULL);
        struct person * philosophier = philosophiers[i];
        //printf("Philosophier%d, package:%d!\n", i, philosophier->package);
    }

    for(int i =0; i < person_count; i++)
    {
        delete chopsticks[i];
        delete tids[i];
        pthread_cond_destroy(conds[i]);
        delete conds[i];
        delete philosophiers[i];
    }

    delete[] philosophiers;
    delete[] tids;
    delete[] conds;
    delete[] lock_state;
    delete[] chopsticks;
    //printf("End!\n");
    return 0;
}