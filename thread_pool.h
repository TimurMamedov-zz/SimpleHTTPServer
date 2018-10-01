#pragma once
#include "join_threads.h"

/*!
    \brief Thread pool for all connections
*/

class thread_pool
{
public:
    static thread_pool& getInstance()
    {
        static thread_pool pool;
        return pool;
    }

    std::size_t getThreadsSize()
    {
        return threads.size();
    }

    ThreadSave_Queue<std::function<void()> >& getQueue(std::size_t index)
    {
        return queue_vector[index];
    }   

    ~thread_pool()
    {
        done = true;
    }

private:
    thread_pool()
        :done(false), joiner(threads, cond_var_vector)
    {
        try
        {
            auto thread_count = std::thread::hardware_concurrency() - 2;
            if(thread_count <= 0)
                thread_count = 1;
            cond_var_vector = std::vector<std::condition_variable>(thread_count);

            for(auto i = std::size_t{0}; i < thread_count; i++)
            {
                queue_vector.emplace_back(cond_var_vector[i], done);
            }

            for(auto i = std::size_t{0}; i < thread_count; i++)
            {
                threads.emplace_back(std::thread(&thread_pool::worker_thread, this, i));
            }
        }
        catch(...)
        {
            done = true;
            throw;
        }
    }

    std::atomic_bool done;
    std::vector<std::condition_variable> cond_var_vector;
    std::vector<ThreadSave_Queue<std::function<void()> > > queue_vector;
    std::vector<std::thread> threads;

    join_threads joiner;

    void worker_thread(std::size_t index)
    {
        while (true)
        {
           std::function<void()> task;
           if(queue_vector[index].wait_and_pop(task))
           {
               task();
           }
           else
               break;
        }
    }
};
