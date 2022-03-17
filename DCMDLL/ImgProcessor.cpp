#include "imgprocessor.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcrledrg.h" 
#include "dcmtk/dcmdata/dctk.h"


bool ImgProcessor::rotateImage(const char *inputFilename, const char *outputFilename, const ROTATE_TYPE type) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset *dataset = dfile.getDataset();
	
	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage *di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	int rotateDegree;

	switch (type) {
	case ROTATE_LEFT:
		rotateDegree = 270; break;
	case ROTATE_RIGHT:
		rotateDegree = 90; break;
	case ROTATE_TOP_DOWN:
		rotateDegree = 180; break;
	default:
		break;
	}

	if (!di->rotateImage(rotateDegree)) return false;

	if (!di->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char *oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good())
	{
		if (oldImageType != NULL)
		{
			// append old image type information beginning with second entry
			const char *pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem *ditem = NULL;
	const char *sopClassUID = NULL;
	const char *sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;

	return true;
}

bool ImgProcessor::flipImage(const char *inputFilename, const char *outputFilename, const FLIP_TYPE type) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset *dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage *di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	int rotateDegree;

	switch (type) {
	case FLIP_HORIZONTALLY:
		if (!di->flipImage(1, 0)) return false;
	case FLIP_VERTICALLY:
		if (!di->flipImage(0, 1)) return false;
	case FLIP_BOTH_AXES:
		if (!di->flipImage(1, 1)) return false;
	default:
		break;
	}

	if (!di->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char *oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good()) {
		if (oldImageType != NULL) {
			// append old image type information beginning with second entry
			const char *pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem *ditem = NULL;
	const char *sopClassUID = NULL;
	const char *sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;

	return true;
}

bool ImgProcessor::scaleImageByFactor(const char *inputFilename, const char *outputFilename, const SCALE_TYPE type, const float factor) {
	
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset *dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage *di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	DicomImage *newImage = NULL;

	switch (type)
	{
	case SCALE_X:
		newImage = di->createScaledImage(factor, 0.0, 4);
		break;
	case SCALE_Y:
		newImage = di->createScaledImage(0.0, factor, 4);
		break;
	default:
		break;
	}

	if (!newImage->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char *oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good()) {
		if (oldImageType != NULL) {
			// append old image type information beginning with second entry
			const char *pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem *ditem = NULL;
	const char *sopClassUID = NULL;
	const char *sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;
	delete newImage;

	return true;
}

bool ImgProcessor::clipAndScaleImageByFactor(const char *inputFilename, const char *outputFilename, const int left, const int top, const int width, const int height, const SCALE_TYPE type, const float factor) {
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset *dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage *di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	DicomImage *newImage = NULL;

	switch (type)
	{
	case SCALE_X:
		newImage = di->createScaledImage(left, top, width, height, factor, 0.0, 4);
		break;
	case SCALE_Y:
		newImage = di->createScaledImage(left, top, width, height, 0.0, factor, 4);
		break;
	default:
		break;
	}

	if (!newImage->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char *oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good()) {
		if (oldImageType != NULL) {
			// append old image type information beginning with second entry
			const char *pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem *ditem = NULL;
	const char *sopClassUID = NULL;
	const char *sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;
	delete newImage;

	return true;
}

bool ImgProcessor::changeContrastOfImage(const char *inputFilename, const char *outputFilename) {
	
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset *dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage *di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	if (!di->setPolarity(EPP_Reverse)) return false;

	if (!di->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char *oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good())
	{
		if (oldImageType != NULL)
		{
			// append old image type information beginning with second entry
			const char *pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem *ditem = NULL;
	const char *sopClassUID = NULL;
	const char *sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;

	return true;
}