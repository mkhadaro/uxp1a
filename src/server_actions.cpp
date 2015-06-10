#include "../include/server.h"
#include <map>
#include <set>

using namespace std;

void server::setNewInodeData(int inodeNumber, int type, int r, int w, int x, char *name)
{
    strcpy(fs->inodes[inodeNumber].name , name);
	fs->inodes[inodeNumber].type = type;
	fs->inodes[inodeNumber].r = r;
	fs->inodes[inodeNumber].w = w;
	fs->inodes[inodeNumber].x = x;
	fs->inodes[inodeNumber].size = 0;
	fs->inodes[inodeNumber].address = EMPTY_ADDRESS;
}

int_l server::findFreeBlockNumber(int_l blocksNeeded)
{
 	int_l blocksFound = 0;
 	bool previousFree = false;
 	int_l blockNumber = -1;

 	for(int_l i = 0; i < BLOCK_COUNT / 8; i++)
 	{
 		char check = fs->blockBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 0)
 			{
 				if(previousFree == false)
 					blockNumber = i * 8 + j;
 				previousFree = true;
 				blocksFound++;
 				if(blocksFound == blocksNeeded)
 					return blockNumber;

 			}
 			else
 			{
 				previousFree = false;
 				blocksFound = 0;
 			}
 			check >>= 1;
 		}
 	}
 	return -1;
}

void server::setBlockBit(int_l number, int_l amount, bool value)
{
	int_l byteNumber = number / 8;
	int_l bitNumber = number % 8;

	while(amount > 0)
	{
		if(value == true)
			fs->blockBitmap[byteNumber] |= (1 << bitNumber);
		else
			fs->blockBitmap[byteNumber] &= ~(1 << bitNumber);
		bitNumber++;
		bitNumber %= 8;
		if(bitNumber == 0)
			byteNumber++;
		amount--;
	}
}

bool server::canAddToFile(int_l startingBlock, int_l amount)
{
	int_l byteNumber = startingBlock / 8;
	int_l bitNumber = startingBlock % 8;

	while (amount > 0)
	{
		char bit = fs->blockBitmap[byteNumber] & (1 << bitNumber);
		if (bit == (1 << bitNumber))
			return false;

		bitNumber++;
		bitNumber %= 8;
		if(bitNumber == 0)
			byteNumber++;
		amount--;
	}
	return true;
}

int_l server::findFreeInodeNumber()
{
 	for(int_l i = 0; i < INODE_COUNT / 8; i++)
 	{
 		char check = fs->inodeBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 0)
 				return i * 8 + j;
 			check >>= 1;
 		}
 	}
 	return -1;
}

void server::setInodeBit(int number, bool value)
{
	int_l byteNumber = number / 8;
	int_l bitNumber = number % 8;

	if(value == true)
		fs->inodeBitmap[byteNumber] |= (1 << bitNumber);
	else
		fs->inodeBitmap[byteNumber] &= ~(1 << bitNumber);
}

