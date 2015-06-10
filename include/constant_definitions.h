/**
<--Opis pliku -->
Plik naglowkowy zawiera definicje stalych
wykorzystywanych
w implementacji systemu plikow
**/

 /**klucz odwolan do pamieci wspoldzielonej*/
#define MEMORY_KEY     1091

/**Stale dotyczace systemu plikow --> struktury FileSystem*/
#define INODE_COUNT 512
#define BLOCK_COUNT 1031
#define DATA_BLOCK_COUNT 1024
#define DESCRIPTION_TABLE_SIZE 128
#define BLOCK_SIZE 4096

/**Stala okreslajaca rozmiar nazwy inode-a*/
#define INODE_NAME_SIZE 16

/**Stale dotyczace typow inode-ow
pole 'type' w strukturze INode */
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_HELPER 2

/**Stale okreslajace parametr whence - atrybut okreslajacy sposob
wg ktorego dokonujemy przesuniecia aktualnej pozycji w pliku
przy wykonaniu operacji lseek*/
#define ACTUAL_POSITION 1
#define END_POSITION    2
#define START_POSITION  3

/**Rodzaje uprawnien*/
#define READ 1
#define WRITE 2
#define EXECUTE 3

/**Stala okreslajaca pusty adres pliku/katalogu*/
#define EMPTY_ADDRESS 0xFFFFFFFFFFFFFFFF

/**Typy operacji*/
#define DELETE 1
#define GET_VALUE 1
#define CREATE 2

/**Pomocnicze zmienne potrzebne
(po podaniu pelnej sciezki dostepu)
do wyszukania nr inode  -->
PARENT_DIR - nadrzednego katalogu pliku/lub katalogu docelowego
CHILD - nr inode  pliku/lub katalogu docelowego*/
#define CHILD       1
#define PARENT_DIR  2


#define int_l long long int
