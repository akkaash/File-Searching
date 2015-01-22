# File-Searching

## Comparative Study of File Searching Techniques

Searching sorted files using 
  - In-memory Linear Search
  - In-memory Binary Search
  - Disk-based Linear Search
  - Disk-based Binary Search

### Compiling
Simply run `make` in the directory. 

### Arguments
The following arguments to the program must be passed in the order listed

  1. Search mode: To specify the mode of searching use following options to the executable
    - In-memory Linear Search
      `--mem-lin`
    - In-memory Binary Search
      `--mem-bin`
    - Disk-based Linear Search
      `--disk-lin`
    - Disk-based Binary Search
      `--disk-bin`

  2. Key File: Contains the sorted list of keys to be searched against.
  
  3. Seek File: Contains the list of keys to be search in the Key File
  
