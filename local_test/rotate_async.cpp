#include <hpx/modules/algorithms.hpp>
#include <hpx/modules/program_options.hpp>
#include <hpx/modules/testing.hpp>

#include <hpx/execution_base/sender.hpp>
#include <hpx/execution_base/receiver.hpp>
#include <hpx/modules/execution.hpp>
#include <hpx/execution_base/this_thread.hpp>
#include <hpx/local/thread.hpp>
//#include <execution>
#include <hpx/execution/algorithms/sync_wait.hpp>
#include <experimental/executor>
#include <hpx/config.hpp>
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/include/datapar.hpp>
#include <hpx/include/compute.hpp>
#include <hpx/parallel/algorithms/rotate.hpp>


#include <random>
#include <utility>
#include <string>
#include <type_traits>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include <chrono>
#include <iostream>

struct gen_float_t{
    std::mt19937 mersenne_engine {42};
    std::uniform_real_distribution<float> dist_float {1, 1024};
    auto operator()()
    {
        return dist_float(mersenne_engine);
   }
} gen_float{};

//std::size_t threads;
using namespace hpx::execution;
namespace ex = hpx::execution::experimental;
namespace tt = hpx::this_thread::experimental;
std::size_t mid = 0;

template <typename ExPolicy, typename Flag> 
auto rotate_async(ExPolicy policy, std::size_t n, Flag flag)
{
    using allocator_type = hpx::compute::host::block_allocator<float>;
    using executor_type = hpx::compute::host::block_executor<>;
    
    auto numa_domains = hpx::compute::host::numa_domains();
    allocator_type alloc(numa_domains);
    executor_type executor(numa_domains);

    hpx::compute::vector<float, allocator_type> c(n, 0.0, alloc);
    // generate numbers according to gen() function
    if constexpr (hpx::is_parallel_execution_policy_v<std::decay_t<ExPolicy>>){
        hpx::generate(par, c.begin(), c.end(), gen_float_t{});
    }
    else
    {
        hpx::generate(seq, c.begin(), c.end(), gen_float_t{});
    }
 
    // count time begin
    auto t1 = std::chrono::high_resolution_clock::now();
        if constexpr (std::is_same_v<Flag, std::true_type>){
            hpx::rotate(policy, c.begin(), c.begin()+ mid, c.end()) |
                tt::sync_wait();
        }
        else
        {
            hpx::rotate(policy, c.begin(), c.begin()+ mid, c.end()); 
        }   
    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = t2 - t1;
    return diff.count();
}

template <typename ExPolicy, typename Flag>
auto test(ExPolicy policy, std::size_t iterations, std::size_t n, Flag flag)
{
    double avg_time = 0.0;
    for (std::size_t i = 0; i < iterations; i++)
    {
        avg_time += rotate_async<ExPolicy, Flag>(policy, n, flag);
    }
    avg_time /= (double) iterations;
    return avg_time;
}


int hpx_main()
{
    // define launch policy p
    hpx::execution::experimental::explicit_scheduler_executor<
        hpx::execution::experimental::thread_pool_scheduler> p;

    // lanuch executor
    auto exec = hpx::execution::experimental::with_priority(p, hpx::threads::thread_priority::bound);
    
    auto& seq_pol = hpx::execution::seq;
    auto& par_pol = hpx::execution::par;
    auto par_sr_pol = hpx::execution::par.on(exec);
    auto par_task_sr_pol = hpx::execution::par(task).on(exec);

    std::cout << "rotate test Sender&Receiver \n";
    int threadsNum = hpx::get_os_thread_count();
    std::cout << "Threads : " << threadsNum << '\n';
    std::ofstream fout("rotate_local_bigsize_threads=1_res.csv");

    fout << "threadsNum = " << threadsNum <<'\n';
    fout << "n,i,seq,par,par_SR,par_task_SR,seq/par, seq/par_SR, seq/par_task_SR \n";
    for (std::size_t i = 10; i <= 27; i++)
    {
        std::size_t n = std::pow(2, i);
        mid = n/8;
        double SEQ = test(seq_pol, 10, n, std::false_type{});
        double PAR = test(par_pol, 10, n, std::false_type{});
        double Par_SR = test(par_sr_pol, 10, n, std::false_type{});
        double Par_Task_SR = test(par_task_sr_pol, 10, n, std::true_type{});

        // double SEQ = test<decltype(seq_pol), std::false_type>(seq_pol, 10, n, std::false_type{});
        // double PAR = test<decltype(par_pol), std::false_type>(par_pol, 10, n, std::false_type{});
        // double Par_SR = test<decltype(par_sr_pol), std::false_type>(par_sr_pol, 10, n, std::false_type{});
        // double Par_Task_SR = test<decltype(par_task_sr_pol), std::true_type>(par_task_sr_pol, 10, n, std::true_type{});
        
        #if defined(OUTPUT_TO_CSV)
        // std::cout << "N : " << i << '\n';
        // std::cout << "SEQ: " << seq << '\n';
        // std::cout << "PAR: " << par << "\n\n";
        #endif
        
        fout << n << ","
            << i << "," 
            << SEQ << ","
            << PAR  << ","
            << Par_SR <<","
            << Par_Task_SR <<","
            << SEQ/PAR <<","
            << SEQ/Par_SR <<","
            << SEQ/Par_Task_SR << "\n";
    }
    fout.close();
    return hpx::local::finalize();

}

int main(int argc, char *argv[]) {
    namespace po = hpx::program_options;

    po::options_description desc_commandline;
    hpx::init_params init_args;
    init_args.desc_cmdline = desc_commandline;

    return hpx::init(argc, argv, init_args); 
  //return hpx::local::init(hpx_main, argc, argv);
}
