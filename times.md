Shelfsort speed test  
times are in ns/item

**test: 2^16 size blocks, 64 cycles**  
shelfsort time: 5.24 / presorted: 1.90  
std::stable_sort time: 12.39 / presorted: 9.29

**test: 2^17 size blocks, 32 cycles**  
shelfsort time: 6.67 / presorted: 1.90  
std::stable_sort time: 15.97 / presorted: 12.15

**test: 2^18 size blocks, 16 cycles**  
shelfsort time: 9.53 / presorted: 1.90  
std::stable_sort time: 18.83 / presorted: 12.63

**test: 2^19 size blocks, 8 cycles**  
shelfsort time: 15.25 / presorted: 2.14  
std::stable_sort time: 24.8 / presorted: 14.6

**test: 2^20 size blocks, 4 cycles**  
shelfsort time: 28.37 / presorted: 2.14  
std::stable_sort time: 33.85 / presorted: 15.49

**test: 2^21 size blocks, 2 cycles**  
shelfsort time: 51.2 / presorted: 2.14  
std::stable_sort time: 53.64 / presorted: 19.7

**test: 2^22 size blocks, 1 cycles**  
shelfsort time: 100.13 / presorted: 2.14  
std::stable_sort time: 88.69 / presorted: 21.21

**test: 2^23 size blocks, 1 cycles**  
shelfsort time: 103.35 / presorted: 2.26  
std::stable_sort time: 92.2 / presorted: 23.0

**test: 2^24 size blocks, 1 cycles**  
shelfsort time: 108.65 / presorted: 2.26  
std::stable_sort time: 92.26 / presorted: 23.54
