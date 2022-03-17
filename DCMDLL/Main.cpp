#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include "imgprocessor.h"
#include "dcmtk/dcmdata/dcddirif.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

void load(int argc, char const *argv[]) {
	if (argc == 1) {
		cout << "Enter '-h' to view help\n";
		cout << "Exit.\n";
		exit(1);
	}
	else if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0) {
			cout << "Command format: -i 'input_filename' -o 'output_filename' ...\n";

			cout << "For rotating image:\n";
			cout << "\t+Rl\t--rotate-left\n";
			cout << "\t+Rr\t--rotate-right\n";
			cout << "\t+Rtd\t--rotate-top-down\n";

			cout << "For flipping image:\n";
			cout << "\t+Fh\t--flip-horizontally\n";
			cout << "\t+Fv\t--flip-vertically\n";
			cout << "\t+Fba\t--flip-both-axes\n";

			cout << "For scaling image by factor\n";
			cout << "\t+Sxf 'factor'(float)\t--scale-x-by-factor\n";
			cout << "\t+Syf 'factor'(float)\t--scale-y-by-factor\n";

			cout << "For clipping and scaling image by factor\n";
			cout << "\t+Cxf 'left'(int) 'top'(int) 'width'(int) 'height'(int) 'factor'(float)\t--clip-and-scale-x-by-factor\n";
			cout << "\t+Cyf 'left'(int) 'top'(int) 'width'(int) 'height'(int) 'factor'(float)\t--clip-and-scale-y-by-factor\n";
			
			cout << "For changing contrast of image:\n";
			cout << "\t+Cc\t--change-contrast\n";
		}
		else if (strcmp(argv[1], "-i") == 0 && argc >= 5) {
			char *inputFilename = new char[strlen(argv[2]) + 1];
			strcpy(inputFilename, argv[2]);

			if (strcmp(argv[3], "-o") == 0) {
				char *outputFilename = new char[strlen(argv[4]) + 1];
				strcpy(outputFilename, argv[4]);

				ImgProcessor *imgProcessor = new ImgProcessor();

				for (int i = 5; i < argc; ++i) {
					char *command = new char[strlen(argv[i]) + 1];
					strcpy(command, argv[i]);

					if (strcmp(command, "+Rl") == 0) {
						if (imgProcessor->rotateImage(inputFilename, outputFilename, ROTATE_LEFT)) {
							cout << "Rotate image left successfully\n";
						}
						else {
							cout << "Failed to rotate image left\n";
						}
					}
					else if (strcmp(command, "+Rr") == 0) {
						if (imgProcessor->rotateImage(inputFilename, outputFilename, ROTATE_RIGHT)) {
							cout << "Rotate image right successfully\n";
						}
						else {
							cout << "Failed to rotate image right\n";
						}
					}
					else if (strcmp(command, "+Rtd") == 0) {
						if (imgProcessor->rotateImage(inputFilename, outputFilename, ROTATE_TOP_DOWN)) {
							cout << "Rotate image top-down successfully\n";
						}
						else {
							cout << "Failed to rotate image top-down\n";
						}
					}
					else if (strcmp(command, "+Fh") == 0) {
						if (imgProcessor->flipImage(inputFilename, outputFilename, FLIP_HORIZONTALLY)) {
							cout << "Flip image horizontally successfully\n";
						}
						else {
							cout << "Failed to flip image horizontally\n";
						}
					}
					else if (strcmp(command, "+Fv") == 0) {
						if (imgProcessor->flipImage(inputFilename, outputFilename, FLIP_VERTICALLY)) {
							cout << "Flip image vertically successfully\n";
						}
						else {
							cout << "Failed to flip image vertically\n";
						}
					}
					else if (strcmp(command, "+Fba") == 0) {
						if (imgProcessor->flipImage(inputFilename, outputFilename, FLIP_BOTH_AXES)) {
							cout << "Flip image both axes successfully\n";
						}
						else {
							cout << "Failed to flip image both axes\n";
						}
					}
					else if (strcmp(command, "+Sxf") == 0 && i + 1 < argc) {
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (imgProcessor->scaleImageByFactor(inputFilename, outputFilename, SCALE_X, factor)) {
							cout << "Scale image's x axis by factor successfully\n";
						}
						else {
							cout << "Failed to scale image's x axis by factor\n";
						}
					}
					else if (strcmp(command, "+Syf") == 0 && i + 1 < argc) {
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (imgProcessor->scaleImageByFactor(inputFilename, outputFilename, SCALE_Y, factor)) {
							cout << "Scale image's y axis by factor successfully\n";
						}
						else {
							cout << "Failed to scale image's y axis by factor\n";
						}
					}
					else if (strcmp(command, "+Cxf") == 0 && i + 5 < argc) {
						strcpy(command, argv[++i]);
						int left = atoi(command);
						strcpy(command, argv[++i]);
						int top = atoi(command);
						strcpy(command, argv[++i]);
						int width = atoi(command);
						strcpy(command, argv[++i]);
						int height = atoi(command);
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (imgProcessor->clipAndScaleImageByFactor(inputFilename, outputFilename, left, top, width, height, SCALE_X, factor)) {
							cout << "Clip and scale image's x axis by factor successfully\n";
						}
						else {
							cout << "Failed to clip and scale image's x axis by factor\n";
						}
					}
					else if (strcmp(command, "+Cyf") == 0 && i + 5 < argc) {
						strcpy(command, argv[++i]);
						int left = atoi(command);
						strcpy(command, argv[++i]);
						int top = atoi(command);
						strcpy(command, argv[++i]);
						int width = atoi(command);
						strcpy(command, argv[++i]);
						int height = atoi(command);
						strcpy(command, argv[++i]);
						float factor = atof(command);
						if (imgProcessor->clipAndScaleImageByFactor(inputFilename, outputFilename, left, top, width, height, SCALE_Y, factor)) {
							cout << "Clip and scale image's y axis by factor successfully\n";
						}
						else {
							cout << "Failed to clip and scale image's y axis by factor\n";
						}
					}
					else if (strcmp(command, "+Cc") == 0) {
						if (imgProcessor->changeContrastOfImage(inputFilename, outputFilename)) {
							cout << "Change contrast of image successfully\n";
						}
						else {
							cout << "Failed to change contrast of image\n";
						}
					}
				}
			}
			else {
				cout << "Error: wrong command format. Enter '-h' to view help\n";
				cout << "Exit.\n";
				exit(2);
			}
		}
		else {
			cout << "Error: wrong command format. Enter '-h' to view help\n";
			cout << "Exit.\n";
			exit(2);
		}
	}
}

int main(int argc, char const *argv[]) {
	load(argc, argv);
	DicomDirInterface dicomdir;
	OFCondition status = dicomdir.createNewDicomDir();
	if (status.good()) {
		int n = 11;
		for (int i = 1; i <= n; ++i) {
			string s;
			if (i <= 9) {
				s = "0000000"; s += char(i + '0');
			}
			else {
				s = "000000"; 
				s += char(i / 10 + '0');
				s += char(i % 10 + '0');
			}
			OFString filename(s.c_str());
			dicomdir.addDicomFile(OFFilename(filename));
		}
		dicomdir.writeDicomDir();
	}
}