// zwraca numer inoda katalogu - nazwe pliku do utworzenia/usuniecie(okresla typ operacji)
filesName server::checkName(char* name,int INODE_TYPE,int type_of_operation)
{
    if(name[0] !='/')
    {
        printf("[Error]Blędnie podana ścieżka\n");
        filesName k("",-1);
        return k;
    }
    char fileName[16] = {0};
    fileName[ 0] = '/';
    INode* root_inode = &(fs->inodes[0]);
    if(strlen(name) == 1)
    {
        if(root_inode->type == -1 && type_of_operation == CREATE)
        {
            int_l nodeNumebr = findFreeInodeNumber();
            char *file =(char*) malloc(strlen(fileName)+1);
            memcpy(file, fileName, strlen(fileName));
            strcpy(file,fileName);
            setNewInodeData(nodeNumebr,INODE_TYPE, 1, 1, 1,file);
            setInodeBit(nodeNumebr, true);

        }
        filesName k("",-1);
        return k;
    }
    int j = 0;
    int nrInode = 0;
    int dirNode = 0;
    for(int l = 1; l!= strlen(name);++l)
    {
            if(name[l] =='/')
            {
                INode* inode = &(fs->inodes[nrInode]);
                nrInode = checkValueInMap(inode->pointers,fileName,TYPE_DIR);
                if(nrInode == -1)
                {
                    filesName k("",-1);
                    return k;
                }
                inode = &(fs->inodes[nrInode]);
                if(inode->type != TYPE_DIR)
                {
                    filesName k("",-1);
                    return k;
                }
                for( int i = 0; i < sizeof( fileName); i++ )
                        fileName[ i ] = '\0';

                j = 0;
                continue;
            }
            fileName[j] = name[l];
            ++j ;
            if(l + 1 == strlen(name))
            {
                INode* inode = &(fs->inodes[nrInode]);
                dirNode = nrInode;
                nrInode = checkValueInMap(inode->pointers,fileName,INODE_TYPE);
                //jesli nie znaleziony zostal taki nr-inode - >wychodzimy z petli,dla delete wysylamy blad a dla create - tworzymy inode
                if(nrInode == -1)
                        break;
                inode = &(fs->inodes[nrInode]);
                //jesli mamy operacje tworzenia oraz nie mamy o takim samym typie stworzonego obiektu - break;
                if(type_of_operation == CREATE && inode->type != INODE_TYPE)
                    break;
                //jesli mamy operacje usuwania oraz mamy inode o podanym wyzej typie - zwracamy inode dira oraz nazwe pliku do usuniecia;
                if(type_of_operation == DELETE && inode->type == INODE_TYPE)
                {
                            char *nameOfFile =(char*) malloc(strlen(fileName)+1);
                            nameOfFile[strlen(fileName)] = 0;
                            memcpy(nameOfFile, fileName, strlen(fileName));

                            filesName k(nameOfFile,dirNode);
                            return k;
                }
                filesName k ("",-1);
                return k;
            }
    }
    if(type_of_operation == CREATE)
    {
        char *nameOfFile =(char*) malloc(strlen(fileName)+1);
        nameOfFile[strlen(fileName)] = 0;
        memcpy(nameOfFile, fileName, strlen(fileName));
        filesName k (nameOfFile,dirNode);
        return  k;
    }
    filesName k (fileName,-1);
    return  k;

}
// tworzenie pliku w katalogu, sam znajduje numer inoda
/**
@return nowy nrInode - do utworzenia katalogu
*/
int server::updateLinksMapAndCreateFile(int & dirNode)
{
    int nodeNumber = findFreeInodeNumber();
    if(nodeNumber == -1)
        return -1;
    INode* inode = &(fs->inodes[dirNode]);
    for(int i = 0; i < sizeof (inode->pointers)/sizeof(int); ++i)
    {
        if(inode->pointers[i] == 0 && i < 7)
        {
            inode->pointers[i] = nodeNumber;
            break;
        }
        if(i == 7)
        {
            if(inode->pointers[i] == 0)//warunek czy istnieje przypisanie do tego nr inodu, 0 -nr inode roota
            {
                    inode->pointers[i] = nodeNumber;//dowiązuje pomocniczy inode do tablicy inode-ow danego katalogu
                    //tworze pomocniczy inode,który będzie przechowywał dowiązania do kojenych nrInode,należących do katalogu
                    setNewInodeData(nodeNumber, TYPE_HELPER, 1, 1, 1,"help");
                    setInodeBit(nodeNumber, true);
                    inode = &(fs->inodes[nodeNumber]);//pobieram inode helpera
                    return updateLinksMapAndCreateFile(nodeNumber);
            }
            else
                return updateLinksMapAndCreateFile(inode->pointers[i]);
        }

    }
    return nodeNumber;
}

