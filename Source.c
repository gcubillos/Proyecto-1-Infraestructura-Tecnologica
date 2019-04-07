// ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1 - 201910
// EL GRUPO DEBE SER DESARROLLADO EN GRUPOS DE A 3 PERSONAS MAXIMO
//
// DESARROLLADO POR:
// Gabriel Cubillos Bolivar - 201729365 g.cubillosb
// Nombre - C�digo
// Nombre - C�digo

#define _CRT_SECURE_NO_DEPRECATE 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

// La representacion de la imagen
typedef struct img
{
	int ancho;
	int alto;
	unsigned char *informacion;
} Imagen;


// Funci�n que carga el bmp en la estructura Imagen
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada);

// Funci�n que guarda el contenido de la estructura imagen en un archivo binario
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida);

//Funci�n que inserta un mensaje en la imagen usando n bits por Byte
void insertarMensaje(Imagen * img, unsigned char mensaje[], int n);

//Funci�n que lee un mensaje de una imagen dando la longitud del mensaje y el numero de bits por byte usados
void leerMensaje(Imagen * img, unsigned char msg[], int l, int n);

//Función que saca n bits de una secuencia de caracteres a partir de una posición dada
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n, char doneP[]);

// Programa principal
// NO MODIFICAR
int main(int argc, char* argv[]) {

	Imagen *img = (Imagen *)malloc(sizeof(Imagen));
	char msg[10000];
	int op, num, l, i, n;
	char nomArch[256];

	printf("ISIS-1304 - FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - PROYECTO 1\n");
	printf("Indique la accion a realizar:\n\t1) Insertar mensaje\n\t2) Leer mensaje\n\n");
	printf("Opcion: ");
	scanf("%d", &op);

	printf("Ingrese el nombre/ruta de la imagen (incluya el formato .bmp): ");
	
	char temp;
	scanf("%c", &temp);
	gets(nomArch);

	// Cargar los datos
	cargarBMP24(img, nomArch);

	if (op == 1) {
		printf("Ingrese el mensaje a insertar:\n\n");

		gets(msg);

		printf("\nLongitud mensaje: %d bytes\n", strlen(msg));
		msg[strlen(msg)] = '\0';

		printf("Ingrese el numero de bits por Byte: ");
		scanf("%d", &num);

		printf("Insertando . . .\n");
		insertarMensaje(img, msg, num);
		printf("Insertado correctamente !\n");

		char nuevo[256];
		printf("Ingrese el nombre del archivo a guardar (incluya el formato .bmp): ");

		scanf("%c", &temp);
		gets(nuevo);

		guardarBMP24(img, nuevo);

		printf("Mensaje insertado y guardado exitosamente en el archivo/ruta '%s' ! :D\n\n", nuevo);
	}
	else if (op == 2) {
		printf("Ingrese la longitud a leer del mensaje insertado: ");

		scanf("%d", &l);

		printf("Ingrese el numero de bits por Byte: ");
		scanf("%d", &n);

		for (i = 0; i < l; i++) {
			msg[i] = 0;
		}

		leerMensaje(img, msg, l, n);

		msg[l] = 0;

		printf("Mensaje obtenido exitosamente ! El mensaje es:\n\n%s\n\n", msg);
	}
	else {
		printf("La opcion ingresada (%d) no es correcta\n", op);
		system("pause");
		return -1;
	}
	system("pause");
	return 0;
}

/**
* Inserta un mensaje, de a n bits por componente de color, en la imagen apuntada por img
* parámetro img: Apuntador a una imagen en cuyos pixeles se almacenará el mensaje.
* parámetro mensaje: Apuntador a una cadena de caracteres con el mensaje.
* parámetro n: Cantidad de bits del mensaje que se almacenarán en cada componente de color de cada pixel. 0 < n <= 8.
*/
void insertarMensaje(Imagen* img, unsigned char mensaje[], int n) {

	// ------------------------------------------------------------------------------
	// Insert the message to the image information
	// ------------------------------------------------------------------------------
	if (mensaje == NULL) return 0; /* no input string */
	int number_bits = 0;
	size_t len = strlen(mensaje);
	char* binary = malloc(len * 8 + 1);	// each char is one byte (8 bits) and + 1 at the end for null terminator
	binary[0] = '\0';
	for (size_t i = 0; i < len; ++i)
	{
		char ch = mensaje[i];
		for (int j = 7; j >= 0; --j)
		{
			number_bits++;
			if (ch & (1 << j))
			{
				strcat(binary, "1");
			}
			else
			{
				strcat(binary, "0");
			}
		}
	}
	int bmpHeight, bmpWidth;
	number_bits = strlen(binary);
	bmpHeight = img->alto;
	bmpWidth = img->ancho;

	unsigned char* info = img->informacion;

	size_t ilen = strlen(img->informacion); // Numero de bits imagen
	int posMsg = 0; // Posicion en el mensaje
	char done = 1; // Se termina de escribir
	char* doneP = &done; // Pointer to the position where the done variable is


		// First we're gonna go through the image information
	for (int i = 0; i < ilen && done; i++)
	{
		unsigned char a = info[i];
		// Make cero the n less significant bits of the byte i of the image information
		a = a >> n;
		a = a << n;

		// Char with the n bits that we're gonna insert. The n bits are the less significant bits of the char
		unsigned char bitsToInsert = sacarNbits(binary, i * n, n, doneP);

		// Insert the n bits to the char with the image info
		info[i] = a | bitsToInsert;
		unsigned char finalByte = info[i];
	}



	img->informacion = info;
	// ------------------------------------------------------------------------------
	// End
	// ------------------------------------------------------------------------------


}

