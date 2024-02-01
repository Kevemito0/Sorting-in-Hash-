#ifndef H_Htable
#define H_Htable


#include <iostream>
#include <cassert>


struct LogEntry {
    std::string filename;
    int visited;
};

struct FilenameCount {
    std::string filename;
    int count;
};

bool compareLogEntry(const LogEntry& entry1, const LogEntry& entry2) {
    return entry1.filename < entry2.filename;
}

bool compareFilenameCount(const FilenameCount& count1, const FilenameCount& count2) {
    return count1.count < count2.count;
}
bool operator!=(const LogEntry& lhs, const LogEntry rhs){
    return lhs.filename != rhs.filename;
};

class hash{
public:
    void insert(LogEntry& log);
    void retrieve(std::string& filename, LogEntry& log);
    void print();
    int hashFunction(std::string& filename);
    
    LogEntry getByIndex(int index);
    hash(int size = 800000);
    ~hash();
private:
    LogEntry *hash_table;
    int *indexlist;
    int length;
    int HTSize;
};


void hash::insert(LogEntry& log){
    int hashIndex = hashFunction(log.filename);
    int count = 0;
    int increment = 1;

    while (indexlist[hashIndex] == 1
		  && hash_table[hashIndex] != log
		  && count < HTSize / 2)
	{
		count++;
		hashIndex = (hashIndex + increment ) % HTSize;
		increment = increment + 2;
	}

	if (indexlist[hashIndex] != 1)
	{
		hash_table[hashIndex] = log;
		indexlist[hashIndex] = 1;
		length++;
	} else {
        hash_table[hashIndex].visited++;
    } 

}	
int hash::hashFunction(std::string& fname){
    int hashval = 0;
    for(size_t i = 0; i < fname.length();i++){
        char ch = fname[i];
        hashval = (hashval * 31 + ch) % HTSize;
    }

    return hashval;
}
void hash::retrieve(std::string& filename, LogEntry& log){
    int hashIndex = hashFunction(filename);
    if(indexlist[hashIndex] == 1 && hash_table[hashIndex].filename == filename){
        log = hash_table[hashIndex];
    }
}

void hash::print()
{
    for (int i = 0; i < HTSize; i++)
        if (indexlist[i] != 0)
            std::cout << i << "  " << hash_table[i].visited
                 << "  " << hash_table[i].filename << std::endl;
}
hash::hash(int size) : HTSize(size), length(0) {
    hash_table = new LogEntry[HTSize];
    indexlist = new int[HTSize]();  // Initialize all elements to 0
}
hash::~hash() {
    delete[] hash_table;
    delete[] indexlist;
}
#endif