/**
@args struktua - zawiera nr inode katalogu nadrzednego,w którym trzeba zrobic update na tablicy pointerów
                    oraz nazwę pliku do update i dalszego usunięcia
@return  return nr Inode do usuniecia
*/

int server::updateLinksMapAndDeletePointer(filesName & fileStruct,int TYP_INODE)
{
    INode* inode = &(fs->inodes[fileStruct.second]);//get dirInode

    for(int i = 0; i < sizeof(inode->pointers)/sizeof(int) ; ++i)
    {
        if(strcmp(fs->inodes[inode->pointers[i]].name , fileStruct.first) == 0)
        {
           if(fs->inodes[inode->pointers[i]].type == TYP_INODE)
           {
                int inodeFileToDelete = inode->pointers[i];
                inode->pointers[i] = 0;//ustawiam w pointerze na 0 dowiazanie
                return inodeFileToDelete;//zwracam inode danego pliku
           }
        }
        if(fs->inodes[inode->pointers[i]].type == TYPE_HELPER)
        {
            filesName fileTemporaryStruct(fileStruct.first,inode->pointers[i]);
            updateLinksMapAndDeletePointer(fileTemporaryStruct,TYP_INODE);
        }
    }
    return -1;
}


int server::checkValueInMap(int *maps,char* value,int TYP_INODE)
{
    for(int i = 0; i < sizeof(maps)/sizeof(int) ; ++i)
    {
        if(strcmp(fs->inodes[maps[i]].name , value) == 0)
        {
           if(fs->inodes[maps[i]].type == TYP_INODE)
                return maps[i];
        }
        if(fs->inodes[maps[i]].type == TYPE_HELPER)
            checkValueInMap(fs->inodes[maps[i]].pointers,value,TYP_INODE);
    }
    return -1;
}


/**
@args   maps - wskaznik na listę pointerów inode-a
        value - nazwa pliku/katalogu
        TYP_INODE - typ inode który musimy sprawdzić lub przy usunięciu TYP_INODE musimy być równy TYPE_FILE
        type_of_operation - typ operacji - albo sprawdzanie czy dany plik/katalog nalezy do listy pointerów inode-a
@return
        IF type_of_operation == CHECK -> return nr Inode pod którym znalezliśmy dany obiekt typu TYP_INODE else -1
        IF type_of_operation == DELETE -> ustawiamy w tablicy pointerów wskazanie na 0,return nr Inode
*/


int server::getInodeNumber(char *name,int TYP_INODE,int file_type)
{
        filesName dirNodeAndFileName = checkName(name,TYP_INODE,GET_VALUE);
        if(dirNodeAndFileName.second == -1)
            return -1;
        if(file_type == PARENT_DIR)
            return dirNodeAndFileName.second;
        if(file_type == CHILD)
        {
            INode & node = fs->inodes[dirNodeAndFileName.second];
            return checkValueInMap(node.pointers,dirNodeAndFileName.first,TYP_INODE);
        }
}

//tworzy deskryptor pliku z podanym nrInod oraz trybem
int server::createDescription(int & nrInod,int & mode)
{
    for(int i = 0; i < DESCRIPTION_TABLE_SIZE; ++i)
    {
        if(fs->descriprionTable[i].fileDescriptor == 0)
        {
            fs->descriprionTable[i].fileDescriptor = i + 1;
            fs->descriprionTable[i].mode = mode;
            fs->descriprionTable[i].nrInode = nrInod;
            return fs->descriprionTable[i].fileDescriptor;
        }
    }
    return -1;
}

//funkcja sprawdza uprawnienia inode-a pod nrInode
int server::checkMode(int & nrInode,int & mode)
{
    INode & node = fs->inodes[nrInode];
    if(mode == READ)
    {
        if(node.r == 1)
            return 1;
    }
    if(mode == WRITE)
    {
        if(node.w == 1)
            return 1;
    }
    return 0;
}
