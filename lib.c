#include "string.h"
#define BASEOFFSET 0x40

void setup()
{
	seti();
}

int mod(int a, int b)
{
        while(a>=b)
                a=a-b;
        return a;
}

int div(int a, int b)  
{               
        int q=0;
        while (q*b<=a)
                q++;
        return q-1;
}               

void readsector(int sectornumber, char* buffer)
{
        int sec,head,cyl;
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);
        readsect(buffer,sec,head,cyl);
}

void writesector(int sectornumber, char* buffer)
{
        int sec,head,cyl;
        sec=mod(sectornumber,0x12)+1;
        head=mod(div(sectornumber,0x12),2);
        cyl=div(sectornumber,0x24);
        writesect(buffer,sec,head,cyl);
}

void putchar(char c)
{
	printc(c);
}

char getchar()
{
	return (char)readc();
}


void printstring(char* string)
{
	int21(1,string);
}

void printnumber(int number)
{
	char num[12];
	char pnum[12];
	int d=1;
	int i=0;
	int j;
	while(i<10)
	{
		num[i]=mod(div(number,d),10)+0x30;
		i++;
		d=d*10;
		if (div(number,d)==0)
			break;
	}
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
	printstring(pnum);
}

void readstring(char* buffer)
{
	int21(2,buffer);
}

void readfile(char* name, char* buffer)
{
	int21(3,name,buffer);
}

void writefile(char* name, char* buffer, int sectorlength)
{
	int21(4,name,buffer,sectorlength);
}

void deletefile(char* name)
{
	int21(5,name);
}

void executeprogram(char* buffer, int bytelength)
{
	int21(8,buffer,bytelength);
}

void executeprogrambackground(char* buffer, int bytelength)
{
	int21(6,buffer,bytelength);
}

void exit()
{
	int21(7);
}

void setpixel(int color, int column, int row)
{
	drawpixel(color,row*320+column);
}

void setcursor(int row, int column)
{
	interrupt(0x10,2*256,0,0,row*256+column);
}

void clearscreen()
{
    int r,c;
    for (c=0; c<80; c++)
            for (r=0; r<25; r++)
            {
                    putInMemory(0xB800,(r*80+c)*2,0x20);
                    putInMemory(0xB800,(r*80+c)*2+1,7);
            }
}

void setchar(char c, char color, int row, int column)
{
	putInMemory(0xB800,(row*80+column)*2,c);
	putInMemory(0xB800,(row*80+column)*2+1,color);
}

void setstring(char* c, char color, int row, int column)
{
	int i=0;
	while(c[i]!=0x0)
	{
		putInMemory(0xB800,(row*80+column+i)*2,c[i]);
		putInMemory(0xB800,(row*80+column+i)*2+1,color);
		i++;
	}
}

void getnumberstring(char* pnum, int number)
{
	char num[12];
	int d=1;
	int i=0;
	int j;
	while(i<10)
	{
		num[i]=mod(div(number,d),10)+0x30;
		i++;
		d=d*10;
		if (div(number,d)==0)
			break;
	}
	j=0;
	for (d=i-1; d>=0; d--)
		pnum[j++]=num[d];
	pnum[j]=0;
}

void entradaM(int sect, char* map, char * dir, int dirindex, char* content, int x)
{
	int entradaMapa;
	for (entradaMapa=0; entradaMapa<256; entradaMapa++)
		{
			if (map[entradaMapa]==0)
				break;
		}	
		if (entradaMapa==256)
		{
			printstring("Not enough room for file\n");
			return;
		}
		map[entradaMapa]=0x46;
		dir[dirindex+6+x] = entradaMapa;
		writesector(BASEOFFSET+entradaMapa,content+(x*512));
}