/**
* Extrae un mensaje de tamanio l, guardado de a n bits por componente de color, de la imagen apuntada por img
* parámetro img: Apuntador a una imagen que tiene almacenado el mensaje en sus pixeles.
* parámetro msg: Apuntador a una cadena de caracteres donde se depositará el mensaje.
* parámetro l: Tamanio en bytes del mensaje almacenado en la imagen.
* parámetro n: Cantidad de bits del mensaje que se almacenan en cada componente de color de cada pixel. 0 < n <= 8.
*/
void leerMensaje(Imagen* img, unsigned char msg[], int l, int n) {
	// TODO: Desarrollar OBLIGATORIAMENTE en su totalidad.

	unsigned char* info = img->informacion;

	int mlen = (l*8)/n; // Number of bytes in which the message is hidden
	int posMsg = 0; // Position in which the message should be concatenated


		// First we're gonna go through the image information
	for (int i = 0; i < mlen; i++)
	{
		// Make cero the 8-n more significant bits of the byte i of the image information
		info[i] << 8 - n;
		info[i] >> 8 - n;

		// copy the char into the desired position of another char
		memcpy(msg[posMsg], info[i], strlen(info[i]) + 1);

		// Insert the n bits to the char with the image info
		posMsg = posMsg + n;
	}
}

/**
* Extrae n bits a partir del bit que se encuentra en la posición bitpos en la secuencia de bytes que
* se pasan como parámetro
* parámetro secuencia: Apuntador a una secuencia de bytes.
* parámetro n: Cantidad de bits que se desea extraer. 0 < n <= 8.
* parámetro bitpos: Posición del bit desde donde se extraerán los bits. 0 <= n < 8*longitud de la secuencia
* parámetro doneP: Apuntador a el lugar donde se encuentra la variable que indica que se ha termiado el proceso
* retorno: Los n bits solicitados almacenados en los bits menos significativos de un unsigned char
*/
unsigned char sacarNbits(unsigned char secuencia[], int bitpos, int n, char doneP[]) 
{
	size_t len = strlen(secuencia);
	unsigned char* info = secuencia;
	// Know when to stop
	if (bitpos >= len || (bitpos + n) >= len)
	{
		*doneP = 0;
	}

	// Byte where starts the n bits sequence
	int posByte = bitpos / 8;

	// Bit inside the byte where starts the n bits sequence
	int posBit = bitpos % 8;

	// Char with the n bits of the message in the less significant bits
	unsigned char number = 0;

	unsigned char data = info[posByte];
	unsigned char* dataP = &data;

	// If n is 8
	if (n == 8)
	{

	}

	// If the n bits are in two bytes of the message
	else if ((posBit + n) > 8)
	{
		int numberBitsByteOne = 8 - posBit;
		int numberBitsByteTwo = n - numberBitsByteOne;
		// Byte one
		data = data << posBit;		// Now, data has a fraction of the n bits in the more significant bits 
		int aux = numberBitsByteOne;

		// Put the info of the byte one in the correct position 
		while (aux != numberBitsByteTwo)
		{
			data = data >> 1;
			aux++;
		}	// Now the info of the byte one is correctly inside the data char		

		// Byte two
		char aa = secuencia[posByte + 1];
		char data2 = aa;
		data2 = data2 >> (8 - numberBitsByteTwo);	// Now, data2 has a fraction of the n bits in the less significant bits 

		// Concatenate data with data2
		data = data | data2;
	}

	// If the n bits are just in one byte
	else
	{
		int aux = posBit;
		data = data << posBit;
		aux = 8 - n;
		while (aux != 0)
		{
			data = data >> 1;
			aux--;
		}
	}

	return data;
}

