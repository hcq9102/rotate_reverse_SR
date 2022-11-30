Local test:
HPX_DIR=/home/chuanqiu/workhpx/hpx/build cmake ..


or:
cmake -DCMAKE_PREFIX_PATH= /home/chuanqiu/workhpx/installhpx -DHPX_DIR= /home/chuanqiu/workhpx/installhpx/lib64/cmake/HPX -DCMAKE_BUILD_TYPE=Release -DHPX_IGNORE_COMPILER_COMPATIBILITY=on .

Results show over 20 threads, the perfromance get better.
there is no issue as test result on rostam.
