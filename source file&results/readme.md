
HPX  tag:

      chuanqiu@medusa03:/work/chuanqiu/rotate_reverse_SR/hpx$ git log

      commit 598ab05fa299f097da4e07f9cd9ff4dcaec899bb (HEAD -> master, origin/staging, origin/master, origin/HEAD)
      Merge: e572cdfbd1 20e74c2def 0d0fb04e27
      Author: StellarBot <contact@stellar-group.org>
      Date:   Sun Oct 30 13:18:39 2022 +0000

          Merge #6054 #6058          
          
build &test log :

      chuanqiu@rostam1:/work/chuanqiu$ srun -p medusa -N 1 --pty /bin/bash -l
      chuanqiu@medusa00:/work/chuanqiu$ module list

      Currently Loaded Modules:
        1) openmpi/4.1.4   3) gcc/12.2.0             5) hwloc/2.7.1
        2) cmake/3.23.2    4) boost/1.79.0-release   6) git/2.36.1



      chuanqiu@medusa00:/work/chuanqiu$ module unload openmpi/4.1.4 
      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR$ git clone https://github.com/STEllAR-GROUP/hpx.git

      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/hpx/build$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/work/chuanqiu/rotate_reverse_SR/installhpx -DHPX_WITH_FETCH_ASIO=ON -DHPX_WITH_MALLOC=system ..
      
      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/hpx/build$ make -j20
      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/hpx/build$ make install

## rotate test  __test folder
      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/test$ cmake -DCMAKE_BUILD_TYPE=Release -DHPX_DIR=/work/chuanqiu/rotate_reverse_SR/installhpx/lib64/cmake/HPX -DHPX_IGNORE_COMPILER_COMPATIBILITY=on .


      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/test$ make

  
## reverse test  __test_reverse folder
   
   chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/test_reverse$ cmake -DCMAKE_BUILD_TYPE=Release -DHPX_DIR=/work/chuanqiu/rotate_reverse_SR/installhpx/lib64/cmake/HPX -DHPX_IGNORE_COMPILER_COMPATIBILITY=on .


      chuanqiu@medusa00:/work/chuanqiu/rotate_reverse_SR/test_reverse$ make
    
    
    
    