// Lee un archivo en formato BMP y lo almacena en la estructura img
// NO MODIFICAR
void cargarBMP24(Imagen * imagen, char * nomArchivoEntrada) {
	// bmpDataOffset almacena la posici�n inicial de los datos de la imagen. Las otras almacenan el alto y el ancho
	// en pixeles respectivamente
	int bmpDataOffset, bmpHeight, bmpWidth;
	int y;
	int x;
	int	residuo;

	FILE *bitmapFile;
	bitmapFile = fopen(nomArchivoEntrada, "rb");
	if (bitmapFile == NULL) {
		printf("No ha sido posible cargar el archivo: %s\n", nomArchivoEntrada);
		exit(-1);
	}

	fseek(bitmapFile, 10, SEEK_SET); // 10 es la posici�n del campo "Bitmap Data Offset" del bmp	
	fread(&bmpDataOffset, sizeof(int), 1, bitmapFile);

	fseek(bitmapFile, 18, SEEK_SET); // 18 es la posici�n del campo "height" del bmp
	fread(&bmpWidth, sizeof(int), 1, bitmapFile);
	bmpWidth = bmpWidth * 3;

	fseek(bitmapFile, 22, SEEK_SET); // 22 es la posici�n del campo "width" del bmp
	fread(&bmpHeight, sizeof(int), 1, bitmapFile);

	residuo = (4 - (bmpWidth) % 4) & 3; // Se debe calcular los bits residuales del bmp, que surjen al almacenar en palabras de 32 bits

	imagen->ancho = bmpWidth;
	imagen->alto = bmpHeight;
	imagen->informacion = (unsigned char *)calloc(bmpWidth * bmpHeight, sizeof(unsigned char));

	fseek(bitmapFile, bmpDataOffset, SEEK_SET); // Se ubica el puntero del archivo al comienzo de los datos

	for (y = 0; y < bmpHeight; y++) {
		for (x = 0; x < bmpWidth; x++) {
			int pos = y * bmpWidth + x;
			fread(&imagen->informacion[pos], sizeof(unsigned char), 1, bitmapFile);
		}
		fseek(bitmapFile, residuo, SEEK_CUR); // Se omite el residuo en los datos
	}
	fclose(bitmapFile);
}

// Esta funci�n se encarga de guardar una estructura de Imagen con formato de 24 bits (formato destino) en un archivo binario
// con formato BMP de Windows.
// NO MODIFICAR
void guardarBMP24(Imagen * imagen, char * nomArchivoSalida) {
	unsigned char bfType[2];
	unsigned int bfSize, bfReserved, bfOffBits, biSize, biWidth, biHeight, biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant;
	unsigned short biPlanes, biBitCount;
	FILE * archivoSalida;
	int y, x;
	int relleno = 0;

	int residuo = (4 - (imagen->ancho) % 4) & 3; // Se debe calcular los bits residuales del bmp, que quedan al forzar en palabras de 32 bits


	bfType[2];       // Tipo de Bitmap
	bfType[0] = 'B';
	bfType[1] = 'M';
	bfSize = 54 + imagen->alto * ((imagen->ancho) / 3) * sizeof(unsigned char);       // Tamanio total del archivo en bytes
	bfReserved = 0;   // Reservado para uso no especificados
	bfOffBits = 54;    // Tamanio total del encabezado
	biSize = 40;      // Tamanio del encabezado de informacion del bitmap	
	biWidth = (imagen->ancho) / 3;     // Ancho en pixeles del bitmap	
	biHeight = imagen->alto;    // Alto en pixeles del bitmap	
	biPlanes = 1;    // Numero de planos	
	biBitCount = 24;  // Bits por pixel (1,4,8,16,24 or 32)	
	biCompression = 0;   // Tipo de compresion
	biSizeImage = imagen->alto * imagen->ancho;   // Tamanio de la imagen (sin ecabezado) en bits
	biXPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada x
	biYPelsPerMeter = 2835; // Resolucion del display objetivo en coordenada y
	biClrUsed = 0;       // Numero de colores usados (solo para bitmaps con paleta)	
	biClrImportant = 0;  // Numero de colores importantes (solo para bitmaps con paleta)	

	archivoSalida = fopen(nomArchivoSalida, "w+b"); // Archivo donde se va a escribir el bitmap
	if (archivoSalida == 0) {
		printf("No ha sido posible crear el archivo: %s\n", nomArchivoSalida);
		exit(-1);
	}

	fwrite(bfType, sizeof(char), 2, archivoSalida); // Se debe escribir todo el encabezado en el archivo. En total 54 bytes.
	fwrite(&bfSize, sizeof(int), 1, archivoSalida);
	fwrite(&bfReserved, sizeof(int), 1, archivoSalida);
	fwrite(&bfOffBits, sizeof(int), 1, archivoSalida);
	fwrite(&biSize, sizeof(int), 1, archivoSalida);
	fwrite(&biWidth, sizeof(int), 1, archivoSalida);
	fwrite(&biHeight, sizeof(int), 1, archivoSalida);
	fwrite(&biPlanes, sizeof(short), 1, archivoSalida);
	fwrite(&biBitCount, sizeof(short), 1, archivoSalida);
	fwrite(&biCompression, sizeof(int), 1, archivoSalida);
	fwrite(&biSizeImage, sizeof(int), 1, archivoSalida);
	fwrite(&biXPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite(&biYPelsPerMeter, sizeof(int), 1, archivoSalida);
	fwrite(&biClrUsed, sizeof(int), 1, archivoSalida);
	fwrite(&biClrImportant, sizeof(int), 1, archivoSalida);

	// Se escriben en el archivo los datos RGB de la imagen.
	for (y = 0; y < imagen->alto; y++) {
		for (x = 0; x < imagen->ancho; x++) {
			int pos = y * imagen->ancho + x;
			fwrite(&imagen->informacion[pos], sizeof(unsigned char), 1, archivoSalida);
		}
		fwrite(&relleno, sizeof(unsigned char), residuo, archivoSalida);
	}
	fclose(archivoSalida);
}