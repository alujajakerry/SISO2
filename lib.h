void readsector(int, char*);
void writesector(int, char*);
void putchar(char);
char getchar();
void printstring(char*);
void printnumber(int);
void readstring(char*);
void readfile(char*, char*);
void writefile(char*, char*, int);
void deletefile(char*);
void exit();
void executeprogram(char*, int,char*);
void allow_preemption();
int mod(int,int);
int div(int,int);
void setvideo(int);
void setpixel(int,int,int);
void clearscreen();
void setcursor(int,int);
void setchar(char,char,int,int);
void setstring(char*,char,int,int);
void getnumberstring(char*,int);
void mkdir(char* path);
void Format();
void Remove(char* path);
void List();
void PrintList(char sectorNumber, int depth, char* mapsector);
int StringCompare(char* nombre, char* name, int size);
void strTok(char* path);
void innerRemove(char sectorNumber, char* map);
void guardarArchivo(char* path, char*content, int size);
void leerArchivo(char* path);