int otherFor(char* direccion, char* dir)
{
	char nombre[6];
	char found = 0;
	char sectorNumber;
	char otro[6];
	int indexx;
	int n;
	int y;
	int b=0;
	int w=0;
	int z = 0;
	char token [10];
	int toks = CountTokens(direccion);
	sectorNumber = 0x01;
	for (b=0; b<10; b++)
		token[b]=0x0;
	while(w < toks)
	{
		z = getTok(direccion,token,z);
		found = 0;
		for(n=0;n<6;n++)
		{
			nombre[n] = token[n];
		}
		for (indexx=0;indexx<15;indexx++)
		{
			for(y=0;y<6;y++)
			{
				otro[y] = dir[indexx*0x20+y]; //si existe o no el directorio
			}
			if(cmp(otro,nombre,6)==1) //si encontramos el directorio
			{
				sectorNumber = dir[indexx*0x20+6];
				readsector(BASEOFFSET+sectorNumber,dir);
				w++;
				found = 1;
				break;
			}				
		}

		if(found == 1)
		{
			continue;
		}
		w++;		
	}
	return sectorNumber;
}

void guardarArchivo(char* direccion, char* content, int size)
{

	int i;
	char r;
	int sect;
	char map[512];
	char dir[512];
	char nombre[6];
	char found = 0;
	char sectorNumber;
	char otro[6];
	int indexx;
	int n;
	int y;
	int sectorLibre = 0;
	int entradaMapa = 0;
	int dirindex = 0;
	int sectcount;
	int b=0;
	int w=0;
	int z = 0;
	char token [10];
	int toks = CountTokens(direccion);
	readsector(BASEOFFSET,map);
	readsector(BASEOFFSET +1,dir);	
	//sectorNumber = otherFor(direccion, dir);
	sectorNumber = 0x01;
	for (b=0; b<10; b++)
		token[b]=0x0;
	while(w < toks)
	{
		z = getTok(direccion,token,z);
		found = 0;
		for(n=0;n<6;n++)
		{
			nombre[n] = token[n];
		}
		for (indexx=0;indexx<15;indexx++)
		{
			for(y=0;y<6;y++)
			{
				otro[y] = dir[indexx*0x20+y]; //si existe o no el directorio
			}
			if(cmp(otro,nombre,6)==1) //si encontramos el directorio
			{
				sectorNumber = dir[indexx*0x20+6];
				readsector(BASEOFFSET+sectorNumber,dir);
				w++;
				found = 1;
				break;
			}				
		}

		if(found == 1)
		{
			continue;
		}
		w++;		
	}//busca que es el ultimo token no me acuerdo que hace
	for (indexx=0;indexx<15;indexx++)
	{
		dirindex = indexx*0x20;
		sectorLibre = dir[dirindex]; //buscamos la primera entrada libre
		if(sectorLibre==0)
		{
			for (i=0; i<6; i++)
				dir[dirindex+i]=0x20;
			for (i=0; i<6; i++)
			{
				if(nombre[i]==0)
				break;
				dir[dirindex+i]=nombre[i];		
			}
			break;
		}
	}	
	sect = (div(size,512)+1);
	for(indexx=0;indexx<sect;indexx++)
	{
		printnumber(dirindex);
		entradaM(sect,map,dir,dirindex,content,indexx);
	}
	writesector(BASEOFFSET+sectorNumber,dir);
	writesector(BASEOFFSET,map);
	return;
}




void leerArchivo(char* path)
{
	int i;
	char r;
	char map[512];
	char dir[512];
	char nombre[6];
	char found = 0;
	char sectorNumber;
	char prevSectorNumber;
	char comparar[6];
	int index=0;
	int indexBuff=0;
	int x;
	int n;
	int y;
	int sectorLibre = 0;
	int entradaMapa = 0;
	int dirindex = 0;
	int sectcount;
	int b=0;
	int w=0;
	int z = 0;
	char token [10];
	char buffFile[512];
	int toks = CountTokens(path);//cantidad de subdirectorios
	readsector(BASEOFFSET,map);
	readsector(BASEOFFSET +1,dir);	
	sectorNumber = 0x01;
	prevSectorNumber = 0x01;
	for (b=0; b<10; b++)
		token[b]=0x0;
	while(w < toks)
	{
		z = getTok(path,token,z);
		found = 0;
		for(n=0;n<6;n++)
		{
			nombre[n] = token[n];
		}
		for (x=0;x<15;x++)
		{		
			for(y=0;y<6;y++)
			{
				comparar[y] = dir[x*0x20+y]; //copiando el nombre del directorio
			}
			if(cmp(comparar,nombre,6)==1) //si encontramos el directorio
			{
				prevSectorNumber = sectorNumber;
				sectorNumber = dir[x*0x20+6];
				index = sectorNumber;
				readsector(BASEOFFSET+sectorNumber,dir);
				found = 1;
				break;
			}
		}
		w++;
	}
	if(found == 1)
	{
		while(dir[index]!=0x00)
		{
			readsector(BASEOFFSET+index,buffFile+indexBuff);
			index++;
			indexBuff=indexBuff+512;
		}
		printstring(buffFile);
		
	}else
	{
		printstring("Directorio no Encontrando");
	}	

}

