#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

using namespace std;


int main(int argc, char** argv)
{
	/* Make sure the command line is correct */
	if (argc < 2)
	{
		cout << "FILE NAME missing\n";
		exit(1);
	}

	/* Open the input file */
	int inFile = open(argv[1], O_RDWR);


	if (inFile < 0)
	{
		cout << "\n" << "Input file cannot be opened." << "\n";
		exit(1);
	}

	/* Open the output file */
	int outFile = open(argv[2], O_RDWR | O_CREAT| O_TRUNC, 0666);

	if (outFile < 0)
	{
		cout << "\n" << "Output file cannot be opened not created." << "\n";
		exit(1);
	}

	struct stat stats;
	if (stat(argv[1], &stats) == 0)
		cout << "File Size = " << stats.st_size << endl;
	else
		cout << "Unable to get file properties.\n";

	/* Get the page size  */
	int pagesize = getpagesize();
	cout << "Page Size = " << pagesize << "\n";

	/* go to the location corresponding to the last byte */
	 if (lseek (outFile, stats.st_size - 1, SEEK_SET) == -1)
	 {
	   printf ("lseek error\n");
	   exit(0);
	 }

 /* write a dummy byte at the last location */
 	write (outFile, "", 1);

	for (int fIndex = 0; fIndex < stats.st_size; fIndex += pagesize)
	{

		/* map input file into memory */
		char* inData = (char*) mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, inFile, fIndex);

		/* Did the mapping succeed ? */
		if (!inData)
		{
			cout << "\n" << "mapping did not succeed" << "\n";
			exit(1);
		}

		// Print the whole file character-by-character //
		/*for (int index = 0; fIndex < pagesize; ++fIndex)
		{
			cout << inData[index];
		}
		cout << endl;*/

		// map output file into memory //
		char* outData = (char*) mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, outFile, fIndex);

		// Did the mapping succeed ? //
		if (!outData)
		{
			cout << "\n" << "mapping did not succeed" << "\n";
			exit(1);
		}

		memcpy(outData, inData, pagesize);

		munmap(inData, pagesize);
		munmap(outData, pagesize);
	}

	/* Close the file */
	close(inFile);
	close(outFile);

	return 0;
}
