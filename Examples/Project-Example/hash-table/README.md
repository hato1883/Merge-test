# Inlämningsuppgift 1 - IOOPM
# Description
A library for simple data structures in C. 

## Hash table
Simple, generic hash table that maps unique keys to values. Handles conflicts using linked list chaining. Uses modular hashing to find the index of a key-value pair. 

## Linked list
An implementation of a linked list data structure.

## Iterator
An implementation of an iterator data structure. 

# Getting Started
## Organisation
A simplification of the folder setup.

inlupp1/  
├──/src  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── text_file/  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── bible.txt  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── small.txt  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── 16k-words.txt  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── 10k-words.txt  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── 1k-long-words.txt  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── hash_table.c # hash table code  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── linked_list.c # linked list and iterator code  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── linked_list.h # linked list declarations  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── iterator.h # iterator declarations  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── common.h # all common declarations  
├──/test  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├──linked_list_test.c # unit tests for linked list  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├──hash_table_test.c # unit tests for linked list  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├──iterator_test.c # unit tests for linked list  
├── Makefile              # for compiling, testing, running  

## Run tests

Use gprof to generate the profiles described under the [profiling results section](#Profiling-Results).
```
make profile 
```

The coverage of hash_table.c and linked_list.c is 100%. To test the coverage, run:
```
make coverage 
```

## Error handling
All public functions return an exit status that can be used to catch and handle errors. 

## Aliasing
The hash table does not take ownership of the data of its keys and values. Functions that may cause aliasing problems have tagsg that notifies the user of its correct usage. 

# Profiling Results
All runtime was gather by using the `time` command from a ubuntu 24.04 multipass instance.  
Most common function was gathered by running the program with the gcc flag `-O3` and `-pg` and then using gprof to create report. For both 16k words and bible the commands were run multiple times to reduce jitter in results.
## Initial Profiling Results

#### Small.txt
Time (seconds): 0.00 (3% CPU Usage)  
No profiling could be done because the gprof sampling period is 0.01s.

#### 1k-long-words.txt
Time (seconds): 0.00 (9% CPU Usage)  
No profiling could be done because the gprof sampling period is 0.01s.

#### 10k-words.txt
Time (seconds): 0.02 (48% CPU Usage)  
No profiling could be done because the gprof sampling period is 0.01s.

#### 16k-words.txt
Time (seconds): 0.44 (87% CPU Usage)  
We finaly get some results which can be used. However, because the sample resolution is 0.01 and the maximum cumulative run time for any function was 0.2, we clearly see the need to increase the complexity of the file read to get more reliable results.

##### 3 top used functions (across 3 runs) (Can not order due to small sample size)
with varying percent of runtime spent on:`ioopm_hash_table_has_key`  
with varying percent of runtime spent on `ioopm_hash_table_lookup`  
with varying percent of runtime spent on `ioopm_hash_table_insert`  

#### bible.txt (855 136 words)
New Time (seconds): 76.91 (99% CPU Usage)  
##### 3 top used functions (across 3 runs)
with 32.81% of runtime spent on `ioopm_hash_table_has_key`  
with 27.08% of runtime spent on `ioopm_hash_table_lookup`  
with 22.40% of runtime spent on `ioopm_hash_table_insert`  

### Summary
Most input files are too small to get detailed profiling results, but when we do we can clearly see that `ioopm_hash_table_has_key`, `ioopm_hash_table_insert` and `ioopm_hash_table_lookup` are taking alot of our time. 

## Profiling Results After Resize implemntation

#### Small.txt
New Time (seconds): 0.00 (3% CPU usage)
No profiling could be done.

#### 1k-long-words.txt
New Time (seconds): 0.00 (11% CPU usage)
No profiling could be done

#### 10k-words.txt
New Time (seconds): 0.02 (37% CPU usage)
No profiling could be done

#### 16k-words.txt
New Time (seconds): 0.17 (79% CPU usage)
Due to the quicker time the accuracy of result fell even more to the point that everything reports 0 time. We can however see that lookup time and insert time is much lower now that we approach O(1) time complexity depending on hash function.
##### 3 top used functions (across 3 runs)
with 0% of runtime spent on `string_eq`  
with 0% of runtime spent on `string_sum_hash`  
with 0% of runtime spent on `ioopm_hash_table_lookup`  
#### bible.txt (855 136 words)
New Time (seconds): 41.80 (98% CPU Usage)
##### 3 top used functions (across 3 runs) (Percetage is rough due to low sample size)
with ~50% of runtime spent on `ioopm_hash_table_has_key`  
with ~10% of runtime spent on `ioopm_hash_table_insert`  
with ~10% of runtime spent on `ioopm_hash_table_lookup`  

### Summary
Most input files are to small to get detailed profiling results, but when we do we can clearly see that `ioopm_hash_table_has_key`, `ioopm_hash_table_insert` and `ioopm_hash_table_lookup` are still the taking the most time. However, the time to run the largest file, bible.txt, has almost halved. 