void ImprimirLista(char sectorNumber, int depth, char* mapsector)
{
	char dirsector[512];
	char name[7];
	int i,j,index,files,x;
	char symbol [2];
	symbol[0] = 195;
	symbol[1] = 0;
	readsector(BASEOFFSET + sectorNumber,dirsector);
	index=0;
	files=0;
	for (i=0; i<16; i++)
	{
		if (dirsector[index]!=0)
		{
			for (j=0; j<6; j++)
				name[j]=dirsector[index+j];
			name[6]=0;
			for(x =0;x<depth;x++)
				printstring("\t");
			printstring(symbol);
			printstring(name);
			sectorNumber = dirsector[i*0x20+6];
			if(mapsector[sectorNumber] == 0x44)
			{	
				j=0;
				while(dirsector[index+j+6]!=0)
					j++;
				printstring("  \0");
				printstring( "D\0");
				printstring("\r\n\0");
				ImprimirLista(sectorNumber,depth+1,mapsector);
				files++;
			}
			else if(mapsector[sectorNumber] == 0x46)
			{	
				j=0;
				while(dirsector[index+j+6]!=0)
					j++;
				printstring(".txt");
				printstring("  \0");
				printstring("F\0");
				printstring("\r\n\0");
				files++;
			}
		}
		index=index+0x20;
	}
	j=0;
	for (i=0; i<256; i++)
	{
		if (mapsector[i]==0x0)
			j++;
	}
}

void Listar()
{
	char mapsector[512];
	readsector(BASEOFFSET,mapsector);
	printstring("Directory:\r\n\0");
	ImprimirLista(0x01,0,mapsector);
	
}

void innerRemove(char sectorNumber, char* map)
{
	char directory[512];
	int x;
	int numBloque;
	readsector(BASEOFFSET+sectorNumber,directory);
	for (x=0;x<15;x+=0x20)
	{	
		if(directory[x] != 0)
		{
			for(numBloque = 6; numBloque<0x20;numBloque++)
			{
				if(map[directory[x+numBloque]]==0x44)
				{
					innerRemove(directory[x+numBloque],map);
					break;						
				}
				else if(map[directory[x+numBloque]]==0x46)
				{
					innerRemove(directory[x+numBloque],map);
					break;						
				}
			}
		}			
	}
	map[sectorNumber] = 0x0;
	for(x=0;x<512;x++)
	{
		directory[x] = 0x0;
	}
	writesector(BASEOFFSET+sectorNumber,directory);
	writesector(BASEOFFSET,map);
}




int cmp(char* nombre, char* name, int size)
{

	int i=0;
	int retonar = 1;
	for(i=0;i<size;i++)
	{
		 if(nombre[i]!=name[i])
			return 0;
	}
	return retonar;
}

int CountTokens(char* path)
{
	int count =0, i=0;
	if(path[0]=='/')
		i++;
	while(path[i] != 0x0)
	{
		if(path[i] == '/')
			count++;
		i++;
	}
	if(path[i-1]!='/')
		count++;
	return count;
}

int getTok(char* path, char* buffer, int next)
{
	int i = 0;
	int j=0;
	for(i =0; i<10; i++)
		buffer[i] = 0x20;
	for(i=next;i<10;i++)
	{
		if(path[i]=='/' || path[i] == 0)
			break;
		buffer[j]=path[i];
		j++;		
	}
	return i+1;

}

void strTok(char* path)
{
	int y = CountTokens(path);
	int i=0;
	int x=0,w=0;
	int z = 0;
	char token [10];
	for (i=0; i<10; i++)
		token[i]=0x0;
	while(w < y)
	{
		z = getTok(path,token,z);
		w++;
	}
}

