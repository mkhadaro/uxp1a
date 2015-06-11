/**
<--Opis pliku -->
Plik naglowkowy zawiera definicje stalych
wykorzystywanych
w implementacji systemu plikow
**/

 /**klucz odwolan do pamieci wspoldzielonej*/
#define MEMORY_KEY     1013

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

/**Stale okreslajace *pathname
potrzebny do komunikacji przez FIFO
*/
#define SERVER_FIFO "tmp/server"
#define CLIENT_FIFO "tmp/client_"

/**
Stale okreslajace typy komunikatow otrzymanych
w zapytaniu klienta
*/
#define OPEN_ACT 1
#define UNLINK_ACT 2
#define MKDIR_ACT 3
#define CREATE_ACT 4
#define READ_ACT 5
#define WRITE_ACT 6
#define LSEEK_ACT 7
#define CLOSE 8

/**Stale okreslajace typy bledow,
obslugiwanych przez system plikow*/
#define ERROR_NO_FILE                 -1
#define ERROR_FILE_ALREADY_EXIST      -2
#define ERROE_DIR_ALREDY_EXIST        -3

#define ERROR_NO_FREE_INODE           -4

#define ERROR_NO_OPEN_FILE            -5

/**Bledy zwiazane z manipulacja na
pamieci współdzielonej*/
#define ERROR_ALLOCATE_MEMORY -1