void Remove(char* path)
{
	int i;
	char r;
	char map[512];
	char dir[512];
	char nombre[6];
	char found = 0;
	char sectorNumber;
	char prevSectorNumber;
	char comparar[6];
	int x;
	int n;
	int y;
	int sectorLibre = 0;
	int entradaMapa = 0;
	int dirindex = 0;
	int sectcount;
	int b=0;
	int w=0;
	int z = 0;
	char token [10];
	int toks = CountTokens(path);//cantidad de subdirectorios
	readsector(BASEOFFSET,map);
	readsector(BASEOFFSET +1,dir);	
	sectorNumber = 0x01;
	prevSectorNumber = 0x01;
	for (b=0; b<10; b++)
		token[b]=0x0;
	while(w < toks)
	{
		z = getTok(path,token,z);
		found = 0;
		for(n=0;n<6;n++)
		{
			nombre[n] = token[n];
		}
		for (x=0;x<15;x++)
		{		
			for(y=0;y<6;y++)
			{
				comparar[y] = dir[x*0x20+y]; //copiando el nombre del directorio
			}
			if(cmp(comparar,nombre,6)==1) //si encontramos el directorio
			{
				prevSectorNumber = sectorNumber;
				sectorNumber = dir[x*0x20+6];
				readsector(BASEOFFSET+sectorNumber,dir);
				found = 1;
				break;
			}
		}
		w++;
	}
	if(found == 1)
	{
		readsector(BASEOFFSET + prevSectorNumber,dir);
		for(y=0;y<7;y++)
		{
			dir[x*0x20+y] = 0;
		}
		writesector(BASEOFFSET + prevSectorNumber,dir);
	
	}else
	{
		printstring("Directorio no Encontrando");
	}	
}



void mkdir(char* path)
{
	
	int b=0;
	int w=0;
	int z = 0;
	char token [10];
	int toks = CountTokens(path);
	int k;
	char r;
	char mapa[512];
	char dir[512];
	char nombre[6];
	char found = 0;
	char sectorNumber;
	char comparar[6];
	int x;
	int n;
	int y;
	int sectorLibre = 0;
	int entrada = 0;
	int dirindex = 0;
	int sectcount;	
	readsector(BASEOFFSET,mapa);
	readsector(BASEOFFSET +1,dir);
	sectorNumber = 0x01;
	for (b=0; b<10; b++)
		token[b]=0x0;
	while(w < toks)
	{
		z = getTok(path,token,z);
		found = 0;
		for(n=0;n<6;n++)
		{
			nombre[n] = token[n];
		}
		for (x=0;x<15;x++)
		{
			for(y=0;y<6;y++)
			{
				comparar[y] = dir[x*0x20+y]; //si existe o no el directorio
			}
			if(cmp(comparar,nombre,6)==1) //si encontramos el directorio
			{
				sectorNumber = dir[x*0x20+6];
				readsector(BASEOFFSET+sectorNumber,dir);
				w++;
				found = 1;
				break;
			}				
		}
		if(found == 1)
		{
			continue;
		}		
		for (entrada=0; entrada<256; entrada++)
			if (mapa[entrada]==0)
				break;
		if (entrada==256)
		{
			printstring("Not enough room for file\n");
			return;
		}
		mapa[entrada]=0x44;
		for (x=0;x<15;x++)
		{
			dirindex = x*0x20;
			sectorLibre = dir[dirindex]; //buscamos la primera entrada libre
			if(sectorLibre==0)
			{
				for (k=0; k<6; k++)
					dir[dirindex+k]=0x20;
				for (k=0; k<6; k++)
				{
					if(nombre[k]==0)
					break;
					dir[dirindex+k]=nombre[k];		
				}
				dir[dirindex+6] = entrada;
				break;
			}
		}
		writesector(BASEOFFSET+sectorNumber,dir);
		writesector(BASEOFFSET,mapa);
		readsector(BASEOFFSET+entrada,dir);
		sectorNumber = entrada;
		w++;
	}
}

void Format()
{
	int i =0;
	char arreglo [512];
	for(i=0; i<512;i++)
	{
		arreglo[i]=0;
	}
	for(i=0; i<512;i++)
	{
		writesector(0X40+i,arreglo); 
	}
	arreglo[0] = 'M';
	arreglo[1] = 'D';
	writesector(0X40,arreglo);
	printstring("Format Done!");
}
